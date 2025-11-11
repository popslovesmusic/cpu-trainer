#pragma once

#include <string>

namespace sur {

class Logger {
 public:
  virtual ~Logger() = default;
  virtual void info(const std::string& message) = 0;
  virtual void warn(const std::string& message) = 0;
  virtual void error(const std::string& message) = 0;
};

Logger& default_logger();

}  // namespace sur

