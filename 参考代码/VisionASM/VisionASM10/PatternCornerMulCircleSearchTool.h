#pragma once

// CircleSearchTool.h: interface for the CPatternCornerMulCircleSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseSearchTool.h"
#include "svPatternSearchRot.h"
#include "svFind.h"
#include "svGuiFindCircle.h"
#include "svGuiAffineRect.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svGuiFindCircle.h"
#include "svGuiCircle.h"
#include "svGuiCoordCross.h"
#include "svGuiLine.h"
#include "svGuiLineSeg.h"

#pragma pack(push,8)

#define PATTERN_CIRCLE_NUM 2
#define LINE_NUM   2

// CircleSearch定位工具训练Data参数类
typedef struct tagPatternCornerMulCircleSearchTrainDataParam : public CBaseTrainDataParam
{
	CoarseScale		m_CoarseScale;			// 最粗定位层次
	Accuracy		m_Accuracy;				// 定位精度
	double			m_dAngleLow;			// 低角度
	double			m_dAngleHigh;			// 高角度
	bool			m_bMaskEnable;						// 是否设置掩模图像
	int				m_nLineAngleType;					// 直线角度类型：0,任意角度；1,水平线；2,垂直线；

	double			m_dCircleContrastThre[PATTERN_CIRCLE_NUM];					// 对比度阈值
	long			m_lCircleFilterHalfWidth[PATTERN_CIRCLE_NUM];					// 滤波器半宽
	int				m_nCircleOutLineNum[PATTERN_CIRCLE_NUM];						// 局外点
	EdgePolarity	m_edgeCircleProperty[PATTERN_CIRCLE_NUM];						// 边缘属性
	bool			m_bCircleSortByScore[PATTERN_CIRCLE_NUM];						// 按分数排序
	bool			m_bEnableCircleDualEdge[PATTERN_CIRCLE_NUM];					// 启用/禁用找圆工具双边缘

	double			m_dLineContrastThre[LINE_NUM] ;			// 对比度阈值
	long			m_lLineFilterHalfWidth[LINE_NUM];		// 滤波器半宽
	int				m_nLineOutLineNum[LINE_NUM];			// 局外点
	EdgePolarity	m_edgeLineProperty[LINE_NUM];			// 边缘属性
	bool			m_bLineSortByScore[LINE_NUM];			// 按分数排序
	bool			m_bEnableLineDualEdge[LINE_NUM];		// 启用/禁用找线工具双边缘

	tagPatternCornerMulCircleSearchTrainDataParam()
	{
		m_CoarseScale       = CoarseScale8;
		m_Accuracy			= AccuracyFine;
		m_dAngleLow			= -5;
		m_dAngleHigh        = 5;
		m_bMaskEnable		= false;
		m_nLineAngleType	= 0;

		for (int i = 0; i < PATTERN_CIRCLE_NUM; i++)
		{
			m_dCircleContrastThre[i]	= 10.0;
			m_lCircleFilterHalfWidth[i]	= 1;
			m_nCircleOutLineNum[i]		= 10;
			m_edgeCircleProperty[i]		= eDarkToLight;
			m_bCircleSortByScore[i]		= true;
			m_bEnableCircleDualEdge[i]	= false;
		}

		for (int i = 0; i < LINE_NUM; i++)
		{
			m_dLineContrastThre[i]		= 10.0;
			m_lLineFilterHalfWidth[i]	= 1;
			m_nLineOutLineNum[i]		= 10;
			m_edgeLineProperty[i]		= eDontCare;
			m_bLineSortByScore[i]		= true;
			m_bEnableLineDualEdge[i]	= false;
		}

	}

	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternCornerMulCircleSearchTool;
	}

}CPatternCornerMulCircleSearchTrainDataParam;


