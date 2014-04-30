#include <gtest/gtest.h>

#include "filetrans.h"

TEST(File,ExtractPath) {
  std::string path_test = "/apps/ldrive/test\\asdf/asdf";
  std::string ee = by::ExtractPath(path_test);
  EXPECT_STREQ("/test/asdf/asdf",ee.c_str());
}
