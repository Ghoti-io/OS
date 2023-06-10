/**
 * @file
 */

#include "os/file.hpp"
#include <cstdlib>    // for mkstemp()
#include <filesystem> // for filesystem::
#include <iostream>
#include <sstream>
#include <unistd.h>   // for ::close()

using namespace std;
using namespace Ghoti::OS;

File::File() : file{}, path{}, mode{}, isRead{false}, isWrite{false}, isTemp{false} {}

File::File(const string & path) : file{}, path{path}, mode{}, isRead{false}, isWrite{false}, isTemp{false} {}

File::File(File && source) {
  this->file = move(source.file);
  this->path = move(source.path);
  this->mode = move(source.mode);
  this->isRead = source.isRead;
  this->isWrite = source.isWrite;
  this->isTemp = source.isTemp;
  // This is a transfer of ownership, so the source must no longer identify
  // the target file as a temp file.
  source.isTemp = false;
}

File & File::operator=(File && source) {
  swap(this->file, source.file);
  swap(this->path, source.path);
  swap(this->mode, source.mode);
  swap(this->isRead, source.isRead);
  swap(this->isWrite, source.isWrite);
  swap(this->isTemp, source.isTemp);
  return *this;
}

File::~File() {
  this->close();
  if (this->isTemp) {
    filesystem::remove(this->path);
  }
}

bool File::open(const char * mode) {
  // Close any open file.
  if (this->file.is_open()) {
    if (!this->close()) {
      return false;
    }
  }

  // Open the file in the appropriate mode.
  this->mode = mode;
  if (this->mode == "r") {
    this->file.open(this->path, ios::in | ios::binary);
    this->isRead = true;
  }
  else if (this->mode == "w") {
    this->file.open(this->path, ios::out | ios::binary);
    this->isWrite = true;
  }
  return this->file.is_open();
}

bool File::close() {
  this->file.close();

  this->isRead = false;
  this->isWrite = false;

  return !this->file.fail();
}

File File::createTemp([[maybe_unused]]const std::string & pattern) {
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

File::operator string() {
  stringstream ss;
  this->file.seekg(0);
  ss << this->file.rdbuf();
  return ss.str();
}

const string & File::getPath() const {
  return this->path;
}

