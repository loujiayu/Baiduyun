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
#include <map>

#include "macro.h"
#include "log.h"
namespace by {
enum FileOperation {kDownloads=1,kRemoteDelete,kLocalDelete,kUploads,kPass};

typedef std::map<std::string,FileOperation> MemTable;
typedef std::forward_list<JsonEntry> list;

const std::string kDPcsURL     = "https://d.pcs.baidu.com/rest/2.0/pcs/file";
const std::string kCPcsURL     = "https://c.pcs.baidu.com/rest/2.0/pcs/file";
const std::string kPcsURL      = "https://pcs.baidu.com/rest/2.0/pcs/file";

const std::string kRemoteRoot  = "/apps/ldrive";
const std::string kMarkfile    = boost::filesystem::current_path().string() + "/Baidu_Yun/.baiduyun";
const std::string kLocalRoot   = boost::filesystem::current_path().string() + "/Baidu_Yun";

class JsonEntry;
class FileSystem;

class FileTrans {
 public:
  explicit FileTrans(const std::string&  access_token);
  ~FileTrans();

  void Drive(const std::string &p);
  void Downloads(const std::string& p);
  void DownloadFile(const std::string& path);
  list FileInfo(const std::string& sub_dir = "");
  void Sync(const std::string& path);
  void SynOperation();
  void LocalUpdate(const JsonEntry& jobj,list& flist);
  void Uploads(const std::string& p);
  void UploadFile(const std::string& path);
  void DeleteFile(const std::string& path);

  FileOperation LocalMtimeCmp(const std::string& path);
  FileOperation RemoteMtimeCmp(const JsonEntry& json);
  void AddToMemTable(FileOperation flag,const std::string& path);

 private:
  std::string access_token_;
  std::string markf_;
  MemTable mem_table_;
  FileSystem* fs_;
  log::LogFile* log_;
  DISALLOW_COPY_AND_ASSIGN(FileTrans);
};

std::string ExtractPath(const std::string& p);
bool IsExists(const std::string& path,const JsonEntry& jobj);
bool IsMd5Match(const std::string& path,const std::string& md5,const JsonEntry& jobj);
bool MapToString(const MemTable &mem,char **ptr);
void StringToMap(MemTable &mem,char **ptr);
}  // namespace by

#endif  // SRC_FILETRANS_H_
