// PatternCircleLineSearchTool.h: interface for the CPatternCircleLineSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "BaseSearchTool.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svPatternSearchRot.h"
#include "svGuiAffineRect.h"
#include "svGuiCross.h"
#include "svFind.h"
#include "svGuiFindLine.h" 
#include "svGuiLine.h"
#include "svGuiFindCircle.h"
#include "svGuiCircle.h"
#include "svGuiCoordCross.h"

#define PATTERN_CIRCLE_4LINE_NUM 4

// PatternCircleLineSearch定位工具训练Data参数类
typedef struct tagPatternCircle4LineSearchTrainDataParam : public CBaseTrainDataParam
{
	// 区域定位
	CoarseScale						m_CoarseScale;			// 最粗定位层次
	Accuracy						m_Accuracy;				// 定位精度
	double							m_dAngleLow;			// 低角度
	double							m_dAngleHigh;			// 高角度
	bool							m_bMaskEnable;			// 是否设置掩模图像

	// 找圆工具
	double			m_dCircleContrastThre;					   // 对比度阈值
	long			m_lCircleFilterHalfWidth;					// 滤波器半宽
	int				m_nCircleOutLineNum;						// 局外点
	EdgePolarity	m_CircleedgeProperty;						// 边缘属性
	bool			m_bCircleSortByScore;						// 按分数排序		;
	bool			m_bEnableCircleDualEdge;					// 启用/禁用找圆工具双边缘
	// 找线工具
	double			m_dContrastThre[PATTERN_CIRCLE_4LINE_NUM] ;			// 对比度阈值
	long			m_lFilterHalfWidth[PATTERN_CIRCLE_4LINE_NUM];		// 滤波器半宽
	int				m_nOutLineNum[PATTERN_CIRCLE_4LINE_NUM];			// 局外点
	EdgePolarity	m_edgeProperty[PATTERN_CIRCLE_4LINE_NUM];			// 边缘属性
	bool			m_bSortByScore[PATTERN_CIRCLE_4LINE_NUM];			// 按分数排序
	int				m_nLineAngleType[PATTERN_CIRCLE_4LINE_NUM];			// 直线角度类型：0,任意角度；1,水平线；2,垂直线；
	bool            m_bEnableLine[PATTERN_CIRCLE_4LINE_NUM];			// 直线启用
	bool			m_bEnableLineDualEdge[PATTERN_CIRCLE_4LINE_NUM];		// 启用/禁用找线工具双边缘
	tagPatternCircle4LineSearchTrainDataParam()
	{
		m_CoarseScale	=	CoarseScale1;
		m_Accuracy		=	AccuracyFine;
		m_dAngleLow		=	-10.0;
		m_dAngleHigh	=	10.0;
		m_bMaskEnable	=	false;

		for (int i=0; i<PATTERN_CIRCLE_4LINE_NUM; i++)
		{
			m_dContrastThre[i]		= 1.0;
			m_lFilterHalfWidth[i]	= 1;
			m_nOutLineNum[i]		= 10;
			m_edgeProperty[i]		= eLightToDark;
			m_bSortByScore[i]		= true;
			m_nLineAngleType[i]		= 0;
			m_bEnableLine[i]        = true;
			m_bEnableLineDualEdge[i] = false;
		}

		m_dCircleContrastThre		= 10.0;
		m_lCircleFilterHalfWidth	= 1;
		m_nCircleOutLineNum		    = 10;
		m_CircleedgeProperty		= eDarkToLight;
		m_bCircleSortByScore		= true;
		m_bEnableCircleDualEdge		= false;

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternCircle4LineSearchTool;
	}

}CPatternCircle4LineSearchTrainDataParam;

