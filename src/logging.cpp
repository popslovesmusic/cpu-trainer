#include "surrogate/logging.hpp"

namespace sur {

namespace {
class NullLogger : public Logger {
 public:
  void info(const std::string&) override {}
  void warn(const std::string&) override {}
  void error(const std::string&) override {}
};
}  // namespace

Logger& default_logger() {
  static NullLogger logger;
  return logger;
}

}  // namespace sur

