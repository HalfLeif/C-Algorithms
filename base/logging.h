// Logging
//
// Example:
// LOG(ERROR) << "My int " << int;

#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <set>

// LOG(ERROR) or LOG(INFO)
#define LOG(stream) stream##_LOG() << "\r" << __FILE__ << ":" << __LINE__ << " "

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RESET "\033[0m"

#define CHECK(expr) if(!(expr)) LOG(FATAL)

template <typename Container, typename T>
std::ostream& PrintContainer(std::ostream& stream, const Container& items) {
  stream << "{";
  bool first = true;
  for (const T& item : items) {
    if (first) {
      first = false;
    } else {
      stream << ", ";
    }
    stream << item;
  }
  return stream << "}";
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& items) {
  return PrintContainer<std::vector<T>, T>(stream, items);
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const std::set<T>& items) {
  return PrintContainer<std::set<T>, T>(stream, items);
}

class Logger {
 public:
  virtual ~Logger() {
    Reset();
  }

  template <typename T>
  std::ostream& operator<<(const T& value) {
    return *stream_ << value;
  }

 protected:
  Logger(std::ostream* stream, const std::string& escape_sequence) : stream_(stream) {
    *stream_ << escape_sequence;
  }

  void Reset() {
    *stream_ << COLOR_RESET << "\n";
  }

 private:
  std::ostream* stream_;
};

class ERROR_LOG : public Logger {
 public:
  ERROR_LOG() : Logger(&std::cerr, COLOR_RED){}
};

class INFO_LOG : public Logger {
 public:
  INFO_LOG() : Logger(&std::cout, ""){}
};

class FATAL_LOG : public ERROR_LOG {
 public:
  ~FATAL_LOG() {
    Reset();
    exit(EXIT_FAILURE);
  }
};

#endif
