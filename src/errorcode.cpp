/**
 * @file
 */

#include "os/errorcode.hpp"

using namespace std;
using namespace Ghoti;
using namespace Ghoti::OS;

// See http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-5.html

const char * OS::ErrorCategory::name() const noexcept {
  return "Ghoti.io OS";
}

string OS::ErrorCategory::message(int ev) const {
  switch (ev) {
    case static_cast<int>(ErrorCode::NO_FILE_PATH_SPECIFIED):
      return "No file path specified";
    case static_cast<int>(ErrorCode::FILE_DOES_NOT_EXIST):
      return "File does not exist";
    case static_cast<int>(ErrorCode::FILE_EXISTS_AT_TARGET_PATH):
      return "File exists at target path";
    case static_cast<int>(ErrorCode::FILE_COULD_NOT_BE_CLOSED):
      return "File could not be closed";
    case static_cast<int>(ErrorCode::FILE_COULD_NOT_BE_OPENED):
      return "File could not be opened";
    case static_cast<int>(ErrorCode::ERROR_WRITING_TO_FILE):
      return "Error writing to file";
  }
  return "";
}

bool OS::ErrorCategory::equivalent([[maybe_unused]] const std::error_code & code,[[maybe_unused]]  int condition) const noexcept {
  return false;
}

const std::error_category & OS::getErrorCategory() {
  static OS::ErrorCategory instance{};
  return instance;
}

std::error_code make_error_code(OS::ErrorCode e) {
  return std::error_code(static_cast<int>(e), OS::getErrorCategory());
}

std::error_condition make_error_condition(OS::ErrorCode e) {
  return std::error_condition(static_cast<int>(e), OS::getErrorCategory());
}

bool std::operator==(const std::error_code & lhs, Ghoti::OS::ErrorCode rhs) {
  return lhs == ::make_error_code(rhs);
}

