/**
 * @file
 * Contains Ghoti.io OS library error code classes.
 */

#ifndef GHOTI_OS_ERRORCODE_HPP
#define GHOTI_OS_ERRORCODE_HPP

#include <string>
#include <system_error>

namespace Ghoti::OS {

/**
 * List of error codes this library may generate.
 */
enum class error_code {
  file_does_not_exist = 1,
  file_exists_at_target_path,
  file_could_not_be_closed,
  file_could_not_be_opened,
};

/**
 * A category to which all error codes from this library will belong.
 */
class error_category : public std::error_category {
  public:

  /**
   * Provide a user-friendly name for this category, useful for when printing
   * error messages.
   *
   * @return The user-friendly name of this category.
   */
  virtual const char * name() const noexcept;

  /**
   * Provide a user-friendly error message from the supplied error code.
   *
   * @param ev The error code.
   * @return A user-friendly error message.
   */
  virtual std::string message(int ev) const;

  /**
   * Determine equivalence of an error condition from the current category
   * against an error code from any other category.
   *
   * @param code An error code from another category.
   * @param condition An error condition from the current category.
   * @return True if the two are equivalent, false otherwise.
   */
  virtual bool equivalent(const std::error_code & code, int condition) const noexcept;
};

/**
 * Return the singleton category object.
 */
const std::error_category & getErrorCategory();

}

/**
 * Create an error code from this library's category.
 *
 * @param e The error code enum value.
 * @return The error code object.
 */
std::error_code make_error_code(Ghoti::OS::error_code e);

/**
 * Create an error condition from this library's category.
 *
 * @param e The error code enum value.
 * @return The error condition object.
 */
std::error_condition make_error_condition(Ghoti::OS::error_code e);

namespace std {
  /**
   * Declare to C++ that the Ghoti::OS::error_code enum is intended to be used
   * as an extension to the std::error_code functionality.
   */
  template<>
  struct is_error_condition_enum<Ghoti::OS::error_code> : public true_type {};

  /**
   * Custom comparitor so that OS::error_code values can be compared directly
   * against std::error_code objects.
   *
   * This is a quality-of-life improvement to make writing Google Test asserts
   * more compact.
   *
   * @param lhs The std::error_code to compare.
   * @param rhs The Ghoti::OS::error_code to compare.
   * @return True if they are equal, false otherwise.
   */
  bool operator==(const std::error_code & lhs, Ghoti::OS::error_code rhs);
}


#endif // GHOTI_OS_ERRORCODE_HPP

