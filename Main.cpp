#include "tests/tests.hpp"

#define DEBUG 0

int main()
{
	bool testValid = sk::graphic_test(false);

	std::cout << "Test is " << (testValid ? "valid" : "not valid") << std::endl;

	return EXIT_SUCCESS;
}