#pragma once

#include "svDef.h"
#include "svError.h"
#include "svVision3DDef.h"
#include "vpImage.h"

#pragma pack(push,8)

// 24/32位融合图像按照先镜像后旋转的顺序进行变换
// 旋转角度：0°，90°，180°，270°
// 镜像：无，水平镜像，垂直镜像，水平垂直镜像
SV_VISION_3D_API HStatus sf3DRotateMirror(const cpImage& srcimage, cpImage& dstimage, RotateType typeR, MirrorType typeM);

#pragma pack(pop)