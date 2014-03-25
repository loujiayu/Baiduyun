/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include <fstream>
#include <iostream>

#include "filesystem.h"
#include "filetrans.h"
#include "http.h"
#include "jsonentry.h"

namespace by {

const std::string token_url   = "https://d.pcs.baidu.com/rest/2.0/pcs/file";
const std::string root_name   = "/apps/ldrive/";
FileTrans::FileTrans(const std::string&  access_token) :
            access_token_(access_token) {}

void FileTrans::FileInfo() {
  std::string url = token_url +
                    "?method=list"
                    "&access_token=" + access_token_ +
                    "&path=/apps/ldrive";

  JsonEntry resp = JsonEntry::Parse(HttpGet(url));
  JsonEntry::list filearray = resp["list"].Value<JsonEntry::list>();
  std::cout << IsDir(*filearray.begin()) << std::endl;
}

void FileTrans::DownLoads() {
  std::string filelist_url = token_url +
                    "?method=list"
                    "&access_token=" + access_token_ +
                    "&path="         + root_name;

  JsonEntry resp = JsonEntry::Parse(HttpGet(filelist_url));
  JsonEntry::list filelist = resp["list"].Value<JsonEntry::list>();

  for (auto iter = filelist.begin(); iter != filelist.end(); ++iter) {
    if (IsDir(*iter))
      continue;
    else
      Update(*iter);
  }
}

void FileTrans::Update(const JsonEntry& jobj) {
  std::string filename = ParseFileName(jobj);
  std::cout << filename << std::endl;

  std::string url = token_url +
                    "?method=download"
                    "&access_token=" + access_token_ +
                    "&path="         + filename;

  std::ofstream mfile(
    filename.substr(root_name.size()),
    std::ios::out | std::ios::binary);
  HttpGetFile(url, &mfile);
}

}  // namespace by
