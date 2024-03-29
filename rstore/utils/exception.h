#pragma once
#include <rstore/exports.h>
#include <rstore/utils/logger.h>
#include <rstore/utils/strings.h>
#include <stdexcept>
#include <string>

#define CODE_POS (rstore::utils::exceptions::CodePosition(__FILE__, __LINE__, __FUNCTION__))

#define MAKE_EXCEPTION(msg)                                                              \
  rstore::utils::exceptions::Exception::create_and_log(CODE_POS, msg)
// macros, because need CODE_POS

#define THROW_EXCEPTION(...)                                                             \
  throw rstore::utils::exceptions::Exception::create_and_log(CODE_POS, __VA_ARGS__)

namespace rstore::utils::exceptions {

struct CodePosition {
  const char *_file;
  const int _line;
  const char *_func;

  CodePosition(const char *file, int line, const char *function)
      : _file(file)
      , _line(line)
      , _func(function) {}

  [[nodiscard]] std::string to_string() const {
    auto ss = std::string(_file) + " line: " + std::to_string(_line)
              + " function: " + std::string(_func) + "\n";
    return ss;
  }
  CodePosition &operator=(const CodePosition &) = delete;
};

class Exception final : public std::exception {
public:
  template <typename... T>
  [[nodiscard]] static Exception create_and_log(const CodePosition &pos, T... message) {

    auto expanded_message = utils::strings::to_string(
        std::string("FATAL ERROR. The Exception will be thrown! "),
        pos.to_string() + " Message: ",
        message...);
    return Exception(expanded_message);
  }

public:
  [[nodiscard]] const char *what() const noexcept override { return _msg.c_str(); }
  [[nodiscard]] const std::string &message() const { return _msg; }

  EXPORT Exception();
  EXPORT Exception(const char *&message);
  EXPORT Exception(const std::string &message);

protected:
  void init_msg(const std::string &msg_);

private:
  std::string _msg;
};
} // namespace rstore::utils::exceptions

#define NOT_IMPLEMENTED THROW_EXCEPTION("Not implemented");

#ifdef DOUBLE_CHECKS
#define ENSURE_MSG(A, E)                                                                 \
  if (!(A)) {                                                                            \
    THROW_EXCEPTION(E);                                                                  \
  }
#define ENSURE(A) ENSURE_MSG(A, #A)
#else
#define ENSURE_MSG(A, E)
#define ENSURE(A)
#endif
