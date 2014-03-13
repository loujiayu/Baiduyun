/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include <iostream>

#include "pcs.h"
#include "http.h"
#include "jsonentry.h"

namespace by {
  const std::string kTokenURL    = "https://openapi.baidu.com/oauth/2.0/token";

  Pcs::Pcs(const std::string client_id,
      const std::string client_secret) :
    client_id_(client_id),
    client_secret_(client_secret) {}

  void Pcs::Auth(std::string auth_key) {
    std::string post = "grant_type=authorization_code"
            "&code=" + auth_key +
            "&client_id=dOiFFnAqiGhzpsT19ijBqpaM"
            "&client_secret=QVCr57iC3g8AjX5pRlkbSPIrivAtY1BE"
            "&redirect_uri=oob";

    JsonEntry resp = JsonEntry::Parse(HttpPostData(kTokenURL, post));
    // JsonEntry resp (HttpPostData(token_url, post));
    access_token_  = resp["access_token"].Value<std::string>();
    std::cout << access_token_ << std::endl;
  }

  std::string Pcs::AuthURL(const std::string&  client_id) {
      return "https://openapi.baidu.com/oauth/2.0/authorize?"
          "scope=netdisk"
          "&redirect_uri=oob"
          "&response_type=code"
          "&client_id="+client_id;
  }

}  // namespace by
