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
  //std::cout << path << " " << rpath << std::endl;
  std::string p1 = ExtractPath(path);
  std::string p2 = ExtractPath(rpath);
  //std::cout << p1 << " " << p2 << std::endl;
  if(p2 == p1)
    return true;
  else
    return false;
}

void FileTrans::SynOperation(int flag,const std::string& path) {
  std::cout << path <<std::endl;
  switch(flag) {
    case KPass :
      std::cout << "KPass" <<std::endl;
      break;
    case KDownloads : {
      std::cout << "KDownloads" <<std::endl;
     // DownloadFile(path);
      break;
    }
    case KDelete : {
      std::cout << "KDelete" <<std::endl;
      // if(path.compare(0,remote_rpath.size(),remote_rpath)) {
      //   DeleteFile(path);
      // }
      // else
      //   RmDir(path);
      break;
    }
    case KUploads : {
      std::cout << "KUploads" <<std::endl;
     // UploadFile(path);
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
  std::cout << resp <<std::endl;
  JsonEntry::list filelist = resp["list"].Value<JsonEntry::list>();
  return filelist;
}

void FileTrans::Downloads(const std::string& p) {
  std::string sub_dir = "";
  if(p != local_rpath)
    sub_dir = p.substr(local_rpath.size());
 //std::cout << sub_dir << std::endl;
  if(!fs::exists(p))
    fs::create_directory(p);
  JsonEntry::list remote_flist = FileInfo(sub_dir);
  for (auto iter = remote_flist.begin(); iter != remote_flist.end(); ++iter) {
    std::string local_path = p + '/' + FileFromPath(ParseFileName(*iter));
    if (IsDir(*iter))
      Downloads(local_path);
    else
      DownloadFile(local_path);
  }
}

void FileTrans::DownloadFile(const std::string& path) {
  std::string file = path.substr(local_rpath.size());
  std::string url = token_url +
                    "?method=download"
                    "&access_token=" + access_token_ +
                    "&path="         + remote_rpath  +
                    "/"              + file;
  std::ofstream mfile(path, std::ios::out | std::ios::binary);
  HttpGetFile(url, &mfile);
}

bool FileTrans::UploadFile(const std::string& path) {
  std::ifstream ifile(path.c_str());
  std::string file_contents(
      (std::istreambuf_iterator<char>(ifile)),
      (std::istreambuf_iterator<char>()));
  std::string remote_path  = ExtractPath(path);
  std::cout << remote_path << std::endl;
  std::string post = post_url +
                     "?method=upload"
                     "&access_token=" + access_token_ +
                     "&path="         + remote_rpath  + '/' + remote_path;
  JsonEntry resp = JsonEntry::Parse(Put(post, file_contents));
  //if(resp.getstring().empty())
  auto ss = resp["error_msg"].Value<std::string>();
  printf("%s",ss.c_str());
}

void FileTrans::DeleteFile(const std::string& path) {
  std::string remote_path  = ExtractPath(path);
  std::string post =  delete_url +
                     "?method=delete"
                     "&access_token=" + access_token_ +
                     "&path="         + remote_rpath + '/' +remote_path;
                     std::cout <<post;
  std::cout << HttpGet(post);
}

void FileTrans::Syn(const std::string& p) {
  if(p.empty()) {
    printf("Please input path name");
    return;
  }
  using namespace std::placeholders;
  int op_flag;
  std::string sub_dir = "";
  if(!fs::exists(markf))
    std::ofstream ofile(markf);

  if(!fs::exists(p)) {
    if(fs::create_directory(p)){
      printf("Pathname invalid");
      return;
    }
    Downloads(p);
    std::ofstream ofile(markf);
    return;
  }

  if(p != local_rpath)
    sub_dir = p.substr(local_rpath.size());

  JsonEntry::list remote_flist = FileInfo(sub_dir);
  std::forward_list<JsonEntry> local_flist;
  copy(DirIter(p),DirIter(),front_inserter(local_flist));
  for (auto iter = local_flist.begin(); iter != local_flist.end(); ++iter) {
    std::string path = ParseFileName(*iter);
   // std::cout << path << std::endl;
    bool isdir = fs::is_directory(path);
    if(isdir) {
      auto file = find_if(remote_flist.begin(),remote_flist.end(),std::bind(IsExists,path,_1));
      if(file == remote_flist.end()) {
        op_flag = LocalMtimeCmp(path);
        SynOperation(op_flag,path);
      }
      if(op_flag == KDelete)
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
  unsigned int mtime = fs::last_write_time(markf);
  if(mtime < ParseFilemTime(json))
    op_flag = KDownloads;
  else
    op_flag = KDelete;
  return op_flag;
}

int FileTrans::LocalMtimeCmp(const std::string& path) {
  int op_flag;
  auto mtime = fs::last_write_time(markf);
  auto ptime = fs::last_write_time(path);
  if(mtime <= ptime)
    op_flag = KUploads;
  else
    op_flag = KDelete;
  return op_flag;
}

void FileTrans::LocalUpdate(const JsonEntry& jobj,JsonEntry::list& flist) {
  using namespace std::placeholders;

  std::string path = jobj["path"].Value<std::string>();
  int op_flag;
  //std::cout << path << std::endl;
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
        op_flag = KUploads;
      else
        op_flag = KDownloads;
    } else {
      op_flag = KPass;
    }
    flist.remove(*file);
  }
  SynOperation(op_flag,path);
}

}  // namespace by
