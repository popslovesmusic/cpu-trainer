#include "surrogate/logging.hpp"

<<<<<<< ours
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

=======
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <system_error>

namespace sur {

namespace {

class CsvLogger : public Logger {
 public:
  CsvLogger() { configure(default_path(), 1); }

  void configure(const std::string& path, int log_interval) {
    std::lock_guard<std::mutex> guard(mutex_);
    stream_.close();
    path_ = path.empty() ? default_path() : path;
    log_interval_ = log_interval;
    header_written_ = false;
    batch_count_ = 0;
  }

  void on_batch(int epoch,
                int batch,
                float loss,
                float learning_rate,
                float grad_norm,
                double milliseconds) override {
    std::lock_guard<std::mutex> guard(mutex_);
    ensure_stream();
    write_header_if_needed();

    stream_ << epoch << ',' << batch << ',' << std::setprecision(6) << std::fixed << loss << ','
            << learning_rate << ',' << grad_norm << ',' << milliseconds << '\n';

    ++batch_count_;
    if (log_interval_ > 0 && (batch_count_ % log_interval_ == 0)) {
      std::cout << "epoch=" << epoch << " batch=" << batch << " loss=" << loss
                << " lr=" << learning_rate << " grad_norm=" << grad_norm
                << " time_ms=" << milliseconds << std::endl;
    }
  }

  void flush() override {
    std::lock_guard<std::mutex> guard(mutex_);
    if (stream_.is_open()) {
      stream_.flush();
    }
  }

 private:
  static std::string default_path() { return "logs/train_metrics.csv"; }

  void ensure_stream() {
    if (stream_.is_open()) {
      return;
    }

    const std::filesystem::path file_path(path_);
    const auto parent = file_path.parent_path();
    if (!parent.empty()) {
      std::error_code ec;
      std::filesystem::create_directories(parent, ec);
    }

    stream_.open(file_path, std::ios::out | std::ios::trunc);
  }

  void write_header_if_needed() {
    if (header_written_) {
      return;
    }
    stream_ << "epoch,batch,loss,lr,grad_norm,ms" << '\n';
    header_written_ = true;
  }

  std::mutex mutex_;
  std::string path_;
  std::ofstream stream_;
  bool header_written_ = false;
  std::size_t batch_count_ = 0;
  int log_interval_ = 1;
};

CsvLogger& get_default_logger() {
  static CsvLogger logger;
  return logger;
}

}  // namespace

Logger& default_logger() { return get_default_logger(); }

void configure_default_logger(int log_interval) {
  get_default_logger().configure("", log_interval);
}

void configure_default_logger(const std::string& csv_path, int log_interval) {
  get_default_logger().configure(csv_path, log_interval);
}

>>>>>>> theirs
}  // namespace sur