// CircleSearch定位工具训练Gui参数类
typedef struct tagPatternCornerMulCircleSearchTrainGuiParam : public CBaseTrainGuiParam
{
	scRect      	m_TrainRect;					// 训练区域
	scRect			m_SearchRect;						// 搜索区域
	sc2Vector		m_ModelPoint;						// 参考点

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainAffineRectVisible;			// 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见

	sc2Vector		m_CircleCenter_1[PATTERN_CIRCLE_NUM];						// 圆心X
	double			m_dCircleRadius[PATTERN_CIRCLE_NUM];							// 半径
	scRadian		m_CircleArcStart[PATTERN_CIRCLE_NUM];							// 起始弧度
	scRadian        m_CircleArcSpan[PATTERN_CIRCLE_NUM];							// 弧度跨度
	int				m_nCircleCaliperNum[PATTERN_CIRCLE_NUM];						// 卡尺个数
	double			m_dCircleCaliperProjectLen[PATTERN_CIRCLE_NUM];				// 投影长度
	double			m_dCircleCaliperSearchLen[PATTERN_CIRCLE_NUM];				// 搜索长度
	CaliperSearchDirection m_CircleCaliperSearchDirection[PATTERN_CIRCLE_NUM];	// 搜索方向
	bool			m_bCircleVisible_1[PATTERN_CIRCLE_NUM];					// 找圆工具是否可见		
	bool			m_bCircleCaliperVisible[PATTERN_CIRCLE_NUM];				//找圆卡尺是否可见
	// 找圆工具掩摸
	svStd vector<double> m_vdCircleCaliperMaskData[PATTERN_CIRCLE_NUM];				// 掩膜数据
	svStd vector<bool>   m_vbCircleCaliperMask[PATTERN_CIRCLE_NUM];					// 卡尺是否被掩膜
	bool			     m_bEnableCircleMask[PATTERN_CIRCLE_NUM];					// 是否启用掩膜


	int				m_nLineCaliperNum[LINE_NUM];			// 卡尺个数
	double			m_dLineStartX[LINE_NUM];				// 起始点X
	double			m_dLineStartY[LINE_NUM];				// 起始点Y
	double			m_dLineEndX[LINE_NUM];					// 终止点X
	double			m_dLineEndY[LINE_NUM];					// 终止点Y
	double			m_dLineCaliperProjectLen[LINE_NUM];		// 投影长度
	double			m_dLineCaliperSearchLen[LINE_NUM];		// 搜索长度
	scRadian		m_LineCaliperSearchRotation[LINE_NUM];	// 搜索角度
	
	bool			m_bLineVisible[LINE_NUM];				// 找线是否可见
	bool			m_bLineCaliperVisible[LINE_NUM];		//找线卡尺是否可见	

	// 找线工具掩摸
	svStd vector<double> m_vdLineCaliperMaskData[LINE_NUM];			// 掩膜数据
	svStd vector<bool>   m_vbLineCaliperMask[LINE_NUM];				// 卡尺是否被掩膜
	bool			     m_bEnableLineMask[LINE_NUM];			// 是否启用掩膜

	tagPatternCornerMulCircleSearchTrainGuiParam()
	{
		m_LineColor					= RGB(0, 0, 255);
		m_nLineWidth				= 1;
		m_bTrainAffineRectVisible	= true;
		m_bModelPointVisible		= true;
		m_bSearchRectVisible		= true;

		for (int i = 0; i < PATTERN_CIRCLE_NUM; i++)
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
			m_bCircleCaliperVisible[i]		= false;
			m_bEnableCircleMask[i]			= false;
		}

		for(int i =0; i < LINE_NUM; i++)
		{
			m_nLineCaliperNum[i]				= 20;
			m_dLineStartX[i]					= 100.0;
			m_dLineStartY[i]					= 100.0 * (i+1);
			m_dLineEndX[i]						= 100.0;
			m_dLineEndY[i]						= 100.0 * (i+1);
			m_dLineCaliperProjectLen[i]			= 20.0;
			m_dLineCaliperSearchLen[i]			= 100.0;
			m_LineCaliperSearchRotation[i]		= scRadian(skPI/2.0);
			m_bLineVisible[i]					= true;
			m_bLineCaliperVisible[i]			= false;
			m_bEnableLineMask[i]				= false;
		}
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternCornerMulCircleSearchTool;
	}

}CPatternCornerMulCircleSearchTrainGuiParam;

