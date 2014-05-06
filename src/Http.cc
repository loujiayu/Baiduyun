/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <stdexcept>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <fstream>
#include <memory>
#include <cstring>

#include "http.h"

namespace by {

HttpException::HttpException(int curl_code, int http_code, const char *err_buf)
  : runtime_error(Format(curl_code, http_code, err_buf)) {
}

std::string HttpException::Format(int curl_code,
                                  int http_code,
                                  const char *err_buf) {
  std::ostringstream ss;

  ss << "CURL code = " << curl_code
     << " HTTP code = " << http_code
     << " (" << err_buf << ")";

  return ss.str();
}

std::size_t WriteCallback(char *data,
                          size_t size,
                          size_t nmemb,
                          std::string *resp) {
  assert(resp != 0);
  assert(data != 0);
  std::size_t count = size * nmemb;
  resp->append(data, count);
  return count;
}

size_t ReadCallback( void *ptr,
                     std::size_t size,
                     std::size_t nmemb,
                     std::string *data ) {
  assert( ptr != 0 ) ;
  assert( data != 0 ) ;
  std::size_t count = std::min( size * nmemb, data->size() ) ;
  if ( count > 0 ) {
    std::memcpy( ptr, &(*data)[0], count ) ;
    data->erase( 0, count ) ;
  }
  return count ;
}

std::size_t Callback(char *data,
                     size_t size,
                     size_t nmemb,
                     std::ofstream *file) {
  assert(data != 0);
  std::size_t count = size * nmemb;

  return file->rdbuf()->sputn( data, count ) ;;
}

CURL* InitCurl(const std::string& url, std::string *resp, const Headers& hdr) {
  CURL *curl = curl_easy_init();
  if (curl == 0)
    throw std::bad_alloc();
  curl_easy_setopt(curl, CURLOPT_URL,             url.c_str());
  curl_easy_setopt(curl, CURLOPT_HEADER,          0);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,  1);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,   WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA,       resp);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER,  0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST,  0L);
  struct curl_slist *curl_hdr = 0;
    for (Headers::const_iterator i = hdr.begin(); i != hdr.end(); ++i)
    curl_hdr = curl_slist_append(curl_hdr, i->c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_hdr);
  return curl;
}

void DoCurl(CURL *curl) {
  char error_buf[CURL_ERROR_SIZE] = {};
  curl_easy_setopt(curl, CURLOPT_ERRORBUFFER,   error_buf);
  CURLcode curl_code = curl_easy_perform(curl);
  int http_code = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
  curl_easy_cleanup(curl);
  if (curl_code != CURLE_OK) {
    throw HttpException(curl_code, http_code, error_buf);
  } else if (http_code >= 400) {
    std::cout << "http error " << http_code << std::endl;
    throw HttpException(curl_code, http_code, error_buf);
  }
}

std::string HttpPostData(
    const std::string& url,
    const std::string& data,
    const Headers& hdr) {
  std::string resp;
  CURL *curl = InitCurl(url, &resp, hdr);
  std::string post_data = data;
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS,    &post_data[0]);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.size());
  curl_easy_setopt(curl, CURLOPT_VERBOSE,     1 ) ;
  DoCurl(curl);
  return resp;
}

std::string HttpGet(
    const std::string& url,
    const Headers& hdr) {
  std::string resp;
  CURL *curl = InitCurl(url, &resp, hdr);
  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
  curl_easy_perform(curl);
  return resp;
}

void HttpGetFile(
  const std::string& url,
  std::ofstream *mfile,
  const Headers& hdr) {
  CURL *curl = InitCurl(url, 0, hdr);
  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA,     mfile);
  curl_easy_perform(curl);
}

std::string Put(const std::string& url,const std::string& data,const Headers& hdr) {
  std::string resp ;
  CURL *curl = InitCurl( url, &resp, hdr ) ;

  std::string put_data = data ;

  curl_easy_setopt(curl, CURLOPT_UPLOAD,      1L ) ;
  curl_easy_setopt(curl, CURLOPT_READFUNCTION,  &ReadCallback ) ;
  curl_easy_setopt(curl, CURLOPT_READDATA ,   &put_data ) ;
  curl_easy_setopt(curl, CURLOPT_INFILESIZE,    put_data.size() ) ;
  curl_easy_setopt(curl, CURLOPT_VERBOSE,     0 ) ;
  curl_easy_perform(curl);
  return resp;
}

std::string Customizing(
    const std::string& url,
    const std::string& data,
    const Headers& hdr) {
  std::string resp;
  CURL *curl = InitCurl(url, &resp, hdr);
  std::string post_data = data;
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_VERBOSE,     1 ) ;
  DoCurl(curl);
  return resp;
}

}  // namespace by
