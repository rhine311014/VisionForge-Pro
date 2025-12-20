#pragma once

#include "BaseSearchTool.h"
#include "svPatternSearch.h"
#include "svFind.h"
#include "svGuiFindCircle.h"
#include "svGuiAffineRect.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svGuiFindCircle.h"
#include "svGuiCircle.h"
#include "svGuiFindLine.h"
#include "svGuiLine.h"
#include "svGuiCoordCross.h"
#include "svGuiLineSeg.h"

#pragma pack(push,8)

// PatternChordSearch定位工具训练Data参数类
typedef struct tagPatternChordSearchTrainDataParam : public CBaseTrainDataParam
{
	CoarseScale						m_CoarseScale;			// 最粗定位层次
	Accuracy						m_Accuracy;				// 定位精度
	double							m_dAngleLow;			// 低角度
	double							m_dAngleHigh;			// 高角度
	bool			m_bMaskEnable;						// 是否设置掩模图像

	double			m_dContrastThre;					// 对比度阈值
	long			m_lFilterHalfWidth;					// 滤波器半宽
	int				m_nOutLineNum;						// 局外点
	EdgePolarity	m_edgeProperty;						// 边缘属性
	bool			m_bSortByScore;						// 按分数排序
	bool			m_bCircleEnableDualEdge;			// 启用/禁用找圆工具双边缘

	double			m_dLineContrastThre;					// 对比度阈值
	long			m_lLineFilterHalfWidth;					// 滤波器半宽
	int				m_nLineOutLineNum;						// 局外点
	EdgePolarity	m_edgeLineProperty;						// 边缘属性
	bool			m_bLineSortByScore;						// 按分数排序
	int				m_nLineAngleType;					// 直线角度类型：0,任意角度；1,水平线；2,垂直线；
	bool			m_bLineEnableDualEdge;				// 启用/禁用找线工具双边缘
	bool			m_bEnableFitLineDMS;				// 启用拟合直线系数判断
	double			m_dFitLineDMS;						// 拟合直线残差系数
	tagPatternChordSearchTrainDataParam()
	{
		m_CoarseScale       = CoarseScale2;
		m_Accuracy          = AccuracyFine;
		m_dAngleLow         = -5.0;
		m_dAngleHigh        = 5.0;
		m_bMaskEnable		= false;

		m_dContrastThre		= 10.0;
		m_lFilterHalfWidth	= 1;
		m_nOutLineNum		= 10;
		m_edgeProperty		= eDarkToLight;
		m_bSortByScore		= true;
		m_bCircleEnableDualEdge = false;

		m_dLineContrastThre = 10.0;
		m_lLineFilterHalfWidth = 1;
		m_nLineOutLineNum = 10;
		m_edgeLineProperty = eDarkToLight;
		m_bLineSortByScore = true;
		m_bLineEnableDualEdge = false;

		m_nLineAngleType	= 0;
		m_bEnableFitLineDMS = TRUE;				// 启用拟合直线系数判断
		m_dFitLineDMS       = 0.3;						// 拟合直线残差系数
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternChordSearchTool;
	}
}CPatternChordSearchTrainDataParam;

