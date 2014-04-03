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
const std::string post_url      = "https://c.pcs.baidu.com/rest/2.0/pcs/file";
const std::string delete_url    = "https://pcs.baidu.com/rest/2.0/pcs/file";

const std::string remote_rpath  = "/apps/ldrive";
const std::string local_rpath   = fs::current_path().string() + "/Baidu_Yun";

std::string ExtractPath(const std::string& p) {
  std::string path = p;
  if(!path.compare(0,remote_rpath.size(),remote_rpath))
    path = p.substr(remote_rpath.size());
  else if(!path.compare(0,local_rpath.size(),local_rpath))
    path =  p.substr(local_rpath.size());
  else
    return p;
  std::size_t found = path.find_first_of("/\\");
  while (found!=std::string::npos) {
    path[found] = '/';
    found = path.find_first_of("/\\",found+1);
  }
  return path;
}

bool IsMd5Match(const std::string& path,const std::string& md5,const JsonEntry& jobj) {
  std::string rmd5 = jobj["md5"].Value<std::string>();
  if(md5.compare(rmd5)) {
    return false;
  } else {
    std::string rpath = jobj["path"].Value<std::string>();
    std::string p1 = ExtractPath(path);
    std::string p2 = ExtractPath(rpath);
    if(p1.compare(p2))
      return false;
    else
      return true;
  }
}

bool IsExists(const std::string& path,const JsonEntry& jobj) {
  std::string rpath = jobj["path"].Value<std::string>();
  std::string p1 = ExtractPath(path);
  std::string p2 = ExtractPath(rpath);
  std::cout << p1 << std::endl << p2 << std::endl;
  if(p2 == p1) {
    return true;
  } else {
    return false;
  }
}

void FileTrans::SynOperation(int flag,const std::string& path) {
  std::cout << path <<std::endl;
  switch(flag) {
    case KPass :
      std::cout << "KPass" <<std::endl;
      break;
    case KDownloads : {
      std::cout << "KDownloads" <<std::endl;
      DownloadFile(path);
      break;
    }
    case KDelete : {
      std::cout << "KDelete" <<std::endl;
      if(path.compare(0,remote_rpath.size(),remote_rpath)) {
        DeleteFile(path);
      }
      else
        fs::remove(path);
      break;
    }
    case KUploads : {
      std::cout << "KUploads" <<std::endl;
      UploadFile(path);
      break;
    }
    default: break;
  }
}

FileTrans::FileTrans(const std::string&  access_token) :
            access_token_(access_token) ,
            markf(local_rpath + "/.baiduyun"){}

JsonEntry::list FileTrans::FileInfo(const std::string& sub_dir) {
  std::string filelist_url = token_url +
                    "?method=list"
                    "&access_token=" + access_token_ +
                    "&path="         + remote_rpath  + sub_dir;

  JsonEntry resp = JsonEntry::Parse(HttpGet(filelist_url));
  //std::cout << resp <<std::endl;
  JsonEntry::list filelist = resp["list"].Value<JsonEntry::list>();
  return filelist;
}

void FileTrans::Downloads(const std::string& p) {
  std::string sub_dir = "";
  if(p != local_rpath) {
    sub_dir = p.substr(local_rpath.size());
  }
 //std::cout << sub_dir << std::endl;
  if(!fs::exists(p))
    fs::create_directory(p);
  JsonEntry::list remote_flist = FileInfo(sub_dir);
  for (auto iter = remote_flist.begin(); iter != remote_flist.end(); ++iter) {
    std::string local_path = p + '/' + FileFromPath(ParseFileName(*iter));
    if (IsDir(*iter)) {
      Downloads(local_path);
    }
    else {
      DownloadFile(local_path);
    }
  }
}

void FileTrans::DownloadFile(const std::string& path) {
  //std::string remote_path = ParseFileName(jobj);
// std::cout << path << std::endl;
  std::string file = path.substr(local_rpath.size());
  std::string url = token_url +
                    "?method=download"
                    "&access_token=" + access_token_ +
                    "&path="         + remote_rpath  +
                    "/"              + file;
  //std::string local_path = p.string() + '/' + FileFromPath(remote_path);
  std::ofstream mfile(path, std::ios::out | std::ios::binary);
  HttpGetFile(url, &mfile);
}

void FileTrans::UploadFile(const std::string& path) {
  std::ifstream ifile(path.c_str());
  std::string file_contents(
      (std::istreambuf_iterator<char>(ifile)),
      (std::istreambuf_iterator<char>()));
  std::string remote_path  = ExtractPath(path);
  std::cout << remote_path << std::endl;
  std::string post = post_url +
                     "?method=upload"
                     "&access_token=" + access_token_ +
                     "&path="         + remote_rpath  +remote_path;
  std::cout << Put(post, file_contents);
}

void FileTrans::DeleteFile(const std::string& path) {
  std::string remote_path  = ExtractPath(path);
  std::string post =  delete_url +
                     "?method=delete"
                     "&access_token=" + access_token_ +
                     "&path="         + remote_rpath + remote_path;
                     std::cout <<post;
  std::cout << HttpGet(post);
}

void FileTrans::Syn(const std::string& p) {
  std::string sub_dir = "";
  if(p != local_rpath) {
    sub_dir = p.substr(local_rpath.size());
  } else {
  //  std::ofstream ofile(markf);
  }
  if(!fs::exists(p)) {
    fs::create_directory(p);
    Downloads(p);
  }

  JsonEntry::list remote_flist = FileInfo(sub_dir);
  std::forward_list<JsonEntry> local_flist;
  copy(DirIter(p),DirIter(),front_inserter(local_flist));
  for (auto iter = local_flist.begin(); iter != local_flist.end(); ++iter) {
    bool isdir = fs::is_directory(ParseFileName(*iter));
    if(isdir) {
      std::string local_path = p + '/' +
                            FileFromPath(ParseFileName(*iter));
      Syn(local_path);
    } else {
      LocalUpdate(*iter,remote_flist);
    }
  }
}

void FileTrans::LocalUpdate(const JsonEntry& jobj,JsonEntry::list& flist) {
  using namespace std::placeholders;

  std::string path = jobj["path"].Value<std::string>();
  int op_flag;
  auto file = find_if(flist.begin(),flist.end(),std::bind(IsExists,path,_1));
  if(file == flist.end()) {
    auto mtime = fs::last_write_time(markf);
    auto ptime = fs::last_write_time(path);
    if(mtime <= ptime) {
      op_flag = KUploads;
    }
    else {
      op_flag = KDelete;
    }
  } else {
    std::ifstream ifile(path.c_str(), std::ios::binary | std::ios::out);
    std::string md5 = MD5(ifile.rdbuf());
    auto remote_file = find_if(flist.begin(),flist.end(),std::bind(IsMd5Match,path,md5,_1));
    if(remote_file == flist.end()) {
      auto remote_time = (*file)["mtime"].Value<unsigned int>();
      auto local_time = fs::last_write_time(path);
      if(remote_time < local_time) {
        op_flag = KUploads;
      } else {
        op_flag = KDownloads;
      }
    } else {
      op_flag = KPass;
    }
    flist.remove(*file);
  }
  SynOperation(op_flag,path);
}

}  // namespace by
