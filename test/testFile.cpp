/**
 * @file
 *
 * Test the general Wave server behavior.
 */

#include <string>
#include <gtest/gtest.h>
#include "os/file.hpp"

using namespace std;
using namespace Ghoti::OS;

TEST(File, ExistingFile) {
  string path{"./build/apps/fileExists.txt"};

  // Open an existing file.
  File f{"./build/apps/fileExists.txt"};
  EXPECT_TRUE(f.open("r"));
  EXPECT_TRUE(f.file.is_open());
  EXPECT_EQ(f.getPath(), path);

  // Read contents.
  EXPECT_EQ(string{f}, "Hello World\n");

  // Reading should reset the file to the beginning.
  EXPECT_EQ(string{f}, "Hello World\n");

  // Close the file.
  EXPECT_TRUE(f.close());
  EXPECT_FALSE(f.file.is_open());

  // Reading a closed file shouldn't work.
  EXPECT_EQ(string{f}, "");

  // Reopening the file works.
  EXPECT_TRUE(f.open("r"));
  EXPECT_TRUE(f.file.is_open());
  EXPECT_EQ(string{f}, "Hello World\n");
}

TEST(File, MissingFile) {
  {
    File f{"fileDoesntExist.txt"};
    f.open("r");
    EXPECT_FALSE(f.file.is_open());
  }
}

TEST(File, TempFile) {
  {
    auto f{File::createTemp("abc123")};
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

