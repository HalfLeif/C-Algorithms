
#include "testing.h"

#include <iostream>

#include "logging.h"


namespace testing {
namespace {
static TestEnvironment* test_environment_instance = nullptr;
}  // namespace

// static
TestEnvironment* TestEnvironment::GetInstance() {
  if (test_environment_instance == nullptr) {
    test_environment_instance = new TestEnvironment();
  }
  return test_environment_instance;
}

void TestEnvironment::RunAllTests() {
  int num_success = 0;
  int num_tests = tests_.size();

  std::cerr << "Running " << num_tests << " tests...\n";

  for (TestInterface* test : tests_) {
    if (test->IsDisabled()) {
      --num_tests;
      std::cerr << COLOR_RED << test->name() << " is disabled!" << COLOR_RESET << "\n";
      continue;
    }

    std::cerr << test->name() << " started";
    test->Run();
    if(test->HasFailed()) {
      std::cerr << "\r" << COLOR_RED << test->name() << " failed!" << COLOR_RESET << "\n";
    } else {
      ++num_success;
      std::cerr << "\r" << COLOR_GREEN << test->name() << " succeded." << COLOR_RESET << "\n";
    }
  }
  std::cerr << "\n\t" << num_success << " out of " << num_tests << " passed.\n\n";
}

}  // namespace testing

// TestingMain
int main() {
  testing::TestEnvironment::GetInstance()->RunAllTests();
}