// PatternCircleLineSearch定位工具训练Gui参数类
typedef struct tagPatternCircle4LineSearchTrainGuiParam : public CBaseTrainGuiParam
{
	// 区域定位
	scRect			m_TrainRect;						// 训练区域
	sc2Vector		m_ModelPoint;						// 参考点
	scRect			m_SearchRect;						// 搜索区域

	// 找圆工具
	sc2Vector		m_CircleCenter;						// 圆心X
	double			m_dCircleRadius;					// 半径
	scRadian		m_CircleArcStart;							// 起始弧度
	scRadian        m_CircleArcSpan;							// 弧度跨度
	int				m_nCircleCaliperNum;						// 卡尺个数
	double			m_dCircleCaliperProjectLen;				// 投影长度
	double			m_dCircleCaliperSearchLen;				// 搜索长度
	CaliperSearchDirection m_CircleCaliperSearchDirection;	// 搜索方向
	bool			m_bCircleVisible;						// 找圆工具是否可见
	bool			m_bCircleCaliperVisible;				//找圆卡尺是否可见
	// 找圆工具掩摸
	svStd vector<double> m_vdCircleCaliperMaskData;				// 掩膜数据
	svStd vector<bool>   m_vbCircleCaliperMask;					// 卡尺是否被掩膜
	bool			     m_bEnableCircleMask;					// 是否启用掩膜

	// 找线工具
	int				m_nCaliperNum[PATTERN_CIRCLE_4LINE_NUM];			// 卡尺个数
	double			m_dStartX[PATTERN_CIRCLE_4LINE_NUM];				// 起始点X
	double			m_dStartY[PATTERN_CIRCLE_4LINE_NUM];				// 起始点Y
	double			m_dEndX[PATTERN_CIRCLE_4LINE_NUM];					// 终止点X
	double			m_dEndY[PATTERN_CIRCLE_4LINE_NUM];					// 终止点Y
	double			m_dCaliperProjectLen[PATTERN_CIRCLE_4LINE_NUM];		// 投影长度
	double			m_dCaliperSearchLen[PATTERN_CIRCLE_4LINE_NUM];		// 搜索长度
	scRadian		m_CaliperSearchRotation[PATTERN_CIRCLE_4LINE_NUM];	// 搜索角度
	bool			m_bLineCaliperVisible[PATTERN_CIRCLE_4LINE_NUM];	//找线卡尺是否可见
	// 找线工具掩摸
	svStd vector<double> m_vdLineCaliperMaskData[PATTERN_CIRCLE_4LINE_NUM];			// 掩膜数据
	svStd vector<bool>   m_vbLineCaliperMask[PATTERN_CIRCLE_4LINE_NUM];				// 卡尺是否被掩膜
	bool			     m_bEnableLineMask[PATTERN_CIRCLE_4LINE_NUM];			// 是否启用掩膜
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainRectVisible;				// 搜索区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	bool			m_bLineVisible;						// 找线工具是否可见
	


	tagPatternCircle4LineSearchTrainGuiParam()
	{
		//圆参数
		m_CircleCenter				    = sc2Vector(200,200);
		m_dCircleRadius					= 100.0;
		m_CircleArcStart				= scRadian(0);
		m_CircleArcSpan					= scRadian(sk2PI);
		m_nCircleCaliperNum				= 20;
		m_dCircleCaliperProjectLen		= 10.0;
		m_dCircleCaliperSearchLen		= 50.0;
		m_CircleCaliperSearchDirection	= eInward;
		m_bCircleVisible			    = true;
		m_bCircleCaliperVisible			= false;
		m_bEnableCircleMask				= false;
		//线参数
		for(int nIndex = 0; nIndex < PATTERN_CIRCLE_4LINE_NUM;nIndex++)
		{
			m_nCaliperNum[nIndex]				= 20;
			m_dStartX[nIndex]					= 100.0;
			m_dStartY[nIndex]					= 100.0;
			m_dEndX[nIndex]						= 100.0;
			m_dEndY[nIndex]						= 100.0;
			m_dCaliperProjectLen[nIndex]		= 20.0;
			m_dCaliperSearchLen[nIndex]			= 100.0;
			m_CaliperSearchRotation[nIndex]		= scRadian(skPI/2.0);
			m_bLineCaliperVisible[nIndex]		= false;
			m_bEnableLineMask[nIndex]			= false;
		}
		

		m_LineColor			 = RGB(0, 0, 255);
		m_nLineWidth		 = 1;
		m_bModelPointVisible = true;
		m_bTrainRectVisible	 = true;
		m_bSearchRectVisible = true;
		m_bLineVisible		  = true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternCircle4LineSearchTool;
	}

}CPatternCircle4LineSearchTrainGuiParam;

// PatternCircleLineSearch定位工具搜索Data参数类
typedef struct tagPatternCircle4LineSearchSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double  m_dConfusionThre;							// 混淆阈值。混淆阈值不小于接收阈值
	int		m_nSearchOutLineAngleType;					// 输出直线的类型，0水平，1垂直
	int     m_nSearchOutPointType;						// 输出点的类型，0左边，1右边，2上边，3下边
	bool    m_bEnableLineAngleJudge;					// 启用直线角度判断
	double  m_dJudgeLineAngle;							// 角度判断范围;

	bool    m_bEnable2LineAngleJudge;					// 启用两线夹角判断
	double  m_dAngleRange;							// 90°附近角度波动范围;
	tagPatternCircle4LineSearchSearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.6;
		m_dConfusionThre = 0.8;
		m_nSearchOutLineAngleType = 0;
		m_nSearchOutPointType     = 0;
		m_bEnableLineAngleJudge   = false;
		m_dJudgeLineAngle         = 1.0;
		m_bEnable2LineAngleJudge   = false;
		m_dAngleRange			  = 1.0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternCircle4LineSearchTool;
	}

}CPatternCircle4LineSearchSearchDataParam;

