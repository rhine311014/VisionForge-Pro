#pragma once
#pragma pack(push,8)

#include "svVision3DToolBase.h"

#include "svLineSeg.h"
#include "svLine.h"
#include "svCircle.h"
#include <vector>


// 轮廓结果
typedef struct tagProfileResult
{
	std::vector<int>		m_viValidNum;	// 有效点数
	std::vector<double>     m_vdTruncated;  // 轮廓点截断高度
	std::vector<double>		m_vdProfile;	// 轮廓点真实高度
	std::vector<sc2Vector>	m_vProfilePos;	// 轮廓点图像坐标
}ProfileResult;

// 检测范围
typedef struct tagInspectObjectRegion
{
	double m_dProfileLeft;
	double m_dProfileRight;

	tagInspectObjectRegion()
	{
		m_dProfileLeft  = 0.0;
		m_dProfileRight = 128.0;
	}
}CInspectObjectRegion,*PCInspectObjectRegion;

// 检测对象参数基类
typedef struct tagInspectObjectParamBase
{
	virtual ProfileInspectObjectType GetType() {return eObjUnknown;}
}CInspectObjectParamBase,*PCInspectObjectParamBase;

// 平均参数
typedef struct tagInspectObjectAverageParam : public tagInspectObjectParamBase
{
	CInspectObjectRegion m_InspectRegion;   // 检测范围
	virtual ProfileInspectObjectType GetType() {return eObjAverage;}
}CInspectObjectAverageParam,*PCInspectObjectAverageParam;

// 峰值参数
typedef struct tagInspectObjectPeakParam : public tagInspectObjectParamBase
{
	CInspectObjectRegion m_InspectRegion;   // 检测范围
	virtual ProfileInspectObjectType GetType() {return eObjPeak;}
}CInspectObjectPeakParam,*PCInspectObjectPeakParam;

// 谷值参数
typedef struct tagInspectObjectBottomParam : public tagInspectObjectParamBase
{
	CInspectObjectRegion m_InspectRegion;   // 检测范围
	virtual ProfileInspectObjectType GetType() {return eObjBottom;}
}CInspectObjectBottomParam,*PCInspectObjectBottomParam;

// 拐点参数
typedef struct tagInspectObjectInflectionPointParam : public tagInspectObjectParamBase
{
	CInspectObjectRegion m_InspectRegion;   // 检测范围
	double m_dThresholdPercent;             // 阈值百分比
	double m_dThresholdRange;               // 阈值范围
	int    m_nInspectShapeType;             // 检测形状 0-山谷,1-山，2-谷
	int    m_nInspectScanDir;               // 检测方向 0：->,1:<-
	int    m_nFilterMethod;                 // 滤波算法 0:均值,1:高斯
	int    m_nInspectLabel;                 // 检测标签
	int    m_nFilterHalfWidth;              // 滤波半宽
	int    m_nIntervalPoints;               // 间隔点数
	double m_dSigma;                        // sigma(高斯滤波)
	bool   m_bInspectMaxInflection;			// 是否检测拐点最大值
	bool   m_bEnableNeighborSearch;         // 是否进行邻近搜索
	int    m_nNeighborSearchMode;           // 邻近搜索方式，0，最近点；1，向左；2，向右

	tagInspectObjectInflectionPointParam()
	{
		m_dThresholdPercent = 50.0;
		m_dThresholdRange   = 256.0;
		m_nInspectShapeType = 0;
		m_nInspectScanDir = 0;
		m_nFilterMethod = 0;
		m_nInspectLabel = 0;
		m_nFilterHalfWidth = 1;
		m_nIntervalPoints = 2;
		m_dSigma = 0.8;
		m_bInspectMaxInflection = false;
		m_bEnableNeighborSearch = false;
		m_nNeighborSearchMode = 0;
	}

	virtual ProfileInspectObjectType GetType() {return eObjInflectionPoint;}
}CInspectObjectInflectionPointParam,*PCInspectObjectInflectionPointParam;

