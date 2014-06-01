Baiduyun
========
Introduction
--------------------------------------------------
Linux client for Baidu Yun.It will download or upload all the files locally on your computer or in Baidu Yun respectively.
Besides, it will detect files which need to be downloaded or uploaded when you run sync function.

Notice:
The root directory is /apps/ldrive(/我的应用数据/ldrive), so the first time run baiduyun please add option "-a" to authenticate your account and creat the root directory.
The Maximum size of a uploading file is 2GB.

Dependencies
--------------------------------------------------
You may need following libraries:
 - Boost(Boost filesystem)
 - json-c(https://github.com/json-c/json-c)
 - openssl
 - cmake
 - libcurl

Installation
--------------------------------------------------
```bash
$mkdir build
$cd build
$cmake ..
$make
```
