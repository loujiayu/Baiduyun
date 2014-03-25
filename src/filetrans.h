/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_FILETRANS_H_
#define SRC_FILETRANS_H_

#include <string>

#include "macro.h"

namespace by {

class JsonEntry;

class FileTrans {
 public:
  explicit FileTrans(const std::string&  access_token);
  void DownLoads();
  void FileInfo();
  void Update(const JsonEntry& jobj);

 private:
  std::string access_token_;

  DISALLOW_COPY_AND_ASSIGN(FileTrans);
};

}  // namespace by

#endif  // SRC_FILETRANS_H_
