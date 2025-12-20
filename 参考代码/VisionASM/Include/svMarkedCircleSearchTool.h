#pragma once

#include "svDef.h"
#include "vpImage.h"
#include "svFind.h"
#include "svAnnulusSection.h"
#include "svPose.h"

#pragma pack(push,8)

enum MatchMethod
{
	eGrayMatch = 0,		// 灰度匹配
	eEdgeMatch = 1,		// 边缘匹配	
};

//找标记圆参数数据结构
struct SVCORE_API FindMarkedCircleParam
{
	sc2Vector		m_CircleCenter;						// 圆心X
	double			m_dRadius;							// 半径
	scRadian		m_rArcStart;						// 起始弧度
	scRadian        m_rArcSpan;							// 弧度跨度
	int				m_nCaliperNum;						// 卡尺个数
	int				m_nOutLinePercent;					// 局外点百分比
	int				m_nFilterHalfWidth;					// 滤波器半宽
	double			m_dContrastThre;					// 对比度阈值
	double			m_dCaliperProjectLen;				// 投影长度
	double			m_dCaliperSearchLen;				// 搜索长度
	CaliperSearchDirection m_CaliperSearchDirection;	// 搜索方向
	EdgePolarity	m_edgeProperty;						// 边缘属性
	bool			m_bSortByScore;						// 按分数排序

	bool			    m_bEnableCircleMask;			// 是否启用掩膜
	std::vector<double> m_vdMaskData;					// 掩膜数据
	std::vector<bool>   m_vbCaliperMaskValid;			// 卡尺是否被掩膜

	double			m_dMarkRegionStart;					// 标记区域起始位置
	double			m_dMarkRegionEnd;					// 标记区域终止位置

	bool		m_bEnableMarkDepth;						// 开启标记深度，默认不开启
	int			m_nMarkDepth;							// 标记深度
	bool		m_bMarkAngleCorrection;					// 开启MarkAngle修正，默认不开启

	MatchMethod m_nMatchMethod;							// 匹配方法

	FindMarkedCircleParam()
	{
		m_CircleCenter = sc2Vector(200, 200);
		m_dRadius = 100;
		m_rArcStart = scRadian(0);
		m_rArcSpan = scRadian(skPI_2);
		m_nCaliperNum = 10;
		m_nOutLinePercent = 20;
		m_nFilterHalfWidth = 1;
		m_dContrastThre = 5;
		m_dCaliperProjectLen = 15;
		m_dCaliperSearchLen = 100.0;
		m_CaliperSearchDirection = eInward;
		m_edgeProperty = eDontCare;
		m_bSortByScore = true;

		m_bEnableCircleMask = false;

		m_dMarkRegionStart = 0;
		m_dMarkRegionEnd = 0;

		m_bEnableMarkDepth = false;
		m_nMarkDepth = 0;
		m_bMarkAngleCorrection = false;

		m_nMatchMethod = eGrayMatch;
	}
};

//找标记圆定位结果
class SVCORE_API scMarkedCircleSearchResult : public scPose
{
public:
	scMarkedCircleSearchResult();

	double	 GetScore()		{ return m_dScore; };

	double   GetRadius()	{ return m_dRadius; };

	sc2Vector GetCenter()	{return m_Position; };

	scCircle GetCircle()	{return m_Circle; };

	scDegree GetMarkAngle() { return m_MarkAngle; };   

public:
	double	  m_dScore;
	double    m_dRadius;
	scCircle  m_Circle;
	scDegree  m_MarkAngle;		//标记的角度 [-180, 180)
};

//找标记圆定位工具
class SVCORE_API scMarkedCircleSearchTool
{
public:
	scMarkedCircleSearchTool();
	~scMarkedCircleSearchTool();

public:
	// 设置/获取找标记圆数据结构
	void SetFindMarkedCircleParam(FindMarkedCircleParam param);
	FindMarkedCircleParam GetFindMarkedCircleParam()const;