// CircleSearch定位工具搜索Data参数类
typedef struct tagPatternCornerMulCircleSearchSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double  m_dConfusionThre;                           // 混淆阈值
	//double  m_dAngleLow;								// 低角度
	//double	m_dAngleHigh;								// 高角度

	tagPatternCornerMulCircleSearchSearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.6;
		m_dConfusionThre = 0.8;
		//m_dAngleLow		= -5.0;
		//m_dAngleHigh	= 5.0;
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternCornerMulCircleSearchTool;
	}

}CPatternCornerMulCircleSearchSearchDataParam;

// CircleSearch定位工具搜索Gui参数类
typedef struct tagPatternCornerMulCircleSearchGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bAffineRectVisible;				// 搜索结果矩形可见
	bool			m_bModelPointVisible;				// 参考点是否可见

	bool			m_bCircleVisible_1[PATTERN_CIRCLE_NUM];					// 圆形是否可见
	bool			m_bCircleCenterVisible_1[PATTERN_CIRCLE_NUM];				// 圆心是否可见
	bool			m_bEnableShowCirclePoint[PATTERN_CIRCLE_NUM];				// 拟合数据点是否可见


	bool			m_bLineCrossVisible;							// 找线交点是否可见
	bool			m_bLineVisible[LINE_NUM];						// 直线否可见
	bool            m_bEnableShowLinePoint[LINE_NUM];               // 拟合数据点是否可见



	COLORREF        m_cLineColorOKFitPoint1;			// 拟合数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint1;			// 拟合数据点NG颜色

	tagPatternCornerMulCircleSearchGuiParam()
	{
		m_LineColor				= RGB(0, 255, 0);
		m_nLineWidth			= 1;
		m_bAffineRectVisible	= true;
		m_bModelPointVisible	= true;

		m_cLineColorOKFitPoint1 = RGB(255,255,0);
		m_cLineColorNGFitPoint1 = RGB(255,0,0);

		for (int i = 0; i < PATTERN_CIRCLE_NUM; i++)
		{
			m_bCircleVisible_1[i] = true;
			m_bCircleCenterVisible_1[i] = true;
			m_bEnableShowCirclePoint[i] = false;

		}
		m_bLineCrossVisible = true;
		for(int i = 0 ;i < LINE_NUM; i++)
		{
			m_bLineVisible[i] = true;
			m_bEnableShowLinePoint[i] = false;
		}

	}

	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternCornerMulCircleSearchTool;
	}

}CPatternCornerMulCircleSearchSearchGuiParam;

// PatternSearch训练结果
typedef struct tagPatternCornerMulCircleSearchTrainResult
{
	double m_dModelX;
	double m_dModelY;

	double m_dCircleCenterX[PATTERN_CIRCLE_NUM];
	double m_dCircleCenterY[PATTERN_CIRCLE_NUM];
	double m_dCircleRadius[PATTERN_CIRCLE_NUM];

	double m_dStartX[LINE_NUM]; // 线段起点X
	double m_dStartY[LINE_NUM]; // 线段起点Y
	double m_dEndX[LINE_NUM];	  // 线段终点X
	double m_dEndY[LINE_NUM];	  // 线段终点Y


	tagPatternCornerMulCircleSearchTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;

		for (int i = 0; i < PATTERN_CIRCLE_NUM; i++)
		{
			m_dCircleCenterX[i] = 0;
			m_dCircleCenterY[i] = 0;
			m_dCircleRadius[i] = 0;
		}

		for (int i = 0; i < LINE_NUM; i++)
		{
			m_dStartY[i] = 0;
			m_dStartY[i] = 0;
			m_dEndX[i] = 0;
			m_dEndY[i] = 0;
		}
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternCornerMulCircleSearchTool;
	}

}CPatternCornerMulCircleSearchTrainResult;

//
//	圆定位工具
//
class /*SV_SEARCHTOOL_API*/ CPatternCornerMulCircleSearchTool : public CBaseSearchTool    
{
public:
	CPatternCornerMulCircleSearchTool();
	CPatternCornerMulCircleSearchTool(CPatternCornerMulCircleSearchTool &cCircleSearchTool);
	CPatternCornerMulCircleSearchTool& operator=(CPatternCornerMulCircleSearchTool &cCircleSearchTool);
	virtual ~CPatternCornerMulCircleSearchTool();

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

