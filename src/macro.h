/*
 * Copyright (c) 2014 loujiayu
 * This file is released under the MIT License
 * http://opensource.org/licenses/MIT
 */

#ifndef SRC_MACRO_H_
#define SRC_MACRO_H_

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
            TypeName(const TypeName&) = delete;\
            void operator=(const TypeName&) = delete;
#endif

#endif  // SRC_MACRO_H_
