#pragma once

#include "svDef.h"
#include "svImage.h"
#include "vpImage.h"
#include "svShape.h"
#include "svBlob.h"
#include "svImageAnalysisDef.h"

#pragma pack(push,8)

struct IntensityBlob;

class SVIMAGEANALYSIS_API scIntensityBlobTool
{
public:
	scIntensityBlobTool();
	~scIntensityBlobTool();

	enum BaseThreType
	{
		eSpecified,			// 指定值
		eAverage,			// 均值
		eMedian,			// 中值
		eHighFrequency		// 最频值
	};

	enum ObjectType
	{
		eWhite,				// 明
		eBlack,				// 暗
		eWhiteAndBlack,		// 明暗
		eWhiteAndBlackEx	// 明暗（个别）
	};

	enum FilterType
	{
		eFilterArea,          // 面积
		eFilterAcircularity,  // 圆度
		eFilterVolume,        // 体积
		eFilterAvgInsLevel,   // 平均浓淡等级
		eFilterMaxInsLevel,   // 最大浓淡等级
		eFilterAcircularityNormal  // 规范圆度
	};

	enum SortType
	{
		eSortArea,            // 面积
		eSortMassCenterX,     // 重心X
		eSortMassCenterY,     // 重心Y
		eSortAcircularity,    // 圆度
		eSortAngle,           // 顺时针逆时针
		eSortVolume,          // 体积
		eSortAvgInsLevel,     // 平均浓淡等级
		eSortMaxInsLevel,     // 最大浓淡等级
		eSortAcircularityNormal     // 规范圆度
	};

	void SetAvgSize(int nX,int nY);                           // 设置均值滤波参数
	void SetBaseThre(BaseThreType baseType,long lThre = 128);  // 设置基准阈值
	void SetObjectType(ObjectType objType);         // 设置检测目标颜色
	void SetThreshold(long lLowThre,long lHighThre);  // 设置上下阈值幅值
	void SetInspectNum(int nNum);  // 设置检测个数

	void SetFilterParam(FilterType filtertype,long lLow,long lHigh,int nIndex = 0); // 设置滤波方式及参数
	void SetSortParam(SortType sorttype,SortOrder order);       // 设置排序方式

	void SetConnectivityType(scBlobSceneDescription::Analysis c);             // 设置连通性参数        
	void SetConnectivityCleanup(scBlobSceneDescription::ConnectCleanup c);    // 设置修剪填充参数
	void SetConnectivityMinPels(int m);                                       // 设置修剪填充最小面积

	bool Execute(const cpImage& image,const scShape* roi = NULL);  // 执行检测

	std::vector<IntensityBlob>& GetResultIntensityBlob();   // 获取检测结果
	long GetResultTotalArea()const;                         // 获取检测总面积

	cpImage GetAveImage()const {return m_AveImage;}
	cpImage GetMaskImage()const {return m_MaskImage;}

private:
	bool RoiFilter(const cpImage& src,cpImage& dst,long& lOffsetX,long& lOffsetY,const scRect* pRect = NULL);   // Roi处理
	bool AvgFilter(const cpImage& src,cpImage& dst,int nX,int nY);                // 均值处理
	void CalIntensityParam(const cpImage& image,IntensityBlob& intensityBlob);    // 计算Blob体积、平均浓淡等级、最大浓淡等级
	void Filter(std::vector<IntensityBlob> &newIntensityBlob,FilterType fMeasure,double dFilterLow,double dFilterHigh);  // Blob过滤

private:
	int m_nAvgSizeX;
	int m_nAvgSizeY;

	BaseThreType m_baseType;
	long m_lBaseThre;

	ObjectType  m_ObjType;
	long m_lWhiteThre;
	long m_lBlackThre;

	int  m_nInspectNum;
	scBlobTool m_BlobTool;

	cpImage m_AveImage;      // 均值图像
	cpImage m_MaskImage;     // 掩膜图像

	std::vector<FilterType> m_vFilterMeasure;
	std::vector<long>    m_vFilterLow;
	std::vector<long>    m_vFilterHigh;

	SortType  m_SortType;
	SortOrder m_SortOrder;

	long m_lTotalArea;
	std::vector<IntensityBlob> m_vResultInsBlob;

};

struct IntensityBlob
{
	scBlob cBlob;

	double dAverInLev;    // 平均浓淡等级 
	double dMaxInLev;     // 最大浓淡等级
	double dVolume;       // 体积
	long lBaseThre;       // 基准阈值
	long lThre;           // 检测阈值
	bool bWhite;          // 检测目标颜色
	double dAngle;        // 顺时针、逆时针角度
	scIntensityBlobTool::SortType sortType;  // 排序方式
	SortOrder  sortOrder;  // 升序降序
};

#pragma pack(pop)