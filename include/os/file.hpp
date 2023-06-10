/**
 * @file
 * Declare the Tang::Error class used to describe syntax and runtime errors.
 */

#ifndef GHOTI_OS_FILE_HPP
#define GHOTI_OS_FILE_HPP

#include <string>

namespace Ghoti::OS {
/**
 * Represents a file on the file system.
 */
class File {
  public:
  /**
   * Constructor for a named path.
   *
   * @param path The file to open.
   * @param mode The mode in which to open the file, taken from
   *   <a href="https://en.cppreference.com/w/cpp/io/c/fopen">std::fopen</a>.
   */
  File(std::string path, const char * mode);

  private:
  /**
   * Default constructor, private.
   */
  File();

  std::string path;
  std::string mode;
};
}

#endif // GHOTI_OS_FILE_HPP

