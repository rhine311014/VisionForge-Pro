#pragma once
#include "vpImage.h"
#pragma pack(push,8)

//速度等级(越大越快)
enum SpeedLevel
{
	eSpeedLevel0=0,
	eSpeedLevel1,
	eSpeedLevel2,
};

HStatus SVCORE_API sfTranspose(const cpImage& srcimage,cpImage& dstimage,SpeedLevel speedlevel = eSpeedLevel2);

HStatus SVCORE_API sfRotate2(const cpImage& srcimage,cpImage& dstimage,RotateType type,SpeedLevel speedlevel = eSpeedLevel2);

HStatus SVCORE_API sfMirror2(const cpImage& srcimage,cpImage& dstimage,MirrorType type,SpeedLevel speedlevel = eSpeedLevel2);

HStatus SVCORE_API sfRotateMirror2(const cpImage& srcimage,cpImage& dstimage,RotateType typeR,MirrorType typeM,SpeedLevel speedlevel = eSpeedLevel2);

HStatus SVCORE_API sfRotateMirror2_3D(const cpImage& srcimage,cpImage& dstimage,RotateType typeR,MirrorType typeM,SpeedLevel speedlevel = eSpeedLevel2);

#pragma pack(pop)