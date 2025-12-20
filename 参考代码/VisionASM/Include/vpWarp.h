#pragma once
#include "svImage.h"
#include "vpImage.h"
#pragma pack(push,8)

HStatus SVCORE_API sfWarpRotateShift(cpImage& srcimage, cpImage& dstimage, scRadian angle, double aboutx, double abouty, double shiftx, double shifty, unsigned char clipvalue = 0, InterpolationType type = Linear);

HStatus SVCORE_API sfWarpRotateShift2(const scImageGray& srcimage, scImageGray& dstimage, scRadian angle, double aboutx, double abouty, double shiftx, double shifty, unsigned char clipvalue = 0, InterpolationType type = Linear);
HStatus SVCORE_API sfWarpRotateShift2(cpImage& srcimage, cpImage& dstimage, scRadian angle, double aboutx, double abouty, double shiftx, double shifty, unsigned char clipvalue = 0, InterpolationType type = Linear);

HStatus SVCORE_API sfWarpAffine(cpImage& srcimage, cpImage& dstimage, const double coeffs[2][3], unsigned char clipvalue = 0, InterpolationType type = Linear);

HStatus SVCORE_API sfWarpAffine2(cpImage& srcimage, cpImage& dstimage, const double coeffs[2][3], unsigned char clipvalue = 0, InterpolationType type = Linear);

#pragma pack(pop)