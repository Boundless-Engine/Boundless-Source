#include <iostream>
#include <vector>
#include <string>
#include <functional>

// TestFramework Class
class TestFramework {
private:
    std::vector<std::string> failures;
    bool currentTestPassed;
    std::vector<std::pair<std::string, std::function<void()>>> tests;

    TestFramework() : currentTestPassed(true) {}

public:
    static TestFramework& getInstance() {
        static TestFramework instance;
        return instance;
    }

    void registerTest(const std::string& testName, std::function<void()> testFunction) {
        tests.emplace_back(testName, testFunction);
    }

    void reportFailure(const std::string& condition, const std::string& file, int line) {
        failures.push_back("  FAIL: " + condition + " at " + file + ":" + std::to_string(line));
        currentTestPassed = false;
    }

    void runTest(const std::string& testName, std::function<void()> testFunction) {
        std::cout << "Running Test: " << testName << std::endl;
        failures.clear();
        currentTestPassed = true;
        testFunction();

        if (currentTestPassed) {
            std::cout << "  PASS\n";
        }
        else {
            std::cout << "  " << failures.size() << " failures:\n";
            for (const auto& failure : failures) {
                std::cout << failure << std::endl;
            }
        }
    }

    void runAllTests() {
        for (const auto& [testName, testFunction] : tests) {
            runTest(testName, testFunction);
        }
    }

    // Prevent copy/move operations
    TestFramework(const TestFramework&) = delete;
    TestFramework& operator=(const TestFramework&) = delete;
    TestFramework(TestFramework&&) = delete;
    TestFramework& operator=(TestFramework&&) = delete;
};

// Macros for Test Cases
#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

#define TEST_CASE(testName) \
    void CONCAT(testFunction_, __LINE__)(); \
    namespace { \
        struct CONCAT(TestCase_, __LINE__) { \
            CONCAT(TestCase_, __LINE__)() { \
                TestFramework::getInstance().registerTest(testName, CONCAT(testFunction_, __LINE__)); \
            } \
        } CONCAT(instance_, __LINE__); \
    } \
    void CONCAT(testFunction_, __LINE__)()

#define REQUIRE(condition) \
    if (!(condition)) { \
        TestFramework::getInstance().reportFailure(#condition, __FILE__, __LINE__); \
    }
