#pragma once

#include "svDef.h"
#include "svImage.h"
#include "vpImage.h"
#include "svShape.h"
#include "svBlob.h"
#include "svImageAnalysisDef.h"

#pragma pack(push,8)

class SVIMAGEANALYSIS_API scStainTool
{
public:
	scStainTool();
	~scStainTool();

	void EnableGroup(bool bEnable = false);                   // 设置分组
	void SetAvgSize(int nX,int nY);                           // 设置均值滤波参数
	void SetStainHalfWidth(int nHalfWidth);                   // 设置瑕疵处理半宽
	void SetStainThreshold(int nThreld);                      // 设置瑕疵阈值

	void SetFilterParam(Measure measure,long lLow,long lHigh,int nIndex = 0); // 设置滤波方式及参数
	void SetSortParam(Measure measure,SortOrder order);       // 设置排序方式

	void SetConnectivityType(scBlobSceneDescription::Analysis c);             // 设置连通性参数        
	void SetConnectivityCleanup(scBlobSceneDescription::ConnectCleanup c);    // 设置修剪填充参数
	void SetConnectivityMinPels(int m);                                       // 设置修剪填充最小面积

	long GetTotalStainNum()const;                    // 获取检测总瑕疵
	long GetGroupNum()const;                         // 获取检测分组数量,分组模式下有效
	const scBlob* GetCurrentBlob(int nIndex = 0)const;     // 获取当前Blob,不分组模式下为NULL

	bool Execute(const cpImage& image,const scShape* roi = NULL);

	cpImage GetAveImage()const {return m_AveImage;}
	cpImage GetStainImage()const {return m_StainImage;}
	cpImage GetMaskImage()const {return m_MaskImage;}

private:
	bool RoiFilter(const cpImage& src,cpImage& dst,long& lOffsetX,long& lOffsetY,const scRect* pRect = NULL);
	bool AvgFilter(const cpImage& src,cpImage& dst,int nX,int nY);
	bool StainFilter(const cpImage& src,cpImage& dst,int nHalfWidth);
	void SetDefaults();

private:
	bool m_bGroup;
	int m_nGroupNum;

	int m_nAvgSizeX;
	int m_nAvgSizeY;
	int m_nStainHalfWidth;
	int m_nThreshold;

	long m_lTotalStainNum;

	cpImage m_AveImage;      // 均值图像
	cpImage m_StainImage;    // 瑕疵图像
	cpImage m_MaskImage;     // 掩膜图像

	std::vector<Measure> m_vFilterMeasure;
	std::vector<long>    m_vFilterLow;
	std::vector<long>    m_vFilterHigh;

	Measure m_SorMeasure;
	SortOrder m_SortOrder;

	mutable scBlobTool m_BlobTool;
};

#pragma pack(pop)