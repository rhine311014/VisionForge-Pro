#pragma once

#include "svDef3D.h"
#include "svVision3DDef.h"

#include "svMath.h"

#pragma pack(push,8)

// 双线性插值（涉及无效像素处理）
// 输入：lt，rt，lb，rb为插值点周围四个点的像素值
//		dPosX, dPosY为插值点坐标
// 输出：插值后的像素值

SV_VISION_3D_API double sfBilinearInterpolation(WORD lt, WORD rt, WORD lb, WORD rb, double dPosX, double dPosY);