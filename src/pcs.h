/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_PCS_H_
#define SRC_PCS_H_

#include <string>

#include "macro.h"

namespace by {

class Pcs {
 public:
  Pcs(const std::string client_id,
    const std::string client_secret);

  static std::string AuthURL(const std::string&  client_id);

  void  Auth(std::string auth_key);

  std::string access_token() const { return access_token_; }

 private:
  std::string access_token_;
  const std::string  client_id_;
  const std::string  client_secret_;

  DISALLOW_COPY_AND_ASSIGN(Pcs);
};

}  // namespace by

#endif  // SRC_PCS_H_
