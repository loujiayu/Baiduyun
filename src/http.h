/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_HTTP_H_
#define SRC_HTTP_H_

#include <curl/curl.h>
#include <string>
#include <vector>
#include <stdexcept>

namespace by {

class HttpException : public std::runtime_error {
 public:
  HttpException(int curl_code, int http_code, const char *err_buf);

 private:
  static std::string Format(int curl_code, int http_code, const char *err_buf);
};

typedef std::vector<std::string> Headers;

CURL* InitCurl(
  const std::string& url,
  std::string *resp,
  const Headers& hdr);

void DoCurl(CURL *curl);

std::string HttpPostData(
  const std::string& url,
  const std::string& data,
  const Headers&     hdr = Headers());

void HttpGetFile(
  const std::string& url,
  std::ofstream *mfile,
  const Headers&     hdr = Headers());

std::string HttpGet(const std::string& url,
                    const Headers&     hdr = Headers());

}  // namespace by

#endif  // SRC_HTTP_H_