    virtual bool SetGuiByCam(double nWidth,double nHeight);
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
	virtual CString GetResultErrInfo();
	/////////////////////////参数加载与保存////////////////////////////////	
	virtual bool LoadModelFromFile(CString strDir);											// 加载模板文件, strDir为文件夹路径
	virtual bool SaveModelToFile(CString strDir);											// 保存模板文件, strDir为文件夹路径
	virtual bool LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern);			// 加载模板训区域图片， strDir为文件夹路径,ImagePattern为获取得到的训练区域图片
	//////////////////////语言类型//////////////////////////////////////
	virtual void	SetLanguage(int nLanguage);									// 设置中英文菜单 0：eng；1：chs
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

	bool CalcLineCross(scLineSeg cLineSeg1, scLineSeg cLineSeg2, sc2Vector& centerLine1, sc2Vector& centerLine2, std::vector<sc2Vector>& vtCrossArray);

private:	
	scPatternSearchRot                  m_PatternSearchTool;				// PatternSearch搜索工具
	scFindCircle						m_FindCircleTool_1[PATTERN_CIRCLE_NUM];				// 找圆工具
	scFindLine							m_FindLineTool[LINE_NUM];                    // 找线工具

	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀

	CPatternCornerMulCircleSearchTrainDataParam			m_tmpTrainDataParam;			// 当前训练Data参数
	CPatternCornerMulCircleSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CPatternCornerMulCircleSearchSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CPatternCornerMulCircleSearchSearchGuiParam			m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CPatternCornerMulCircleSearchTrainResult			m_tmpTrainResult;				// 当前训练结果
	CPatternCornerMulCircleSearchTrainDataParam			m_TrainDataParam;				// 训练Data参数
	CPatternCornerMulCircleSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CPatternCornerMulCircleSearchSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CPatternCornerMulCircleSearchSearchGuiParam			m_SearchGuiParam;				// 搜索Gui参数
	CPatternCornerMulCircleSearchTrainResult			m_TrainResult;					// 训练结果

	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
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
	scGuiFindCircle                     m_GuiFindCircle_1[PATTERN_CIRCLE_NUM];				// 找圆工具
	scGuiCircle                         m_GuiTrainResultCircle_1[PATTERN_CIRCLE_NUM];         // 训练结果圆
	scGuiFindLine						m_GuiFindLine[LINE_NUM];		// 找线GUI
	scGuiLine							m_GuiTrainResultLine[LINE_NUM]; // 训练结果线

	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchAffineRectArray;	// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleArray;		// 搜索结果圆
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleCenterArray;	// 搜索结果圆心

	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;			// 搜索圆心连线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点

	std::vector<scGuiGraphic*>          m_pGuiSearchFindLineArray;          // 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCornerCrossArray;   // 搜索结果角点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCenterCrossArray;       // 搜索结果直线中心点

	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];
	scGuiCircle							m_GuiSearchCircle[MAXRESNUM];
	scGuiCross							m_GuiSearchCircleCenter[MAXRESNUM];

	scGuiLineSeg						m_GuiSearchLineSeg[MAXRESNUM];
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];

	scGuiLine                           m_GuiSearchFindLine[MAXRESNUM];
	scGuiCross                          m_GuiSearchLineCornerCross[MAXRESNUM];
	scGuiCross						    m_GuiSearchLineCenterCross[MAXRESNUM];



	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽

	std::vector<scGuiGraphic*>          m_pGuiSearchResultCircleFitPointArray1;
	scGuiCoordCross						m_GuiSearchResultCircleFitPoint1[4096];

	std::vector<scGuiGraphic*>          m_pGuiSearchResultCircleFitPointArray2;
	scGuiCoordCross						m_GuiSearchResultCircleFitPoint2[4096];

	std::vector<scGuiGraphic*>			m_pGuiSearchResultLineFitPointArray1;
	scGuiCoordCross						m_GuiSearchResultLineFitPoint1[4096];

	std::vector<scGuiGraphic*>			m_pGuiSearchResultLineFitPointArray2;
	scGuiCoordCross						m_GuiSearchResultLineFitPoint2[4096];
};