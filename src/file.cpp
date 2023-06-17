/**
 * @file
 */

#include "os/file.hpp"
#include "os/errorcode.hpp"
#include <cstdlib>    // for mkstemp()
#include <filesystem> // for filesystem::
#include <iostream>
#include <sstream>
#include <unistd.h>   // for ::close()

using namespace std;
using namespace Ghoti::OS;

File::File() : file{}, path{}, lastError{}, isTemp{false} {
  this->lastError = make_error_code(ErrorCode::NO_FILE_PATH_SPECIFIED);
}

File::File(const string & path) : file{}, path{path}, lastError{}, isTemp{false} {
  // Verify that the file exists.
  if (!filesystem::exists(this->path, this->lastError)) {
    this->lastError = make_error_code(ErrorCode::FILE_DOES_NOT_EXIST);
  }
}

File::File(File && source) {
  this->file = move(source.file);
  this->path = move(source.path);
  this->lastError = move(source.lastError);
  this->isTemp = source.isTemp;
  // This is a transfer of ownership, so the source must no longer identify
  // the target file as a temp file.
  source.isTemp = false;
}

File & File::operator=(File && source) {
  swap(this->file, source.file);
  swap(this->path, source.path);
  swap(this->lastError, source.lastError);
  swap(this->isTemp, source.isTemp);
  return *this;
}

File::~File() {
  if (this->isTemp) {
    filesystem::remove(this->path);
  }
}

const std::error_code & File::rename(const string & destinationPath) {
  std::error_code ec{};

  // The behavior of filesystem::rename() is implementation-specific when the
  // target already exists, and therefore unreliable for this library.
  // As such, we will check manually to verify that the destination path is not
  // already in use.  Unfortunately, this is not atomic.
  // https://en.cppreference.com/w/cpp/io/c/rename
  // https://en.cppreference.com/w/cpp/filesystem/rename
  if (filesystem::exists(destinationPath)) {
    this->lastError = make_error_code(OS::ErrorCode::FILE_EXISTS_AT_TARGET_PATH);
  }
  else {
    filesystem::rename(this->path, destinationPath, ec);
    this->lastError = ec;
    this->isTemp = false;
  }
  return this->lastError;
}

const std::error_code & File::remove() {
  this->lastError = {};

  if (!filesystem::remove(this->path, this->lastError)) {
    // The file was not removed.  If there is no error, it is because there was
    // no file to be deleted in the first place.
    if (!this->lastError) {
      this->lastError = make_error_code(OS::ErrorCode::FILE_DOES_NOT_EXIST);
    }
  }
  this->file = {};
  this->isTemp = false;

  return this->lastError;
}

File File::createTemp(const std::string & pattern) {
  // Attempt to create a file in the OS temp directory.
  string path = filesystem::temp_directory_path() / (pattern + ".XXXXXX");
  char * pathCStr = const_cast<char *>(path.c_str());
  auto fd = mkstemp(pathCStr);

  // It either succeeded or it did not.
  if (fd != -1) {
    ::close(fd);
    File f{path};
    f.isTemp = true;
    return f;
  }

  return File{};
}

File::operator string() const {
  this->lastError = {};

  // Verify file open.
  fstream f{this->path, ios::in | ios::binary};
  if (!f.is_open()) {
    this->lastError =  make_error_code(ErrorCode::FILE_COULD_NOT_BE_OPENED);
    return "";
  }

  // Get the contents.
  stringstream ss;
  ss << f.rdbuf();
  f.close();
  return ss.str();
}

const string & File::getPath() const {
  return this->path;
}

const error_code & File::getLastError() const {
  return this->lastError;
}

const error_code & File::append(string_view sv) {
  this->lastError = {};

  // Open the file for writing then verify.
  fstream f{this->path, ios::out | ios::binary | ios::app};
  if (!f.is_open()) {
    this->lastError = make_error_code(ErrorCode::FILE_COULD_NOT_BE_OPENED);
  }
  else {
    // Write then verify.
    f << sv;
    if (file.fail()) {
      this->lastError = make_error_code(ErrorCode::ERROR_WRITING_TO_FILE);
    }
  }

  return this->lastError;
}

