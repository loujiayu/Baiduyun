/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#include <string>

namespace by {

class JsonEntry;

namespace log {

class FileIO {
 public:
  typedef std::map<std::string,FileOperation> MemTable;
  FileIO();
  bool LogReader(MenTable& mem);
  bool LogWriter(MenTable& mem);
  JsonEntry ReadConfig();
  void SaveConfig(const JsonEntry& config);

 private:
  std::string home_dir_;
  std::shared_ptr<FileSystem> fs_;
};



}  // namespace log
}  // namespace by


#endif  // SRC_CONFIG_H_
