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
	template<class V, class E>
	class Result
	{
	public:
		//Essential enums and constants
		Result(V value, E error)
		{
			mValue = value;
			mError = error;
		}

		V value() { return mValue; }
		E error() { return mError; }

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

	private:
		ErrorCode mCode = ErrorCode::NO_ERROR;
	};

	//Vulkan related classes
	class Instance
	{
	public:
		Instance() {}

		//Instance() {} with params

		void prepare(std::string appName = "Vulkan Application", std::string engineName = "Vulkan Engine",
			         std::vector<const char*> extensions = std::vector<const char*>(),
			         std::vector<const char*> validationLayers = std::vector<const char*>())
		{
			if (extensions.empty())
			{
				if (validationLayers.empty())
				{
					auto getRequiredInstanceExtensionsResult = get_required_instance_extensions(false);
				}
			}
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

			return Result(Window(), Error());
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