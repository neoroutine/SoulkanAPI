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
		GENERAL_PARAMETER_ERROR = 2,
		GENERAL_VULKAN_ERROR    = 4,
		GENERAL_GLFW_ERROR      = 8
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
		bool is_error() { return (mError.code() == ErrorCode::NO_ERROR); }

	private:
		V mValue = {};
		E mError = {};
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
		bool is_error() { return (mCode == ErrorCode::NO_ERROR); }

	private:
		ErrorCode mCode = ErrorCode::NO_ERROR;
	};

	//Vulkan related classes
	class Instance
	{
	public:
		Instance() {}

		//Instance() {} with params

		Result<Instance, Error> prepare(bool validationEnabled, std::string appName, std::string engineName = "Vulkan Engine",
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
					return Result(Instance(), Error(getRequiredInstanceExtensionsResult.error().code()));
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
		

		Result<std::vector<const char*>, Error> get_required_instance_extensions(bool validationEnabled)
		{
			uint32_t glfwExtensionCount  = 0;
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
	private:
		vk::Instance mInstance = {};
		vk::ApplicationInfo mAppInfo = {};

		bool mValidationEnabled = false;

		std::vector<const char*> mExtensions = {};
	};

	//GLFW related classes
	class Window
	{
	public:
		Window(){}

		Window(std::string title, uint32_t height, uint32_t width)
		{
			prepare(title, height, width);
		}

		void prepare(std::string title, uint32_t height, uint32_t width)
		{
			mTitle = title;
			mHeight = height;
			mWidth = width;
		}

		Result<Window, Error> build()
		{
			if (mTitle == "" || mHeight == 0 || mWidth == 0)
			{
				return Result(Window(), Error(ErrorCode::GENERAL_PARAMETER_ERROR));
			}

			//VkSurfaceKHR rawSurface;
			//VkResult result = glfwCreateWindowSurface()

			return Result(*this, Error());
		}

		std::string title() const { return mTitle; }
		uint32_t height() const { return mHeight; }
		uint32_t width() const { return mWidth; }
		GLFWwindow* ptr() const { return mPtr; }

	private:
		std::string mTitle = "";
		uint32_t mHeight   = 0;
		uint32_t mWidth    = 0;
		GLFWwindow* mPtr = nullptr;
	};

	//Vulkan related classes

}
#endif