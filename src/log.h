/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#include <string>
#include <map>

namespace by {

class JsonEntry;
class File;
class FileSystem;

namespace log {

class LogFile {
 public:
  LogFile();
  LogFile(File* dest);
  ~LogFile();

  bool LogReader(char **ptr);
  bool LogWriter(const char *ptr);

  JsonEntry ReadConfig();
  void SaveConfig(const JsonEntry& config);

 private:

  std::string home_dir_;
  FileSystem* fs_;
  File *dest_;
};



}  // namespace log
}  // namespace by


#endif  // SRC_CONFIG_H_
