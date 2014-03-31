/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include <fstream>
#include <iostream>
#include <json-c/json.h>
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

}

void FileTrans::DownLoads(const fs::path& p) {
  std::string sub_dir = "";
  if(FileFromPath(p.string()) != "Baidu Yun") {
    sub_dir = FileFromPath(p.string());
   // fs::path sub_p = fs::absolute(sub_dir);
    //fs::create_directory(sub_p);
  }
  if(!fs::exists(p))
    fs::create_directory(p);
  std::string filelist_url = token_url +
                    "?method=list"
                    "&access_token=" + access_token_ +
                    "&path="         + root_name      +sub_dir;

  JsonEntry resp = JsonEntry::Parse(HttpGet(filelist_url));
  JsonEntry::list filelist = resp["list"].Value<JsonEntry::list>();

  for (auto iter = filelist.begin(); iter != filelist.end(); ++iter) {
    if (IsDir(*iter)) {
      std::string local_path = p.string() + '/' +
                            FileFromPath(ParseFileName(*iter));
      DownLoads(local_path);
    }
    else
      Update(*iter,p);
  }
}

void FileTrans::Update(const JsonEntry& jobj,const fs::path& p) {
  std::string remote_path = ParseFileName(jobj);
  std::cout << remote_path << std::endl;

  std::string url = token_url +
                    "?method=download"
                    "&access_token=" + access_token_ +
                    "&path="         + remote_path;
  std::string local_path = p.string() + '/' + FileFromPath(remote_path);
  std::ofstream mfile(local_path, std::ios::out | std::ios::binary);
  HttpGetFile(url, &mfile);
}

void FileTrans::Syn(const fs::path& p) {
  std::string vf = p.string() + "/.baiduyun";
  if(!fs::exists(p)) {
    fs::create_directory(p);
    DownLoads(p);
    std::ofstream ofile(vf);
  }

  JsonEntry jsonentry;
  jsonentry.Add("path",JsonEntry(p.string()));

  std::cout << *DirIter(p.string());
}
}  // namespace by
