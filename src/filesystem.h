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
  DirIter();
  DirIter(const std::string& path);
  DirIter(const DirIter& mit) : path_(mit.path_),
                                filesystem_iter(mit.filesystem_iter),
                                jstring(mit.jstring) {}
  DirIter& operator++();
  DirIter operator++(int);
  JsonEntry operator*() const { return JsonEntry::Parse(jstring) };
  bool operator!=(const DirIter& rhs) const {
    return filesystem_iter != rhs.filesystem_iter;
  }
  bool operator==(const DirIter& rhs) const {
    return filesystem_iter == rhs.filesystem_iter;
  }
  void UpdatePara();
 private:
  std::string path_;
  fs::directory_iterator filesystem_iter;
  std::string jstring;
};

std::string  MD5(std::streambuf *file);
bool IsDir(const JsonEntry& jobj);
void RmDir(const std::string& path);
std::string ParseFileName(const JsonEntry& jobj);
unsigned int ParseFilemTime(const JsonEntry& json);
std::string FileFromPath(const std::string& path);
}  // namespace by

#endif  // SRC_FILESYSTEM_H_
