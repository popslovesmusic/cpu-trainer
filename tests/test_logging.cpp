#include <cassert>
#include <fstream>
#include <string>

#include "surrogate/logging.hpp"

int main() {
  const std::string path = "logs/test_logging_metrics.csv";
  sur::configure_default_logger(path, 2);
  sur::Logger& logger = sur::default_logger();

  logger.on_batch(0, 0, 1.0f, 0.1f, 0.5f, 2.5);
  logger.on_batch(0, 1, 0.9f, 0.1f, 0.4f, 2.2);
  logger.flush();

  std::ifstream file(path);
  assert(file.good());

  std::string line;
  std::getline(file, line);
  assert(line == "epoch,batch,loss,lr,grad_norm,ms");

  std::getline(file, line);
  assert(line.rfind("0,0", 0) == 0);

  std::getline(file, line);
  assert(line.rfind("0,1", 0) == 0);

  return 0;
}
