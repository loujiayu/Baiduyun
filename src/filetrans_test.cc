/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "filetrans.h"
#include "jsonentry.h"
#include "config.h"

namespace fs = boost::filesystem;

namespace by {

const std::string test_filename = "mytest781239safjk";

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
  using namespace std::placeholders;
  JsonEntry config = ReadConfig();
  FileTrans ft(config["access_token"].Value<std::string>());

  std::ofstream ofile(test_filename);
  if(ofile.is_open()) {
    ofile << "haha";
    ofile.close();
  } else {
    printf("Error opening file");
  }
  ft.UploadFile(test_filename);
  JsonEntry::list flist = ft.FileInfo("");
  auto file = find_if(flist.begin(),flist.end(),std::bind(IsExists,test_filename,_1));
  EXPECT_TRUE(file != flist.end());

  if(!fs::remove(test_filename)) {
    printf("%s does not exist.",test_filename.c_str());
  }
  ft.DownloadFile("/apps/ldrive/"+test_filename);
  EXPECT_TRUE(fs::exists("Baidu_Yun/" + test_filename));

  ft.DeleteFile(test_filename);
  flist = ft.FileInfo("");
  file = find_if(flist.begin(),flist.end(),std::bind(IsExists,test_filename,_1));
  EXPECT_TRUE(file == flist.end());

  if(!fs::remove("Baidu_Yun/" + test_filename)) {
    printf("%s does not exist ./Baidu_Yun/.",test_filename.c_str());
  }
}

}
