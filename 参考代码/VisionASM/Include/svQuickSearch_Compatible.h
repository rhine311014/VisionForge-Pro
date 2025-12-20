#pragma once

#include "svDef.h"
#include "svRect.h"
#include "svAffineRect.h"
#include "svImage.h"
#include "svPose.h"
#include "svfeature.h"
#include "svXMLConfigurator.h"
#pragma pack(push,8)

class scQuickSearchTool_Compatible;

class SVQUICKSEARCH_API scQuickSearchResult_Compatible : public scPose
{
public:
	scQuickSearchResult_Compatible();

	double		GetScore() { return m_dScore; };

	double	 GetSizeX() { return m_dSizeX; };
	double	 GetSizeY() { return m_dSizeY; };

	double	 GetContrast() { return m_dContrast; };

	scRect		 MatchRect()   { return m_MatchRect; };
	scAffineRect MatchRegion() { return m_MatchRegion; };

	sc2Vector	 GetAuxiliaryModelOrigin1() { return m_AuxiliaryModelOrigin1; }; 
	sc2Vector	 GetAuxiliaryModelOrigin2() { return m_AuxiliaryModelOrigin2; }; 

public:
	double	  m_dScore;
	double	  m_dSizeX;
	double	  m_dSizeY;
	double	  m_dContrast;
	scRect	  m_MatchRect;
	scAffineRect m_MatchRegion;

	sc2Vector m_AuxiliaryModelOrigin1;
	sc2Vector m_AuxiliaryModelOrigin2;
};

class SVQUICKSEARCH_API scQuickSearchTool_Compatible
{
public:
	scQuickSearchTool_Compatible();
	~scQuickSearchTool_Compatible();

public:
	// 设置/获取颗粒度
	bool	SetGranularity(double dGranularity);
	double	GetGranularity();
	bool	SetGranularityFine(double dGranularityFine);
	double	GetGranularityFine();

	// 设置/获取噪声阈值
	bool	SetNoiseThre(unsigned char ucNoiseThre);
	int		GetNoiseThre();

	// 设置/获取梯度幅值和阈值
	bool	SetTotalGradMagThre(long lTotalGradMagThre);
	long	GetTotalGradMagThre();

	// 设置/获取2个辅助的模板标记点
	void	SetAuxiliaryModelOrigin(sc2Vector  origin1, sc2Vector  origin2);
	void	GetAuxiliaryModelOrigin(sc2Vector &origin1, sc2Vector &origin2);

	// 训练
	bool	Train(const scImageGray& image, scAffineRect affineRect, double dModelOriginX, double dModelOriginY);
	bool	Train(const scImageGray& image, const scImageGray& maskimage, scAffineRect affineRect, double dModelOriginX, double dModelOriginY);
	bool	Train(const scImageGray& image, const scShape* roi, double dModelOriginX, double dModelOriginY);
	bool	Train(const scImageGray& image, const scImageGray& maskimage, const scShape* roi, double dModelOriginX, double dModelOriginY);

	// 模板链集是否可编辑	
	bool    TrainFeaturesEditable();

	// 获取训练模板特征对应的链集
	bool    GetTrainFeatures(scFeatureletChainSet& featureChainSet);
	bool    GetTrainFeaturesFine(scFeatureletChainSet& featureChainSet);

	bool    SetEditedFeatures(const scFeatureletChainSet& featureChainSet);
	bool    SetEditedFeaturesFine(const scFeatureletChainSet& featureChainSet);

	// 获取训练参考点
	bool	GetModelOrigin(sc2Vector& origin);

	// 获取训练结果图像
	bool	GetTrainResultImage(scImageGray& image);

	// 获取训练模板图像
	bool	GetTrainModelImage(scImageGray& image);

	// 获取精细训练模板图像
	bool	GetTrainModelImageFine(scImageGray& image);

	// 获取训练区域
	scRect		 GetTrainRectRegion();
	scAffineRect GetTrainAffineRectRegion();

	// 保存/载入训练结果
	bool	SaveTrainModelToFile(const TCHAR* strFilePath);
	bool	LoadTrainModelFromFile(const TCHAR* strFilePath);

	bool	SaveTrainModelToBinaryFile(const TCHAR* strFilePath);
	bool	LoadTrainModelFromBinaryFile(const TCHAR* strFilePath);

public:
	// 设置rotate广义自由度
	bool	SetDofNominalRotate(scDegree rotate);
	bool	SetDofZoneRotate(scDegree low, scDegree high, scDegree dupRange);

	// 设置size广义自由度
	bool	SetDofNominalSize(double size);
	bool	SetDofZoneSize(double low, double high, double dupRange);

	// 设置sizex广义自由度
	bool	SetDofNominalSizeX(double sizex);
	bool	SetDofZoneSizeX(double low, double high, double dupRange);

	// 设置sizey广义自由度
	bool	SetDofNominalSizeY(double sizey);
	bool	SetDofZoneSizeY(double low, double high, double dupRange);

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

	// 设置是否忽略极性
	void	SetIgnorePolarity(bool bIgnore);
	bool	GetIgnorePolarity();

	// 是否优化精度
	void	EnableRefinement(bool bEnable = true);
	bool	GetIsRefinement();

	// 是否优化速度
	void	EnableSpeedUp(bool bEnable = true);
	bool	GetIsSpeedUp();

	// 是否采用高精度算法
	void	EnableAlgoAccu(bool bEnable = true);
	bool	GetIsAlgoAccu();

	// 定位
	bool	Execute(const scImageGray& image, scRect roi);
	bool	Execute(const scImageGray& image, const scShape* roi = NULL);
	bool	Execute(const scImageGray& image, const scImageGray& imagemask, const scShape* roi = NULL);

	// 获取定位结果个数
	long	GetResultNum();

	// 获取序号为index的定位结果
	scQuickSearchResult_Compatible	GetResult(long index);

private:
	void*	m_hQuickSearch;
	void*	m_hQuickSearchFine;
	void*	m_hQuickSearchAccu;
	long	m_lAcceptNum;
	bool	m_bRefinement;
	bool	m_bSpeedUp;
	bool	m_bEnableAccu;
	svStd vector<scQuickSearchResult_Compatible> m_pResultArray;
	double  m_dOriginX1;
	double  m_dOriginY1;
	double  m_dOriginX2;
	double  m_dOriginY2;
};

#pragma pack(pop)