// PatternChordSearch定位工具训练Gui参数类
typedef struct tagPatternChordSearchTrainGuiParam : public CBaseTrainGuiParam
{
	scRect	        m_TrainRect;					    // 训练区域
	scRect			m_SearchRect;						// 搜索区域
	sc2Vector		m_ModelPoint;						// 参考点

	// 找圆工具参数
	sc2Vector		m_CircleCenter;						// 圆心X
	double			m_dRadius;							// 半径
	scRadian		m_ArcStart;							// 起始弧度
	scRadian        m_ArcSpan;							// 弧度跨度
	int				m_nCaliperNum;						// 卡尺个数
	double			m_dCaliperProjectLen;				// 投影长度
	double			m_dCaliperSearchLen;				// 搜索长度
	CaliperSearchDirection m_CaliperSearchDirection;	// 搜索方向
	bool			m_bCircleCaliperVisible;			//找圆卡尺是否可见
	// 找圆工具掩摸
	svStd vector<double> m_vdCircleCaliperMaskData;				// 掩膜数据
	svStd vector<bool>   m_vbCircleCaliperMask;					// 卡尺是否被掩膜
	bool			     m_bCircleEnableMask;					// 是否启用掩膜

	// 找线工具参数
	int				m_nLineCaliperNum;						// 卡尺个数
	double			m_dLineStartX;							// 起始点X
	double			m_dLineStartY;							// 起始点Y
	double			m_dLineEndX;							// 终止点X
	double			m_dLineEndY;							// 终止点Y
	double			m_dLineCaliperProjectLen;				// 投影长度
	double			m_dLineCaliperSearchLen;				// 搜索长度
	scRadian		m_LineCaliperSearchRotation;			// 搜索角度
	bool			m_bLineCaliperVisible;					//找线卡尺是否可见			
	// 找线工具掩摸
	svStd vector<double> m_vdLineCaliperMaskData;			// 掩膜数据
	svStd vector<bool>   m_vbLineCaliperMask;				// 卡尺是否被掩膜
	bool			     m_bLineEnableMask;					// 是否启用掩膜

	bool m_bCorrectAngle;								// 角度修正

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainRectVisible;			    // 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	bool			m_bCircleVisible;					// 找圆工具是否可见
	bool			m_bLineVisible;						// 找线工具是否可见
	tagPatternChordSearchTrainGuiParam()
	{
		m_CircleCenter				= sc2Vector(200,200);
		m_dRadius					= 100.0;
		m_ArcStart					= scRadian(0);
		m_ArcSpan					= scRadian(sk2PI);
		m_nCaliperNum				= 20;
		m_dCaliperProjectLen		= 10.0;
		m_dCaliperSearchLen			= 50.0;
		m_CaliperSearchDirection	= eInward;
		m_bCircleCaliperVisible		= false;
		m_bCircleEnableMask			= false;
		m_nLineCaliperNum				=	20;
		m_dLineStartX					=   100.0;
		m_dLineStartY					=	100.0;
		m_dLineEndX						=	200.0;
		m_dLineEndY						=	200.0;
		m_dLineCaliperProjectLen		=	20.0;
		m_dLineCaliperSearchLen			=	100;
		m_LineCaliperSearchRotation		=	scRadian(skPI/2.0);
		m_bLineCaliperVisible			=	false;
		m_bLineEnableMask				=	false;

		m_bCorrectAngle					= false;

		m_LineColor					= RGB(0, 0, 255);
		m_nLineWidth				= 1;
		m_bTrainRectVisible	        = true;
		m_bModelPointVisible		= true;
		m_bSearchRectVisible		= true;
		m_bCircleVisible			= true;
		m_bLineVisible				= true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternChordSearchTool;
	}
}CPatternChordSearchTrainGuiParam;

// PatternChordSearch定位工具搜索Data参数类
typedef struct tagPatternChordSearchSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double  m_dConfusionThre;							// 混淆阈值。混淆阈值不小于接收阈值
	bool			m_bEnableLineCircleCross;			// 启用线圆交点		
	int				m_nLineCircleCrossMode;				// 交点偏移模式
	double			m_dOffsetRatio;						// 偏移比例;

	tagPatternChordSearchSearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.6;
		m_dConfusionThre = 0.8;
		m_bEnableLineCircleCross=false;	
		m_nLineCircleCrossMode	= 0;
		m_dOffsetRatio		=1.0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternChordSearchTool;
	}

}CPatternChordSearchSearchDataParam;

// PatternChordSearch定位工具搜索Gui参数类
typedef struct tagPatternChordSearchGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bRectVisible;				        // 搜索结果矩形可见
	bool			m_bModelPointVisible;				// 参考点是否可见
	bool			m_bCircleVisible;					// 圆形是否可见
	bool			m_bCircleCenterVisible;				// 圆心是否可见
	bool			m_bLineVisible;			// 找线工具是否可见

	bool			m_bEnableShowLinePoint;				// 拟合数据点是否可见
	bool			m_bEnableShowCirclePoint;			// 拟合数据点是否可见

	COLORREF        m_cLineColorOKFitPoint1;			// 拟合数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint1;			// 拟合数据点NG颜色

	tagPatternChordSearchGuiParam()
	{
		m_LineColor				= RGB(0, 255, 0);
		m_nLineWidth			= 1;
		m_bRectVisible	        = true;
		m_bModelPointVisible	= true;
		m_bCircleVisible		= true;
		m_bCircleCenterVisible	= false;
		m_bLineVisible			= true;

		m_bEnableShowLinePoint = false;
		m_bEnableShowCirclePoint = false;
		m_cLineColorOKFitPoint1 = RGB(255,255,0);
		m_cLineColorNGFitPoint1 = RGB(255,0,0);
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternChordSearchTool;
	}

}CPatternChordSearchSearchGuiParam;

// QuickSearch训练结果
typedef struct tagPatternChordSearchTrainResult
{
	double m_dModelX;
	double m_dModelY;
	double m_dCenterX;
	double m_dCenterY;
	double m_dRadius;
	sc2Vector m_pointPatternChord;

	double m_dLineStartX; // 线段起点X
	double m_dLineStartY; // 线段起点Y
	double m_dLineEndX;	  // 线段终点X
	double m_dLineEndY;	  // 线段终点Y

	tagPatternChordSearchTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
		m_dCenterX = 0;
		m_dCenterY = 0;
		m_dRadius = 0;
		m_dLineStartX = 0;
		m_dLineStartY = 0;
		m_dLineEndX = 0;
		m_dLineEndY = 0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternChordSearchTool;
	}
}CPatternChordSearchTrainResult;

