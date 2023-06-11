/**
 * @file
 *
 * Test the general Wave server behavior.
 */

#include <string>
#include <gtest/gtest.h>
#include "os/file.hpp"
#include "os/errorcode.hpp"

using namespace std;
using namespace Ghoti::OS;

TEST(File, DefaultConstructor) {
  // Default-constructed file should not actually point to anything.
  File f{};
  EXPECT_EQ(f.getPath(), "");
  EXPECT_FALSE(f.file.is_open());
  EXPECT_FALSE(f.open("w"));
  EXPECT_FALSE(f.open("r"));
  EXPECT_FALSE(f.close());
}

TEST(File, ExistingFile) {
  string path{"./build/apps/fileExists.txt"};

  // Open an existing file.
  File f{"./build/apps/fileExists.txt"};
  EXPECT_FALSE(f.file.is_open());
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
    EXPECT_FALSE(f.open("r"));
    EXPECT_FALSE(f.file.is_open());
  }
}

TEST(File, TempFile) {
  string path{};
  {
    auto f{File::createTemp("abc123")};
    string contents{"file contents"};

    // Save the path of the temporary file so that later we can verify that the
    // file was deleted.
    path = f.getPath();

    // Open the temporary file.
    EXPECT_TRUE(f.open("w"));

    // Write to the temporary file.
    f.file << contents;
    EXPECT_TRUE(f.close());

    // Read back the contents of the temporary file.
    EXPECT_TRUE(f.open("r"));
    EXPECT_EQ(string{f}, contents);
    EXPECT_TRUE(f.close());

    // The temporary file will now pass out of scope and should be
    // automatically deleted.
  }

  // The file should not exist.
  File f{path};
  EXPECT_FALSE(f.open("r"));
}

TEST(Delete, MissingFile) {
  // Delete a file that does not exist.
  {
    File f{"fileDoesntExist.txt"};
    EXPECT_EQ(f.remove(), make_error_code(Ghoti::OS::error_code::file_does_not_exist));
  }
}

TEST(Delete, ExistingFile) {
  string newName{};
  string contents{"file contents"};

  // Create a temp file and rename it so that it is not automatically deleted.
  {
    auto f{File::createTemp("abc123")};
    f.open("w");
    f.file << contents;

    newName = f.getPath() + ".2";
    f.rename(newName);
  }

  // Open the file again to verify that the rename succeeded, then delete the
  // file.
  {
    File f{newName};
    EXPECT_TRUE(f.open("r"));
    EXPECT_EQ(string{f}, contents);
    EXPECT_TRUE(f.close());

    f.remove();
  }

  // Verify that the file was deleted.
  {
    File f{newName};
    EXPECT_FALSE(f.open("r"));
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