// PatternCircleLineSearch定位工具搜索Gui参数类
typedef struct tagPatternCircle4LineSearchGuiParam : public CBaseSearchGuiParam
{
	bool			m_bRectVisible;						// 搜索结果矩形可见
	bool			m_bPointVisible;					// 参考点是否可见

	//圆参数
	bool			m_bCircleVisible;					// 圆形是否可见
	bool			m_bCircleCenterVisible;				// 圆心是否可见

	//线参数
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽


	COLORREF        m_cLineColorOKFitPoint1;			// 拟合数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint1;			// 拟合数据点NG颜色
	bool			m_bEnableShowLinePoint;				// 拟合数据点是否可见

	bool			m_bLineVisible;						// 找线工具是否可见
	bool			m_bLineCrossVisible;				// 找线工具是否可见

	COLORREF        m_ChordLineColor;					// 图形颜色

	COLORREF        m_cLineColorTwoCrossLine;			// 拟合数据点OK颜色
	COLORREF        m_cLineColorFixLine;			    // 拟合数据点OK颜色

	bool			m_bEnableShowFitCirclePoint;		// 显示圆拟合点
	bool			m_bEnableShowFitLinePoint[PATTERN_CIRCLE_4LINE_NUM];		// 显示直线拟合点

	tagPatternCircle4LineSearchGuiParam()
	{
		m_LineColor		= RGB(0, 255, 0);
		m_nLineWidth	= 1;
		m_bRectVisible	= true;
		m_bPointVisible = true;
		m_bLineVisible			= true;
		m_bLineCrossVisible		= true;

		m_bCircleVisible		= true;
		m_bCircleCenterVisible	= true;

		m_cLineColorOKFitPoint1 = RGB(255,255,0);
		m_cLineColorNGFitPoint1 = RGB(255,0,0);
		m_ChordLineColor = RGB(0,0,255);
		m_bEnableShowLinePoint = true;
		m_cLineColorTwoCrossLine = RGB(0,255,255);
		m_cLineColorFixLine     = (200,255,255);
		m_bEnableShowFitCirclePoint = false;
		for (int i=0;i<PATTERN_CIRCLE_4LINE_NUM;i++)
		{
			m_bEnableShowFitLinePoint[i] =false;
		}

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternCircle4LineSearchTool;
	}

}CPatternCircle4LineSearchSearchGuiParam;

// PatternSearch训练结果
typedef struct tagPatternCircle4LineSearchTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y

	double m_dStartX[PATTERN_CIRCLE_4LINE_NUM]; // 线段起点X
	double m_dStartY[PATTERN_CIRCLE_4LINE_NUM]; // 线段起点Y
	double m_dEndX[PATTERN_CIRCLE_4LINE_NUM];	  // 线段终点X
	double m_dEndY[PATTERN_CIRCLE_4LINE_NUM];	  // 线段终点Y

	// 圆的参数
	double m_dCenterX;
	double m_dCenterY;
	double m_dRadius;

	tagPatternCircle4LineSearchTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;

		for (int i=0; i<PATTERN_CIRCLE_4LINE_NUM; i++)
		{
			m_dStartX[i] = 0;
			m_dStartY[i] = 0;
			m_dEndX[i]   = 0;
			m_dEndY[i]   = 0;
		}
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternCircle4LineSearchTool;
	}
}CPatternCircle4LineSearchTrainResult;

class CPatternCircle4LineSearchTool :public CBaseSearchTool
{
public:
	CPatternCircle4LineSearchTool(void);
	CPatternCircle4LineSearchTool(CPatternCircle4LineSearchTool &cPatternCircleLineSearchTool);
	CPatternCircle4LineSearchTool& operator=(CPatternCircle4LineSearchTool &cPatternCircleLineSearchTool);
	~CPatternCircle4LineSearchTool(void);

public:
// 
// 	//////////////////////////工具属性相关////////////////////////////////
 	virtual CSearchToolType GetSearchToolType();										    // 获取工具类型

