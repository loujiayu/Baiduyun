/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "filesystem.h"
#include "config.h"
#include "jsonentry.h"

namespace by {

namespace log {

FileIO::FileIO() : home_dir_(getenv("HOME") + "/MyBaidu"),
                   fs_(NULL) {
    fs_->CreatDir(home_dir_);
}

JsonEntry FileIO::ReadConfig() {
  std::string config_file = home_dir_ + "/.baiduyun";
  std::ifstream ifile(config_file);
  if ( ifile ) {
    std::string cfg_str(
      (std::istreambuf_iterator<char>(ifile)),
      (std::istreambuf_iterator<char>()));
    return JsonEntry::Parse(cfg_str);
  } else {
    return JsonEntry();
  }
}

void FileIO::SaveConfig(const JsonEntry& config) {
  std::ofstream ofile(ConfigFilename().c_str());
  ofile << config;
}
}  // namespace by
