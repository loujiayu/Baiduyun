/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include <gtest/gtest.h>

#include "filesystem.h"

namespace by {

const std::string test_filename = "mytest781239safjk";

TEST(RmdirTest, FileDelte) {
  FileSystem *fs(NULL);
  if(!fs->IsExist(test_filename)) {
    if(!fs->CreatDir(test_filename)){
      printf("Pathname invalid.");
      return;
    }
  }
  std::ofstream ofile(test_filename + "/gtest.txt");
  if(ofile.is_open()) {
    ofile << "haha";
    ofile.close();
  } else {
    printf("Error opening file");
  }
  EXPECT_TRUE(fs->IsExist(test_filename));
  fs->DeleteDir(test_filename);
  EXPECT_FALSE(fs->IsExist(test_filename));
  delete fs;
}


}  // namespace by
