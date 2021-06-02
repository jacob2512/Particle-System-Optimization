
#include <cstdio>
#include "CustomAssert.h"

bool HandleAssert(const char* message, const char* file, int line)
{
  fprintf(stderr, "%s(%d): %s\n", file, line, message);
  __debugbreak();
  return true;
}