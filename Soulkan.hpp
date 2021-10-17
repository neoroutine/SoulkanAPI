/*Include guards to avoid including this .hpp file multiple times*/
#ifndef SOULKAN_HPP
#define SOULKAN_HPP
#pragma once

/*Defining namespaces as macros so that the user can change them*/
#define SOULKAN_NAMESPACE sk



/*Includes from the std library or from needed tools*/
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_VULKAN

namespace SOULKAN_NAMESPACE
{
	//Essential enums and types
	enum class ErrorCode : uint32_t
	{
		NO_ERROR                = 1,
		GENERAL_HARDWARE_ERROR  = 2,
		GENERAL_PARAMETER_ERROR = 4,
		GENERAL_VULKAN_ERROR    = 8,
		GENERAL_GLFW_ERROR      = 16,
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
			    case ErrorCode::NO_ERROR:                return "NO_ERROR"; break;
				case ErrorCode::GENERAL_HARDWARE_ERROR:  return "GENERAL_HARDWARE_ERROR"; break;
				case ErrorCode::GENERAL_PARAMETER_ERROR: return "GENERAL_PARAMETER_ERROR"; break;
				case ErrorCode::GENERAL_VULKAN_ERROR:    return "GENERAL_VULKAN_ERROR"; break;
				case ErrorCode::GENERAL_GLFW_ERROR:      return "GENERAL_GLFW_ERROR"; break;
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
			return (lhs.instance() == rhs.instance());
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

		Result<Instance, Error> build()
		{
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
			}

			std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

			auto instanceCreateInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), 
				                                             &mAppInfo,
				                                             static_cast<uint32_t>(validationLayers.size()),
				                                             validationLayers.data(),
				                                             static_cast<uint32_t>(mExtensions.size()),
				                                             mExtensions.data());

			mInstance = vk::createInstance(instanceCreateInfo);

			return Result(*this, Error());
		}

		void cleanup()
		{
			mInstance.destroy();
		}
		

		vk::Instance instance() const { return mInstance; }
		vk::ApplicationInfo appInfo() const { return mAppInfo; }
		bool validation() const { return mValidationEnabled; }
		std::vector<const char*> extensions() const { return mExtensions; }
	private:
		vk::Instance mInstance = {};
		vk::ApplicationInfo mAppInfo = {};

		bool mValidationEnabled = false;

		std::vector<const char*> mExtensions = {};
	};

	inline Result<std::vector<vk::PhysicalDevice>, Error> get_available_physical_devices(const vk::Instance instance)
	{
		if (instance == vk::Instance())
		{
			return Result(std::vector<vk::PhysicalDevice>(), Error(ErrorCode::GENERAL_PARAMETER_ERROR));
		}
		auto availablePhysicalDevices = instance.enumeratePhysicalDevices();

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

	inline Result<std::vector<vk::PhysicalDevice>, Error> get_suitable_physical_devices(const vk::Instance instance)
	{
		auto availablePhysicalDevicesResult = get_available_physical_devices(instance);
		if (availablePhysicalDevicesResult.is_error())
		{
			return Result(std::vector<vk::PhysicalDevice>(), Error(availablePhysicalDevicesResult.error().code()));
		}

		auto availablePhysicalDevices = availablePhysicalDevicesResult.value();
		auto suitablePhysicalDevices  = std::vector<vk::PhysicalDevice>();

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

	inline Result<vk::PhysicalDevice, Error> get_best_physical_device(const vk::Instance instance, bool discrete)
	{
		auto suitablePhysicalDevicesResult = get_suitable_physical_devices(instance);
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
			if (mInstance == Instance())
			{
				return Result(*this, Error(ErrorCode::GENERAL_PARAMETER_ERROR));
			}

			auto getBestPhysicalDeviceResult = get_best_physical_device(mInstance.instance(), mDiscreteWish);

			if (getBestPhysicalDeviceResult.is_error())
			{
				return Result(*this, Error(getBestPhysicalDeviceResult.error().code()));
			}

			mDevice = getBestPhysicalDeviceResult.value();
			auto physicalDeviceProperties = mDevice.getProperties();
			mName   = std::string(physicalDeviceProperties.deviceName.data());
			mType   = physicalDeviceProperties.deviceType;


			return Result(*this, Error());
		}

		void cleanup()
		{}

		Instance instance() const { return mInstance; }
		vk::PhysicalDevice device() const { return mDevice; }
		std::string name() const { return mName; }
		vk::PhysicalDeviceType type() const { return mType; }
	private:
		Instance mInstance = {};
		vk::PhysicalDevice mDevice = {};

		std::string mName = "";
		vk::PhysicalDeviceType mType = {};

		bool mDiscreteWish = true;
	};

	class LogicalDevice
	{
		LogicalDevice() {}

		void prepare() {}

		Result<LogicalDevice, Error> build() {}

		void cleanup() {}

		private:
			PhysicalDevice mPhysicalDevice = {};
			vk::Device mDevice = {};
	};

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

		std::string title() const { return mTitle; }
		uint32_t height() const { return mHeight; }
		uint32_t width() const { return mWidth; }
		bool resizable() const { return mResizable; }
		GLFWwindow* ptr() const { return mPtr; }

	private:
		std::string mTitle = "";
		uint32_t mHeight   = 0;
		uint32_t mWidth    = 0;
		bool mResizable    = false;
		GLFWwindow* mPtr   = nullptr;
	};


}
#endif