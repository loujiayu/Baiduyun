/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_FILETRANS_H_
#define SRC_FILETRANS_H_

#include <stdlib.h>
#include <string>
#include <boost/filesystem.hpp>
#include <forward_list>

#include "macro.h"

namespace by {

const std::string kDPcsURL     = "https://d.pcs.baidu.com/rest/2.0/pcs/file";
const std::string kCPcsURL     = "https://c.pcs.baidu.com/rest/2.0/pcs/file";
const std::string kPcsURL      = "https://pcs.baidu.com/rest/2.0/pcs/file";

const std::string kRemoteRoot  = "/apps/ldrive";
const std::string kMarkfile    = boost::filesystem::current_path().string() + "/Baidu_Yun/.baiduyun";
const std::string kLocalRoot   = boost::filesystem::current_path().string() + "/Baidu_Yun";

class JsonEntry;

std::string ExtractPath(const std::string& p);
bool IsExists(const std::string& path,const JsonEntry& jobj);
bool IsMd5Match(const std::string& path,const std::string& md5,const JsonEntry& jobj);

typedef std::forward_list<JsonEntry> list;

enum FileOperation {kDownloads=1,kDelete,kUploads,kPass};

class FileTrans {
 public:
  explicit FileTrans(const std::string&  access_token);
  void Downloads(const std::string& p);
  list FileInfo(const std::string& sub_dir = "");
  void DownloadFile(const std::string& path);
  void Syn(const std::string& path);
  void SynOperation(int flag,const std::string& path);
  void LocalUpdate(const JsonEntry& jobj,list& flist);
  void UploadFile(const std::string& path);
  void DeleteFile(const std::string& path);
  int LocalMtimeCmp(const std::string& path);
  int RemoteMtimeCmp(const JsonEntry& json);

 private:
  std::string access_token_;
  std::string markf;
  DISALLOW_COPY_AND_ASSIGN(FileTrans);
};

}  // namespace by

#endif  // SRC_FILETRANS_H_
