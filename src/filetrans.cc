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



std::string ExtractPath(const std::string& p) {
  std::string path = p;
  if(!path.compare(0,kRemoteRoot.size(),kRemoteRoot))
    path = p.substr(kRemoteRoot.size());
  else if(!path.compare(0,kLocalRoot.size(),kLocalRoot))
    path =  p.substr(kLocalRoot.size());

  std::size_t found = 0;
  std::size_t pos;
  while (found!=std::string::npos) {
    pos = path.find_first_of("/\\",found);
    if(pos > path.size())
      break;
    if(pos - found != 0) {
      path[pos] = '/';
    } else {
      path.erase(path.begin()+pos);
      continue;
    }
    found  = pos + 1;
  }
  found = path.find_last_of("/\\");
  if(found == path.size()-1)
    path.erase(path.begin()+found);
  return path;
}

bool IsMd5Match(const std::string& path,const std::string& md5,const JsonEntry& jobj) {
  std::string rmd5 = jobj["md5"].Value<std::string>();
  if(md5!=rmd5) {
    return false;
  } else {
    std::string rpath = jobj["path"].Value<std::string>();
    std::string p1 = ExtractPath(path);
    std::string p2 = ExtractPath(rpath);
    if(p1 != p2)
      return false;
    else
      return true;
  }
}

bool IsExists(const std::string& path,const JsonEntry& jobj) {
  std::string rpath = jobj["path"].Value<std::string>();
  std::string p1 = ExtractPath(path);
  std::string p2 = ExtractPath(rpath);
  if(p2 == p1)
    return true;
  else
    return false;
}

void FileTrans::SynOperation(int flag,const std::string& path) {
  printf("%s \n",path.c_str());
  switch(flag) {
    case kPass :
      printf("pass\n");
      break;
    case kDownloads : {
      printf("download...");
      DownloadFile(path);
      break;
    }
    case kDelete : {
      printf("deleting...");
      if(!path.compare(0,kRemoteRoot.size(),kRemoteRoot))
        DeleteFile(path);
      else
        RmDir(path);
      break;
    }
    case kUploads : {
      printf("uploading...");
      UploadFile(path);
      break;
    }
    default: break;
  }
}

FileTrans::FileTrans(const std::string&  access_token) :
            access_token_(access_token) ,
            markf(kMarkfile){}

JsonEntry::list FileTrans::FileInfo(const std::string& sub_dir) {
  std::string filelist_url = kDPcsURL      +
                   "?method=list"
                    "&access_token=" + access_token_ +
                    "&path="         + kRemoteRoot  + sub_dir;

  JsonEntry resp = JsonEntry::Parse(HttpGet(filelist_url));
  JsonEntry::list filelist = resp["list"].Value<JsonEntry::list>();
  return filelist;
}

void FileTrans::Downloads(const std::string& p) {
  std::string sub_dir = "";
  if(p != kLocalRoot)
    sub_dir = p.substr(kLocalRoot.size());
  if(!fs::exists(p))
    fs::create_directory(p);
  JsonEntry::list remote_flist = FileInfo(sub_dir);
  if(remote_flist.empty()) {
    printf("%s is empty.\n",p.c_str());
    return;
  }
  for (auto iter = remote_flist.begin(); iter != remote_flist.end(); ++iter) {
    if (IsDir(*iter)) {
      std::string local_path = p + '/' + FileFromPath(ParseFileName(*iter));
      Downloads(local_path);
    }
    else {
      DownloadFile(ParseFileName(*iter));
    }
  }
}

void FileTrans::DownloadFile(const std::string& download_file) {
  std::string path = download_file;
  if(path.compare(0,kRemoteRoot.size(),kRemoteRoot)) {
    path = kRemoteRoot + '/' + path;
  }
  std::string file = path.substr(kRemoteRoot.size());
  std::string url = kDPcsURL      +
                   "?method=download"
                    "&access_token=" + access_token_ +
                    "&path="         + kRemoteRoot  +
                    "/"              + file;
  std::string local_path = kLocalRoot + file;
  std::ofstream mfile(local_path, std::ios::out | std::ios::binary);
  HttpGetFile(url, &mfile);
}

