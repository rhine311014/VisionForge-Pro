#pragma once
#include "svVision3DDef.h"
#include "svImagePreprocessToolBase.h"
#include "svBlob.h"
#include "svBlobSceneDescription.h"

#include <vector>
#pragma pack(push,8)

class SV_VISION_3D_API scImagePreprocessBlobFilterTool : public scImagePreprocessToolBase
{
public:
	scImagePreprocessBlobFilterTool();
	virtual ~scImagePreprocessBlobFilterTool();

	virtual PreprocessType GetPreprocessType() const {return ePreprocessBlobFilter;}  // 获取预处理类型

	enum BlobFilterSortMeasure
	{
		// scBlob不支持的排序是方式
		eBlobFilterYMoreThanX = eAcircularityNormal + 100, // Y > X升序
		eBlobFilterXMoreThanY,       // X > Y升序
		eBlobFilterAngle,            // 顺时针或逆时针排序

		// scBlob支持的排序方式
		eBlobFilterCenterMassX = eCenterMassX, // 重心X坐标排序
		eBlobFilterCenterMassY = eCenterMassY, // 重心Y坐标排序
		eBlobFilterArea        = eArea,        // 面积排序
		eBlobFilterAcircularityNormal = eAcircularityNormal // 原形度排序
	};

	// 设置/获取检测目标颜色
	// 0：黑色，1：白色
	void SetInspectTarget(int nMode);
	int GetInspectTarget()const;

	// 设置/获取检测个数
	void SetInspectNum(long nNum);
	long GetInspectNum()const;

	// 是否填充
	void EnableFill(bool bFill);
	bool IsFill()const;

	// 设置/获取填充最小面积
	void SetMinFillPels(int nPels);
	int GetMinFillPels()const;

	// 边界除外
	void SetBoundExcept(bool bExcept);
	bool IsBoundExcept()const;

	// 是否处理全部标签
	void EnableProcessAllLabel(bool bAll);
	bool IsProcessAllLabel()const;

	// 设置/获取标签号（不处理全部标签）
	void SetLabelIndex(int nIndex);
	int GetLabelIndex()const;

	// 设置/获取排序方式
	void SetSortMethod(BlobFilterSortMeasure measure,SortOrder order);
	void GetSortMethod(BlobFilterSortMeasure& measure,SortOrder& order)const;

	// 添加、删除过滤参数
	void AddFilterParam(Measure measure,double low,double high);
	void RemoveFilterParam(int nIndex);
	void RemoveAllFilter();
	void GetFilterParam(Measure& measure,double& low,double& high,int nIndex)const;

	// 设置/获取基准角度(逆时针、顺时针排序使用)
	void SetBenchMarkAngle(double dAngle);
	double GetBenchMarkAngle()const;

	int FilterNum()const {return (int)m_vFilterMeasure.size();}

	// 执行预处理
	// 8位图像不考虑是否涉及无效像素及膨胀控制，16位图像需考虑是否涉及无效像素及膨胀控制 
	virtual bool Excute(cpImage& src,cpImage& dst, bool bReferInvalidPixel = false, bool bExpandControl = false);
	virtual bool Excute(cpImage& src,cpImage& dst, scRect rcROI, bool bReferInvalidPixel = false, bool bExpandControl = false);
	// 排序相关
private:
	bool IsBlobSameX(const scBlob* b1,const scBlob* b2); // 判断两个blob X坐标是否相同
	bool IsBlobSameY(const scBlob* b1,const scBlob* b2); // 判断两个Blob Y坐标是否相同
	bool IsSwap(const scBlob* b1,const scBlob* b2);
	void QuickSort(std::vector<const scBlob*>& vpBlob,int l,int r);

private:
	int m_nInspectTarget;
	long m_lInspectNum;

	bool m_bBoundExcept;

	bool m_bFill;
	int  m_nMinFillPels;

	bool m_bProcessLabelAll;
	int  m_nLabelnIndex;

	SortOrder m_SortOrder;
	BlobFilterSortMeasure m_BlobFilterSortMeasure;

	double m_dBenchMarkAngle;

	std::vector<Measure> m_vFilterMeasure;
	std::vector<double>  m_vdFilterLow;
	std::vector<double>  m_vdFilterHigh;
};

#pragma pack(pop)