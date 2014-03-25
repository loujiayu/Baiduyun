/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_FILESYSTEM_H_
#define SRC_FILESYSTEM_H_

#include <string>

#include "macro.h"

namespace by {

class JsonEntry;

bool IsDir(const JsonEntry& jobj);
std::string ParseFileName(const JsonEntry& jobj);

}  // namespace by

#endif  // SRC_FILESYSTEM_H_
