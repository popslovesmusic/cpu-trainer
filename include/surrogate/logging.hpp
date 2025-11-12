#pragma once

#include <string>

namespace sur {

class Logger {
 public:
  virtual ~Logger() = default;
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
  virtual void info(const std::string& message) = 0;
  virtual void warn(const std::string& message) = 0;
  virtual void error(const std::string& message) = 0;
};

Logger& default_logger();
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
  virtual void on_batch(int epoch,
                        int batch,
                        float loss,
                        float learning_rate,
                        float grad_norm,
                        double milliseconds) = 0;
  virtual void flush() = 0;
};

Logger& default_logger();
void configure_default_logger(int log_interval);
void configure_default_logger(const std::string& csv_path, int log_interval);
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs

}  // namespace sur

