#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <rstore/utils/logger.h>
#include <sstream>

class UnitTestLogger final : public rstore::utils::logging::AbstractLogger {
public:
  static bool verbose;
  bool _write_to_file;
  std::unique_ptr<std::ofstream> _output;
  UnitTestLogger(bool write_to_file = true)
      : _write_to_file(write_to_file) {
    if (_write_to_file) {

      std::stringstream fname_ss;
      fname_ss << "rstore_unittests";
      fname_ss << ".log";

      auto logname = fname_ss.str();
      if (verbose) {
        std::cout << "See output in " << logname << std::endl;
      }
      _output = std::make_unique<std::ofstream>(logname, std::ofstream::app);
      (*_output) << "Start programm" << std::endl;
    }
  }
  ~UnitTestLogger() {}

  void message(rstore::utils::logging::MESSAGE_KIND kind,
               const std::string &msg) noexcept {
    std::lock_guard lg(_locker);

    std::stringstream ss;
    switch (kind) {
    case rstore::utils::logging::MESSAGE_KIND::fatal:
      ss << "[err] " << msg << std::endl;
      break;
    case rstore::utils::logging::MESSAGE_KIND::info:
      ss << "[inf] " << msg << std::endl;
      break;
    case rstore::utils::logging::MESSAGE_KIND::warn:
      ss << "[wrn] " << msg << std::endl;
      break;
    case rstore::utils::logging::MESSAGE_KIND::message:
      ss << "[dbg] " << msg << std::endl;
      break;
    }

    if (_write_to_file) {
      (*_output) << ss.str();
      _output->flush();
      _output->flush();
    }

    if (kind == rstore::utils::logging::MESSAGE_KIND::fatal) {
      std::cerr << ss.str();
    } else {
      if (verbose) {
        std::cout << ss.str();
      } else {
        if (!_write_to_file) {
          _messages.push_back(ss.str());
        }
      }
    }
  }

  void dump_all() {
    for (auto m : _messages) {
      std::cerr << m;
    }
  }

private:
  std::mutex _locker;
  std::list<std::string> _messages;
};

bool UnitTestLogger::verbose = false;

struct LoggerControl : Catch::TestEventListenerBase {
  using TestEventListenerBase::TestEventListenerBase; // inherit constructor

  virtual void testCaseStarting(Catch::TestCaseInfo const &) override {
    _raw_ptr = new UnitTestLogger();
    _logger = rstore::utils::logging::AbstractLoggerPtr{_raw_ptr};
    rstore::utils::logging::LoggerManager::start(_logger);
  }

  virtual void testCaseEnded(Catch::TestCaseStats const &testCaseStats) override {
    if (testCaseStats.testInfo.expectedToFail()) {
      _raw_ptr->dump_all();
    }
    rstore::utils::logging::LoggerManager::stop();
    _logger = nullptr;
  }
  UnitTestLogger *_raw_ptr;
  rstore::utils::logging::AbstractLoggerPtr _logger;
};

CATCH_REGISTER_LISTENER(LoggerControl);

std::tuple<int, char **> init_logger(int argc, char **argv) {
  int _argc = argc;
  char **_argv = argv;
  for (int i = 0; i < argc; ++i) {
    if (std::strcmp(argv[i], "--verbose") == 0) {
      UnitTestLogger::verbose = true;
      _argc--;
      _argv = new char *[_argc];
      int r_pos = 0, w_pos = 0;
      for (int a = 0; a < argc; ++a) {
        if (a != i) {

          _argv[w_pos] = argv[r_pos];
          w_pos++;
        }
        r_pos++;
      }
      break;
    }
  }
  return std::tuple(_argc, _argv);
}

int main(int argc, char **argv) {
  size_t runs = 1;
  auto [_argc, _argv] = init_logger(argc, argv);

  Catch::Session sesssion;
  sesssion.configData().showDurations = Catch::ShowDurations::OrNot::Always;
  int result = 0;
  for (size_t i = 0; i < runs; ++i) {
    result = sesssion.run(_argc, _argv);

    if (runs > 1) {
      std::cout << "result: " << result << std::endl;
      std::cout << "iteration: " << i << std::endl;
    }
    if (result != 0) {
      break;
    }
  }
  if (UnitTestLogger::verbose) {
    delete[] _argv;
  }
  return (result < 0xff ? result : 0xff);
}
