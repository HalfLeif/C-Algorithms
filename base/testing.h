#ifndef TESTING_H
#define TESTING_H

// Unit tests
//
// Example:
//
// TEST(integer_test) {
//   ASSERT_TRUE(5 > 3) << "Hmm...";
// }

#include <iostream>
#include <string>
#include <vector>

#include "logging.h"


#define ASSERT_FALSE(expr) \
if(expr && Fail()) LOG(ERROR) << "Assertion failed! "

#define ASSERT_TRUE(expr) ASSERT_FALSE(!expr)

#define TEST(classname) \
class classname##Test : public testing::UnitTest { \
 public: \
  classname##Test() : testing::UnitTest(#classname) {} \
  void Run() override; \
}; \
static classname##Test classname##_test_instance_; \
void classname##Test::Run()

namespace testing {

class TestInterface {
 public:
  virtual void Run() = 0;
  virtual std::string name() const = 0;

  bool HasFailed() const { return failed_; }

 protected:
  bool Fail() {
    failed_ = true;
    return true;
  }

 private:
  bool failed_ = false;
};

// Singleton that gathers the tests and runs them in a fashionable order.
class TestEnvironment {
 public:
  // Not threadsafe!
  static TestEnvironment* GetInstance();

  void RegisterTest(TestInterface* test_instance) {
    if (test_instance != nullptr) {
      tests_.push_back(test_instance);
    }
  }

  void RunAllTests();

 private:
  TestEnvironment() {}

  std::vector<TestInterface*> tests_;
};

class UnitTest : public TestInterface {
 public:
  std::string name() const final { return test_name_; }

 //protected:
  UnitTest(const std::string& test_name) : test_name_(test_name) {
    TestEnvironment::GetInstance()->RegisterTest(this);
  }

 private:
  const std::string test_name_;
};

}  // namespace testing

#endif
