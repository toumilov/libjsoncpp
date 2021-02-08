
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <limits>

int main( int argc, char** argv)
{
	MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
	return CommandLineTestRunner::RunAllTests( argc, argv );
}