// 两直线交点参数
typedef struct tagInspectObjectTwoLinesIntersectionParam : public tagInspectObjectParamBase
{
	CInspectObjectRegion m_FitLineRegion[2];   // 检测范围
	int m_nOutLinePercent[2];                  // 局外点百分比

	tagInspectObjectTwoLinesIntersectionParam()
	{
		m_nOutLinePercent[0] = 0;
		m_nOutLinePercent[1] = 0;
	}

	virtual ProfileInspectObjectType GetType() {return eObjTwoLinesIntersection;}
}CInspectObjectTwoLinesIntersectionParam,*PCInspectObjectTwoLinesIntersectionParam;

// 圆与直线交点参数
typedef struct tagInspectObjectCircleAndLineIntersectionParam : public tagInspectObjectParamBase
{
	CInspectObjectRegion m_FitLineRegion;   // 拟合直线检测范围
	CInspectObjectRegion m_FitCircleRegion; // 拟合圆检测范围

	int m_nLineFitOutLinePercent;           // 拟合直线局外点百分比
	int m_nCircleFitOutLinePercent;         // 拟合圆局外点百分比

	int m_nIntersectionPointType;           // 交点选取,0-中点,1-左边点,2-右边点

	tagInspectObjectCircleAndLineIntersectionParam()
	{
		m_nLineFitOutLinePercent = 0;
		m_nCircleFitOutLinePercent = 0;
		m_nIntersectionPointType = 0;
	}

	virtual ProfileInspectObjectType GetType() {return eObjCircleAndLineIntersection;}
}CInspectObjectCircleAndLineIntersectionParam,*PCInspectObjectCircleAndLineIntersectionParam;

// 圆心
typedef struct tagInspectObjectCircleCenterParam : public tagInspectObjectParamBase
{
	CInspectObjectRegion m_FitCircleRegion; // 拟合圆检测范围
	int m_nCircleFitOutLinePercent;         // 拟合圆局外点百分比

	tagInspectObjectCircleCenterParam()
	{
		m_nCircleFitOutLinePercent = 0;
	}

	virtual ProfileInspectObjectType GetType() {return eObjCircleCenter;}
}CInspectObjectCircleCenterParam,*PCInspectObjectCircleCenterParam;

// 到基准模型线的距离
typedef struct tagInspectObjectDistanceToReferenceLineParam : public tagInspectObjectParamBase
{
	CInspectObjectRegion m_InspectRegion; // 检测范围
	int m_nRefenceLineType;               // 参考基准线类型 0-直线，1-圆
	int m_nInspectDir;                    // 检测方向
	double m_dThreshold;                  // 检测阈值
	double m_dWidthLimit;                 // 检测宽度下限
	int m_nFitOutLinePercent;             // 拟合线局外点百分比

	tagInspectObjectDistanceToReferenceLineParam()
	{
		m_nRefenceLineType = 0;
		m_nInspectDir = 0;
		m_dThreshold = 0.0;
		m_dWidthLimit = 0.0;
		m_nFitOutLinePercent = 0;
	}

	virtual ProfileInspectObjectType GetType() {return eObjDistanceToReferenceLine;}
}CInspectObjectDistanceToReferenceLineParam,*PCInspectObjectDistanceToReferenceLineParam;

// 边缘
typedef struct tagInspectObjectEdgeParam : public tagInspectObjectParamBase
{
	CInspectObjectRegion m_InspectRegion; // 检测范围
	double m_dInspectHeight;              // 检测高度(mm)
	int    m_nEdgeDirection;              // 边缘方向，0-双方向，1-上升沿，2-下降沿
	int    m_nInspectDirection;           // 检测方向，0-从左至右，1-从右至左
	int    m_nInspectLabel;               // 检测标签

	tagInspectObjectEdgeParam()
	{
		m_dInspectHeight = 0.0;
		m_nEdgeDirection = 0;
		m_nInspectDirection = 0;
		m_nInspectLabel = 0;
	}

	virtual ProfileInspectObjectType GetType() {return eObjEdge;}
}CInspectObjectEdgeParam,*PCInspectObjectEdgeParam;

// 直线
typedef struct tagInspectObjectLineParam : public tagInspectObjectParamBase
{
	CInspectObjectRegion m_InspectRegion; // 检测范围
	int m_nFitOutlinePercent;             // 拟合线局外点百分比

	tagInspectObjectLineParam()
	{
		m_nFitOutlinePercent = 0;
	}

	virtual ProfileInspectObjectType GetType() {return eObjLine;}
}CInspectObjectLineParam,*PCInspectObjectLineParam;

