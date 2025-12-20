#pragma once

#include "svDef.h"
#include "svError.h"
#include "svMath.h"
#include "svImage.h"
#include "vpImage.h"
#include "svRect.h"
#include "svAffineRect.h"
#include "svPolyline.h"
#include <vector>

#pragma pack(push,8)

//根据给定的图像数据计算对应的直方图数据
//要求histogram的size不为0，图像的灰度值直接用来索引histogram，不做边界处理，因此必须确保图像最大灰阶小于histogram的size
//如果image无效，返回HInvalidImage；否则，返回HSuccess。
HStatus SVCORE_API sfHistogram(const scImageGray& image, svStd vector<long>& histogram);
HStatus SVCORE_API sfHistogram(const scImageGray& image, const scRect& rect, svStd vector<long>& histogram);
HStatus SVCORE_API sfHistogram(const scImageGray& image, const scAffineRect& affrect, svStd vector<long>& histogram);
HStatus SVCORE_API sfHistogram(const scImageGray& image, const scPolyline& polyline, svStd vector<long>& histogram);
HStatus SVCORE_API sfHistogram(const scImageGray& image, const scImageGray& mask, svStd vector<long>& histogram);
HStatus SVCORE_API sfHistogram(cpImage& image, svStd vector<long>& histogram);
HStatus SVCORE_API sfHistogram(cpImage& image, const scRect& rect, svStd vector<long>& histogram);
HStatus SVCORE_API sfHistogram(cpImage& image, const scAffineRect& affrect, svStd vector<long>& histogram);
HStatus SVCORE_API sfHistogram(cpImage& image, const scPolyline& polyline, svStd vector<long>& histogram);
HStatus SVCORE_API sfHistogram(cpImage& image, cpImage& mask, svStd vector<long>& histogram);

//根据给定的图像数据计算对应的直方图数据
//要求histogram的size不为0，图像的灰度值经过边界处理以后用来索引histogram。
//当图像中灰阶大于或等于histogram的size时，进行边界处理，当前像素记入其最近的bin。
//如果image无效，返回HInvalidImage；如果histogram的size等于0，返回HInvalidHistogramSize；否则，返回HSuccess。
HStatus SVCORE_API sfHistogram(const scImageGray& image, svStd vector<long>& histogram, bool& flag);
HStatus SVCORE_API sfHistogram(const scImageGray& image, const scRect& rect, svStd vector<long>& histogram, bool& flag);
HStatus SVCORE_API sfHistogram(cpImage& image, svStd vector<long>& histogram, bool& flag);
HStatus SVCORE_API sfHistogram(cpImage& image, const scRect& rect, svStd vector<long>& histogram, bool& flag);

class SVCORE_API scHistoStats
{
public:
	//构造一个空的直方图对象
	scHistoStats();

	//构造一个直方图对象，拷贝histogram中的数据
	scHistoStats(const svStd vector<long>& histogram);

	//返回该直方图对象是否有效
	bool IsValid() const { return m_Histogram.size()>0; }

	//返回直方图中的像素数量
	long Samples() const { return m_lSamples; }

	//返回直方图模值，即频率出现最高的灰阶（当多个灰阶出现的频率一样时，返回较小的灰阶）
	long Mode() const { return m_lMode; }

	//返回直方图中值（灰阶小于中值的像素数量占像素总数的一半）
	long Median() const { return m_lMedian; }

	//返回直方图中的最小灰阶
	long HistMin() const { return m_lHistMin; }

	//返回直方图中的最大灰阶
	long HistMax() const { return m_lHistMax; }

	//返回直方图均值
	double Mean() const { return m_dMean; }

	//返回直方图方差
	double Variance() const { return m_dVariance; }

	//返回直方图标准差
	double StdDev() const { return m_dStdDev; }

	//返回直方图数据
	const svStd vector<long>& Histogram() const { return m_Histogram; }

	//返回相对阈值
	long GetRelativeThreshold(double percent, double lowtail=0, double hightail=0) const;

	//返回动态阈值
	long GetDynamicThreshold(double& score, double lowtail=0, double hightail=0) const;

private:
	void ComputeStatistics();			// 计算统计值（均值、方差、标准差等）

	long m_lSamples;					// 像素数量
	long m_lMode;						// 模值
	long m_lMedian;						// 中值
	long m_lHistMin;					// 最小值
	long m_lHistMax;					// 最大值
	double m_dMean;						// 均值
	double m_dVariance;					// 方差
	double m_dStdDev;					// 标准差

	svStd vector<long> m_Histogram;		// 直方图数据
	svStd vector<long> m_HistogramCum;	// 累计直方图数据
};

#pragma pack(pop)
