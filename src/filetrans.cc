/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include <fstream>
#include <iostream>
#include <json-c/json.h>
#include <forward_list>
#include <algorithm>

#include "filesystem.h"
#include "filetrans.h"
#include "http.h"
#include "jsonentry.h"

namespace by {

const std::string token_url     = "https://d.pcs.baidu.com/rest/2.0/pcs/file";
const std::string remote_rpath  = "/apps/ldrive";
const std::string local_rpath   = fs::current_path().string() + "/Baidu Yun";
FileTrans::FileTrans(const std::string&  access_token) :
            access_token_(access_token) {}

JsonEntry::list FileTrans::FileInfo(const std::string& sub_dir) {
  std::string filelist_url = token_url +
                    "?method=list"
                    "&access_token=" + access_token_ +
                    "&path="         + remote_rpath  + sub_dir;

  JsonEntry resp = JsonEntry::Parse(HttpGet(filelist_url));
  JsonEntry::list filelist = resp["list"].Value<JsonEntry::list>();
  return filelist;
}

void FileTrans::DownLoads(const fs::path& p) {
  std::string sub_dir = "";
  if(p.string() != local_rpath) {
    sub_dir = p.string().substr(local_rpath.size());
  }
  std::cout << sub_dir << std::endl;
  if(!fs::exists(p))
    fs::create_directory(p);
  JsonEntry::list remote_flist = FileInfo(sub_dir);
  for (auto iter = remote_flist.begin(); iter != remote_flist.end(); ++iter) {
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
  std::string sub_dir = "";
  if(p.string() != local_rpath) {
    std::string vf = p.string() + "/.baiduyun";
    std::ofstream ofile(vf);
  } else {
    sub_dir = p.string().substr(local_rpath.size());
  }

  if(!fs::exists(p)) {
    fs::create_directory(p);
    DownLoads(p);
  }

  JsonEntry::list remote_flist = FileInfo(sub_dir);
  std::forward_list<JsonEntry> local_flist;
  copy(DirIter(p.string()),DirIter(),front_inserter(local_flist));
  std::cout << (*local_flist.begin())["path"].Value<std::string>();

  for (auto iter = local_flist.begin(); iter != local_flist.end(); ++iter) {
    bool isdir = fs::is_directory((*iter)["path"].Value<std::string>());
    if(isdir) {
      std::string local_path = p.string() + '/' +
                            FileFromPath(ParseFileName(*iter));
      Syn(p);
    } else {
    //  CompareList(*iter,remote_flist);
    }
  }
}

// void FileTrans::CompareList(const JsonEntry& jobj,JsonEntry::list& flist) {
//   find_if(flist.begin(),flist.end(),)
// }

// bool IsExists(const JsonEntry& jobj,const JsonEntry::list& flist) {
//   std::string path = jobj["path"].Value<std::string>();

// }
}  // namespace by