// 截面积
typedef struct tagInspectObjectCrossSectionAreaParam : public tagInspectObjectParamBase
{
	CInspectObjectRegion m_InspectRegion; // 检测范围
	scLine               m_BenchLine;     // 基准线

	tagInspectObjectCrossSectionAreaParam()
	{
		m_BenchLine.SetPosDirection(sc2Vector(0.0,0.0),sc2Vector(1.0,0.0));
	}

	virtual ProfileInspectObjectType GetType() {return eObjCrossSectionArea;}
}CInspectObjectCrossSectionAreaParam,*PCInspectObjectCrossSectionAreaParam;

// 检测对象结果基类
typedef struct tagInspectObjectResultBase
{
	bool m_bInspectResult;
	tagInspectObjectResultBase()
	{
		m_bInspectResult = false;
	}
	virtual ProfileInspectObjectType GetType() {return eObjUnknown;}
}CInspectObjectResultBase,*PCInspectObjectResultBase;

// 平均结果
typedef struct tagInspectObjectAverageResult : public tagInspectObjectResultBase
{
	double m_dInspectValue;
	tagInspectObjectAverageResult()
	{
		m_dInspectValue = 0.0;
	}

	virtual ProfileInspectObjectType GetType() {return eObjAverage;}
}CInspectObjectAverageResult,*PCInspectObjectAverageResult;

// 峰值结果
typedef struct tagInspectObjectPeakResult : public tagInspectObjectResultBase
{
	sc2Vector m_vPeakCoord;
	virtual ProfileInspectObjectType GetType() {return eObjPeak;}
}CInspectObjectPeakResult,PCInspectObjectPeakResult;

// 谷值结果
typedef struct tagInspectObjectBottomResult : public tagInspectObjectResultBase
{
	sc2Vector m_vBottomCoord;
	virtual ProfileInspectObjectType GetType() {return eObjBottom;}
}CInspectObjectBottomResult,*PCInspectObjectBottomResult;

// 拐点结果
typedef struct tagInspectObjectInflectionPointResult : public tagInspectObjectResultBase
{
	sc2Vector m_vInspectPoint;
	std::vector<sc2Vector> m_vAllInspectPoints;
	std::vector<sc2Vector> m_vAllInflectionPoints;

	virtual ProfileInspectObjectType GetType() {return eObjInflectionPoint;}
}CInspectObjectInflectionPointResult,*PCInspectObjectInflectionPointResult;

// 两直线交点结果
typedef struct tagInspectObjectTwoLinesIntersectionResult : public tagInspectObjectResultBase
{
	sc2Vector m_vIntersectionPoint;
	scLine m_ResultLineOne;
	scLine m_ResultLineTwo;
	virtual ProfileInspectObjectType GetType() {return eObjTwoLinesIntersection;}
}CInspectObjectTwoLinesIntersectionResult,*PCInspectObjectTwoLinesIntersectionResult;

// 直线与圆交点结果
typedef struct tagInspectObjectCircleAndLineIntersectionResult : public tagInspectObjectResultBase
{
	sc2Vector m_vIntersectionPoint;
	scLine m_ResultLine;
	scCircle m_ResultCircle;
	virtual ProfileInspectObjectType GetType() {return eObjCircleAndLineIntersection;}
}CInspectObjectCircleAndLineIntersectionResult,*PCInspectObjectCircleAndLineIntersectionResult;

// 圆心
typedef struct tagInspectObjectCircleCenterResult : public tagInspectObjectResultBase
{
	sc2Vector m_vCircleCenter;
	scCircle m_ResultCircle;
	virtual ProfileInspectObjectType GetType() {return eObjCircleCenter;}
}CInspectObjectCircleCenterResult,*PCInspectObjectCircleCenterResult;

// 到基准模型线的距离
typedef struct tagInspectObjectDistanceToReferenceLineResult : public tagInspectObjectResultBase
{
	sc2Vector m_vInspectPoint;
	std::vector<scLine> m_vResultLine;
	std::vector<scCircle> m_vResultCircle;
	std::vector<sc2Vector> m_vInspectSection;
	std::vector<std::vector<sc2Vector>> m_vAllInspectSection;

	virtual ProfileInspectObjectType GetType() {return eObjDistanceToReferenceLine;}
}CInspectObjectDistanceToReferenceLineResult,*PCInspectObjectDistanceToReferenceLineResult;

