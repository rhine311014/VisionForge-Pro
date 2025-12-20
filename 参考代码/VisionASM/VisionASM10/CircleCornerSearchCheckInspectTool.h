
#pragma once
#include "BaseSearchTool.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svPatternSearchRot.h"
#include "svGuiAffineRect.h"

#include "svFind.h"
#include "svGuiFindLine.h" 
#include "svGuiLine.h"
#include "svGuiCoordCross.h"
#include "svGuiLineSeg.h"
#include "svGuiText.h"
#include "svGuiFindObround.h"
#include "svGuiObround.h"
#include "svObround.h"
#include "svFindObround.h"

#define LINE_NUM 2
#define CIRCLE_NUM 2
#define DIST_MEASURE_NUMS 12
#define INI_SEARCH_ANGLE 2
#define INI_CONTRASTTHRE 5
#define INI_ACCEPTTHRE	0.4
#define INI_CONFUSIONTHRE 0.6
//训练Data参数类
typedef struct tagCircleCornerCheckSearchTrainDataParam : public CBaseTrainDataParam
{
	CoarseScale		m_CoarseScale;						// 最粗定位层次
	Accuracy		m_Accuracy;							// 定位精度
	double			m_dAngleLow;						// 低角度
	double			m_dAngleHigh;						// 高角度
	bool			m_bMaskEnable;						// 是否设置掩模图像

	double			m_dGrau;							// 颗粒度
	long			m_lNoiseThre;						// 噪声阈值
	long			m_lTotalMag;						// 幅值和

	double			m_dContrastThre[LINE_NUM] ;			// 对比度阈值
	long			m_lFilterHalfWidth[LINE_NUM];		// 滤波器半宽
	int				m_nOutLineNum[LINE_NUM];			// 局外点
	EdgePolarity	m_edgeProperty[LINE_NUM];			// 边缘属性
	bool			m_bSortByScore[LINE_NUM];			// 按分数排序
	BOOL			m_bEnableDualEdge[LINE_NUM];		// 启用/禁用找线工具双边缘


	bool			m_bEnableCheck;						//启用检测
	CSearchToolType m_SearchToolType;					//定位工具类型
	int				m_nCircleToolType;					//检测工具类型,1个圆，2个圆，长圆

	//找圆工具
	double			m_dCircleContrastThre[CIRCLE_NUM];						// 对比度阈值
	long			m_lCircleFilterHalfWidth[CIRCLE_NUM];					// 滤波器半宽
	int				m_nCircleOutLineNum[CIRCLE_NUM];						// 局外点
	EdgePolarity	m_edgeCircleProperty[CIRCLE_NUM];						// 边缘属性
	bool			m_bCircleSortByScore[CIRCLE_NUM];						// 按分数排序

	//找长圆工具
	double			m_dObroundContrastThre;						// 对比度阈值
	long			m_lObroundFilterHalfWidth;					// 滤波器半宽
	int				m_nObroundOutLineNum;						// 局外点
	EdgePolarity	m_edgeObroundProperty;						// 边缘属性
	bool			m_bObroundSortByScore;						// 按分数排序

	tagCircleCornerCheckSearchTrainDataParam()
	{
		m_CoarseScale	=	CoarseScale32;
		m_Accuracy		=	AccuracyCoarse;
		m_dAngleLow		=	-INI_SEARCH_ANGLE;
		m_dAngleHigh	=	INI_SEARCH_ANGLE;
		m_bMaskEnable	=	false;

		m_dGrau			= 6.0;
		m_lNoiseThre	= 10;
		m_lTotalMag		= 200;

		for (int i=0; i<LINE_NUM; i++)
		{
			m_dContrastThre[i]		= 1.0;
			m_lFilterHalfWidth[i]	= 1;
			m_nOutLineNum[i]		= 0;
			m_edgeProperty[i]		= eDontCare;
			m_bSortByScore[i]		= true;
			m_bEnableDualEdge[i]	= false;
		}

		m_bEnableCheck = false;
		m_SearchToolType = ePatternCornerSearchTool;
		m_nCircleToolType = 1;

		for (int i = 0; i < CIRCLE_NUM; i++)
		{
			m_dCircleContrastThre[i]	= 10.0;
			m_lCircleFilterHalfWidth[i]	= 1;
			m_nCircleOutLineNum[i]		= 10;
			m_edgeCircleProperty[i]		= eDarkToLight;
			m_bCircleSortByScore[i]		= true;
		}
		m_dObroundContrastThre		= 10.0;
		m_lObroundFilterHalfWidth	= 1;
		m_nObroundOutLineNum		= 10;
		m_edgeObroundProperty		= eDarkToLight;
		m_bObroundSortByScore		= true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCircleCornerSearchCheckInspectTool;
	}

	virtual CSearchToolType GetSubSearchToolType()
	{
		return m_SearchToolType;
	}

	virtual bool SetSubSearchToolType(CSearchToolType mSearchToolType)
	{
		m_SearchToolType = mSearchToolType;
		return true;
	}

	virtual int GetCircleToolType()
	{
		return m_nCircleToolType;
	}

	virtual bool SetSubCircleToolType(int mInspectToolType)
	{
		m_nCircleToolType = mInspectToolType;
		return true;
	}

}CCircleCornerCheckSearchTrainDataParam;

