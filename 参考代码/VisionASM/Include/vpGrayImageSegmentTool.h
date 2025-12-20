#pragma once

#include "svDef.h"
#include "vpImage.h"

#pragma pack(push,8)

class SVCORE_API cpGrayImageSegmentTool
{
public:
	cpGrayImageSegmentTool(void);
	virtual ~cpGrayImageSegmentTool(void);

	//二值化灰度图像img，取usTherLow~usTherHigh之间为前景，其他为背景
	//输出二值图像保存到img
	bool Execute(cpImage& img, unsigned short usTherLow = 0, unsigned short usTherHigh = 128, unsigned short usBackground = 0, unsigned short usForeground = 255);
	
	//二值化灰度图像imgSrc，取usTherLow~usTherHigh之间为前景，其他为背景
	//输出二值图像保存到imgDst，imgDst由调用人员初始化格式和宽高
	bool Execute(const cpImage& imgSrc, cpImage& imgDst, unsigned short usTherLow = 0, unsigned short usTherHigh = 128, unsigned short usBackground = 0, unsigned short usForeground = 255);
};

#pragma pack(pop)