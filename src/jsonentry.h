/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_JSONENTRY_H_
#define SRC_JSONENTRY_H_

#include <string>
#include <forward_list>
#include <json-c/json.h>

namespace by {

class JsonEntry {
 public:
  typedef std::forward_list<JsonEntry> list;

  explicit JsonEntry(struct json_object *JsonEntry);
  explicit JsonEntry(const std::string& str);

  JsonEntry();
  JsonEntry(const JsonEntry& obj);

  ~JsonEntry();

  void Swap(JsonEntry& tmp);
  static JsonEntry Parse(const std::string& str);
  std::string Getstring() { return json_object_get_string(jobj_); }
  void Add(const std::string& key, const JsonEntry& jsonentry);

  JsonEntry& operator=( const JsonEntry& rhs );
  JsonEntry operator[](const std::string& key) const;
  friend std::ostream& operator<<(std::ostream& os, const JsonEntry& JsonEntry);

  template <typename T>
  T Value() const;

 private:
  struct json_object  *jobj_;
};
}  // namespace by

#endif  // SRC_JSONENTRY_H_
