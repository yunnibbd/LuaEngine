#ifndef __UTILS_H__
#define __UTILS_H__
#include "cbuffer.h"

char* LoadFileContent(const char* file_name);

CBuffer LoadFileContentToCBuffer(const char* file_name);

#endif
