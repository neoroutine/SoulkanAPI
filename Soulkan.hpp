/*Include guards to avoid including this .hpp file multiple times*/
#ifndef SOULKAN_HPP
#define SOULKAN_HPP
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>
#pragma once

/*Defining namespaces as macros so that the user can change them*/
#define SOULKAN_NAMESPACE sk



/*Includes from the std library or from needed tools*/
#include <deque>
#include <fstream>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_VULKAN

namespace SOULKAN_NAMESPACE
{
	//Essential enums and types
	enum class ErrorCode : uint32_t
	{
		//General errors, up to 2**10
		NO_ERROR                   = 1,
		GENERAL_HARDWARE_ERROR     = 2,
		GENERAL_PARAMETER_ERROR    = 4,
		GENERAL_UNBUILT_ERROR = 8, //UNBUILT method call : Calling instance.get_available_physical_devices() before instance.build()
		                           //or calling QueueFamilies.concentrate() on an empty QueueFamilies
		GENERAL_VULKAN_ERROR       = 16,
		GENERAL_GLFW_ERROR         = 32,

		//GENERAL errors, 2**11 to 2**31
	};

	//Commonly used in Result
	class Error
	{
	public:
		Error()
		{}
		Error(ErrorCode code)
		{
			mCode = code;
		}

		ErrorCode code() { return mCode; }
		bool is_error() { return (mCode != ErrorCode::NO_ERROR); }

		std::string to_string()
		{
			switch (mCode)
			{
			    case ErrorCode::NO_ERROR:                   return "NO_ERROR"; break;
				case ErrorCode::GENERAL_HARDWARE_ERROR:     return "GENERAL_HARDWARE_ERROR"; break;
				case ErrorCode::GENERAL_PARAMETER_ERROR:    return "GENERAL_PARAMETER_ERROR"; break;
				case ErrorCode::GENERAL_UNBUILT_ERROR:      return "GENERAL_UNBUILT_ERROR"; break;
				case ErrorCode::GENERAL_VULKAN_ERROR:       return "GENERAL_VULKAN_ERROR"; break;
				case ErrorCode::GENERAL_GLFW_ERROR:         return "GENERAL_GLFW_ERROR"; break;
			}
		}

	private:
		ErrorCode mCode = ErrorCode::NO_ERROR;
	};

	//Essential classes

	//Common return type used
	template<class V, class E = Error>
	class Result
	{
	public:
		Result() {}
		//Essential enums and constants
		Result(V value, E error)
		{
			mValue = value;
			mError = error;
		}

		V value() { return mValue; }
		E error() { return mError; }
		bool is_error() { return (mError.code() != ErrorCode::NO_ERROR); }

	private:
		V mValue = {};
		E mError = {};
	};

	class FunctionQueue
	{
	public:
		FunctionQueue() {}
		FunctionQueue(std::deque<std::function<void()>> functions)
		{
			mFunctions = functions;
		}
		void push(std::function<void()>&& function)
		{
			mFunctions.emplace_back(function);
		}

		void flush()
		{
			for (auto func = mFunctions.begin(); func != mFunctions.end(); func++)
			{
				(*func)();
			}

			mFunctions.clear();
		}

		void rflush()
		{
			for (auto func = mFunctions.rbegin(); func != mFunctions.rend(); func++)
			{
				(*func)();
			}

			mFunctions.clear();
		}

	private:
		std::deque<std::function<void()>> mFunctions = {};
	};

	//Vulkan related classes
	inline Result<std::vector<const char*>, Error> get_required_instance_extensions(bool validationEnabled)
	{
		uint32_t glfwExtensionCount = 0;
		const char** ppGlfwExtensions = nullptr;

		ppGlfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		if (glfwExtensionCount == 0 || ppGlfwExtensions == nullptr)
		{
			return Result(std::vector<const char*>(), Error(ErrorCode::GENERAL_GLFW_ERROR));
		}

		auto extensions = std::vector<const char*>(ppGlfwExtensions, ppGlfwExtensions + glfwExtensionCount);
		if (validationEnabled)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return Result(extensions, Error());
	}

	//TODO:Convert prepares to -> ClassName() : member1(member), ...
	//TODO:Replace all instances of ...createInfo = ... with createInfo = ...
	//GLFW related classes
	class Window
	{
	public:
		Window(){}