// 边缘
typedef struct tagInspectObjectEdgeResult : public tagInspectObjectResultBase
{
	sc2Vector m_vInspectPoint;
	std::vector<sc2Vector> m_vAllInspectPoint;
	virtual ProfileInspectObjectType GetType() {return eObjEdge;}
}CInspectObjectEdgeResult,*PCInspectObjectEdgeResult;

// 直线
typedef struct tagInspectObjectLineResult : public tagInspectObjectResultBase
{
	scLine m_ResultLine;
	scLineSeg m_ResultLineSeg;
	virtual ProfileInspectObjectType GetType() {return eObjLine;}
}CInspectObjectLineResult,*PCInspectObjectLineResult;

// 截面积
typedef struct tagInspectObjectCrossSectionAreaResult : public tagInspectObjectResultBase
{
	double m_dConvexSectionArea;   // 凸面积
	double m_dConcaveSectionArea;  // 凹面积

	std::vector<std::vector<sc2Vector>> m_vPosSection; // 凸面积段
	std::vector<std::vector<sc2Vector>> m_vNegSection; // 凹面积段

	tagInspectObjectCrossSectionAreaResult()
	{
		m_dConvexSectionArea = 0.0;
		m_dConcaveSectionArea = 0.0;
	}

	virtual ProfileInspectObjectType GetType() {return eObjCrossSectionArea;}
}CInspectObjectCrossSectionAreaResult,*PCInspectObjectCrossSectionAreaResult;

// 检测项参数
class SV_VISION_3D_API scProfileInspectItemParam
{
public:
	scProfileInspectItemParam(ProfileInspectItemType itemType = eItemHeight);
	scProfileInspectItemParam(const scProfileInspectItemParam& param);
	virtual ~scProfileInspectItemParam();

	// 检测项类型
	void SetProfileInspectItenType(ProfileInspectItemType itemType);
	ProfileInspectItemType GetProfileInspectItemType()const {return m_ItemType;}

	// 线线距参数
	// nType = 0, 线段1中点到直线2的距离
	//       = 1, 线段2中点到直线1的距离
	//       = 2, 线段1到线段2的最短距离
	void SetLineToLineDistanceType(int nType);
	int GetLineToLineDistanceType()const;

	scProfileInspectItemParam& operator=(const scProfileInspectItemParam& param);

	void SetInspectObjectParam(CInspectObjectParamBase* param,int nIndex);
	CInspectObjectParamBase* GetInspectObjectParam(int nIndex)const;

	void SetResultCorrection(ResultCorrection& resCorrection);
	ResultCorrection GetResultCorrection()const;

	void SetJudgeCombine(JudgeCombine& judgeCombine);
	JudgeCombine GetJudgeCombine()const;

	int GetInspectObjectParamCount()const {return (int)m_vInspectObjectParam.size();}

private:
	void CopyFrom(CInspectObjectParamBase* param,int nIndex);

private:
	ProfileInspectItemType m_ItemType;
	int m_nLineToLineDistanceType;
	ResultCorrection m_ResultCorrection;
	JudgeCombine     m_JudgeCombine;
	std::vector<CInspectObjectParamBase*> m_vInspectObjectParam;
};

// 检测项结果
class SV_VISION_3D_API scProfileInspectItemResult
{
public:
	scProfileInspectItemResult();
	scProfileInspectItemResult(ProfileInspectItemType itemType);
	scProfileInspectItemResult(const scProfileInspectItemResult& res);
	virtual ~scProfileInspectItemResult();

	ProfileInspectItemType GetProfileInspectItemType()const {return m_ItemType;}

	scProfileInspectItemResult& operator=(const scProfileInspectItemResult& param);

	int GetInspectObjectResultCount()const {return (int)m_vInspectObjectResult.size();} // 获取检测对象结果数量

	CInspectObjectResultBase* GetInspectObjectResult(int nIndex)const; // 获取检测对象检测结果

	double GetInspectResult()const {return m_dInspectResult;} // 获取检测结果

