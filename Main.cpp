#include "tests/tests.hpp"

#define DEBUG 0

int main()
{
	static auto mainSoulkanTestResult = sk::mainSoulkanTest(DEBUG);
	static sk::SkTestData testData    = sk::retLog(mainSoulkanTestResult);

	std::cout << "Total number of frames elapsed      : " << testData.totalFrames << std::endl;
	std::cout << "Total time elapsed (in seconds)     : " << testData.totalTimeElapsed << std::endl;
	std::cout << "Init time elapsed (in seconds)      : " << testData.initTimeElapsed << std::endl;
	std::cout << "Average number of frames per second : " << testData.averageFramesPerSecond << std::endl;
	std::cout << "Average frametime (in ms)           : " << testData.averageFrametime << std::endl;
	std::cout << "Debug mode is                       : " << (DEBUG ? "ON" : "OFF") << std::endl;

	return 1;
}