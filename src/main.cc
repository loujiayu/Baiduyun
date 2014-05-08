/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <getopt.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include "pcs.h"
#include "jsonentry.h"
#include "filetrans.h"
#include "filesystem.h"
#include "config.h"

int main(int argc, char *argv[]) {
  using namespace by;
  JsonEntry config = ReadConfig();
  int c;
  const std::string kClientID  = "dOiFFnAqiGhzpsT19ijBqpaM";
  const std::string kClientSecret  = "QVCr57iC3g8AjX5pRlkbSPIrivAtY1BE";
  while ((c = getopt(argc, argv, "a")) != -1) {
    switch (c) {
      case 'a': {
        std::cout
          << "-----------------------\n"
          << "Please go to this URL and get an authenication code:\n\n"
          << Pcs::AuthURL(kClientID)
          << std::endl;

        std::cout
          << "\n-----------------------\n"
          << "Please input the authenication code here" << std::endl;
        std::string code;
        std::cin >> code;

        Pcs baidu_oauth(kClientID, kClientSecret);
        baidu_oauth.Auth(code);
        config.Add("access_token", JsonEntry(baidu_oauth.access_token()));
        assert(config["access_token"].Value<std::string>() ==
          baidu_oauth.access_token());
        SaveConfig(config);
      }
    }
  }
  FileTrans ft(config["access_token"].Value<std::string>());
  std::string p = fs::current_path().string() + "/Baidu_Yun";
  ft.Syn(p);
  return 0;
}
