/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_FILESYSTEM_H_
#define SRC_FILESYSTEM_H_

#include <string>
#include <boost/filesystem.hpp>
#include <forward_list>
#include "macro.h"

namespace fs = boost::filesystem;

namespace by {

class JsonEntry;

class WritableFile {
 public:
  WritableFile(const std::string& fname, FILE* f);
  bool Append(const char *data,size_t n);
  bool Close();
  bool Flush();

 private:
  std::string filename_;
  FILE *file_;

  DISALLOW_COPY_AND_ASSIGN(WritableFile);
};

class DirIter : public std::iterator<std::input_iterator_tag,JsonEntry> {
 public:
  DirIter();
  DirIter(const std::string& path);
  DirIter(const DirIter& mit) : path_(mit.path_),
                                filesystem_iter_(mit.filesystem_iter_),
                                jstring_(mit.jstring_) {}
  DirIter& operator++();
  DirIter operator++(int);
  JsonEntry operator*();
  bool operator!=(const DirIter& rhs) const {
    return filesystem_iter_ != rhs.filesystem_iter_;
  }
  bool operator==(const DirIter& rhs) const {
    return filesystem_iter_ == rhs.filesystem_iter_;
  }
  void UpdatePara();
 private:
  std::string path_;
  fs::directory_iterator filesystem_iter_;
  std::string jstring_;
};

class FileSystem {
 public:
  FileSystem() {};

  bool CreatDir(const std::string &path);
  bool DeleteDir(const std::string &path);
  bool IsLocalDir(const std::string &path);
  bool IsExist(const std::string &path);
  bool DeleteFile(const std::string &path);
  bool DirIsEmpty(const std::string &path);

  void GetChild(std::forward_list<JsonEntry> &list, const std::string &path);
  unsigned int LastWriteTime(const std::string &path);

  bool NewWritableFile(WritableFile **file,const std::string &fname);
 private:
  DISALLOW_COPY_AND_ASSIGN(FileSystem);
};

std::string  MD5(std::streambuf *file);
bool IsDir(const JsonEntry& jobj);
std::string ParseFileName(const JsonEntry& jobj);
unsigned int ParseFilemTime(const JsonEntry& json);
std::string FileFromPath(const std::string& path);
}  // namespace by

#endif  // SRC_FILESYSTEM_H_