// CircleCornerCheckSearch定位工具训练Gui参数类
typedef struct tagCircleCornerCheckSearchTrainGuiParam : public CBaseTrainGuiParam
{
	//角定位工具GUI训练参数 
	scRect			m_TrainRect;						// 训练区域
	scAffineRect	m_TrainAffineRect;					// 几何训练区域
	sc2Vector		m_ModelPoint;						// 参考点
	scRect			m_SearchRect;						// 搜索区域
	scObround		m_Obround;							// 长圆形

	// 找线工具
	int				m_nCaliperNum[LINE_NUM];			// 卡尺个数
	double			m_dStartX[LINE_NUM];				// 起始点X
	double			m_dStartY[LINE_NUM];				// 起始点Y
	double			m_dEndX[LINE_NUM];					// 终止点X
	double			m_dEndY[LINE_NUM];					// 终止点Y
	double			m_dCaliperProjectLen[LINE_NUM];		// 投影长度
	double			m_dCaliperSearchLen[LINE_NUM];		// 搜索长度
	scRadian		m_CaliperSearchRotation[LINE_NUM];	// 搜索角度

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainRectVisible;				// 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	bool			m_bLineVisible;						// 线是否可见
    bool			m_bEnableCornerLineMask[LINE_NUM];
	svStd vector<double> m_vdMaskData_FindLine[LINE_NUM];// 掩膜数据
	svStd vector<bool> m_vbCaliperMask_FindLine[LINE_NUM];// 卡尺是否被掩膜

	scRect			m_TrainRect1;						// 训练区域
	scAffineRect	m_TrainAffineRect1;					// 几何训练区域
	sc2Vector		m_ModelPoint1;						// 参考点
	scRect			m_SearchRect1;						// 搜索区域
	COLORREF        m_LineColor1;						// 图形颜色
	int				m_nLineWidth1;						// 图形线宽
	bool			m_bTrainRectVisible1;				// 训练区域是否可见
	bool			m_bModelPointVisible1;				// 模板参考点是否可见
	bool			m_bSearchRectVisible1;				// 搜索区域是否可见

	//找圆工具
	sc2Vector		m_CircleCenter_1[CIRCLE_NUM];						// 圆心X
	double			m_dCircleRadius[CIRCLE_NUM];						// 半径
	scRadian		m_CircleArcStart[CIRCLE_NUM];						// 起始弧度
	scRadian        m_CircleArcSpan[CIRCLE_NUM];						// 弧度跨度
	int				m_nCircleCaliperNum[CIRCLE_NUM];					// 卡尺个数
	double			m_dCircleCaliperProjectLen[CIRCLE_NUM];				// 投影长度
	double			m_dCircleCaliperSearchLen[CIRCLE_NUM];				// 搜索长度
	CaliperSearchDirection m_CircleCaliperSearchDirection[CIRCLE_NUM];	// 搜索方向
	bool			m_bCircleVisible_1[CIRCLE_NUM];						// 找圆工具是否可见

	//找长圆工具
	sc2Vector		m_ObroundCenter;						// 圆心X
	double			m_dObroundRotation;						// 旋转角度
	double			m_dObroundSideXLen;							// X边长度
	double			m_dObroundSideYLen;							// Y边长度
	int				m_nObroundCaliperNum;					// 卡尺个数
	double			m_dObroundCaliperProjectLen;			// 投影长度
	double			m_dObroundCaliperSearchLen;				// 搜索长度
	CaliperSearchDirection m_ObroundCaliperSearchDirection;	// 搜索方向
	bool			m_bObroundVisible;						// 找长圆工具是否可见

	tagCircleCornerCheckSearchTrainGuiParam()
	{
		m_nCaliperNum[0]				= 50;
		m_dStartX[0]					= 1800.0;
		m_dStartY[0]					= 1700.0;
		m_dEndX[0]						= 1800.0;
		m_dEndY[0]						= 2300.0;
		m_dCaliperProjectLen[0]			= 30.0;
		m_dCaliperSearchLen[0]			= 250.0;
		m_CaliperSearchRotation[0]		= scRadian(skPI/2.0);
        m_bEnableCornerLineMask[0]      =false;

		m_nCaliperNum[1]				= 50;
		m_dStartX[1]					= 2600.0;
		m_dStartY[1]					= 800.0;
		m_dEndX[1]						= 3500.0;
		m_dEndY[1]						= 800.0;
		m_dCaliperProjectLen[1]			= 30.0;
		m_dCaliperSearchLen[1]			= 250.0;
		m_CaliperSearchRotation[1]		= scRadian(skPI/2.0);
        m_bEnableCornerLineMask[1]      =false;

		m_LineColor						= RGB(0, 0, 255);
		m_nLineWidth					= 1;
		m_bModelPointVisible			= true;
		m_bTrainRectVisible				= true;
		m_bSearchRectVisible			= true;
		m_bLineVisible					= true;

		m_LineColor1				    = RGB(0, 0, 255);
		m_nLineWidth1					= 1;
		m_bModelPointVisible1			= true;
		m_bTrainRectVisible1			= true;
		m_bSearchRectVisible1			= true;

		

		for (int i = 0; i < CIRCLE_NUM; i++)
		{
			m_CircleCenter_1[i]					= sc2Vector(200,200);
			m_dCircleRadius[i]					= 100.0;
			m_CircleArcStart[i]					= scRadian(0);
			m_CircleArcSpan[i]					= scRadian(sk2PI);
			m_nCircleCaliperNum[i]				= 20;
			m_dCircleCaliperProjectLen[i]		= 10.0;
			m_dCircleCaliperSearchLen[i]		= 50.0;
			m_CircleCaliperSearchDirection[i]	= eInward;
			m_bCircleVisible_1[i]			= true;
		}

		m_Obround = scObround(sc2Vector(200,200),100,100,scRadian(0));
		m_ObroundCenter = sc2Vector(200,200);		// 圆心X
		m_dObroundRotation = 0;						// 旋转角度
		m_dObroundSideXLen = 100;					// X边长度
		m_dObroundSideYLen = 100;					// Y边长度
		m_nObroundCaliperNum	= 20;				// 卡尺个数
		m_dObroundCaliperProjectLen	= 10;			// 投影长度
		m_dObroundCaliperSearchLen = 50;			// 搜索长度
		m_ObroundCaliperSearchDirection = eInward;	// 搜索方向
		m_bObroundVisible          = true;			// 找长圆工具是否可见

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCircleCornerSearchCheckInspectTool;
	}

}CCircleCornerCheckSearchTrainGuiParam;

