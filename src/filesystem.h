/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_FILESYSTEM_H_
#define SRC_FILESYSTEM_H_

#include <string>
#include <boost/filesystem.hpp>

#include "macro.h"

namespace fs = boost::filesystem;

namespace by {

class JsonEntry;

class DirIter : public std::iterator<std::input_iterator_tag,JsonEntry> {
 public:
  DirIter(const std::string& path);
  DirIter(const DirIter& mit) : path_(mit.path_),
                                filesystem_iter(mit.filesystem_iter),
                                jstring(mit.jstring) {}
  DirIter& operator++();
  DirIter operator++(int);
  std::string& operator*();

 private:
  std::string path_;
  fs::directory_iterator filesystem_iter;
  std::string jstring;
};

std::string MD5(std::streambuf *file);
bool IsDir(const JsonEntry& jobj);
std::string ParseFileName(const JsonEntry& jobj);
std::string FileFromPath(const std::string& path);
}  // namespace by

#endif  // SRC_FILESYSTEM_H_
