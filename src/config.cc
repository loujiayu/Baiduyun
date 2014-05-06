/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "config.h"
#include "jsonentry.h"

namespace by {
const std::string& ConfigFilename() {
  static const std::string filename =
    std::string(::getenv("HOME")) + "/.baiduyun";
  return filename;
}

JsonEntry ReadConfig() {
  std::ifstream ifile(ConfigFilename().c_str());
  if ( ifile ) {
    std::string cfg_str(
      (std::istreambuf_iterator<char>(ifile)),
      (std::istreambuf_iterator<char>()));
    return JsonEntry::Parse(cfg_str);
  } else {
    return JsonEntry();
  }
}

void SaveConfig(const JsonEntry& config) {
  std::ofstream ofile(ConfigFilename().c_str());
  ofile << config;
}
}  // namespace by
