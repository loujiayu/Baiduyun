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
$ cd Baiduyun
$ mkdir build
$ cd build
$ cmake ..
$ make
```

if you need google test:
```bash
$ cmake -test=ON ..
$ make
$ make test
```

The first time run baiduyun, you should use option "-a" to get permission to Baidu Yun. A URL should be printed, you should goto that link and copy-paste the authenication code to the command line. IF everything works fine. A file named .baiduyun will be created in your home directory,your authenication code is stored in it, and you should update this code per month(run ./baiduyun -a again).


 - This is a very experimental version. So any bugs appreciate.