// PatternnCircleCornerSearch定位工具搜索Data参数类
typedef struct tagCircleCornerCheckSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double  m_dConfusionThre;							// 混淆阈值。混淆阈值不小于接收阈值
	double  m_dAngleLow;								// 低角度
	double	m_dAngleHigh;								// 高角度

	int		m_nSearchNum1;								// 搜索个数
	double	m_dAcceptThre1;								// 接收阈值
	double  m_dConfusionThre1;							// 混淆阈值。混淆阈值不小于接收阈值
	double  m_dAngleLow1;								// 低角度
	double	m_dAngleHigh1;								// 高角度
	double  m_dSizeLow1;
	double  m_dSizeHigh1;

	bool    m_bSizeCheckUseToTool;										// 尺寸检查结果用于整个定位工具
	bool    m_bEnableSizeCheck;											// 启用尺寸检查
	bool	m_bDistCheckUseToTool;										// 尺寸检查结果用于整个定位工具
	bool	m_bEnableDistCheck;										    // 启用距离检查
	bool	m_bEnableDispLineSeg[CIRCLE_NUM];							// 显示线段
	double 	m_dPixelRatio;												// 像素当量 （mm/piexl）

	bool	m_bEnableFlu[DIST_MEASURE_NUMS];							// 启用公差管控
	double	m_dDistanceBase[DIST_MEASURE_NUMS];							// 距离基准值
	double	m_dDistanceFlu[DIST_MEASURE_NUMS];							// 距离波动值


	bool	m_bEnableMax1[DIST_MEASURE_NUMS];							// 启用最大值管控
	double	m_dDistanceMax1[DIST_MEASURE_NUMS];							// 最大值管控值 (mm)
	bool	m_bEnableMin1[DIST_MEASURE_NUMS];							// 启用最小值管控
	double 	m_dDistanceMin1[DIST_MEASURE_NUMS];							// 最小值管控值 (mm)
	bool	m_bEnableInspectOKNG[DIST_MEASURE_NUMS];					// 是否启用检测NGOK状态，启用时 检测的NG 状态会影响定位结果最后的OKNG状态
	CString m_strInspectDistanceValueLabel[DIST_MEASURE_NUMS];			//测量值显示 标签名

	int m_nDistanceType;		// 双圆-与垂直水平线的距离模式：0 -双圆圆心连线中点与垂直水平线的距离、 1-圆1圆心与垂直水平线的距离、2-圆2圆心与垂直水平线的距离  3 同时管控圆1 圆2 的圆心，圆心连线中点与圆心与垂直水平线的距离；

	//Blob分析参数
	bool			m_bBlobInspectUseToTool;//Blob检查结果用于整个定位工具
	bool			m_bEanbleBlobInspect;	//启用Blob检测
	int  			m_nDefectType;			//黑或者白
	int				m_nBinaryThre;			//阈值
	int  			m_nMinAcceptArea;		//区域最小面积
	int 			m_nMaxAcceptArea;		//区域最大面积
	double			m_dMaxDefectArea;		//
	double			m_dAreaScaleRatio;		//检测面积缩放比例

	tagCircleCornerCheckSearchDataParam()
	{
		m_nSearchNum				= 1;
		m_dAcceptThre				= INI_ACCEPTTHRE;
		m_dConfusionThre			= INI_CONTRASTTHRE;
		m_dAngleLow					= -INI_SEARCH_ANGLE;
		m_dAngleHigh				= INI_SEARCH_ANGLE;

		m_nSearchNum1				= 1;
		m_dAcceptThre1				= INI_ACCEPTTHRE;
		m_dConfusionThre1			= INI_CONTRASTTHRE;
		m_dAngleLow1				= -INI_SEARCH_ANGLE;
		m_dAngleHigh1				= INI_SEARCH_ANGLE;
		m_dSizeLow1					= 1.0;
		m_dSizeHigh1				= 1.0;
		m_bEnableDistCheck			=true;
		m_nDistanceType				=0;
		for(int i=0; i < DIST_MEASURE_NUMS; i++)
		{
			m_dPixelRatio				= 1;
			m_bEnableMax1[i]			= false;
			m_dDistanceMax1[i]			= 1;
			m_bEnableMin1[i]			= false;
			m_dDistanceMin1[i]			= 0.0;

			m_bEnableFlu[i]			= false;
			m_dDistanceBase[i]		 = 1;
			m_dDistanceFlu[i]		 = 0.2;

			CString strTemp;
			strTemp.Format(_T("D%d:"),i);
			m_strInspectDistanceValueLabel[i] = strTemp;
		}

		m_bEanbleBlobInspect = false;
		m_nDefectType = 1;
		m_nBinaryThre = 80;
		m_nMinAcceptArea = 3;
		m_nMaxAcceptArea = 100000;
		m_dMaxDefectArea = 50.0;
		m_dAreaScaleRatio = 0.9;
		m_bEnableDispLineSeg[0] = false;
		m_bEnableDispLineSeg[1] = false;
		
	}

	virtual void InspectOKNG_AndResultInfo()
	{
		
	}


	virtual CSearchToolType GetSearchToolType()
	{
		return eCircleCornerSearchCheckInspectTool;
	}

}CCircleCornerCheckSearchSearchDataParam;

