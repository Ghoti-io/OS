/**
 * @file
 */

#include "os/errorcode.hpp"

using namespace std;
using namespace Ghoti;
using namespace Ghoti::OS;

// See http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-5.html

const char * OS::error_category::name() const noexcept {
  return "Ghoti.io OS";
}

string OS::error_category::message(int ev) const {
  switch (ev) {
    case static_cast<int>(OS::error_code::file_does_not_exist):
      return "File does not exist";
    case static_cast<int>(OS::error_code::file_exists_at_target_path):
      return "File exists at target path";
  }
  return "";
}

bool OS::error_category::equivalent([[maybe_unused]] const std::error_code & code,[[maybe_unused]]  int condition) const noexcept {
  return false;
}

const std::error_category & OS::getErrorCategory() {
  static OS::error_category instance{};
  return instance;
}

std::error_code make_error_code(OS::error_code e) {
  return std::error_code(static_cast<int>(e), OS::getErrorCategory());
}

std::error_condition make_error_condition(OS::error_code e) {
  return std::error_condition(static_cast<int>(e), OS::getErrorCategory());
}

