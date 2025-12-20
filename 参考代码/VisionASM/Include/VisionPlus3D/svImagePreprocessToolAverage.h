#pragma once
#include "svImagePreprocessToolBase.h"

#pragma pack(push,8)

// 图像平均化工具
class SV_VISION_3D_API scImagePreprocessToolAverage : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolAverage();
	virtual ~scImagePreprocessToolAverage();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessAverage;}  // 获取预处理类型

	// 设置/获取平均化模板大小
	void SetStructSize(int nStructSize);
	unsigned int GetStructSize()const;

	// 设置/获取平均化次数
	void SetAverageTimes(int nAverageTimes);
	unsigned int GetAverageTimes()const;

	// 设置/获取平均化模板方向
	void SetAverageDirecton(int nDirection);
	int GetAverageDirecton()const;

	// 执行预处理
	// 8位图像不考虑是否涉及无效像素及膨胀控制，16位图像需考虑是否涉及无效像素及膨胀控制 
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);

private:
	bool GetAverageXGray8(cpImage& src,cpImage& dst,int nSize)const;
	bool GetAverageYGray8(cpImage& src,cpImage& dst,int nSize)const;
	bool GetAverageXYGray8(cpImage& src,cpImage& dst,int nSize)const;
	bool GetAverageXGray16(cpImage& src,cpImage& dst,int nSize,bool bInvalidControl,bool bExpand)const;
	bool GetAverageYGray16(cpImage& src,cpImage& dst,int nSize,bool bInvalidControl,bool bExpand)const;
	bool GetAverageXYGray16(cpImage& src,cpImage& dst,int nSize,bool bInvalidControl,bool bExpand)const;

private:
	int m_nStructSize;		// 平均化模板大小
	int m_nAverageTimes;	// 平均化次数
	int m_nDirection;		// 平均化模板方向
};

#pragma pack(pop)