#include "tests/tests.hpp"

#define DEBUG 0

int main()
{
	bool testValid = sk::graphic_test(true);

	std::cout << "Test is " << (testValid ? "valid" : "not valid") << std::endl;

	std::cout << "Waiting for input" << std::endl;
	std::cin.get();

	return EXIT_SUCCESS;
}