void FileTrans::UploadFile(const std::string& path) {
  std::ifstream ifile(path.c_str());
  std::string file_contents(
      (std::istreambuf_iterator<char>(ifile)),
      (std::istreambuf_iterator<char>()));
  std::string remote_path  = ExtractPath(path);
  std::string post = kCPcsURL +
                     "?method=upload"
                     "&access_token=" + access_token_ +
                     "&path="         + kRemoteRoot  + '/' + remote_path;
  Put(post, file_contents);
}

void FileTrans::DeleteFile(const std::string& path) {
  std::string remote_path  = ExtractPath(path);
  std::string post =  kPcsURL +
                     "?method=delete"
                     "&access_token=" + access_token_ +
                     "&path="         + kRemoteRoot + '/' +remote_path;
  HttpGet(post);
}

void FileTrans::Syn(const std::string& p) {
  if(p.empty()) {
    printf("Please input path name.");
    return;
  }
  using namespace std::placeholders;
  int op_flag;
  std::string sub_dir = "";
  if(!fs::exists(kMarkfile))
    std::ofstream ofile(kMarkfile);

  if(!fs::exists(p)) {
    if(!fs::create_directory(p)){
      printf("Pathname invalid.");
      return;
    }
    Downloads(p);
    std::ofstream ofile(kMarkfile);
    return;
  }

  if(p != kLocalRoot)
    sub_dir = p.substr(kLocalRoot.size());

  JsonEntry::list remote_flist = FileInfo(sub_dir);
  // if(remote_flist.empty()) {
  //   printf("%s is empty.\n",p.c_str());
  //   return;
  // }
  std::forward_list<JsonEntry> local_flist;
  copy(DirIter(p),DirIter(),front_inserter(local_flist));
  for (auto iter = local_flist.begin(); iter != local_flist.end(); ++iter) {
    std::string path = ParseFileName(*iter);
    bool isdir = fs::is_directory(path);
    if(isdir) {
      auto file = find_if(remote_flist.begin(),remote_flist.end(),std::bind(IsExists,path,_1));
      if(file == remote_flist.end()) {
        op_flag = LocalMtimeCmp(path);
        SynOperation(op_flag,path);
      }
      if(op_flag == kDelete)
        continue;
      remote_flist.remove(*file);
      std::string local_path = p + '/' + FileFromPath(path);
      Syn(local_path);
    } else {
      LocalUpdate(*iter,remote_flist);
    }
  }
  if(!remote_flist.empty()){
    int op_flag;
    for(auto iter = remote_flist.begin(); iter != remote_flist.end(); ++iter) {
      std::string remote_path = ParseFileName(*iter);
      op_flag = RemoteMtimeCmp(*iter);
      SynOperation(op_flag,remote_path);
    }
  }
}

int FileTrans::RemoteMtimeCmp(const JsonEntry& json) {
  int op_flag;
  unsigned int mtime = fs::last_write_time(kMarkfile);
  if(mtime < ParseFilemTime(json))
    op_flag = kDownloads;
  else
    op_flag = kDelete;
  return op_flag;
}

int FileTrans::LocalMtimeCmp(const std::string& path) {
  int op_flag;
  auto mtime = fs::last_write_time(kMarkfile);
  auto ptime = fs::last_write_time(path);
  if(mtime <= ptime)
    op_flag = kUploads;
  else
    op_flag = kDelete;
  return op_flag;
}

void FileTrans::LocalUpdate(const JsonEntry& jobj,JsonEntry::list& flist) {
  using namespace std::placeholders;

  std::string path = jobj["path"].Value<std::string>();
  int op_flag;
  auto file = find_if(flist.begin(),flist.end(),std::bind(IsExists,path,_1));
  if(file == flist.end()) {
    op_flag = LocalMtimeCmp(path);
  } else {
    std::ifstream ifile(path.c_str(), std::ios::binary | std::ios::out);
    std::string md5 = MD5(ifile.rdbuf());
    auto remote_file = find_if(flist.begin(),flist.end(),std::bind(IsMd5Match,path,md5,_1));
    if(remote_file == flist.end()) {
      auto remote_time = (*file)["mtime"].Value<unsigned int>();
      unsigned int local_time = fs::last_write_time(path);
      if(remote_time < local_time)
        op_flag = kUploads;
      else
        op_flag = kDownloads;
    } else {
      op_flag = kPass;
    }
    flist.remove(*file);
  }
  SynOperation(op_flag,path);
}

}  // namespace by
