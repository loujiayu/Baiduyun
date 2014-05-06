/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#include <string>

namespace by {
class JsonEntry;

const std::string& ConfigFilename();
JsonEntry ReadConfig();
void SaveConfig(const JsonEntry& config);

}  // namespace by


#endif  // SRC_CONFIG_H_
