// svInterlock.h

#pragma once

#include "svdef.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            锁定计数
// ---------------------------------------------------------------------

// 锁定*p并加1，返回值是*p加1后的值
SVCORE_API long sfInterlockedIncrement(volatile long* p);

// 锁定*p并减1，返回值是p减1后的值
SVCORE_API long sfInterlockedDecrement(volatile long* p);

// 锁定*p并将v赋给*p，返回值是对*p赋值之前的*p值
SVCORE_API long sfInterlockedExchange(volatile long* p, long v);

#pragma pack(pop)

