/**
 * @file
 * Contains Ghoti.io OS library error code classes.
 */

#ifndef GHOTI_OS_ERRORCODE_HPP
#define GHOTI_OS_ERRORCODE_HPP

#include <string>
#include <system_error>

namespace Ghoti::OS {

enum class error_code {
  file_does_not_exist = 1
};

class error_category : public std::error_category {
  public:
  virtual const char * name() const noexcept;
  virtual std::string message(int ev) const;
  virtual bool equivalent(const std::error_code & code, int condition) const noexcept;
};

const std::error_category & getErrorCategory();

}

std::error_code make_error_code(Ghoti::OS::error_code e);
std::error_condition make_error_condition(Ghoti::OS::error_code e);

namespace std {
  template<>
  struct is_error_condition_enum<Ghoti::OS::error_code> : public true_type {};
}

#endif // GHOTI_OS_ERRORCODE_HPP