// CircleCornerCheckSearch定位工具搜索Gui参数类
typedef struct tagCircleCornerCheckSearchGuiParam : public CBaseSearchGuiParam
{

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bRectVisible;						// 搜索结果矩形可见
	bool			m_bPointVisible;					// 参考点是否可见

	bool			m_bLineVisible;						// 找线工具是否可见
	bool			m_bLineCrossVisible;				// 找线工具是否可见
	bool			m_bVisibleFitPoint1;				// 拟合1数据点是否可见
	bool			m_bVisibleFitPoint2;				// 拟合2数据点是否可见

	COLORREF        m_cLineColorOKFitPoint1;			// 拟合1数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint1;			// 拟合1数据点NG颜色

	bool			m_bCircleVisible_1[CIRCLE_NUM];					// 圆形是否可见
	bool			m_bCircleCenterVisible_1[CIRCLE_NUM];			// 圆心是否可见
	bool			m_bEnableShowCirclePoint[CIRCLE_NUM];			// 拟合数据点是否可见

	bool			m_bObroundVisible;					// 长圆形是否可见
	bool			m_bObroundCenterVisible;			// 长圆圆心是否可见
	bool			m_bEnableShowObroundPoint;			// 拟合数据点是否可见




	tagCircleCornerCheckSearchGuiParam()
	{
		m_LineColor					= RGB(0, 255, 0);
		m_nLineWidth				= 1;
		m_bRectVisible				= true;
		m_bPointVisible				= true;
		
		m_bLineVisible				= true;
		m_bLineCrossVisible			= true;


		m_bVisibleFitPoint1			= false;
		m_bVisibleFitPoint2			= false;
		m_cLineColorOKFitPoint1		= RGB(255,255,0);
		m_cLineColorNGFitPoint1		= RGB(255,0,0);

		for (int i = 0; i < CIRCLE_NUM; i++)
		{
			m_bCircleVisible_1[i] = true;
			m_bCircleCenterVisible_1[i] = true;
			m_bEnableShowCirclePoint[i] = false;

		}
		m_bObroundVisible = true;
		m_bObroundCenterVisible =true;
		m_bEnableShowObroundPoint = false;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCircleCornerSearchCheckInspectTool;
	}

}CCircleCornerCheckSearchSearchGuiParam;