	// 设置圆弧参数
	void SetCenterRadiusAngleStartAngleSpan(const sc2Vector& vCenter,	// 圆心坐标
											double dRadius,				// 半径
											const scRadian& rAngleStart,// 起始角度，单位弧度，值域[-π，π)
											const scRadian& rAngleSpan);// 张角，值域[-4π，4π)
	
	// 获取圆弧参数
	void GetCenterRadiusAngleStartAngleSpan(sc2Vector& vCenter,			// 圆心坐标
											double& dRadius,			// 半径
											scRadian& rAngleStart,		// 起始角度，单位弧度，值域[-π，π)
											scRadian& rAngleSpan) const;// 张角，值域[-4π，4π)

	void	SetCircularArc(const scCircularArc& circularArc);			// 设置圆弧
	scCircularArc GetCircularArc() const;								// 获取圆弧

	void	SetCaliperParams(int nCaliperNum,		// 设置卡尺数量
		CaliperSearchDirection nDirection,			// 设置卡尺搜索方向
		double dSearchLength,						// 设置卡尺搜索长度			 
		double dProjectionLength);					// 设置卡尺投影长度

	void	SetCaliperNum(int nCaliperNum);								// 设置卡尺数量				
	void	SetCaliperSearchDirection(CaliperSearchDirection nDirection);// 设置卡尺搜索方向
	void	SetCaliperSearchLength(double dSearchLength);				// 设置卡尺搜索长度
	void	SetCaliperProjectionLength(double dProjectionLength);		// 设置卡尺投影长度
	int		GetCaliperNum()					const;			// 获取卡尺数量
	int		GetCaliperSearchDirection()		const;			// 获取卡尺搜索方向
	double	GetCaliperSearchLength()		const;			// 获取卡尺搜索长度
	double	GetCaliperProjectionLength()	const;			// 获取卡尺投影长度

	// 设置卡尺属性
	void SetCaliperProperty(double dContrastThreshold,			// 设置卡尺对比度阈值
		EdgePolarity nEdgePolarity,			// 设置卡尺极性
		long lSize);						// 设置卡尺滤波器半宽尺寸

	// 获取卡尺属性
	void GetCaliperProperty(double& dContrastThreshold, EdgePolarity& nEdgePolarity, long& lSize);

	// 是否启用按分数排序
	void EnableCaliperSortByScore(bool bSortByScore);
	bool IsEnableCaliperSortByScore()const;

	// 设置/获取局外点百分比
	void SetOutLinePrecent(int precent);	//precent:0~100
	int  GetOutLinePrecent()const;

	void EnableCircleMask(bool bEnable);							// 是否启用掩膜
	bool IsEnableCircleMask()const;								
	void SetCaliperMasked(std::vector<bool>& vMasked);				// 设置卡尺是否被掩模覆盖
	void GetCaliperMasked(std::vector<bool>& vMasked)const;			// 获取卡尺是否被掩模覆盖
	void SetMaskData(std::vector<double> vMaskData);				// 设置工具掩模数据
	void GetMaskData(std::vector<double>& vMaskData) const;			// 获取工具掩模数据

	// 设置标记区域数据
	void SetMarkRegionData(double dRangeStart,double dRangeEnd);
	void GetMarkRegionData(double& dRangeStart,double& dRangeEnd)const;

public:
	// 设置/获取匹配方法
	void SetMatchMethod(MatchMethod method);
	int GetMatchMethod() const;

	// 是否启用设置标记深度,默认不开启
	void EnableMarkDepth(bool bEnable);
	bool IsEnableMarkDepth()const;

	//设置/获取标记深度
	void SetMarkDepth(int nMarkDepth);
	int GetMarkDepth()const;

	// 设置/获取标记角度是否修正，默认不修正
	void SetMarkAngleCorrection(bool bCorrection);
	bool GetMarkAngleCorrection() const;

	// 训练
	bool Train(cpImage& image); 

