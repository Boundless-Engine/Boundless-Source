/*!
* \file TestFramwork.h
* \brief Unit Test Framwork for testing Functionality within the scope of Boundless.
* \author Austin Lynes
* \date 5/21/2024
*/

#include <iostream>
#include <vector>
#include <string>
#include <functional>

/*! NOTE! ACTUAL USE OF THIS CLASS IS THE WRONG WAY TO USE IT. USE PREDEFINED MACROS BELOW */
namespace Boundless {

	class TestFramework {

	private:
		struct UTEntry {
			std::string name;
			std::function<bool()> fn;
		};

	public:
		static void registerTest(const std::string& testName, std::function<bool()> fn) {
			auto test = UTEntry{
				.name = testName,
				.fn = fn,
			};

			instance.tests.emplace_back(test);
		}

		static int RunUserTests(std::function<int()> fn) {
			return fn();
		}

		static int RunAutomatedTests() {
			std::cout << "Running: " << instance.tests.size() << " tests \n";
			int passed = 0;
		
			for (const auto& t : instance.tests) {
				// run the test.
				bool pass = t.fn(); 
				// if it passes count as pass
				if (pass) passed++;
				// else report as a failure
				else instance.report_failure(t);
			}
			int diff = (int)instance.tests.size() - passed;
			
			std::cout << (diff == 0 ? "\033[38;2;0;255;0m" : "") << passed << " / " << instance.tests.size() << "\033[0m" << "  passed.\n";
			
			return diff;
		}

		void report_failure(const UTEntry& t) {
			std::cout << "\033[38;2;255;0;0mFailed!\033[0;0;0m\n"
				<< "Name: " << t.name;
		}

	private:
		TestFramework() {};

		std::vector<UTEntry> tests;
		// Prevent copy/move operations
		TestFramework(const TestFramework&) = delete;
		TestFramework& operator=(const TestFramework&) = delete;
		TestFramework(TestFramework&&) = delete;
		TestFramework& operator=(TestFramework&&) = delete;

		static TestFramework instance;
	};
}

// Macros for Test Cases
#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

#define TEST_CASE(testName, fn) \
    bool CONCAT(testFunction_, __LINE__)(); \
    namespace { \
        struct CONCAT(TestCase_, __LINE__) { \
            CONCAT(TestCase_, __LINE__)() { \
				Boundless::TestFramework::registerTest(testName, fn); \
            } \
        } CONCAT(instance_, __LINE__); \
    } \
