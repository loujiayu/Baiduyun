/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include <iostream>

#include "filesystem.h"
#include "jsonentry.h"

namespace by {

bool IsDir(const JsonEntry& jobj) {
  return jobj["isdir"].Value<unsigned int>();
}

std::string ParseFileName(const JsonEntry& jobj) {
  std::string path = jobj["path"].Value<std::string>();
  return path;
}

}  // namespace by
