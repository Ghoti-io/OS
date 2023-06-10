/**
 * @file
 * Declare the Tang::Error class used to describe syntax and runtime errors.
 */

#ifndef GHOTI_OS_FILE_HPP
#define GHOTI_OS_FILE_HPP

#include <string>
#include <fstream>

namespace Ghoti::OS {
/**
 * Represents a file on the file system.
 */
class File {
  public:
  /**
   * Default constructor.
   */
  File();

  /**
   * Constructor for a named path.
   *
   * @param path The file to open.
   */
  File(const std::string & path);

  /**
   * Copy constructor is deleted.
   */
  File(const File &) = delete;

  /**
   * Move constructor.
   *
   * @param source The source object.
   */
  File(File && source);

  /**
   * Copy assignment operator deleted.
   */
  File & operator=(const File &) = delete;

  /**
   * Move assignment operator.
   */
  File & operator=(File &&);

  /**
   * Destructor.  Will delete the file if it is a temp file.
   */
  ~File();

  /**
   * Open a file for reading or writing.
   *
   * @param mode The mode in which to open the file, taken from
   *   <a href="https://en.cppreference.com/w/cpp/io/c/fopen">std::fopen</a>.
   * @return Whether or not the file open succeded.
   */
  bool open(const char * mode);

  /**
   * Close the file.
   */
  bool close();

  /**
   * Create a temporary file in the OS temp directory, matching the supplied
   * pattern.  Characters will be added to the end if the name as needed to
   * avoid file conflicts.
   *
   * @param pattern The file name pattern that should be used.
   * @return A `File` object.
   */
  static File createTemp(const std::string & pattern);

  /**
   * Return the contents of the file as a string.
   */
  operator std::string();

  /**
   * Get the current path.
   */
  const std::string & getPath() const;

  /**
   * The file stream.
   */
  std::fstream file;

  private:
  std::string path;
  std::string mode;
  bool isRead;
  bool isWrite;
  bool isTemp;
};
}

#endif // GHOTI_OS_FILE_HPP