//
//	
//
class /*SV_SEARCHTOOL_API*/ CPatternChordSearchTool : public CBaseSearchTool    
{
public:
	CPatternChordSearchTool();
	CPatternChordSearchTool(CPatternChordSearchTool &cPatternChordSearchTool);
	CPatternChordSearchTool& operator=(CPatternChordSearchTool &cPatternChordSearchTool);
	virtual ~CPatternChordSearchTool();

public:

	//////////////////////////工具属性相关////////////////////////////////
	virtual CSearchToolType GetSearchToolType();										    // 获取工具类型

	//////////////////////////显示更新相关////////////////////////////////
	virtual bool SetDisplay(scGuiDisplay *pGuiDisplay);										// 设置显示控件指针	
	virtual scGuiDisplay* GetDisplay();														// 获取显示控件指针
	virtual bool SetGuiKeyText(CString strGuiKeyText);										// 设置Gui图形的关键字
	virtual bool GetGuiKeyText(CString &strGuiKeyText);										// 获取Gui图形的关键字
	virtual bool ShowTrainGui(bool bShow) ;													// 是否显示训练界面
	virtual bool ShowTrainResultGui(bool bShow) ;											// 是否显示训练结果界面
	virtual bool ShowSearchResultGui(bool bShow);											// 是否显示搜索界面
	virtual bool UpdateGuiDisplay();														// 更新界面显示
	virtual bool ClearAllGui();															    // 清除定位工具在Display上的控件
	virtual bool ClearResult();																// 清除搜索结果

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
    virtual bool SetGuiByCam(double nWidth,double nHeight);
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
	//////////////////////错误信息///////////////////////////////////////
	virtual CString	GetResultErrInfo();
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

	int GetPointYOnCircle(double dCenterX, double dCenterY, double dRadius, int nXOnCircle); // 根据X坐标获取圆上的Y坐标
	bool GetPatternChordPoint(cpImage &ImageSearch, const scCircle& ResCircle, sc2Vector& outputPoint);
	bool GetPointNearestCircleCenter(sc2Vector& vPoint, scCircle& ResCircle, std::vector<sc2Vector>&  vPoints);

private:	
	scPatternSearchRot                  m_PatternSearchTool;			// PatternSearch搜索工具
	scFindCircle						m_FindCircleTool;				// 找线工具
	scFindLine							m_FindLineTool;					// 找线工具
	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CPatternChordSearchTrainDataParam			m_tmpTrainDataParam;			// 当前训练Data参数
	CPatternChordSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CPatternChordSearchSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CPatternChordSearchSearchGuiParam			m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CPatternChordSearchTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
	CPatternChordSearchTrainDataParam			m_TrainDataParam;				// 训练Data参数
	CPatternChordSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CPatternChordSearchSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CPatternChordSearchSearchGuiParam			m_SearchGuiParam;				// 搜索Gui参数
	CPatternChordSearchTrainResult			m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
	cpImage							m_ImageTrainResult;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;      // 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功


	// 训练GUI
	scGuiRect                           m_GuiTrainRect;					// 训练矩形			
	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint;				// 参考点
	scGuiFindCircle                     m_GuiFindCircle;				// 找圆工具
	scGuiCircle                         m_GuiTrainResultCircle;         // 训练结果圆
	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽

	scGuiFindLine						m_GuiFindLine;					// 找线工具
	scGuiLine							m_GuiLine;						// 训练结果线

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchRectArray;	         // 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleArray;		// 搜索结果圆
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleCenterArray;	// 搜索结果直线
	std::vector<scGuiGraphic*>			m_pGuiSearchLineArray;			// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArrayAuxilary1;	// 搜索结果参考点辅助点1
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArrayAuxilary2;	// 搜索结果参考点辅助点2

	std::vector<scGuiGraphic*>          m_pGuiSearchCircleCenterPointAndLinePointCrossArray;	// 搜索结果直线交点（圆心与直线交点）
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleCenterPointAndLinePointSegArray;	// 搜索结果直线（直线上点与圆心构成的直线）

	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];
	scGuiCircle							m_GuiSearchCircle[MAXRESNUM];
	scGuiCross							m_GuiSearchCircleCenter[MAXRESNUM];
	scGuiLine							m_GuiSearchLine[MAXRESNUM];
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];
	scGuiCross							m_GuiSearchCrossAuxilary1[MAXRESNUM];
	scGuiCross							m_GuiSearchCrossAuxilary2[MAXRESNUM];

	scGuiCross							m_GuiSearchCircleCenterPointAndLinePointCross[MAXRESNUM];
	scGuiLineSeg						m_GuiSearchCircleCenterPointAndLinePointSeg[MAXRESNUM];


	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPoint1Array;
	scGuiCoordCross						m_GuiSearchResultFitPoint1[4096];

	std::vector<scGuiGraphic*>			m_pGuiCircleSearchResultFitPointArray;
	scGuiCoordCross						m_GuiCircleSearchResultFitPoint[4096];
};
#pragma pack(pop)