	// 获取训练数据
	bool GetTrainData(std::vector<double>& vdTrainData);

	// 获取训练的标记角度[-180, 180)
	scDegree GetTrainMarkAngle();

	// 获取训练图像上拟合的圆
	scCircle GetTrainCircle();
	
	// 获取训练图像的标记的宽度
	int GetTrainMarkWidth();

	// 获取训练时调整过的圆环
	scAnnulusSection GetTrainAnnulusAdjusted();	

	// 获取训练时采样后的图像
	bool GetTrainSampleImage(cpImage& sampleImage);

	// 获取训练时找标记圆参与拟合的数据点
	void GetTrainFitData(std::vector<sc2Vector>& points);
	void GetTrainUsed(std::vector<bool>& used);

	// 保存/加载训练文件
	bool SaveTrainModelToBinaryFile(const TCHAR* strFilePath);
	bool LoadTrainModelFromBinaryFile(const TCHAR* strFilePath);

public:
	// 是否使用训练时的掩膜
	void EnableTrainCircleMask(bool bEnable);							
	bool IsEnableTrainCircleMask()const;	


	// 设置搜索时找标记圆工具所在圆弧的参数
	void SetSearchCircularArcParam(const sc2Vector& vCenter,	// 圆心坐标
									double dRadius,				// 半径
									const scRadian& rAngleStart,// 起始角度，单位弧度，值域[-π，π)
									const scRadian& rAngleSpan);// 张角，值域[-4π，4π)

	// 获取搜索时找标记圆工具所在圆弧的参数
	void GetSearchCircularArcParam(sc2Vector& vCenter,			// 圆心坐标
									double& dRadius,			// 半径
									scRadian& rAngleStart,		// 起始角度，单位弧度，值域[-π，π)
									scRadian& rAngleSpan) const;// 张角，值域[-4π，4π)

	// 设置\获取搜索时找标记圆工具所在的圆弧
	void	SetSearchCircularArc(const scCircularArc& circularArc);		
	scCircularArc GetSearchCircularArc() const;					

	// 搜索 
	bool Excute(cpImage& image);

	// 获取搜索结果
	scMarkedCircleSearchResult	GetResult();

	// 获取搜索时调整过的圆环
	scAnnulusSection GetSearchAnnulusAdjusted();	

	// 获取搜索时采样后的图像
	bool GetSearchSampleImage(cpImage& sampleImage);

	// 获取搜索数据
	bool GetSearchData(std::vector<double>& vdSearchData);

	// 获取搜索时找标记圆参与拟合的数据点
	void GetSearchFitData(std::vector<sc2Vector>& points);
	void GetSearchUsed(std::vector<bool>& used);

private:
	bool GetMarkCircleAndSampleImage(cpImage& image, bool bTrainOrSearch,scCircle& circle,scAnnulusSection& annulus,cpImage& SampleImage);

private:
	FindMarkedCircleParam   m_FindMarkedCircleParam;

	// 训练结果参数
	std::vector<double>		m_vdTrainData;
	scDegree				m_TrainMarkAngle;
	scCircle				m_TrainCircle;
	int						m_nTrainMarkWidth;
	scAnnulusSection		m_TrainAnnulusSection;
	cpImage					m_TrainSampleImage;
	double					m_dMarkRegionCenter;
	std::vector<sc2Vector>	m_vTrainFitPoints;
	std::vector<bool>		m_vbTrainUsed;

	// 搜索参数
	bool  m_bEnableTrainCircleMask;
	scCircularArc m_SearchCircularArc;

	// 搜索结果参数
	scMarkedCircleSearchResult	m_Result;
	scAnnulusSection			m_SearchAnnulusSection;
	cpImage						m_SearchSampleImage;
	std::vector<double>			m_vdSearchData;
	std::vector<sc2Vector>		m_vSearchFitPoints;
	std::vector<bool>			m_vbSearchUsed;
};

#pragma pack(pop)