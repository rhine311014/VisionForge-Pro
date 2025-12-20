#pragma once

#include "svDef.h"
#include "svPose.h"
#include "svRect.h"
#include "svMaskRect.h"
#include "svAffineRect.h"
#include "svImage.h"
#include "vpImage.h"

#pragma pack(push,8)

class SVPATTERNSEARCH_API scPatternSearchRotResult : public scPose
{
public:
	scPatternSearchRotResult();
	
	double		GetScore() { return m_dScore; };
		
	scRect		 MatchRect()   { return m_MatchRect; };
	scAffineRect MatchRegion() { return m_MatchRegion; };
	
public:
	double	  m_dScore;
	scRect	  m_MatchRect;
	scAffineRect m_MatchRegion;
	double	  m_dGrayMean;
	double	  m_dGrayMeanContrast;
	double	  m_dSharpness;
	double	  m_dSharpnessContrast;
};

class SVPATTERNSEARCH_API scPatternSearchRot
{
public:
	scPatternSearchRot();
	~scPatternSearchRot();

	scPatternSearchRot(const scPatternSearchRot& retTool);
	scPatternSearchRot& operator=(const scPatternSearchRot& retTool);

	long GetTrainMemorySize();
	long GetRunMemorySize();

public:
	bool SetAccuracy(Accuracy accu);
	Accuracy GetAccuracy();

	bool SetCoarsestScale(CoarseScale scale);
	CoarseScale GetCoarsestScale();

	bool Train(const scImageGray& image, sc2Vector origin, const scShape* roi = NULL); 
	bool Train(const scImageGray& image, const scImageGray& maskimage, sc2Vector origin, const scShape* roi = NULL); 
	bool Train(cpImage& image, sc2Vector origin, const scShape* roi = NULL); 
	bool Train(cpImage& image, cpImage& maskimage, sc2Vector origin, const scShape* roi = NULL); 

	bool GetIsUseMask();								//获取是否使用掩模图像
	bool GetTrainImage(scImageGray& image);				//获取训练图像
	bool GetTrainMaskImage(scImageGray& maskimage);		//获取掩模图像
	bool GetPatternOrigin(sc2Vector& origin);			//获取标记点坐标
	scRect	GetTrainRectRegion();						//获取训练区域
	scMaskRect	GetTrainMaskRectRegion();				//获取训练区域
	bool GetIsEnableMaskRectTrain();					//获取是否使用MaskRect训练区域
	bool GetTrainImage(cpImage& image);					//获取训练图像
	bool GetTrainMaskImage(cpImage& maskimage);			//获取掩模图像

	double GetTrainRegionGrayMean();					//获取训练区域灰度均值
	double GetTrainRegionSharpness();					//获取训练区域清晰度

	bool SetDofNominalRotate(scDegree rotate);
	bool SetDofZoneRotate(scDegree low, scDegree high, scDegree dupRange);
	bool IsDofZoneRotate();
	void GetDofNominalRotate(scDegree& rotate);
	void GetDofZoneRotate(scDegree& low, scDegree& high, scDegree& dupRange);

	bool SetDofRotateCycle(int nCycle);
	int	 GetDofRotateCycle();

	bool SetGray16ToGray8(int nGray16ToGray8);
	int	 GetGray16ToGray8();

	bool GetPatternImage(scImageGray& image);
	bool GetPatternImage(cpImage& image);

	bool SaveTrainModelToFile(const TCHAR* strFilePath);
	bool LoadTrainModelFromFile(const TCHAR* strFilePath);
	bool SaveTrainModelToBinaryFile(const TCHAR* strFilePath);
	bool LoadTrainModelFromBinaryFile(const TCHAR* strFilePath);

	void SetNewAlgoEnable(bool bEnable);
	bool GetNewAlgoEnable();

public:
	void EnableComputeGrayMeanContrast(bool bEnable = true);
	void EnableComputeSharpnessContrast(bool bEnable = true);

	bool SetAcceptNum(long lMaxNum);
	bool SetAcceptThreshold(double dAcceptThre);
	bool SetConfusionThreshold(double dConfThre);
	bool SetOverlapThreshold(double dOverlapThre);

	bool SetAcceptScore(double dScore);	//	新增接口，ConfusionThreshold设置为dScore，AcceptThreshold设置为0.9*dScore

	bool Execute(const scImageGray& image, const scRect& rect);
	bool Execute(const scImageGray& image, const scShape* roi = NULL);
	bool Execute(const scImageGray& image, const scImageGray& imagemask, const scShape* roi = NULL);
	bool Execute(cpImage& image, const scRect& rect);
	bool Execute(cpImage& image, const scShape* roi = NULL);
	bool Execute(cpImage& image, cpImage& imagemask, const scShape* roi = NULL);

	long GetResultNum();
	scPatternSearchRotResult GetResult(long index);

	bool SortByScore(SortOrder order = eDescending);
	bool SortByCoordXY(double dGap = 64, CoordSortMethod method = eCoordYFirst, SortOrder orderX = eAscending, SortOrder orderY = eAscending, BOOL bImageCoord = TRUE);

private:
	bool IsOverLap(scPatternSearchRotResult result1, scPatternSearchRotResult result2);

	bool LoadTrainModelFromFile_old(const TCHAR* strFilePath);
	bool LoadTrainModelFromBinaryFile_old(const TCHAR* strFilePath);
	bool LoadTrainModelFromFile_MaskRect(const TCHAR* strFilePath);
	bool LoadTrainModelFromBinaryFile_MaskRect(const TCHAR* strFilePath);
	bool LoadTrainModelFromFile_MaskRect_Cycle(const TCHAR* strFilePath);
	bool LoadTrainModelFromBinaryFile_MaskRect_Cycle(const TCHAR* strFilePath);
	bool LoadTrainModelFromFile_MaskRect_Cycle_Gray16(const TCHAR* strFilePath);
	bool LoadTrainModelFromBinaryFile_MaskRect_Cycle_Gray16(const TCHAR* strFilePath);

	//训练参数
	Accuracy	m_nAccuracy;
	CoarseScale m_nCoarseScale;

	bool		m_bRotateZone;
	scDegree	m_RotateNominal;
	scDegree	m_RotateLow;
	scDegree	m_RotateHigh;
	scDegree	m_RotateDup;

	int			m_nCycleNumber;

	int			m_nGray16ToGray8;

	//搜索参数
	bool m_bEnableComputeGrayMeanContrast;
	bool m_bEnableComputeSharpnessContrast;

	long	m_lAcceptNum;
	double	m_dAcceptThreshold;
	double	m_dConfusionThreshold;
	double	m_dXYOverlapThreshold;

	//搜索工具
	svStd vector<void*> m_pPatternSearchArray;
	void*	m_hPatternSearch;

	//搜索结果
	svStd vector<scPatternSearchRotResult> m_pResultArray;

	bool m_bNewAlgo;
};

#pragma pack(pop)