/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "filesystem.h"
#include "log.h"
#include "jsonentry.h"

namespace by {

namespace log {

LogFile::LogFile(WritableFile *dest) : home_dir_("MyBaidu"),
                   fs_(NULL),
                   dest_(dest) {
    fs_->CreatDir(home_dir_);
}

LogFile::~LogFile() {
  delete fs_;
  delete dest_;
}

bool LogFile::LogWriter(const char *ptr) {
  bool flag = dest_->Append(ptr,strlen(ptr));
  if(flag)
    flag = dest_->Flush();
  return flag;
}

JsonEntry LogFile::ReadConfig() {
  std::string fname = home_dir_ + "/.baiduyun";
  std::ifstream ifile(fname.c_str());
  if ( ifile ) {
    std::string cfg_str(
      (std::istreambuf_iterator<char>(ifile)),
      (std::istreambuf_iterator<char>()));
    return JsonEntry::Parse(cfg_str);
  } else {
    return JsonEntry();
  }
}

void LogFile::SaveConfig(const JsonEntry& config) {
  std::string fname = home_dir_ + "/.baiduyun";
  std::ofstream ofile(fname.c_str());
  ofile << config;
}

}  // namespace log
}  // namespace by
