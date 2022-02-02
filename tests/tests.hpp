#ifndef SOULKAN_TESTS_HPP
#define SOULKAN_TESTS_HPP
#pragma once

#include <iostream>
#include "../Soulkan.hpp"


namespace SOULKAN_NAMESPACE
{
	//TODO:Garbage for now, plz fix
	template<class V, class E>
	inline bool valid(Result<V, E> result, bool current)
	{
		return result.is_error() ? false : current;
	}

	inline bool graphic_test(bool interactive)
	{
		bool testValid = true;

		FunctionQueue deletionQueue = {};

		/*GLFW*/
		//TODO:Bundle glfwInit and glfwWindowHint later
		if (!glfwInit()) { testValid = false; };
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		deletionQueue.push([=]() { glfwTerminate(); });


		auto builtMainWindow = Window("Soulkan graphic test", 800, 600, true).build();
		testValid = valid(builtMainWindow, testValid);
		auto mainWindow = builtMainWindow.value();

		std::cout << "Built Main Window with title = \"" << mainWindow.title() << "\", height = \"" << mainWindow.height() << "\", width = \"" << mainWindow.width() << "\"" << std::endl;

		/*INSTANCE*/
		auto builtInstance = Instance(true, "Soulkan Test", "Soulkan Engine").build();
		testValid = valid(builtInstance, testValid);
		auto instance = builtInstance.value();
		deletionQueue.push([=]() { instance.cleanup(); });

		std::cout << "Built Instance with validation = \"" << instance.validation() << "\", extensions = \"" << instance.extensions().size() << "\"" << std::endl;

		/*PHYSICALDEVICE*/
		auto builtPhysicalDevice = PhysicalDevice(instance).build();
		testValid = valid(builtPhysicalDevice, testValid);
		auto physicalDevice = builtPhysicalDevice.value();

		std::cout << "Built Physical Device with name = \"" << physicalDevice.name() << "\"" << std::endl;

		/*LOGICALDEVICE*/
		std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		auto surfaceResult = instance.create_surface(mainWindow);
		testValid = valid(surfaceResult, testValid);
		vk::SurfaceKHR surface = surfaceResult.value();
		deletionQueue.push([=]() { instance.get().destroySurfaceKHR(surface); });

		auto builtLogicalDevice = LogicalDevice(physicalDevice, deviceExtensions, surface, {}).build();
		testValid = valid(builtLogicalDevice, testValid);
		auto logicalDevice = builtLogicalDevice.value();
		deletionQueue.push([=]() { logicalDevice.cleanup(); });

		std::cout << "Built Logical Device" << std::endl;

		/*SWAPCHAIN*/
		auto builtSwapchain = Swapchain(logicalDevice, surface, mainWindow).build();
		testValid = valid(builtSwapchain, testValid);
		Swapchain swapchain = builtSwapchain.value();
		deletionQueue.push([=]() { swapchain.cleanup(); });
		
		std::cout << "Built Swapchain" << std::endl;
		
		if (interactive)
		{
			uint32_t i = 0;
			while (!glfwWindowShouldClose(mainWindow.ptr()))
			{
				glfwPollEvents();
				mainWindow.rename("Soulkan graphic test [" + std::to_string(i) + "]");
				i++;
			}

			glfwDestroyWindow(mainWindow.ptr());
		}

		deletionQueue.rflush();

		return testValid;
	}
}
#endif