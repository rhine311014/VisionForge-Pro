#pragma once

#include "svDef.h"
#include "svRect.h"
#include "svImage.h"
#include "vpImage.h"

#pragma pack(push,8)

class SVPATTERNSEARCH_API scPatternSearchResult
{
public:
	double m_dPosX;
	double m_dPosY;
	double m_dScore;
	scRect m_Rect;
};

class SVPATTERNSEARCH_API scPatternSearch
{
public:
	enum Accuracy
	{                                                                  
		AccuracyVeryFine	= 1,		// 高精
		AccuracyFine		= 2,		// 精细
		AccuracyCoarse		= 4			// 粗略
	};

	enum Algorithm 
	{
		AlgorithmLinearSearch	 = 1,	// 激进策略区域定位(速度较快)
		AlgorithmLinearCNLPAS	 = 2,	// 保守策略区域定位
	};

	enum CoarseScale
	{
		CoarseScale1  = 1,				// 原始分辨率
		CoarseScale2  = 2,				// 1/2采样
		CoarseScale4  = 3,				// 1/4采样
		CoarseScale8  = 4,				// 1/8采样
		CoarseScale16 = 5,				// 1/16采样
		CoarseScale32 = 6				// 1/32采样
	};

public:
	scPatternSearch();
	~scPatternSearch();

public:
	bool SetAlgorithm(Algorithm algo);
	bool SetAccuracy(Accuracy accu);
	bool SetCoarsestScale(CoarseScale scale);
	bool SetLowEdgeThreshold(unsigned char ucLowThre);
	bool SetHighEdgeThreshold(unsigned char ucHighThre);
	Algorithm GetAlgorithm();
	Accuracy GetAccuracy();
	CoarseScale GetCoarsestScale();
	unsigned char GetLowEdgeThreshold();
	unsigned char GetHighEdgeThreshold();

	bool Train(const scImageGray& image, const scRect& rect); 
	bool Train(cpImage& image, const scRect& rect); 

	bool SaveTrainModelToFile(const TCHAR* strFilePath);
	bool LoadTrainModelFromFile(const TCHAR* strFilePath);
	bool SaveTrainModelToBinaryFile(const TCHAR* strFilePath);
	bool LoadTrainModelFromBinaryFile(const TCHAR* strFilePath);

	bool GetPatternImage(scImageGray& image);
	bool GetTrainImage(scImageGray& image);
	bool GetTrainRect(scRect& rect);
	bool GetPatternImage(cpImage& image);
	bool GetTrainImage(cpImage& image);

public:
	bool SetMaxResults(long lMaxNum);
	bool SetAcceptThreshold(double dAcceptThre);
	bool SetConfusionThreshold(double dConfThre);
	bool SetOverlapThreshold(double dOverlapThre);

	bool Execute(const scImageGray& image, const scRect& rect);
	bool Execute(const scImageGray& image, const scShape* roi = NULL);
	bool Execute(const scImageGray& image, const scImageGray& imagemask, const scShape* roi = NULL);
	bool Execute(cpImage& image, const scRect& rect);
	bool Execute(cpImage& image, const scShape* roi = NULL);
	bool Execute(cpImage& image, cpImage& imagemask, const scShape* roi = NULL);

	long GetResultNum();
	scPatternSearchResult GetResult(long index);

	bool scPatternSearch::SortByCoordXY(double dGap = 64, CoordSortMethod method = eCoordYFirst, SortOrder orderX = eAscending, SortOrder orderY = eAscending);

private:
	void *m_hCNLSearch;
	bool  m_bTrained;
	scRect m_RectTrained;
	scImage8 m_ImageTrain;
	svStd vector<scPatternSearchResult> m_pResultArray;
};

#pragma pack(pop)