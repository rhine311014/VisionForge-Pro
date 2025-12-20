#pragma once

#include "svDef.h"
#include "svRect.h"
#include "svAffineRect.h"
#include "svImage.h"
#include "svPose.h"

#pragma pack(push,8)

class scFineSearchTool;

class SVFINESEARCH_API scFineSearchResult : public scPose
{
public:
	scFineSearchResult();

	double		GetScore() { return m_dScore; };

	double	 GetContrast() { return m_dContrast; };

	scRect		 MatchRect()   { return m_MatchRect; };
	scAffineRect MatchRegion() { return m_MatchRegion; };

public:
	double	  m_dScore;
	double	  m_dContrast;
	scRect	  m_MatchRect;
	scAffineRect m_MatchRegion;
};

class SVFINESEARCH_API scFineSearchTool
{
public:
	scFineSearchTool();
	~scFineSearchTool();

public:
	// 设置/获取颗粒度
	bool	SetGranularity(double dGranularity);
	double	GetGranularity();

	// 设置/获取噪声阈值
	bool	SetNoiseThre(unsigned char ucNoiseThre);
	int		GetNoiseThre();

	// 设置/获取梯度幅值和阈值
	bool	SetTotalGradMagThre(long lTotalGradMagThre);
	long	GetTotalGradMagThre();

	// 训练
	bool	Train(const scImageGray& image, scAffineRect affineRect, double dModelOriginX, double dModelOriginY);

	// 获取训练参考点
	bool	GetModelOrigin(sc2Vector& origin);

	// 获取训练结果图像
	bool	GetTrainResultImage(scImageGray& image);

	// 获取训练模板图像
	bool	GetTrainModelImage(scImageGray& image);

	// 获取训练区域
	scRect		 GetTrainRectRegion();
	scAffineRect GetTrainAffineRectRegion();

	// 保存/载入训练结果
	bool	SaveTrainModelToFile(const TCHAR* strFilePath);
	bool	LoadTrainModelFromFile(const TCHAR* strFilePath);

public:
	// 设置rotate广义自由度
	bool	SetDofNominalRotate(scDegree rotate);
	bool	SetDofZoneRotate(scDegree low, scDegree high, scDegree dupRange);

	// 设置size广义自由度
	bool	SetDofNominalSize(double size);
	bool	SetDofZoneSize(double low, double high, double dupRange);

	// 设置评价分数阈值
	bool	SetAcceptThreshold(double accept);
	double	GetAcceptThreshold() const;

	// 设置对比度阈值
	bool	SetContrastThreshold(long contrast);
	long	GetContrastThreshold() const;

	// 设置重叠阈值
	bool	SetOverlapThreshold(double overlap);
	double	GetOverlapThreshold() const;

	// 设置目标个数
	bool    SetNumToFind(long n);
	long	GetNumToFind() const;

	// 定位
	bool	Execute(const scImageGray& image, scRect roi);

	// 获取定位结果个数
	long	GetResultNum();

	// 获取序号为index的定位结果
	scFineSearchResult	GetResult(long index);

private:
	void*	m_hFineSearch;
	long	m_lAcceptNum;
	svStd vector<scFineSearchResult> m_pResultArray;
};

#pragma pack(pop)
