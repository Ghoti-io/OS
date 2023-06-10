/**
 * @file
 */

#include "os/file.hpp"

using namespace std;
using namespace Ghoti::OS;

File::File() {}

File::File(string path, const char * mode) : path{path}, mode{mode} {}

