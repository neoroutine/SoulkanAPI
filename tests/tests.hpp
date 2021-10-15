#ifndef SOULKAN_TESTS_HPP
#define SOULKAN_TESTS_HPP
#pragma once

#include <iostream>

#include "../Soulkan.hpp"

namespace SOULKAN_NAMESPACE
{
	inline bool graphic_test(bool interactive)
	{
		bool testValid = true;

		glfwInit();

		auto builtMainWindow = Window("Soulkan graphic test", 800, 600, true).build();
		if (builtMainWindow.is_error()) { testValid = false; }
		auto mainWindow = builtMainWindow.value();

		std::cout << "Created main window with title = \"" << mainWindow.title() << "\", height = \"" << mainWindow.height() << "\", width = \"" << mainWindow.width() << "\"" << std::endl;

		auto builtInstance = Instance(true, "Soulkan Test", "Soulkan Engine").build();
		if (builtInstance.is_error()) { testValid = false; }
		auto instance = builtInstance.value();

		std::cout << "Created instance with validation = \"" << instance.validation() << "\", extensions = \"" << instance.extensions().size() << "\"" << std::endl;
		
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