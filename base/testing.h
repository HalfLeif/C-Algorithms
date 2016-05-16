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
if((expr) && Fail()) LOG(ERROR) << "Assertion failed! "

#define ASSERT_TRUE(expr) ASSERT_FALSE(!(expr))

#define ASSERT_BIN_OP(a, b, oper) ASSERT_TRUE( (a) oper (b) ) << \
"Expected " << #a << " " << #oper << " " << #b << " but found " << a << " vs " << b << ". "

#define ASSERT_EQ(a, b) ASSERT_BIN_OP(a, b, ==)
#define ASSERT_NE(a, b) ASSERT_BIN_OP(a, b, !=)
#define ASSERT_LT(a, b) ASSERT_BIN_OP(a, b, <)
#define ASSERT_LE(a, b) ASSERT_BIN_OP(a, b, <=)
#define ASSERT_GT(a, b) ASSERT_BIN_OP(a, b, >)
#define ASSERT_GE(a, b) ASSERT_BIN_OP(a, b, >=)

#define ASSERT_EMPTY(a) ASSERT_TRUE((a).empty()) << "Expected " << #a << " to be empty. "

#define TEST_INTERNAL(classname, superclass) \
class classname##Test : public superclass { \
 public: \
  classname##Test() : superclass(#classname) {} \
  void Run() override; \
}; \
static classname##Test classname##_test_instance_; \
void classname##Test::Run()

#define TEST(classname) TEST_INTERNAL(classname, testing::UnitTest)
#define D_TEST(classname) TEST_INTERNAL(classname, testing::DisabledTest)

namespace testing {

class TestInterface {
 public:
  virtual void Run() = 0;
  virtual std::string name() const = 0;

  bool HasFailed() const { return failed_; }
  bool IsDisabled() const { return disabled_; }

 protected:
  bool Fail() {
    failed_ = true;
    return true;
  }

  bool Disable() {
    disabled_ = true;
    return true;
  }

 private:
  bool failed_ = false;
  bool disabled_ = false;
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
  UnitTest(const std::string& test_name) : test_name_(test_name) {
    TestEnvironment::GetInstance()->RegisterTest(this);
  }

  std::string name() const final { return test_name_; }

 private:
  const std::string test_name_;
};

class DisabledTest : public UnitTest {
 public:
  DisabledTest(const std::string& test_name) : UnitTest(test_name) {
    Disable();
  }
};

}  // namespace testing

#endif
