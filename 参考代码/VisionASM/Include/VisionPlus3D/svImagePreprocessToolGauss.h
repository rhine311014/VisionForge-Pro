#pragma once

#include "svImagePreprocessToolBase.h"

#pragma pack(push, 8)

// 图像高斯
class SV_VISION_3D_API scImagePreprocessToolGauss : public scImagePreprocessToolBase
{
public:
	scImagePreprocessToolGauss();
	virtual ~scImagePreprocessToolGauss();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessGauss;}  // 获取预处理类型

	// 设置/获取高斯模板大小
	void SetStructSize(int nStructSize);
	unsigned int GetStructSize()const;

	// 设置/获取高斯模板方向
	void SetStructDirection(int nStructDirection);
	unsigned int GetStructDirection()const;

	// 设置/获取高斯次数
	void SetGaussTimes(int nGaussTimes);
	unsigned int GetGaussTimes()const;

	// 设置/获取sigma
	void SetSigma(double dSigma);
	double GetSigma()const;

	// 执行预处理
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);
private:
	void GenerateGaussMask(long long* pMask, int size, double sigma, int direction,long long& coefficient);

private:
	int m_nStructSize;		// 高斯模板大小
	int m_nStructDirection;	// 高斯方向：X, Y, XY
	int m_nGaussTimes;		// 高斯次数
	double m_dSigma;		// sigma
};
#pragma pack(pop)