	double GetInspectResultNotCorrection()const {return m_dInspectResultNotCorrection;} // 获取未补正的检测结果

	int GetInspectStatus()const {return m_nInspectStatus;} // 0：未执行检测,-1：检测失败，-2：判定失败,1:检测成功

	std::vector<sc2Vector> GetNearestPoints()const {return m_vNearestPoints;} // 获取最近点,距离检测使用,保存最近点

	friend class scProfileInspectItem;
private:
	void CopyFrom(CInspectObjectResultBase* param,int nIndex);

private:
	ProfileInspectItemType m_ItemType;
	double m_dInspectResult;
	double m_dInspectResultNotCorrection;
	int m_nInspectStatus;
	std::vector<sc2Vector> m_vNearestPoints; // 距离检测使用
	std::vector<CInspectObjectResultBase*> m_vInspectObjectResult;
};

// 轮廓检测工具
class scProfileInspectItem;
class SV_VISION_3D_API scVision3DProfileInspectTool : public scVision3DToolBase
{
public:
	scVision3DProfileInspectTool();
	virtual ~scVision3DProfileInspectTool();

	virtual void SetZeroPlane(scZeroPlaneResult& zeroPlane);                // 设置起算面
	virtual bool GetZeroPlane(scZeroPlaneResult& zeroPlane)const;           // 获取起算面

	// 设置/获取Z偏移
	void SetOffsetZ(double dOffsetZ);
	double GetOffsetZ()const;

	// 设置/获取检测区域（只能是仿射矩形、圆环、圆环段中的一种）
	void SetInspectRegion(scShape* pRegion);
	scShape* GetInspectRegion()const;

	// 设置/获取计算方式
	void SetCalculateMethod(CalculateMethod method);
	CalculateMethod GetCalculateMethod()const;

	// 设置/获取轮廓补正
	void SetShapeCorrectionType(ShapeCorrectionType type);
	ShapeCorrectionType GetShapeCorrectionType()const;

	// 设置/获取有效像素边界类型
	void SetValidPixelBorderType(ValidPixelBorderType type);
	ValidPixelBorderType GetValidPixelBorderType()const;

	void AddProfileInspectItem(scProfileInspectItemParam& param,bool bInspected = true);   // 添加检测项
	void DeleteProfileInspectItem(int nIndex);                                             // 删除检测项
	void SetProfileInspectItem(scProfileInspectItemParam& param,int nIndex);               // 设置检测项
	void GetProfileInspectItem(scProfileInspectItemParam& param,int nIndex)const;          // 获取检测项

	void SetProfileInspectItemEnable(bool bEnable,int nIndex);                             // 设置检测项是否检测
	bool IsProfileInspectItemEnable(int nIndex)const;                                      // 获取检测项是否检测

	void ClearItem();                                                       // 清空检测项

	int GetProfileInspectCount()const;                                      // 获取检测项数量

	bool GenerateProfile(cpImage& image);                                   // 生成轮廓线
	bool ExecuteProfile(ProfileResult& profileResult);                      // 轮廓检测 

	virtual bool Excute(cpImage& image);                                    // 执行检测

	bool GetProfileResult(ProfileResult& profileResult);                    // 获取轮廓线

	bool GetInspectResult(scProfileInspectItemResult& res,int nIndex);              // 获取检测结果

private:
	scShape*                           m_pInspectRegion;          // 轮廓线检测范围
	CalculateMethod                    m_CalculateMethod;         // 轮廓线计算方式
	ShapeCorrectionType                m_ShapeCorrectionType;     // 轮廓线补正类型
	ValidPixelBorderType               m_ValidPixelBorderType;    // 有效像素边界类型
	scZeroPlaneResult                  m_ZeroPlane;               // 起算面
	double                             m_dOffsetZ;                // Z偏移

	std::vector<scProfileInspectItemParam>     m_vProfileInspectItemParam;     // 检测项参数
	std::vector<scProfileInspectItemResult>    m_vProfileInspectItemResult;    // 检测项结果
	std::vector<bool>                          m_vbEnableProfileInspectItem;   // 检测项是否检测
	std::vector<ProfileResult>                 m_ProfileResult;                // 轮廓线结果
};

#pragma pack(pop)