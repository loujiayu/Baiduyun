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

#include "macro.h"

namespace by {

namespace fs = boost::filesystem;
class JsonEntry;
typedef std::forward_list<JsonEntry> list;

enum FileOperation {KDownloads=1,KDelete,KUploads,KPass};

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
 private:
  std::string access_token_;
  std::string markf;
  DISALLOW_COPY_AND_ASSIGN(FileTrans);
};

}  // namespace by

#endif  // SRC_FILETRANS_H_
