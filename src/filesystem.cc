/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */
#include <unistd.h>
#include <openssl/evp.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "filesystem.h"
#include "jsonentry.h"

namespace by {

DirIter::DirIter() {
  filesystem_iter = fs::directory_iterator();
}

DirIter::DirIter(const std::string& p) : path_(p) {
  filesystem_iter = fs::directory_iterator(path_);
  UpdatePara();
}

void DirIter::UpdatePara() {
  JsonEntry json;
  std::string file= (*filesystem_iter).path().string();
  std::ifstream ifile(file.c_str(), std::ios::binary | std::ios::out);
  std::string md5 = MD5(ifile.rdbuf());
  json.Add("path",JsonEntry((*filesystem_iter).path().string()));
  json.Add("md5",JsonEntry(md5));
  jstring = json.getstring();
}

DirIter& DirIter::operator++() {
  ++filesystem_iter;
  if(filesystem_iter == fs::directory_iterator())
    return *this;
  UpdatePara();
  return *this;
}

DirIter DirIter::operator++(int) {
  DirIter tmp(*this);
  operator++();
  return tmp;
}

bool IsDir(const JsonEntry& json) {
  return json["isdir"].Value<unsigned int>();
}

std::string ParseFileName(const JsonEntry& json) {
  std::string path = json["path"].Value<std::string>();
  return path;
}

unsigned int ParseFilemTime(const JsonEntry& json) {
  return json["mtime"].Value<unsigned int>();
}

std::string FileFromPath(const std::string& path) {
  auto found = path.find_last_of("/\\");
  return path.substr(found + 1);
}

void RmDir(const std::string& path) {
  if(!fs::is_directory(path))
    fs::remove(path);
  std::forward_list<JsonEntry> flist;
  copy(DirIter(path),DirIter(),front_inserter(flist));
  for (auto iter = flist.begin();iter != flist.end();++iter) {
    std::string path_name = ParseFileName(*iter);
    if(fs::is_directory(path_name)) {
      if(fs::is_empty(path_name)) {
        rmdir(path_name.c_str());
      } else {
        RmDir(path_name);
      }
      if(fs::exists(path_name)&&fs::is_empty(path_name))
        rmdir(path_name.c_str());
    } else {
      fs::remove(path_name);
    }
  }
  if(fs::exists(path)&&fs::is_empty(path))
        rmdir(path.c_str());
}

std::string MD5(std::streambuf *file)
{
  char buf[64 * 1024];
  EVP_MD_CTX  md;
  EVP_MD_CTX_init( &md );
  EVP_DigestInit_ex( &md, EVP_md5(), 0 );
  std::size_t count = 0 ;
  while ( (count = file->sgetn( buf, sizeof(buf) )) > 0 ) {
    EVP_DigestUpdate( &md, buf, count );
  }
  unsigned int md5_size = EVP_MAX_MD_SIZE;
  unsigned char md5[EVP_MAX_MD_SIZE];
  EVP_DigestFinal_ex( &md, md5, &md5_size );
  std::ostringstream ss;
  for ( unsigned int i = 0 ; i < md5_size ; i++ )
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md5[i]);
  return ss.str();
}

}  // namespace by
