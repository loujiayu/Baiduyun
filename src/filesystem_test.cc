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
  if(!fs::exists(test_filename)) {
    if(!fs::create_directory(test_filename)){
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
  EXPECT_TRUE(fs::exists(test_filename));
  RmDir(test_filename);
  EXPECT_FALSE(fs::exists(test_filename));
}


}  // namespace by