// 训练结果
typedef struct tagCircleCornerCheckSearchTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y
	double m_dModelX1; // 参考点X1					
	double m_dModelY1; // 参考点Y1

	double m_dStartX[LINE_NUM]; // 线段起点X
	double m_dStartY[LINE_NUM]; // 线段起点Y
	double m_dEndX[LINE_NUM];	  // 线段终点X
	double m_dEndY[LINE_NUM];	  // 线段终点Y

	double m_dCircleCenterX[CIRCLE_NUM];
	double m_dCircleCenterY[CIRCLE_NUM];
	double m_dCircleRadius[CIRCLE_NUM];

	double m_dObroundCenterX;
	double m_dObroundCenterY;
	double m_dObroundSideXLen;
	double m_dObroundSideYLen;
	double m_dObroundRotation;

	tagCircleCornerCheckSearchTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
		m_dModelX1 = 0;
		m_dModelY1 = 0;

		for (int i=0; i<LINE_NUM; i++)
		{
			m_dStartX[i] = 0;
			m_dStartY[i] = 0;
			m_dEndX[i]   = 0;
			m_dEndY[i]   = 0;
		}

		for(int i=0;i < CIRCLE_NUM;i++)
		{
			m_dCircleCenterX[i] = 0;
			m_dCircleCenterY[i] = 0;
			m_dCircleRadius[i]  = 0;	
		}

		m_dObroundCenterX = 0;
		m_dObroundCenterY = 0;
		m_dObroundSideXLen = 100;
		m_dObroundSideYLen = 100;
		m_dObroundRotation = 0;
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eCircleCornerSearchCheckInspectTool;
	}
}CCircleCornerCheckSearchTrainResult;

class CCircleCornerCheckSearchTool :public CBaseSearchTool
{
public:
	CCircleCornerCheckSearchTool(void);
	CCircleCornerCheckSearchTool(CCircleCornerCheckSearchTool &cCircleCornerCheckSearchTool);
	CCircleCornerCheckSearchTool& operator=(CCircleCornerCheckSearchTool &cCircleCornerCheckSearchTool);
	~CCircleCornerCheckSearchTool(void);
private:
	// 初始化成员
	void Initialize_Member_Variables();
	
	// 执行区域粗定位搜索
	bool ExecutePatternSerach(cpImage &ImageSearch,scPatternSearchRot* m_PatternSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dConfusionThre,double dTransX,double dTransY,double dRotation);
	
