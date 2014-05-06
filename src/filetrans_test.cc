#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "filetrans.h"
#include "jsonentry.h"
#include "config.h"

namespace fs = boost::filesystem;

namespace by {

const std::string test_filename = "mytest.txt";

TEST(ExtractPathTest,ExtractPath) {
  std::string t1 = ExtractPath("/apps/ldrive\\mytest//baidu");
  std::string t2 = ExtractPath("/apps/ldrive//\\mytest\\baidu");
  std::string t3 = ExtractPath("/apps/ldrive//\\mytest/\\baidu\\//\\");
  std::string t4 = ExtractPath(fs::current_path().string() + "/Baidu_Yun" +"/mytest\\baidu");
  EXPECT_STREQ("mytest/baidu",t1.c_str());
  EXPECT_STREQ("mytest/baidu",t2.c_str());
  EXPECT_STREQ("mytest/baidu",t3.c_str());
  EXPECT_STREQ("mytest/baidu",t4.c_str());
}

TEST(IsExistsTest,IsExists) {
  JsonEntry p;
  p.Add("path",JsonEntry("\\home/tmp"));
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

TEST(FileTrans,RemoteOperation) {
  JsonEntry config = ReadConfig();
  FileTrans ft(config["access_token"].Value<std::string>());

  std::ofstream ofile(test_filename);
  if(ofile.is_open()) {
    ofile << "haha";
    ofile.close();
  } else {
    printf("Error opening file");
  }
  ft.UploadFile("mytest.txt");

}

}
