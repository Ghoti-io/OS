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
  EXPECT_EQ(f.openWrite(), ErrorCode::FILE_COULD_NOT_BE_OPENED);
  EXPECT_EQ(f.openRead(), ErrorCode::FILE_COULD_NOT_BE_OPENED);
  EXPECT_EQ(f.close(), ErrorCode::FILE_COULD_NOT_BE_CLOSED);
}

TEST(File, ExistingFile) {
  string path{"./build/apps/fileExists.txt"};

  // Open an existing file.
  File f{"./build/apps/fileExists.txt"};
  EXPECT_FALSE(f.file.is_open());
  EXPECT_FALSE(f.openRead());
  EXPECT_TRUE(f.file.is_open());
  EXPECT_EQ(f.getPath(), path);

  // Read contents.
  EXPECT_EQ(string{f}, "Hello World\n");

  // Reading should reset the file to the beginning.
  EXPECT_EQ(string{f}, "Hello World\n");

  // Close the file.
  EXPECT_FALSE(f.close());
  EXPECT_FALSE(f.file.is_open());

  // Reopening the file works.
  EXPECT_FALSE(f.openRead());
  EXPECT_TRUE(f.file.is_open());
  EXPECT_EQ(string{f}, "Hello World\n");
}

TEST(File, MissingFile) {
  {
    File f{"fileDoesntExist.txt"};
    EXPECT_EQ(f.openRead(), ErrorCode::FILE_COULD_NOT_BE_OPENED);
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
    EXPECT_FALSE(f.openWrite());

    // Write to the temporary file.
    f.file << contents;
    EXPECT_FALSE(f.close());

    // Read back the contents of the temporary file.
    EXPECT_FALSE(f.openRead());
    EXPECT_EQ(string{f}, contents);
    EXPECT_FALSE(f.close());

    // The temporary file will now pass out of scope and should be
    // automatically deleted.
  }

  // The file should not exist.
  File f{path};
  EXPECT_EQ(f.openRead(), ErrorCode::FILE_COULD_NOT_BE_OPENED);
}

TEST(Delete, MissingFile) {
  // Delete a file that does not exist.
  {
    File f{"fileDoesntExist.txt"};
    EXPECT_EQ(f.remove(), ErrorCode::FILE_DOES_NOT_EXIST);
  }
}

TEST(Delete, ExistingFile) {
  string newName{};
  string contents{"file contents"};

  // Create a temp file and rename it so that it is not automatically deleted.
  {
    auto f{File::createTemp("abc123")};
    EXPECT_FALSE(f.openWrite());
    f.file << contents;

    newName = f.getPath() + ".2";
    EXPECT_FALSE(f.rename(newName));
  }

  // Open the file again to verify that the rename succeeded, then delete the
  // file.
  {
    File f{newName};
    EXPECT_FALSE(f.openRead());
    EXPECT_EQ(string{f}, contents);
    EXPECT_FALSE(f.close());

    f.remove();
  }

  // Verify that the file was deleted.
  {
    File f{newName};
    EXPECT_EQ(f.openRead(), ErrorCode::FILE_COULD_NOT_BE_OPENED);
  }
}

TEST(Rename, OverExisting) {
  string f1Path{};
  string f2Path{};
  {
    // Create two files.
    auto f1{File::createTemp("abc123")};
    auto f2{File::createTemp("abc123")};
    f1Path = f1.getPath();
    f2Path = f2.getPath();

    // Verify that they do not have the same path.
    EXPECT_NE(f1.getPath(), f2.getPath());

    // Write something into both files to confirm that they both exist.
    EXPECT_FALSE(f1.openWrite());
    f1.file << "1";
    EXPECT_FALSE(f1.close());
    EXPECT_FALSE(f2.openWrite());
    f2.file << "2";
    EXPECT_FALSE(f2.close());


    // Attempt to rename one file to that of the other.
    EXPECT_EQ(f2.rename(f1.getPath()), ErrorCode::FILE_EXISTS_AT_TARGET_PATH);
    EXPECT_EQ(f2.getPath(), f2Path);
  }

  // Verify that all of the rename attempts did not keep the files from being
  // properly cleaned up, since they were both temp files.
  {
    File f1{f1Path};
    File f2{f2Path};
    EXPECT_EQ(f1.openRead(), ErrorCode::FILE_COULD_NOT_BE_OPENED);
    EXPECT_EQ(f2.openRead(), ErrorCode::FILE_COULD_NOT_BE_OPENED);
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

