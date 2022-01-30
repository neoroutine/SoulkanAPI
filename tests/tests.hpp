#ifndef SOULKAN_TESTS_HPP
#define SOULKAN_TESTS_HPP
#pragma once

#include <iostream>
#include "../Soulkan.hpp"


namespace SOULKAN_NAMESPACE
{
	template<class V, class E>
	inline bool valid(Result<V, E> result)
	{
		return result.is_error();
	}

	inline bool graphic_test(bool interactive)
	{
		bool testValid = true;

		/*GLFW*/
		glfwInit();
		auto builtMainWindow = Window("Soulkan graphic test", 800, 600, true).build();
		testValid = valid(builtMainWindow);
		auto mainWindow = builtMainWindow.value();

		std::cout << "Built Main Window with title = \"" << mainWindow.title() << "\", height = \"" << mainWindow.height() << "\", width = \"" << mainWindow.width() << "\"" << std::endl;

		/*INSTANCE*/
		auto builtInstance = Instance(true, "Soulkan Test", "Soulkan Engine").build();
		testValid = valid(builtInstance) ? testValid : false;
		auto instance = builtInstance.value();

		std::cout << "Built Instance with validation = \"" << instance.validation() << "\", extensions = \"" << instance.extensions().size() << "\"" << std::endl;

		/*PHYSICALDEVICE*/
		auto builtPhysicalDevice = PhysicalDevice(instance).build();
		testValid = valid(builtPhysicalDevice);
		auto physicalDevice = builtPhysicalDevice.value();

		std::cout << "Built Physical Device with name = \"" << physicalDevice.name() << "\"" << std::endl;

		/*LOGICALDEVICE*/
		std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		auto testSurfaceResult = instance.create_surface(mainWindow);
		testValid = valid(testSurfaceResult);
		auto testSurface = testSurfaceResult.value();

		auto builtLogicalDevice = LogicalDevice(physicalDevice, deviceExtensions, {}).build();
		testValid = valid(builtLogicalDevice);
		auto logicalDevice = builtPhysicalDevice.value();

		std::cout << "Built Logical Device" << std::endl;
		
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

			std::cout << "Waiting for input" << std::endl;
			std::cin.get();
		}
		
		glfwTerminate();

		
		instance.cleanup();


		return testValid;
	}
}
#endif