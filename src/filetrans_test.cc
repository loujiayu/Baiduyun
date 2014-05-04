#include <gtest/gtest.h>

#include "filetrans.h"
#include "jsonentry.h"

namespace by {

TEST(ExtractPathTest,ExtractPath) {
  std::string t1 = ExtractPath("/apps/ldrive\\mytest//baidu");
  std::string t2 = ExtractPath("/apps/ldrive//\\mytest/\\baidu");
  std::string t3 = ExtractPath("/apps/ldrive//\\mytest/\\baidu\\//\\");
  EXPECT_STREQ("/mytest/baidu",t1.c_str());
  EXPECT_STREQ("/mytest/baidu",t2.c_str());
  EXPECT_STREQ("/mytest/baidu",t3.c_str());
}

TEST(IsExistsTest,IsExists) {
  JsonEntry p;
  p.Add("path",JsonEntry("/home/tmp"));
  EXPECT_TRUE(IsExists("/home/tmp",p));
  EXPECT_FALSE(IsExists("/home/WRONG",p));
}

TEST(IsMd5MatchTest,IsMd5Match) {
  JsonEntry p;
  p.Add("path",JsonEntry("/home/tmp"));
  p.Add("md5",JsonEntry("123456"));
  EXPECT_TRUE(IsMd5Match("/home/tmp","123456",p));
  EXPECT_FALSE(IsMd5Match("/home/tmp","wrong number",p));
}

}
