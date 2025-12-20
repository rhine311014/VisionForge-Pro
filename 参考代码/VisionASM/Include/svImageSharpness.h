#pragma once

#include "svDef.h"
#include "vpImage.h"

#pragma pack(push,8)

class SVCORE_API scImageSharpness
{
public:
	scImageSharpness();
	virtual ~scImageSharpness();

	//计算图像清晰度,范围0~1000,一般大于80可认为图像比较清晰
	bool Execute(const scImage8& image, const scRect& roi, double& dImageSharpness);
	bool Execute(cpImage& image, const scRect& roi, double& dImageSharpness);

};

#pragma pack(pop)
