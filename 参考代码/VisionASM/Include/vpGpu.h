#pragma once

#include "svDef.h"

#ifndef _NOGPU
SVCORE_API bool CreateGpuDevice();
SVCORE_API void ReleaseGpuDevice();
#endif