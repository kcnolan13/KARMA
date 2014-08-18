/*
 * hash_fun.h
 *
 *  Created on: Jun 12, 2009
 *      Author: lmiller
 */

#ifndef HASH_FUN_H_
#define HASH_FUN_H_

#define STL_USING_STRING
#include "stl.h"

#include <iostream>
#include <cstring>
#include <string>
using namespace std;



inline long hash_fun1(const std::string& c, const long prime)
{
      int n = c.length();
      const char* d = c.c_str();
      long h = 0;

      for (int i = 0; i < n; ++i, ++d)
          h = (h << 2) + *d;

      return ((h >= 0) ? (h % prime) : (-h % prime));
}

inline long hash_fun2(const std::string& c, const long prime)
{
      int n = c.length();
      const char* d = c.c_str();
      long h = n;

      for (int i = 0; i < n; ++i, ++d)
          h = 613*h + *d;

      return ((h >= 0) ? (h % prime) : (-h % prime));
}

#endif /* HASH_FUN_H_ */
