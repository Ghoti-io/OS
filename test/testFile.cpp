/**
 * @file
 *
 * Test the general Wave server behavior.
 */

#include<string>
#include<gtest/gtest.h>
#include "os/file.hpp"

using namespace std;
using namespace Ghoti::OS;

TEST(File, Constructor) {
  {
    File f{"./build/apps/fileExists.txt"};
    f.open("r");
  }
  {
    File f{"fileDoesntExist.txt"};
  }
  {
    auto f{File::createTemp("abc123")};
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

