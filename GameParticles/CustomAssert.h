#pragma once

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include<windows.h>


#define _CORE_TO_STRING(x)  #x
#define CORE_TO_STRING(x) _CORE_TO_STRING(x)

#ifdef NDEBUG

#define ASSERT(expression) ((void)0)

#else

bool HandleAssert(const char* message, const char* file, int line);

#define ASSERT(expression) (void)(                                                       \
            (!!(expression)) ||                                                              \
            ( HandleAssert(CORE_TO_STRING(expression), __FILE__, __LINE__ ) ) \
        )

#endif