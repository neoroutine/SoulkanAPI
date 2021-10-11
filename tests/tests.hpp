#ifndef SOULKAN_TESTS_HPP
#define SOULKAN_TESTS_HPP
#pragma once

#include <iostream>

#include "../Soulkan.hpp"

namespace SOULKAN_NAMESPACE
{
	inline bool graphic_test()
	{
		bool testValid = true;

		auto builtMainWindow = Window("Soulkan graphic test", 800, 600).build();
		if (builtMainWindow.is_error()) { testValid = false; }
		auto mainWindow = builtMainWindow.value();

		std::cout << "Created main window with title = \"" << mainWindow.title() << "\", height = \"" << mainWindow.height() << "\", width = \"" << mainWindow.width() << "\"" << std::endl;

		return testValid;
	}
}
#endif