		Window(std::string title, uint32_t height, uint32_t width, bool resizable)
		{
			prepare(title, height, width, resizable);
		}

		void prepare(std::string title, uint32_t height, uint32_t width, bool resizable)
		{
			mTitle     = title;
			mHeight    = height;
			mWidth     = width;
			mResizable = resizable;
		}

		Result<Window, Error> build()
		{
			if (mBuilt)
			{
				return Result(*this, Error());
			}

			if (mTitle == "" || mHeight == 0 || mWidth == 0)
			{
				return Result(*this, Error(ErrorCode::GENERAL_PARAMETER_ERROR));
			}

			if (mResizable)
			{
				glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			}
			else
			{
				glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			}

			mPtr = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), 0, NULL);

			if (mPtr == nullptr)
			{
				return Result(*this, Error(ErrorCode::GENERAL_GLFW_ERROR));
			}

			mBuilt = true;

			return Result(*this, Error());
		}

		Result<std::string, Error> rename(std::string title)
		{
			mTitle = title;

			if (mTitle == "")
			{
				return Result(std::string(""), Error(ErrorCode::GENERAL_PARAMETER_ERROR));
			}

			glfwSetWindowTitle(mPtr, title.c_str());

			return Result(mTitle, Error(ErrorCode::NO_ERROR));
		}

        bool built() const { return mBuilt; }
		std::string title() const { return mTitle; }
		uint32_t height() const { return mHeight; }
		uint32_t width() const { return mWidth; }
		bool resizable() const { return mResizable; }
		GLFWwindow* ptr() const { return mPtr; }

	private:
	    bool mBuilt = false;

		std::string mTitle = "";
		uint32_t mHeight   = 0;
		uint32_t mWidth    = 0;
		bool mResizable    = false;
		GLFWwindow* mPtr   = nullptr;
	};

	class Instance
	{
	public:
		Instance() {}

		Instance(bool validationEnabled, std::string appName, std::string engineName = "Vulkan Engine",
			std::vector<const char*> extensions = std::vector<const char*>())
		{
			prepare(validationEnabled, appName, engineName, extensions);
		}

		friend bool operator==(const Instance lhs, const Instance rhs)
		{
			return (lhs.get() == rhs.get());
		}

		friend bool operator!=(const Instance lhs, const Instance rhs)
		{
			return !(lhs == rhs);
		}

		void prepare(bool validationEnabled, std::string appName, std::string engineName = "Vulkan Engine",
			         std::vector<const char*> extensions = std::vector<const char*>())
		{
			mValidationEnabled = validationEnabled;
			mAppInfo           = vk::ApplicationInfo(appName.c_str(), VK_MAKE_VERSION(1, 0, 0), engineName.c_str(), VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0);
			mExtensions        = extensions;
		}

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			                                                VkDebugUtilsMessageTypeFlagsEXT messageType,
			                                                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			                                                void* pUserData)
		{
			//TODO:Be able to specify a specific filename/filepath 
			//TODO:Specify severity at start of line
			std::ofstream out;
			out.open("debugUtilsMessengerOutput.txt", std::ios::out | std::ios::app);
			out << pCallbackData->pMessage << "\n" << std::endl;
			out.close();

			return VK_FALSE;
		}

		Result<Instance, Error> build()
		{
			if (mBuilt)
			{
				return Result(*this, Error());
			}

			//No extensions specified by the user, querying required ones
			if (mExtensions.empty())
			{
				auto getRequiredInstanceExtensionsResult = Result<std::vector<const char*>, Error>();
				if (mValidationEnabled)
				{
					getRequiredInstanceExtensionsResult = get_required_instance_extensions(true);
				}

				else
				{
					getRequiredInstanceExtensionsResult = get_required_instance_extensions(false);
				}

				if (getRequiredInstanceExtensionsResult.is_error())
				{
					return Result(*this, Error(getRequiredInstanceExtensionsResult.error().code()));
				}

				mExtensions = getRequiredInstanceExtensionsResult.value();
			}

			std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

			auto instanceCreateInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), 
				                                             &mAppInfo,
				                                             static_cast<uint32_t>(validationLayers.size()),
				                                             validationLayers.data(),
				                                             static_cast<uint32_t>(mExtensions.size()),
				                                             mExtensions.data());

			//Debug Utils Messenger
			vk::DebugUtilsMessengerCreateInfoEXT createInfo = {};
			if (mValidationEnabled)
			{
				createInfo.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
					vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
					vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
					vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);
			
				createInfo.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
					vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
					vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);
			
				createInfo.setPfnUserCallback(debugCallback);
				createInfo.setPUserData(nullptr);
				//instanceCreateInfo.pNext = &createInfo;
			}

			

			

			mInstance = vk::createInstance(instanceCreateInfo);

			if (mValidationEnabled)
			{
				auto dynamicLoader = vk::DispatchLoaderDynamic(mInstance, vkGetInstanceProcAddr);
			
				vk::Result result = mInstance.createDebugUtilsMessengerEXT(&createInfo, nullptr, &mDebugUtilsMessenger, dynamicLoader);
			}

			mBuilt = true;

			return Result(*this, Error());
		}

		inline Result<std::vector<vk::PhysicalDevice>, Error> get_available_physical_devices()
		{
			if (!mBuilt)
			{
				return Result(std::vector<vk::PhysicalDevice>(), Error(ErrorCode::GENERAL_UNBUILT_ERROR));
			}

			auto availablePhysicalDevices = mInstance.enumeratePhysicalDevices();

			auto err = Error();
			if (availablePhysicalDevices.size() == 0)
			{
				err = Error(ErrorCode::GENERAL_VULKAN_ERROR);
			}

			return Result(availablePhysicalDevices, err);
		}

		inline Result<bool, Error> is_suitable_device(const vk::PhysicalDevice physicalDevice)
		{
			auto err = Error();
			if (physicalDevice == vk::PhysicalDevice())
			{
				return Result(false, Error(ErrorCode::GENERAL_PARAMETER_ERROR));
			}

			auto physicalDeviceFeatures = physicalDevice.getFeatures();

			if (physicalDeviceFeatures == vk::PhysicalDeviceFeatures())
			{
				return Result(false, Error(ErrorCode::GENERAL_VULKAN_ERROR));
			}

			if (!physicalDeviceFeatures.geometryShader)
			{
				return Result(false, Error(ErrorCode::NO_ERROR));
			}

			return Result(true, Error(ErrorCode::NO_ERROR));
		}

		inline Result<std::vector<vk::PhysicalDevice>, Error> get_suitable_physical_devices()
		{
			auto availablePhysicalDevicesResult = this->get_available_physical_devices();
			if (availablePhysicalDevicesResult.is_error())
			{
				return Result(std::vector<vk::PhysicalDevice>(), Error(availablePhysicalDevicesResult.error().code()));
			}

			auto availablePhysicalDevices = availablePhysicalDevicesResult.value();
			auto suitablePhysicalDevices = std::vector<vk::PhysicalDevice>();

			for (uint32_t i = 0; i < availablePhysicalDevices.size(); i++)
			{
				auto isSuitableResult = is_suitable_device(availablePhysicalDevices[i]);
				if (!isSuitableResult.is_error() && isSuitableResult.value())
				{
					suitablePhysicalDevices.push_back(availablePhysicalDevices[i]);
				}
			}

			return Result(suitablePhysicalDevices, Error());
		}

		inline Result<vk::PhysicalDevice, Error> get_best_physical_device(bool discrete)
		{
			auto suitablePhysicalDevicesResult = this->get_suitable_physical_devices();
			if (suitablePhysicalDevicesResult.is_error())
			{
				return Result(vk::PhysicalDevice(), Error(suitablePhysicalDevicesResult.error().code()));
			}

			auto suitablePhysicalDevices = suitablePhysicalDevicesResult.value();
			switch (suitablePhysicalDevices.size())
			{
				//No physical devices found
			case 0: return Result(vk::PhysicalDevice(), Error(ErrorCode::GENERAL_HARDWARE_ERROR));

				//Only one physical device found
			case 1:
			{
				if (discrete)
				{
					auto physicalDeviceProperties = suitablePhysicalDevices[0].getProperties();
					if (physicalDeviceProperties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu)
					{
						return Result(vk::PhysicalDevice(), Error(ErrorCode::GENERAL_HARDWARE_ERROR));
					}

					return Result(suitablePhysicalDevices[0], Error());
				}

				return Result(suitablePhysicalDevices[0], Error());
			}

			default:
				auto chosenPhysicalDevice = vk::PhysicalDevice();
				for (uint32_t i = 0; i < suitablePhysicalDevices.size(); i++)
				{
					auto physicalDeviceProperties = suitablePhysicalDevices[i].getProperties();
					if (physicalDeviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
					{
						chosenPhysicalDevice = suitablePhysicalDevices[i];
						break;
					}
				}

				//No discrete physical devices found
				if (chosenPhysicalDevice == vk::PhysicalDevice())
				{
					if (discrete) { return Result(vk::PhysicalDevice(), Error(ErrorCode::GENERAL_HARDWARE_ERROR)); }

					//Returning first suitable device as fallback
					return Result(suitablePhysicalDevices[0], Error());
				}

				return Result(chosenPhysicalDevice, Error());
			}
		}

		inline Result<vk::SurfaceKHR, Error> create_surface(Window window)
		{
			//Temporarily using VkSurfaceKHR (instead of vk::SurfaceKHR) because GLFW only returns Vk... stuff
			VkSurfaceKHR rawSurface;
			VkResult result = glfwCreateWindowSurface(mInstance, window.ptr(), nullptr, &rawSurface);

			if (result != VK_SUCCESS || rawSurface == VK_NULL_HANDLE)
			{
				return Result(vk::SurfaceKHR(), Error(ErrorCode::GENERAL_GLFW_ERROR));
			}

			return Result(static_cast<vk::SurfaceKHR>(rawSurface), Error());

		}

		void cleanup() const
		{
			auto dynamicLoader = vk::DispatchLoaderDynamic(mInstance, vkGetInstanceProcAddr);
			mInstance.destroyDebugUtilsMessengerEXT(mDebugUtilsMessenger, nullptr, dynamicLoader);

			mInstance.destroy();
		}
		
        bool built() const { return mBuilt; }
		vk::Instance get() const { return mInstance; }
		vk::ApplicationInfo appInfo() const { return mAppInfo; }
		bool validation() const { return mValidationEnabled; }
		std::vector<const char*> extensions() const { return mExtensions; }
	private:
		vk::Instance mInstance = {};
		vk::ApplicationInfo mAppInfo = {};
		vk::DebugUtilsMessengerEXT mDebugUtilsMessenger = {};

		std::vector<const char*> mExtensions = {};

		bool mBuilt = false;
		bool mValidationEnabled = false;


	};


	class QueueFamilies
	{
	public:
		QueueFamilies() {}
		//In the vector, index 0 = generalFamily, 1 = graphicsFamily, 2 = presentFamily, 3 = computeFamily, 4 = transferFamily, 5 = debug/tmp.
		QueueFamilies(std::array<int32_t, 6> queueFamilyIndexes)
		{
			mQueueFamilyIndexes = queueFamilyIndexes;
		}

		Result<std::vector<uint32_t>, Error> concentrate()
		{
			if (this->empty())
			{
				return Result(std::vector<uint32_t>(), Error(ErrorCode::GENERAL_UNBUILT_ERROR));
			}

			std::vector<uint32_t> concentratedIndexes = {};
			//Get rid of all the -1s (here to say that a queue does not exist, not needed for vulkan)
			//Get rid of duplicates
			for (uint32_t i = 0; i < mQueueFamilyIndexes.size(); i++)
			{
				if (mQueueFamilyIndexes[i] != -1 && !present(concentratedIndexes, mQueueFamilyIndexes[i]))
				{
					concentratedIndexes.push_back(mQueueFamilyIndexes[i]);
				}
			}

			return Result(concentratedIndexes, Error());
		}

		bool empty() const 
		{ 
			if (mQueueFamilyIndexes.empty()) { return true;}

			for (uint32_t i = 0; i < mQueueFamilyIndexes.size(); i++)
			{
				if (mQueueFamilyIndexes[i] != -1) { return false;}
			}

			return true;
		}

		//If -1 is returned, then the queue does not exist/the array is empty
		int32_t general()  const { return mQueueFamilyIndexes[0]; }
		int32_t graphics() const { return mQueueFamilyIndexes[1]; }
		int32_t present()  const { return mQueueFamilyIndexes[2]; }
		int32_t compute()  const { return mQueueFamilyIndexes[3]; }
		int32_t transfer() const { return mQueueFamilyIndexes[4]; }
		int32_t debug()    const { return mQueueFamilyIndexes[5]; }


	private:
	    bool present(std::vector<uint32_t> indexes, uint32_t index)
		{
			for (uint32_t i = 0; i < indexes.size(); i++)
			{
				if (indexes[i] == index) { return true;}
			}

			return false;
		}
		std::array<int32_t, 6> mQueueFamilyIndexes = {-1, -1 , -1 , -1 , -1, -1};

	};

	class PhysicalDevice
	{
	public:
		PhysicalDevice() {}
		PhysicalDevice(const Instance instance, bool discrete = true)
		{
			prepare(instance, discrete);
		}


		void prepare(const Instance instance, bool discrete = true)
		{
			mInstance = instance;
			mDiscreteWish = discrete;
		}

		Result<PhysicalDevice, Error> build()
		{
			if (mBuilt)
			{
				return Result(*this, Error());
			}
			if (mInstance == Instance())
			{
				return Result(*this, Error(ErrorCode::GENERAL_PARAMETER_ERROR));
			}

			auto getBestPhysicalDeviceResult = mInstance.get_best_physical_device(mDiscreteWish);

			if (getBestPhysicalDeviceResult.is_error())
			{
				return Result(*this, Error(getBestPhysicalDeviceResult.error().code()));
			}

			mDevice = getBestPhysicalDeviceResult.value();
			auto physicalDeviceProperties = mDevice.getProperties();
			mName   = std::string(physicalDeviceProperties.deviceName.data());
			mType   = physicalDeviceProperties.deviceType;

			mBuilt = true;

			return Result(*this, Error());
		}

		Result<QueueFamilies, Error> get_queue_families(vk::SurfaceKHR testSurface)
		{
			if (!mBuilt)
			{
				return Result(QueueFamilies(), Error(ErrorCode::GENERAL_UNBUILT_ERROR));
			}

			if (!mQueueFamilies.empty())
			{
				return Result(mQueueFamilies, Error());
			}

			std::array<int32_t, 6> queueFamilyIndexes = { -1 };
			auto availableQueueFamilies = mDevice.getQueueFamilyProperties();

			//Looking for a general queue
			uint32_t i = 0;
			for (const auto& queueFamily : availableQueueFamilies)
			{
				if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics &&
					queueFamily.queueFlags & vk::QueueFlagBits::eCompute &&
					mDevice.getSurfaceSupportKHR(i, testSurface))
				{
					queueFamilyIndexes[0] = i;
				}

				i++;
			}

			//Looking for other queues
			i = 0;
			for (const auto& queueFamily : availableQueueFamilies)
			{
				if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
				{
					queueFamilyIndexes[1] = i;
				}

				if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer &&
					queueFamily.queueFlags & vk::QueueFlagBits::eCompute &&
					!(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics))
				{
					queueFamilyIndexes[2] = i;
				}


				if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer &&
					!(queueFamily.queueFlags & vk::QueueFlagBits::eCompute) &&
					!(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics))
				{
					queueFamilyIndexes[3] = i;
				}

				if (mDevice.getSurfaceSupportKHR(i, testSurface))
				{
					queueFamilyIndexes[4] = i;
				}

				i++;
			}

			mQueueFamilies = QueueFamilies(queueFamilyIndexes);

			return Result(mQueueFamilies, Error());
		}

		Result<vk::Extent2D, Error> get_extent(vk::SurfaceKHR surface, Window window)
		{
			if (!mBuilt)
			{
				return Result(vk::Extent2D(), Error(ErrorCode::GENERAL_UNBUILT_ERROR));
			}

			if (surface == {} || /*TODO:window == {}*/)
			{
				return Result(vk::Extent2D(), Error(ErrorCode::GENERAL_PARAMETER_ERROR))
			}

			auto surfaceCapabiilities = mPhysicalDevice.getSurfaceCapabilitiesKHR(surface);

			if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			{
				return Result(vk::Extent2D(surfaceCapabiilities.currentExtent), Error());
			}

			int width, height = 0;
			glfwGetFramebufferSize(window.ptr(), &width, &height);

			if (width == 0 || height == 0)
			{
				return Result(vk::Extent2D(), Error(ErrorCode::GENERAL_GLFW_ERROR));
			}

			vk::Extent2D extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

			extent.width = std::max(surfacCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, extent.width));
			extent.height = std::max(surfacCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, extent.height));

			return Result(extent, Error());
		}

		Result<vk::SurfaceFormatKHR, Error> get_surface_format(vk::SurfaceKHR surface)
		{
			if (!mBuilt)
			{
				return Result(vk::SurfaceFormatKHR(), Error(ErrorCode::GENERAL_UNBUILT_ERROR));
			}

			std::vector<vk::SurfaceFormatKHR> surfaceFormats = mDevice.getSurfaceFormatKHR(surface);

			if (surfaceFormats.size() == 0)
			{
				return Result(vk::SurfaceFormatKHR(), Error(ErrorCode::GENERAL_VULKAN_ERROR));
			}

			if (surfaceFormats.size() == 1 && surfaceFormats[0].format == vk::Format::eUndefined)
			{
				return Result(vk::SurfaceKHR(), Error(ErrorCode::GENERAL_VULKAN_ERROR));
			}

			if (surfaceFormats.size() == 1)
			{
				return Result(surfaceFormats[0], Error());
			}

			for (const auto& surfaceFormat : surfaceFormats)
			{
				if (surfaceFormat.format == vk::Format::eB8G8R8A8Unorm && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				{
					return Result(surfaceFormat, Error());
				}
			}
		}

		Result<bool, Error> is_presentMode_available(vk::SurfaceKHR surface, vk::PresentModeKHR presentMode)
		{
			if (!mBuilt)
			{
				return Result(false, Error(ErrorCode::GENERAL_UNBUILT_ERROR));
			}

			if (surface = {} || presentMode = {})
			{
				return Result(false, Error(ErrorCode::GENERAL_PARAMETER_ERROR));
			}

			std::vector<vk::SurfaceKHR> presentModes = mDevice.getSurfacePresentModesKHR(surface);

			if (presentModes.size() == 0)
			{
				return Result(false, Error(ErrorCode::GENERAL_VULKAN_ERROR));
			}

			for (const auto mode : presentModes)
			{
				if (mode == presentMode)
				{
					return Result(true, Error());
				}
			}

			return Result(false, Error());
		}

		Result<vk::PresentModeKHR, Error> get_present_mode(vk::SurfaceKHR surface)
		{
			if (!mBuilt)
			{
				return Result(vk::PresentModeKHR(), Error(ErrorCode::GENERAL_UNBUILT_ERROR));
			}

			if (surface = {})
			{
				return Result(vk::PresentModeKHR(), Error(ErrorCode::GENERAL_PARAMETER_ERROR));
			}

			std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
		    if (presentModes.size() == 0)
		    {
			    return Result(vk::PresentModeKHR(), Error(ErrorCode::GENERAL_VULKAN_ERROR));
		    }

            //Priority : MAILBOX > FIFO_RELAXED > FIFO > IMMEDIATE
			if (is_presentMode_available(surface, vk::PresentModeKHR::eMailbox)) 
			{ 
				return Result(vk::PresentModeKHR::eMailbox, Error()));
			}

			if (is_presentMode_available(surface, vk::PresentModeKHR::eFifoRelaxed)) 
			{ 
				return Result(vk::PresentModeKHR::eFifoRelaxed, Error()));
			}

			if (is_presentMode_available(surface, vk::PresentModeKHR::eFifo)) 
			{ 
				return Result(vk::PresentModeKHR::eFifo, Error()));
			}

			if (is_presentMode_available(surface, vk::PresentModeKHR::eImmediate)) 
			{ 
				return Result(vk::PresentModeKHR::eImmediate, Error()));
			}

            return Result(vk::PresentModeKHR(), Error(ErrorCode::GENERAL_VULKAN_ERROR));
		}

		Instance instance() const { return mInstance; }
		vk::PhysicalDevice get() const { return mDevice; }
		std::string name() const { return mName; }
		vk::PhysicalDeviceType type() const { return mType; }
	private:
		Instance mInstance = {};
		vk::PhysicalDevice mDevice = {};

		std::string mName = "";
		vk::PhysicalDeviceType mType = {};

		QueueFamilies mQueueFamilies = {};

		bool mDiscreteWish = true;

		bool mBuilt = false;
	};

	class LogicalDevice
	{
	public:
	    LogicalDevice(){}
		LogicalDevice(const PhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensions, const vk::SurfaceKHR testSurface, const vk::PhysicalDeviceFeatures physicalDeviceFeatures = {})
		{
			prepare(physicalDevice, deviceExtensions, testSurface, physicalDeviceFeatures);
		}

		void prepare(const PhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensions, const vk::SurfaceKHR testSurface, const vk::PhysicalDeviceFeatures physicalDeviceFeatures = {})
		{
			mPhysicalDevice = physicalDevice;
			mPhysicalDeviceFeatures = physicalDeviceFeatures;
			mDeviceExtensions = deviceExtensions;
			mTestSurface = testSurface;
		}

		Result<LogicalDevice, Error> build()
		{
			if (mBuilt)
			{
				return Result(*this, Error());
			}

			/*TODO:Implement == operator in PhysicalDevice
			if (mPhysicalDevice == PhysicalDevice())
			{
				return Result(LogicalDevice(), Error(ErrorCode::GENERAL_PARAMETER_ERROR));
			}*/

			//Queue families
			auto queueFamiliesResult = mPhysicalDevice.get_queue_families(mTestSurface);
			if (queueFamiliesResult.is_error())
			{
				return Result(LogicalDevice(), queueFamiliesResult.error());
			}

			QueueFamilies queueFamilies = queueFamiliesResult.value();

			if (queueFamilies.empty())
			{
				return Result(LogicalDevice(), Error(ErrorCode::GENERAL_VULKAN_ERROR));
			}

			std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos = {};
			auto concentratedQueueFamiliesResult = queueFamilies.concentrate();
			if (concentratedQueueFamiliesResult.is_error())
			{
				return Result(LogicalDevice(), concentratedQueueFamiliesResult.error());
			}

			std::vector<uint32_t> concentratedQueueFamilies = concentratedQueueFamiliesResult.value();
			float queuePriority = 1.0f;

			for (const auto queueFamilyIndex : concentratedQueueFamilies)
			{
				deviceQueueCreateInfos.push_back({vk::DeviceQueueCreateFlags(), queueFamilyIndex, 1, &queuePriority});
			}

			auto deviceCreateInfo = vk::DeviceCreateInfo(vk::DeviceCreateFlags(), static_cast<uint32_t>(deviceQueueCreateInfos.size()), deviceQueueCreateInfos.data());

            //Device extensions
			if (mDeviceExtensions.size() == 0)
			{
				return Result(LogicalDevice(), Error(ErrorCode::GENERAL_PARAMETER_ERROR));
			}

			deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(mDeviceExtensions.size());
			deviceCreateInfo.ppEnabledExtensionNames = mDeviceExtensions.data();

			//Physical Device Features
			mPhysicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
			deviceCreateInfo.pEnabledFeatures = &mPhysicalDeviceFeatures;

			vk::Device device = mPhysicalDevice.get().createDevice(deviceCreateInfo);

			//TODO:Check if device is fine as is (try catch ?)
			mDevice = device;

			mBuilt = true;

			return Result(*this, Error());
		}

		void cleanup() const
		{
			mDevice.destroy();
		}

		PhysicalDevice physicalDevice() const { return mPhysicalDevice;}
		vk::Device get() const { return mDevice;}
		QueueFamilies queueFamilies() const { return mQueueFamilies;}
		vk::PhysicalDeviceFeatures physicalDeviceFeatures() const { return mPhysicalDeviceFeatures;}
		std::vector<const char*> deviceExtensions() const { return mDeviceExtensions;}
		vk::SurfaceKHR testSurface() const { return mTestSurface;}


		private:
			bool mBuilt = false;

			PhysicalDevice mPhysicalDevice = {};
			vk::Device mDevice = {};

			QueueFamilies mQueueFamilies = {};
			vk::PhysicalDeviceFeatures mPhysicalDeviceFeatures = {};
			std::vector<const char*> mDeviceExtensions = {};
			vk::SurfaceKHR mTestSurface = {};
	};

	class Swapchain
	{
	public:
	    Swapchain(){}
		//Swapchain(physicalDevice, surface, imageformat, extent and check for maxImageExtent) {}
		Swapchain(PhysicalDevice physicalDevice, vk::SurfaceKHR surface, Window window, vk::Extent2D extent = {})
		{
			prepare(physicalDevice, surface, window, extent);
		}

		void prepare(PhysicalDevice physicalDevice, vk::SurfaceKHR surface, Window window, vk::Extent2D extent = {})
		{
			mPhysicalDevice = physicalDevice;
			mSurface = surface;
			mWindow = window;
			mExtent = extent;
		}

		Result<Swapchain, Error> build()
		{
			if (mBuilt)
			{
				return Result(*this, Error());
			}

			//TODO
			//if (physicalDevice == {} || window == {})
			if (mSurface == {})
			{
				return Result(Swapchain(), Error(ErrorCode::GENERAL_PARAMETER_ERROR))
			}

			if (!mDevice.built())
			{
				return Result(Swapchain(), Error(ErrorCode::GENERAL_UNBUILT_ERROR));
			}

			vk::PhysicalDevice physicalDevice = mDevice.physicalDevice().get();

			auto surfaceCapabiilities = physicalDevice.getSurfaceCapabilitiesKHR(mSurface);

			//Image count
			mImageCount = surfaceCapabilities.minImageCount + 1;
			if (surfaceCapabilities.maxImageCount > 0 && mImageCount > surfaceCapabilities.maxImageCount)
			{
				mImageCount = surfaceCapabilities.maxImageCount;
		    }

			//Handle empty extent or wrong one
			if (mExtent == {})
			{
				mExtent = mPhysicalDevice.get_extent(mSurface, mWindow);
			}
			else
			{
				if (mExtent.width < surfaceCapabilities.minImageExtent.width || mExtent.width > surfaceCapabiilities.maxImageExtent.width ||
				    mExtent.height < surfaceCapabilities.minImageExtent.height || mExtent.height > surfaceCapabiilities.maxImageExtent.height)
					{
						return Result(Swapchain(), Error(ErrorCode::GENERAL_PARAMETER_ERROR));
					}
			}

			//Concurrency
			auto builtQueueFamilies = mPhysicalDevice.get_queue_families();
			if (builtQueueFamilies.is_error())
			{
				return Result(Swapchain(), builtQueueFamilies.error());
			}

			QueueFamilies queueFamilies = builtQueueFamilies.value();
			auto builtConcentrated = queueFamilies.concentrate();
			if (builtConcentrated.is_error())
			{
				return Result(Swapchain(), builtConcentrated.error());
			}

			std::vector<uint32_t> concentrated = builtConcentrated.value();

			if (concentrated.size() > 1)
			{
				mSharingMode = vk::SharingMode::eConcurrent;
			}
			else
			{
				mSharingMode = vk::SharingMode::eExclusive;
			}

            //TODO:Put all the infos in this single declaration
			auto createInfo = vk::SwapchainCreateInfoKHR(vk::SwapchainCreateFlagsKHR(), mSurface, mImageCount
			                                            mFormat.format, mFormat.colorspace, mExtent,
														1, vk::ImageUsageFlagBits);

			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(concenrated.size());
			createInfo.pQueueFamilyIndices = concentrated.data();
			
			createInfo.imageSharingMode = mSharingMode;

			createInfo.preTransform = surfaceCapabilities.currentTransform;
		    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

		    createInfo.presentMode = presentMode;

		    createInfo.clipped = VK_TRUE;
		    createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

			mSwapchain = mDevice.get().createSwapchainKHR(createInfo);

			mBuilt = true;
		}

		bool built() const { return mBuilt;}
		vk::SwapchainKHR get() const { return mSwapchain;}
		LogicalDevice device() const { return mDevice;}
		Window window() const { return mWindow;}
		vk::SurfaceKHR surface() const { return mSurface;}
		vk::Extent2D extent() const { return mExtent;}
		vk::SharingMode sharing() const { return mSharingMode;}
		uint32_t imageCount() const { return mImageCount;}
		vk::SurfaceFormatKHR format() const { return mFormat;}
	private:
	    bool mBuilt = false;

		LogicalDevice mDevice = {};
		Window window = {};

	    vk::SwapchainKHR mSwapchain = {};
		vk::SurfaceKHR mSurface = {};
		vk::Extent2D mExtent = {};
		vk::SharingMode mSharingMode = {};
		uint32_t mImageCount = 0;
		vk::SurfaceFormatKHR mFormat = {};
		


	};


}
#endif