#ifndef CountAssert

#ifndef NDEBUG
#include <map>
#include <string>
#include <assert.h>
#include <iostream>

#define CountAssert(count) static std::map<std::string, int> countAssertMap; assert("Should be called only specified times:") \
&& ++countAssertMap[std::string(__FILE__) + std::to_string(__LINE__)] <= count)
#else
#define CountAssert(count) ((void)0)
#endif

#endif