	// 执行几何粗定位搜索
	bool ExecuteQuickSearch(cpImage &ImageSearch,scQuickSearchTool* m_QuickSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dAngleLow,double m_dAngleHigh,double m_dSizeLow1,double m_dSizeHigh1,double dTransX,double dTransY,double dRotation);
	
	// 执行找线
	bool ExecuteFindLine(cpImage &ImageSearch,scFindLine* m_FindLineTool,StructFindLineTrainData  m_sFindLineTrainData,double dTransX,double dTransY,double dRotation,scLineSeg &ResultLine);

	// GUI 显示矩形
	void ShowResultRect(std::vector<scGuiGraphic*>& pGuiSearchRectArray,scGuiAffineRect GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* pAffineRect,const CString strRect,COLORREF color,int LineWidth);
	// GUI 显示交叉点
	void ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,COLORREF color,int LineWidth);
	void ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,const CString strCrossLabel,COLORREF color,int LineWidth);
	// GUI 显示线
	void ShowResultLine(std::vector<scGuiGraphic*>& m_pGuiSearchLineArray,scGuiLine m_GuiSearchLine[MAXRESNUM],const int nOffset,scLine *m_scLine,const CString strLine,COLORREF color,int LineWidth);
	void ShowResultLineSeg(std::vector<scGuiGraphic*>& m_pGuiSearchLineArray,scGuiLineSeg m_GuiSearchLine[MAXRESNUM],const int nOffset,scLineSeg *m_scLine,const CString strLine,COLORREF color,int LineWidth);
	void ShowResultLineSegText(std::vector<scGuiGraphic*>& m_pGuiSearchLineArray,scGuiLineSeg m_GuiSearchLine[MAXRESNUM],const int nOffset,scLineSeg *m_scLine,double dInspectDistance,const CString strLine,COLORREF color,int LineWidth);
public:
	// 
	// 	//////////////////////////工具属性相关////////////////////////////////
	virtual CSearchToolType GetSearchToolType();										    // 获取工具类型

	//////////////////////////显示更新相关////////////////////////////////
	virtual bool SetDisplay(scGuiDisplay *pGuiDisplay);										// 设置显示控件指针				
	virtual scGuiDisplay* GetDisplay();														// 获取显示空间指针

	virtual bool SetTrainResultDisplay(scGuiDisplay *pGuiDisplay);							// 设置训练结果显示控件指针
	virtual scGuiDisplay* GetTrainResultDisplay();											// 获取训练结果显示控件指针


	virtual bool SetGuiKeyText(CString strGuiKeyText);										// 设置Gui图形的关键字
	virtual bool GetGuiKeyText(CString &strGuiKeyText);										// 获取Gui图形的关键字
	virtual bool ShowTrainGui(bool bShow) ;													// 是否显示训练界面
	virtual bool ShowTrainResultGui(bool bShow) ;											// 是否显示训练结果界面
	virtual bool ShowSearchResultGui(bool bShow);											// 是否显示搜索界面
	virtual bool UpdateGuiDisplay();														// 更新界面显示
	virtual bool ClearAllGui();															    // 清除定位工具在Display上的控件
	virtual bool ClearResult();																// 清除搜索结果
    virtual bool SetGuiByCam(double nWidth,double nHeight);								// 根据相机宽高设置GUI大小
	///////////////////////////训练相关/////////////////////////////////////
	virtual bool SetTrainDataParamForce(CBaseTrainDataParam *pTrainDataParam);				// 强制 调整m_TrainDataParam 值
	virtual bool SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam);					// 设置训练Data参数				
	virtual bool GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam);					// 获取训练Data参数
	virtual bool SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam);						// 获取搜索Gui参数
	virtual bool GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam);						// 获取搜索Gui参数
	virtual bool SetTrainImage(const cpImage &ImageTrain);									// 设置训练原始图像
	virtual bool GetTrainImage(cpImage &ImageTrain);										// 获取训练原始图像
	virtual bool SetMaskImage(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool SetMaskImage2(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool GetMaskImage(cpImage &ImageMask);											// 获取训练原始图像
	virtual bool GetMaskImage2(cpImage &ImageMask);										// 获取训练原始图像
	virtual bool Train();																	// 执行训练
	virtual bool IsTrained();																// 是否训练成功
	virtual bool GetTrainResultImage(cpImage &ImageTrainResult);							// 获取训练结果图像
	//virtual bool GetTrainResultImage2(cpImage &ImageTrainResult);						// 获取训练结果图像
	virtual bool GetTrainResult(CBaseTrainResult *pTrainResult);							// 获取训练结果
	virtual bool GetPatternImage(cpImage &ImagePattern);	// 获取训练模板原始图像
	virtual bool GetPatternImage2(cpImage &ImagePattern);
	virtual bool RestoreTrainParam();														// 恢复训练参数	

	///////////////////////////搜索相关/////////////////////////////////////	
	virtual bool SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam);				// 设置搜索Data参数
	virtual bool SetSearchDataParamForce(CBaseSearchDataParam *pSearchDataParam);			// 强制 调整m_SearchDataParam 值
	virtual bool GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam);				// 获取搜索Data参数
	virtual bool SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam);					// 设置搜索Gui参数
	virtual bool GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam);					// 获取搜索Gui参数
	virtual bool Search(cpImage &ImageSearch);										// 执行搜索 
	virtual int  GetResultNum();															// 获取搜索结果个数
	virtual bool GetResult(int nIndex, CSearchResult &searchResult);						// 获取搜索结果
	virtual bool RestoreSearchParam();														// 恢复搜索参数	

	/////////////////////////参数加载与保存////////////////////////////////	
	virtual bool LoadModelFromFile(CString strDir);											// 加载模板文件, strDir为文件夹路径
	virtual bool SaveModelToFile(CString strDir);											// 保存模板文件, strDir为文件夹路径
	//////////////////////语言类型//////////////////////////////////////
	virtual bool LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern);			// 加载模板训区域图片， strDir为文件夹路径,ImagePattern为获取得到的训练区域图片
	virtual void	SetLanguage(int nLanguage);									// 设置中英文菜单 0：eng；1：chs


	virtual CString GetResultErrInfo();

