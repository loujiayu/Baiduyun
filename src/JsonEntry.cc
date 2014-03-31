/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <json-c/json.h>
#include <stdexcept>
#include <iostream>

#include "jsonentry.h"

namespace by {

  JsonEntry::JsonEntry() :
    jobj_(::json_object_new_object()) {
    if (jobj_ == 0)
      throw std::runtime_error("cannot create jobj object");
  }

  JsonEntry::JsonEntry(const std::string& str) :
    jobj_(::json_object_new_string(str.c_str())) {
    if (jobj_ == 0)
      throw std::runtime_error("cannot create jobj string \"" + str + "\"");
    assert(::json_object_get_string(jobj_) == str);
  }

  JsonEntry::JsonEntry(const JsonEntry& jsonEntry) :
    jobj_(jsonEntry.jobj_) {
    assert(jobj_ != 0);
    ::json_object_get(jobj_);
  }

  JsonEntry::JsonEntry(struct json_object *jobj) :
    jobj_(jobj) {
    assert(jobj_ != 0);
    ::json_object_get(jobj_);
  }

  void JsonEntry::Add(const std::string& key, const JsonEntry& jsonEntry) {
    assert(jobj_ != 0);
    assert(jsonEntry.jobj_ != 0);
    ::json_object_get(jsonEntry.jobj_);
    ::json_object_object_add(jobj_, key.c_str(), jsonEntry.jobj_);
  }

  JsonEntry JsonEntry::Parse(const std::string& str) {
    struct json_object *jobj_ = ::json_tokener_parse(str.c_str());
    if (jobj_ == 0)
      throw std::runtime_error("jobj parse error");
    return JsonEntry(jobj_);
  }

  JsonEntry& JsonEntry::operator=( const JsonEntry& rhs )
  {
    JsonEntry tmp( rhs ) ;
    Swap( tmp ) ;
    return *this ;
  }

  void JsonEntry::Swap( JsonEntry& other )
  {
    assert( jobj_ != 0 ) ;
    assert( other.jobj_ != 0 ) ;
    std::swap( jobj_, other.jobj_ ) ;
  }

  JsonEntry JsonEntry::operator[](const std::string& key) const {
    assert(jobj_ != 0);
    struct json_object *jobj = ::json_object_object_get(jobj_, key.c_str());
    if (jobj == 0)
      throw std::runtime_error("key: " + key + " is not found in object");
    return JsonEntry(jobj);
  }

  std::ostream& operator<<(std::ostream& os, const JsonEntry& jsonEntry) {
    assert(jsonEntry.jobj_ != 0);
    return os << ::json_object_to_json_string(jsonEntry.jobj_);
  }

  template <>
  std::string JsonEntry::Value<std::string>()  const {
    assert(jobj_ != 0);
    return ::json_object_get_string(jobj_);
  }

  template <>
  unsigned int JsonEntry::Value<unsigned int>()  const {
    assert(jobj_ != 0);
    return ::json_object_get_int(jobj_);
  }

  template <>
  JsonEntry::list JsonEntry::Value<JsonEntry::list>() const {
    std::size_t count = ::json_object_array_length(jobj_);
    list result;
    for (std::size_t i = 0 ; i < count ; ++i)
      result.push_front(JsonEntry(::json_object_array_get_idx(jobj_, i)));
    return result;
  }

  JsonEntry::~JsonEntry() {
    assert(jobj_ != 0);
    if (jobj_ != 0)
      ::json_object_put(jobj_);
  }
}  // namespace by
