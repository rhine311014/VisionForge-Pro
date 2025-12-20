#pragma once

#include "svDef.h"
#include "svImage.h"
#include "vpImage.h"

#pragma pack(push,8)

/////////////////////////////// 滤波操作 //////////////////////////////////////

//低通滤波
//执行3X3均值滤波或加权均值滤波操作，结果图像覆盖原图像
//bWeight为FALSE时为平均滤波器，bWeight为TRUE时为加权平均滤波器
SVCORE_API BOOL sfLowPassFilter(scImageGray& image,scImageGray& dstimage, BOOL bWeight = FALSE);
SVCORE_API BOOL sfLowPassFilter(cpImage& image,cpImage& dstimage, BOOL bWeight = FALSE);

//高通滤波
//执行3X3高通滤波操作，结果图像覆盖原图像
//b8Neighbor为FALSE时为4-邻域高通滤波器，b8Neighbor为TRUE时为8邻域高通滤波器
SVCORE_API BOOL sfHighPassFilter(scImageGray& image,scImageGray& dstimage, BOOL b8Neighbor = TRUE);
SVCORE_API BOOL sfHighPassFilter(cpImage& image,cpImage& dstimage, BOOL b8Neighbor = TRUE);

//用户自定义滤波
//执行iWndMXiWndN用户自定义滤波操作，结果图像覆盖原图像
SVCORE_API BOOL sfCustomFilter(scImageGray& image, LPINT piFiltCoef, int iWndM, int iWndN,scImageGray& dstimage);
SVCORE_API BOOL sfCustomFilter(cpImage& image, LPINT piFiltCoef, int iWndM, int iWndN,scImageGray& dstimage);

//一阶边缘检测(Prewitt算子)
//处理过程中对边缘检测结果取绝对值，损失了符号信息，边缘幅值图覆盖原图
//根据我们规定的坐标系，垂直边缘图像由X方向梯度图像构成，水平边缘图像由Y方向梯度图像构成。
SVCORE_API BOOL sfPrewtEdgeDetect(scImageGray& image, scImageGray& imageHor, scImageGray& imageVer,scImageGray& dstimage);
SVCORE_API BOOL sfPrewtEdgeDetect(cpImage& image, cpImage& imageHor, cpImage& imageVer,cpImage& dstimage);

//一阶边缘检测(sobel算子)
//处理过程中对边缘检测结果取绝对值，损失了符号信息，边缘幅值图覆盖原图
//根据我们规定的坐标系，垂直边缘图像由X方向梯度图像构成，水平边缘图像由Y方向梯度图像构成。
SVCORE_API BOOL sfSobelEdgeDetect(scImageGray& image, scImageGray& imageHor, scImageGray& imageVer,scImageGray& dstimage,double scale = 1.0);
SVCORE_API BOOL sfSobelEdgeDetect(cpImage& image, cpImage& imageHor, cpImage& imageVer,cpImage& dstimage,double scale = 1.0);

//二阶边缘检测
//处理过程中对边缘检测结果取绝对值，损失了符号信息，Laplacian边缘检测结果图像(覆盖原图)
//b8Neighbor为FALSE时为4-邻域Laplacian 算子，b8Neighbor为TRUE时为8邻域Laplacian 算子
SVCORE_API BOOL sfLapEdgeDetect(scImageGray& image,scImageGray& dstimage, BOOL b8Neighbor = TRUE);
SVCORE_API BOOL sfLapEdgeDetect(cpImage& image,cpImage& dstimage, BOOL b8Neighbor = TRUE);

//快速平均滤波
//执行iWndMXiWndN窗口快速平均滤波
SVCORE_API BOOL sfFastAvgFilter(scImageGray& image, int iWndM, int iWndN,scImageGray& dstimage);
SVCORE_API BOOL sfFastAvgFilter(cpImage& image, int iWndM, int iWndN,cpImage& dstimage);

//快速Gaussian滤波
SVCORE_API BOOL sfFastGaussFilter(scImageGray& image, double dbSigmaX, double dbSigmaY,scImageGray& dstimage);
SVCORE_API BOOL sfFastGaussFilter(cpImage& image, double dbSigmaX, double dbSigmaY,cpImage& dstimage);

#pragma pack(pop)