private:

	/////////////////////////Gui控件相关////////////////////////////////	
	bool UpdateTrainGuiData(bool bSaveAndValidate);											// 更新训练控件数据信息	
	bool UpdateSearchGuiData(bool bSaveAndValidate);										// 更新搜索结果控件数据信息	
	bool ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray);					// 清除静态控件及其显示													
	bool UpdateTrainGuiDisplay();															// 更新训练Gui控件显示
	bool UpdateTrainResultGuiDisplay();														// 更新训练结果Gui控件显示
	bool UpdateSearchResultGuiDisplay();													// 更新搜索Gui控件显示
	bool ClearTrainGui();																	// 清除定位工具在Display上的训练控件
	bool ClearResultGui();																	// 清除定位工具在Display上的结果控件

	void ShowFitPoint(std::vector<scGuiGraphic*>& m_pGuiInspectFitPointArray,scGuiCoordCross m_GuiInspectFitPoint[MAXPTSNUM],scFindLine* m_FindLineTool,const CString strFitPoint);
	//bool ExecuteInspectSearch(const double dTransX,const double dTransY,const double dRotation,const cpImage &ImageSearch, CResultL2LInspectDistance &ResultL2LInspectDistance);

	bool GetFindLineCenterPoint(scFindLine* m_FindLineTool,sc2Vector &pointCenter2);

	bool GetTrainLineCircleSearchResult();															//训练时找线和找圆返回的结果
	bool SetSearchLineCircleParam();																//搜索时设置找线和找圆的参数
	bool SearchFindLineFunc(int nIndex ,cpImage &ImageSearch,double dRotation,double dTransX,double dTransY,CSearchResult& searchResult,scLineSeg & ResultLine1,scLineSeg & ResultLine2);//搜索时找直线的函数
	bool SearchFindCircleFunc(int nIndex,cpImage &ImageSearch,double dRotation,double dTransX,double dTransY,scLineSeg & ResultLine1,scLineSeg & ResultLine2,CSearchResult& searchResult);//搜索是找圆的函数
	bool SearchOneCircleDistMeasureFunc(scLineSeg & ResultLine1,scLineSeg & ResultLine2,scCircle & ResultCircle,int nOffset,CSearchResult& searchResult);
	bool SearchTwoCircleDistMeasureFunc(scLineSeg & ResultLine1,scLineSeg & ResultLine2,scCircle & ResultCircle1,scCircle & ResultCircle2,int nOffset,CSearchResult& searchResult);
	
	bool SearchOneObroundDistMeasureFunc(scLineSeg & ResultLine1,scLineSeg & ResultLine2,scObround & ResultObround,int nOffset,CSearchResult& searchResult);
	bool SearchOneObroundBlobInspect(cpImage &ImageSearch,scObround & ResultObround,CSearchResult& searchResult);

	bool SearchOneCircleBlobInspect(cpImage &ImageSearch,scCircle & ResultCircle,CSearchResult& searchResult);