	//////////////////////////显示更新相关////////////////////////////////
	virtual bool SetDisplay(scGuiDisplay *pGuiDisplay);										// 设置显示控件指针				
	virtual scGuiDisplay* GetDisplay();														// 获取显示空间指针
	virtual bool SetGuiKeyText(CString strGuiKeyText);										// 设置Gui图形的关键字
	virtual bool GetGuiKeyText(CString &strGuiKeyText);										// 获取Gui图形的关键字
	virtual bool ShowTrainGui(bool bShow) ;													// 是否显示训练界面
	virtual bool ShowTrainResultGui(bool bShow) ;											// 是否显示训练结果界面
	virtual bool ShowSearchResultGui(bool bShow);											// 是否显示搜索界面
	virtual bool UpdateGuiDisplay();														// 更新界面显示
	virtual bool ClearAllGui();															    // 清除定位工具在Display上的控件
	virtual bool ClearResult();																// 清除搜索结果
	virtual bool SetGuiByCam(double dWidth,double dHeight) ;								// 根据相机宽高设置GUI大小

	///////////////////////////训练相关/////////////////////////////////////
	virtual bool SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam);					// 设置训练Data参数				
	virtual bool GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam);					// 获取训练Data参数
	virtual bool SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam);						// 获取搜索Gui参数
	virtual bool GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam);						// 获取搜索Gui参数
	virtual bool SetTrainImage(const cpImage &ImageTrain);									// 设置训练原始图像
	virtual bool GetTrainImage(cpImage &ImageTrain);										// 获取训练原始图像
	virtual bool SetMaskImage(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool GetMaskImage(cpImage &ImageMask);											// 获取训练原始图像
	virtual bool Train();																	// 执行训练
	virtual bool IsTrained();																// 是否训练成功
	virtual bool GetTrainResultImage(cpImage &ImageTrainResult);							// 获取训练结果图像
	virtual bool GetTrainResult(CBaseTrainResult *pTrainResult);							// 获取训练结果
	virtual bool GetPatternImage(cpImage &ImagePattern);									// 获取训练模板原始图像
	virtual bool RestoreTrainParam();														// 恢复训练参数	

	///////////////////////////搜索相关/////////////////////////////////////	
	virtual bool SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam);				// 设置搜索Data参数
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
	virtual bool LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern);			// 加载模板训区域图片， strDir为文件夹路径,ImagePattern为获取得到的训练区域图片
	//////////////////////语言类型//////////////////////////////////////
	virtual void	SetLanguage(int nLanguage);									// 设置中英文菜单 0：eng；1：chs
	virtual CString	GetResultErrInfo();											// 获取错误序列号
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


private:
	scPatternSearchRot					m_PatternSearchTool;			// PatternSearch搜索工具
	scFindLine							m_FindLineTool[PATTERN_CIRCLE_4LINE_NUM];		// 找线工具
	scFindCircle						m_FindCircleTool;						// 找圆工具
	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CPatternCircle4LineSearchTrainDataParam			m_tmpTrainDataParam;			// 当前训练Data参数
	CPatternCircle4LineSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CPatternCircle4LineSearchSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CPatternCircle4LineSearchSearchGuiParam			m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CPatternCircle4LineSearchTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
	CPatternCircle4LineSearchTrainDataParam			m_TrainDataParam;				// 训练Data参数
	CPatternCircle4LineSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CPatternCircle4LineSearchSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CPatternCircle4LineSearchSearchGuiParam			m_SearchGuiParam;				// 搜索Gui参数	
	CPatternCircle4LineSearchTrainResult			m_TrainResult;					// 训练结果
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
	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint;				// 参考点
	scGuiFindLine                       m_GuiFindLine[PATTERN_CIRCLE_4LINE_NUM];		// 两个找线工具
	scGuiLine                           m_GuiTrainResultLine[PATTERN_CIRCLE_4LINE_NUM];	// 训练结果线段
	scGuiFindCircle                     m_GuiFindCircle;				// 找圆工具
	scGuiCircle                         m_GuiTrainResultCircle;         // 训练结果圆
	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchRectArray;			// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;			// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点//投影点	
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleArray;		// 圆的搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleCenterArray;	// 圆的搜索结果直线

	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];
	scGuiLine							m_GuiSearchLine[MAXRESNUM*PATTERN_CIRCLE_4LINE_NUM];
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];
	scGuiCross							m_GuiSearchCrossAuxilary1[MAXRESNUM];
	scGuiCross							m_GuiSearchCrossAuxilary2[MAXRESNUM];
	scGuiCircle							m_GuiSearchCircle[MAXRESNUM];
	scGuiCross							m_GuiSearchCircleCenter[MAXRESNUM];

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPoint1Array;	//拟合点
	scGuiCoordCross						m_GuiSearchResultFitPoint1[4096];

	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽
};

