// Logging
//
// Example:
// LOG(ERROR) << "My int " << int;

#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
#include <vector>

// LOG(ERROR) or LOG(INFO)
#define LOG(stream) stream##_LOG() << "\r" << __FILE__ << ":" << __LINE__ << " "

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RESET "\033[0m"

template <typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& items) {
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

class Logger {
 public:
  ~Logger() {
    *stream_ << COLOR_RESET << "\n";
  }

  template <typename T>
  std::ostream& operator<<(const T& value) {
    return *stream_ << value;
  }

 protected:
  Logger(std::ostream* stream, const std::string& escape_sequence) : stream_(stream) {
    *stream_ << escape_sequence;
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

#endif
