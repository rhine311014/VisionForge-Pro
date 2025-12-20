#pragma once

#include "svDef.h"
#include "svImage.h"
#include "vpImage.h"

#pragma pack(push,8)

///////////////////////// 灰度形态学操作函数集 05/12/23 ////////////////////////////////

//灰度膨胀
SVCORE_API BOOL sfGrayDilate(const scImageGray& image,MorphStructType iStrucType,scImageGray& dstimage);
SVCORE_API BOOL sfGrayDilate(cpImage& image,MorphStructType iStrucType,cpImage& dstimage);
//灰度腐蚀
SVCORE_API BOOL sfGrayErode(const scImageGray& image, MorphStructType iStrucType,scImageGray& dstimage);
SVCORE_API BOOL sfGrayErode(cpImage& image, MorphStructType iStrucType,cpImage& dstimage);
//形态学梯度
SVCORE_API BOOL sfMorphGrad(const scImageGray& image, MorphStructType iStrucType,scImageGray& dstimage);
SVCORE_API BOOL sfMorphGrad(cpImage& image, MorphStructType iStrucType,cpImage& dstimage);
//灰度开启
SVCORE_API BOOL sfGrayOpen(const scImageGray& image, MorphStructType iStrucType,scImageGray& dstimage);
SVCORE_API BOOL sfGrayOpen(cpImage& image, MorphStructType iStrucType,cpImage& dstimage);
//灰度闭合
SVCORE_API BOOL sfGrayClose(const scImageGray& image, MorphStructType iStrucType,scImageGray& dstimage);
SVCORE_API BOOL sfGrayClose(cpImage& image, MorphStructType iStrucType,cpImage& dstimage);
//白色Top-hat变换
SVCORE_API BOOL sfWhTophat(const scImageGray& image, MorphStructType iStrucType,scImageGray& dstimage);
SVCORE_API BOOL sfWhTophat(cpImage& image, MorphStructType iStrucType,cpImage& dstimage);
//黑色Top-hat变换
SVCORE_API BOOL sfBlkTophat(const scImageGray& image, MorphStructType iStrucType,scImageGray& dstimage);
SVCORE_API BOOL sfBlkTophat(cpImage& image, MorphStructType iStrucType,cpImage& dstimage);
//中值滤波
SVCORE_API BOOL sfMedian(const scImageGray& image,scImageGray& dstimage);
SVCORE_API BOOL sfMedian(cpImage& image,cpImage& dstimage);

#pragma pack(pop)
