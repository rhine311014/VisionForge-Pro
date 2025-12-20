#pragma once

#include "svDef.h"
#include "svError.h"
#include "svAngle.h"
#include "svMath.h"
#include "svAffineRect.h"
#include "svAffsampl.h"
#include "svImage.h"

#pragma pack(push,8)

HStatus SVCORE_API sfProjectImage(const scImageGray& srcImage, scImage32&  dstImage, const scAffineSamplingParams& params);

HStatus SVCORE_API sfProjectImage(const scImageGray& srcImage, scImage32&  dstImage, const scAffineSamplingParams& params, scImage32&  weights, bool& clipped);

#pragma pack(pop)