private:
	scPatternSearchRot					m_PatternSearchTool;			// 区域角定位的粗定位搜索工具
	scQuickSearchTool                   m_QuickSearchTool;				// 几何角定位QuickSearch搜索工具

	scFindLine							m_FindLineTool[LINE_NUM];		// 找线工具
	scFindCircle						m_FindCircleTool_1[CIRCLE_NUM];	// 找圆工具
	scFindObround						m_FindObroundTool;				// 找长圆工具

	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd;			// 显示训练结果对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CCircleCornerCheckSearchTrainDataParam		m_tmpTrainDataParam;			// 当前训练Data参数
	CCircleCornerCheckSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CCircleCornerCheckSearchSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CCircleCornerCheckSearchSearchGuiParam		m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CCircleCornerCheckSearchTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像


	CCircleCornerCheckSearchTrainDataParam		m_TrainDataParam;				// 训练Data参数
	CCircleCornerCheckSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CCircleCornerCheckSearchSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CCircleCornerCheckSearchSearchGuiParam		m_SearchGuiParam;				// 搜索Gui参数	
	CCircleCornerCheckSearchTrainResult			m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像


	cpImage							m_ImageTrainResult;				// 训练结果图像

	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;			// 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功

	// 训练GUI
	scGuiRect							m_GuiTrainRect;					// 训练矩形
	scGuiAffineRect						m_GuiTrainAffineRect;			//几何训练区
	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint;				// 参考点
	scGuiFindLine                       m_GuiFindLine[LINE_NUM];		// 两个找线工具
	scGuiLine                           m_GuiTrainResultLine[LINE_NUM];	// 训练结果线段
	scGuiFindCircle                     m_GuiFindCircle_1[CIRCLE_NUM];		  // 找圆工具
	scGuiCircle                         m_GuiTrainResultCircle_1[CIRCLE_NUM]; // 训练结果圆
	scGuiContour						m_GuiContour;						// Blob交互控件
	scGuiFindObround                    m_GuiFindObround;				 // 长圆圆工具
	scGuiObround                        m_GuiTrainResultObround;         // 训练长圆结果圆

	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽
	scGuiFeaturelet						m_GuiTrainFeaturelet;			// 训练特征链集

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchRectArray;			// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;			// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineSegArray;		// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineTextArray;		// 搜索结果文本
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleArray;		// 搜索结果圆
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleCenterArray;	// 搜索结果圆心
	std::vector<scGuiGraphic*>          m_pGuiSearchObroundCenterArray;	// 搜索结果长圆中心
	std::vector<scGuiGraphic*>          m_pGuiSearchObroundArray;		// 搜索结果长圆



	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];
	scGuiCross							m_GuiSearchCross2[MAXRESNUM];
	scGuiLine							m_GuiSearchLine[MAXRESNUM];
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];
	scGuiLineSeg						m_GuiSearchLineSeg[MAXRESNUM];
    scGuiText							m_GuiSearchLineText[MAXRESNUM];
	scGuiCircle							m_GuiSearchCircle[MAXRESNUM];
	scGuiCross							m_GuiSearchCircleCenter[MAXRESNUM];
	scGuiObround						m_GuiSearchObround[MAXRESNUM];
	scGuiCross							m_GuiSearchObroundCenter[MAXRESNUM];

	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPoint1Array;
	scGuiCoordCross						m_GuiSearchResultFitPoint1[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPoint2Array;
	scGuiCoordCross						m_GuiSearchResultFitPoint2[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiSearchResultCircleFitPointArray1;
	scGuiCoordCross						m_GuiSearchResultCircleFitPoint1[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiSearchResultCircleFitPointArray2;
	scGuiCoordCross						m_GuiSearchResultCircleFitPoint2[MAXPTSNUM];

	bool								m_bBlobInspectFinsh;

	CString							m_strErrorDetail;	//超上限还是下限；
};

