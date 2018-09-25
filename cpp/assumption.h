#ifndef _GOS_ASSUMPTION_H_
#define _GOS_ASSUMPTION_H_

#ifdef _WIN32
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#else
#include <stdio.h>
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#define _GOS_ASSUMPTION_FAST_ "fast"
#define _GOS_ASSUMPTION_THREAD_SAFE_ "threadsafe"

#endif /* _GOS_ASSUMPTION_H_ */
