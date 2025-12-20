#pragma once
#include "svZeroPlaneResult.h"
#include "svVision3DToolBase.h"

#pragma pack(push,8)

// 图像范围生成工具
class SV_VISION_3D_API scImageRegionGeneratorTool : public scVision3DToolBase
{
public:
	scImageRegionGeneratorTool();
	virtual ~scImageRegionGeneratorTool();

	enum ExtractHeight
	{
		eHighPart = 0, // 高
		eLowPart,      // 低
		eBothPart      // 高低
	};

	enum InvalidPixel
	{
		eDefault = 0,  // 既定值
		eBackground,   // 背景值
		eSpecified     // 指定值
	};

	// 设置/获取抽取面
	void SetExtractPlane(const scZeroPlaneResult& zeroplane);
	scZeroPlaneResult GetExtractPlane()const;

	// 设置/获取增益
	void SetGrayGain(unsigned int unGrayGain);
	unsigned int GetGrayGain() const;

	// 设置/获取抽取高度模式
	void SetExtractHeight(ExtractHeight extractHeight);
	ExtractHeight GetExtractHeight() const;

	// 设置/获取无效像素控制模式
	void SetInvalidPixel(InvalidPixel invalidPixel);
	InvalidPixel GetInvalidPixel() const;

	// 设置/获取无效像素指定值
	void SetInvalidSpecifiedValue(unsigned char nValue);
	unsigned char GetInvalidSpecifiedValue() const;

	// 设置/获取除去干扰
	void SetNoiseCut(double dNoiseCut);
	double GetNoiseCut() const;

	// 设置/获取Z偏移
	void SetOffsetZ(double dOffsetZ);
	double GetOffsetZ() const;

	// 执行图像范围生成
	virtual bool Excute(cpImage& image);

	// 获取结果图像
	const cpImage* GetResultImage()const;

private:
	scZeroPlaneResult m_ZeroPlaneResult;

	unsigned int  m_unGrayGain;  // 增益(阶/mm)

	ExtractHeight  m_ExtractHeight; // 抽取高度
	InvalidPixel   m_InvalidPixel;  // 无效像素控制
	unsigned char  m_lInvalidSpecifiedValue; // 无效像素指定值

	double m_dNoiseCut;   // 除去干扰 (mm)
	double m_dOffsetZ;    // Z偏移 (mm)

	cpImage m_ImgResult;
};

#pragma pack(pop)
