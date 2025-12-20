// QuickSearchCheckLineTool.h: interface for the CQuickSearchCheckLineTool class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseSearchTool.h"

#include "svGuiAffineRect.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svQuickSearch.h"
#include "svFind.h"
#include "svGuiFindLine.h"
#include "svGuiLine.h"
#include "svGuiCoordCross.h"
#include "svGuiFeaturelet.h"
// QuickSearch定位工具训练Data参数类
typedef struct tagQuickSearchCheckLineTrainDataParam : public CBaseTrainDataParam
{
	double	m_dGrau;									// 颗粒度	
	long	m_lNoiseThre;								// 噪声阈值
	long	m_lTotalMag;								// 幅值和	
	bool	m_bMaskEnable;								// 是否设置掩模图像

	double			m_dContrastThre;					// 对比度阈值
	long			m_lFilterHalfWidth;					// 滤波器半宽
	int				m_nOutLineNum;						// 局外点
	EdgePolarity	m_edgeProperty;						// 边缘属性
	bool			m_bSortByScore;						// 按分数排序
	int				m_nLineAngleType;					// 直线角度类型：0,任意角度；1,水平线；2,垂直线；
	BOOL			m_bEnableDualEdge;					// 启用/禁用找线工具双边缘

	double			m_dContrastThre1;					// 对比度阈值
	long			m_lFilterHalfWidth1;					// 滤波器半宽
	int				m_nOutLineNum1;						// 局外点
	EdgePolarity	m_edgeProperty1;						// 边缘属性
	bool			m_bSortByScore1;			// 按分数排序
	int				m_nLineAngleType1;					// 直线角度类型：0,任意角度；1,水平线；2,垂直线；
	BOOL			m_bEnableDualEdge1;					// 启用/禁用找线工具双边缘

	double	m_dGrau1;									// 颗粒度
	long	m_lNoiseThre1;								// 噪声阈值
	long	m_lTotalMag1;								// 幅值和
	bool	m_bEnableCheckRegion1;						// 是否启用检查区域1
	bool	m_bEnableCheckRegion2;						// 是否启用检查区域2

	tagQuickSearchCheckLineTrainDataParam()
	{
		m_dGrau			= 2.0;		
		m_lNoiseThre	= 10;		
		m_lTotalMag		= 200;		
		m_bMaskEnable	= false;

		m_dContrastThre		= 10.0;
		m_lFilterHalfWidth	= 1;
		m_nOutLineNum		= 0;
		m_edgeProperty		= eDontCare;
		m_bSortByScore		= true;
		m_nLineAngleType	= 0;
		m_bEnableDualEdge	= FALSE;

		m_dContrastThre1		= 10.0;
		m_lFilterHalfWidth1	= 1;
		m_nOutLineNum1		= 0;
		m_edgeProperty1		= eDontCare;
		m_bSortByScore1		= true;
		m_nLineAngleType1	= 0;
		m_bEnableDualEdge1	= FALSE;

		m_dGrau1		= 2.0;
		m_lNoiseThre1	= 10;
		m_lTotalMag1	= 200;
		m_bEnableCheckRegion1  =   true;
		m_bEnableCheckRegion2  =   false;

	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eQuickSearchCheckLineTool;
	}

}CQuickSearchCheckLineTrainDataParam;

// QuickSearch定位工具训练Gui参数类
typedef struct tagQuickSearchCheckLineTrainGuiParam : public CBaseTrainGuiParam
{
	scAffineRect	m_TrainAffineRect;					// 训练区域
	scAffineRect	m_TrainAffineRect1;					// 训练区域
	scAffineRect	m_TrainAffineRect2;					// 训练区域

	sc2Vector		m_ModelPoint;						// 参考点
	sc2Vector		m_ModelPoint1;						// 参考点
	sc2Vector		m_ModelPoint2;						// 参考点

	scRect			m_SearchRect;						// 搜索区域


	// 找线工具0
	int				m_nCaliperNum;						// 卡尺个数
	double			m_dStartX;							// 起始点X
	double			m_dStartY;							// 起始点Y
	double			m_dEndX;							// 终止点X
	double			m_dEndY;							// 终止点Y
	double			m_dCaliperProjectLen;				// 投影长度
	double			m_dCaliperSearchLen;				// 搜索长度
	scRadian		m_CaliperSearchRotation;			// 搜索角度
	bool			m_bCaliperVisible;					//卡尺是否可见
	// 找线工具掩摸
	svStd vector<double> m_vdMaskData_FindLine;	// 掩膜数据
	svStd vector<bool>   m_vbCaliperMask_FindLine;	// 卡尺是否被掩膜
	bool			     m_bEnableLineMask;			// 是否启用掩膜


	// 找线工具1
	int				m_nCaliperNum1;						// 卡尺个数
	double			m_dStartX1;							// 起始点X
	double			m_dStartY1;							// 起始点Y
	double			m_dEndX1;							// 终止点X
	double			m_dEndY1;							// 终止点Y
	double			m_dCaliperProjectLen1;				// 投影长度
	double			m_dCaliperSearchLen1;				// 搜索长度
	scRadian		m_CaliperSearchRotation1;			// 搜索角度
	bool			m_bCaliperVisible1;					//卡尺是否可见
	// 找线工具掩摸
	svStd vector<double> m_vdMaskData_FindLine1;	// 掩膜数据
	svStd vector<bool>   m_vbCaliperMask_FindLine1;	// 卡尺是否被掩膜
	bool			     m_bEnableLineMask1;			// 是否启用掩膜

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainAffineRectVisible;			// 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	bool			m_bLineVisible;						// 找线工具是否可见

	tagQuickSearchCheckLineTrainGuiParam()
	{
		m_LineColor					= RGB(0, 0, 255);
		m_nLineWidth				= 1;
		m_bTrainAffineRectVisible	= true;
		m_bModelPointVisible		= true;
		m_bSearchRectVisible		= true;
		m_bLineVisible				= true;
		

		m_nCaliperNum				=	3;
		m_dStartX					=   100.0;
		m_dStartY					=	100.0;
		m_dEndX						=	200.0;
		m_dEndY						=	200.0;
		m_dCaliperProjectLen		=	20.0;
		m_dCaliperSearchLen			=	100;
		m_CaliperSearchRotation		=	scRadian(skPI/2.0);
		m_bEnableLineMask			    = false;
		m_bCaliperVisible			=	false;

		m_nCaliperNum1				=	3;
		m_dStartX1					=   100.0;
		m_dStartY1					=	100.0;
		m_dEndX1					=	200.0;
		m_dEndY1					=	200.0;
		m_dCaliperProjectLen1		=	20.0;
		m_dCaliperSearchLen1		=	100;
		m_CaliperSearchRotation1	=	scRadian(skPI/2.0);
		m_bCaliperVisible1			=	false;
		m_bEnableLineMask1			=	false;
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eQuickSearchCheckLineTool;
	}

}CQuickSearchCheckLineTrainGuiParam;

// QuickSearch定位工具搜索Data参数类
typedef struct tagQuickSearchCheckLineSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double	m_dAcceptThre2;								// 接收阈值
	double  m_dAngleLow;								// 低角度
	double	m_dAngleHigh;								// 高角度
	double  m_dSizeLow;
	double  m_dSizeHigh;

	tagQuickSearchCheckLineSearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.8;
		m_dAcceptThre2	= 0.8;
		m_dAngleLow		= -5.0;
		m_dAngleHigh	= 5.0;
		m_dSizeLow = 1.0;
		m_dSizeHigh = 1.0;
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eQuickSearchCheckLineTool;
	}

}CQuickSearchCheckLineSearchDataParam;

// QuickSearch定位工具搜索Gui参数类
typedef struct tagQuickSearchCheckLineSearchGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bAffineRectVisible;				// 搜索结果矩形可见
	bool			m_bModelPointVisible;				// 参考点是够可见

	bool			m_bLineVisible;						// 找线工具是否可见
	bool			m_bEnableShowLinePoint;				// 拟合数据点是否可见
	COLORREF        m_cLineColorOKFitPoint;			// 拟合数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint;			// 拟合数据点NG颜色

//	bool			m_bLineVisible1;						// 找线工具是否可见
	bool			m_bEnableShowLinePoint1;				// 拟合数据点是否可见

	tagQuickSearchCheckLineSearchGuiParam()
	{
		m_LineColor			 = RGB(0, 255, 0);
		m_nLineWidth		 = 1;
		m_bAffineRectVisible = true;
		m_bModelPointVisible = true;

		m_bLineVisible			= true;
		m_bEnableShowLinePoint = false;
		m_cLineColorOKFitPoint = RGB(255,255,0);
		m_cLineColorNGFitPoint = RGB(255,0,0);

//		m_bLineVisible1			= true;
		m_bEnableShowLinePoint1 = false;
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eQuickSearchCheckLineTool;
	}

}CQuickSearchCheckLineSearchGuiParam;

// QuickSearch训练结果
typedef struct tagQuickSearchCheckLineTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y

	double m_dStartX; // 线段起点X
	double m_dStartY; // 线段起点Y
	double m_dEndX;	  // 线段终点X
	double m_dEndY;	  // 线段终点Y

	double m_dStartX1; // 线段起点X
	double m_dStartY1; // 线段起点Y
	double m_dEndX1;	  // 线段终点X
	double m_dEndY1;	  // 线段终点Y

	tagQuickSearchCheckLineTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;

		m_dStartX = 0;
		m_dStartY = 0;
		m_dEndX   = 0;
		m_dEndY   = 0;

		m_dStartX1 = 0;
		m_dStartY1 = 0;
		m_dEndX1   = 0;
		m_dEndY1   = 0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eQuickSearchCheckLineTool;
	}

}CQuickSearchCheckLineTrainResult;

class CQuickSearchCheckLineTool : public CBaseSearchTool
{
public:
	CQuickSearchCheckLineTool();
	CQuickSearchCheckLineTool(CQuickSearchCheckLineTool &cQuickSearchTool);
	CQuickSearchCheckLineTool& operator=(CQuickSearchCheckLineTool &cQuickSearchTool);
	virtual ~CQuickSearchCheckLineTool(void);


public:

	//////////////////////////工具属性相关////////////////////////////////
	virtual CSearchToolType GetSearchToolType();										    // 获取工具类型

	//////////////////////////显示更新相关////////////////////////////////
	virtual bool SetDisplay(scGuiDisplay *pGuiDisplay);										// 设置显示控件指针				
	virtual scGuiDisplay* GetDisplay();														// 获取显示空间指针
	virtual bool SetTrainResultDisplay(scGuiDisplay *pGuiDisplay);							// 设置训练结果显示控件指针
	virtual scGuiDisplay* GetTrainResultDisplay();											// 获取训练结果显示控件指针
	virtual bool SetTrainResultDisplay2(scGuiDisplay *pGuiDisplay);							// 设置训练结果显示控件指针
	virtual scGuiDisplay* GetTrainResultDisplay2();											// 获取训练结果显示控件指针
	virtual bool SetTrainResultDisplay3(scGuiDisplay *pGuiDisplay);							// 设置训练结果显示控件指针
	virtual scGuiDisplay* GetTrainResultDisplay3();											// 获取训练结果显示控件指针
	virtual bool SetGuiKeyText(CString strGuiKeyText);										// 设置Gui图形的关键字
	virtual bool GetGuiKeyText(CString &strGuiKeyText);										// 获取Gui图形的关键字
	virtual bool ShowTrainGui(bool bShow) ;													// 是否显示训练界面
	virtual bool ShowTrainResultGui(bool bShow) ;											// 是否显示训练结果界面
	virtual bool ShowSearchResultGui(bool bShow);											// 是否显示搜索界面
	virtual bool UpdateGuiDisplay();														// 更新界面显示
	virtual bool ClearAllGui();															    // 清除定位工具在Display上的控件
	virtual bool ClearResult();																// 清除搜索结果
	virtual bool SetGuiByCam(double m_Width,double m_Height);																// 设置根据相机设置GUI大小

	///////////////////////////训练相关/////////////////////////////////////
	virtual bool SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam);					// 设置训练Data参数				
	virtual bool GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam);					// 获取训练Data参数
	virtual bool SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam);						// 获取搜索Gui参数
	virtual bool GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam);						// 获取搜索Gui参数
	virtual bool SetTrainImage(const cpImage &ImageTrain);									// 设置训练原始图像
	virtual bool GetTrainImage(cpImage &ImageTrain);										// 获取训练原始图像
	virtual bool SetMaskImage(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool GetMaskImage(cpImage &ImageMask);											// 获取训练原始图像
	virtual bool SetMaskImage2(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool GetMaskImage2(cpImage &ImageMask);										// 获取训练原始图像

	virtual bool Train();																	// 执行训练
	virtual bool IsTrained();																// 是否训练成功
	virtual bool GetTrainResultImage(cpImage &ImageTrainResult);							// 获取训练结果图像
	virtual bool GetTrainResultImage2(cpImage &ImageTrainResult);							// 获取训练结果图像
	virtual bool GetTrainResultImage3(cpImage &ImageTrainResult);							// 获取训练结果图像
	virtual bool GetTrainResult(CBaseTrainResult *pTrainResult);							// 获取训练结果
	virtual bool GetPatternImage(cpImage &ImagePattern);									// 获取训练模板原始图像
	virtual bool GetPatternImage2(cpImage &ImagePattern);									// 获取训练模板原始图像
	virtual bool GetPatternImage3(cpImage &ImagePattern);									// 获取训练模板原始图像
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
	virtual bool LoadModelTrainPatternImage(CString strDir , cpImage &ImagePattern);		// 加载模板训区域图片， strDir为文件夹路径,ImagePattern为获取得到的训练区域图片

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
	scQuickSearchTool                   m_QuickSearchTool;				// QuickSearch搜索工具
	scQuickSearchTool                   m_QuickSearchTool1;				// QuickSearch搜索工具
	scQuickSearchTool                   m_QuickSearchTool2;				// QuickSearch搜索工具

	scFindLine							m_FindLineTool;				// 找线工具
	scFindLine							m_FindLineTool1;				// 找线工具

	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd;			// 显示训练结果对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay2;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd2;			// 显示训练结果对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay1;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd1;			// 显示训练结果对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀

	CQuickSearchCheckLineTrainDataParam			m_tmpTrainDataParam;			// 当前训练Data参数
	CQuickSearchCheckLineTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CQuickSearchCheckLineSearchDataParam			m_tmpSearchDataParam;			// 当前搜索Data参数	
	CQuickSearchCheckLineSearchGuiParam			m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CQuickSearchCheckLineTrainResult				m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像

	CQuickSearchCheckLineTrainDataParam			m_TrainDataParam;				// 训练Data参数
	CQuickSearchCheckLineTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CQuickSearchCheckLineSearchDataParam			m_SearchDataParam;				// 搜索Data参数	
	CQuickSearchCheckLineSearchGuiParam			m_SearchGuiParam;				// 搜索Gui参数
	CQuickSearchCheckLineTrainResult				m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
	
	cpImage							m_ImageTrainResult;				// 训练结果图像
	cpImage							m_ImageTrainResult1;			// 训练结果图像
	cpImage							m_ImageTrainResult2;			// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存

	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;      // 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功

	// 训练GUI
	scGuiAffineRect                     m_GuiTrainRect;					// 训练矩形		
	scGuiAffineRect                     m_GuiTrainRect1;				// 训练矩形		
	scGuiAffineRect						m_GuiTrainRect2;				// 训练矩形		
	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint;				// 参考点

	scGuiFindLine                       m_GuiFindLine;				// 两个找线工具
	scGuiFindLine                       m_GuiFindLine1;				// 两个找线工具
	scGuiLine                           m_GuiTrainResultLine;       // 训练结果线段
	scGuiLine                           m_GuiTrainResultLine1;       // 训练结果线段
	COLORREF                            m_TrainGUIColor;            // 训练图形颜色
	int                                 m_nTrainGuiLineWidth;		// 训练图形线宽
	scGuiFeaturelet						m_GuiTrainFeaturelet;			// 训练特征链集
	scGuiFeaturelet						m_GuiTrainFeaturelet1;			// 训练特征链集
	scGuiFeaturelet						m_GuiTrainFeaturelet2;			// 训练特征链集

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchAffineRectArray;	// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点

	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;		// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArrayAuxilary1;	// 搜索结果参考点辅助点1
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArrayAuxilary2;	// 搜索结果参考点辅助点2

	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray1;		// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray1;		// 搜索结果直线交点
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArrayAuxilary1_1;	// 搜索结果参考点辅助点1
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArrayAuxilary2_1;	// 搜索结果参考点辅助点2


	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];

	scGuiRect							m_GuiSearchRect1[MAXRESNUM];
	scGuiAffineRect						m_GuiSearchAffineRect1[MAXRESNUM];
	
	scGuiRect							m_GuiSearchRect2[MAXRESNUM];
	scGuiAffineRect						m_GuiSearchAffineRect2[MAXRESNUM];

	scGuiLine							m_GuiSearchLine[MAXRESNUM];
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];
	scGuiCross							m_GuiSearchCrossAuxilary1[MAXRESNUM];
	scGuiCross							m_GuiSearchCrossAuxilary2[MAXRESNUM];

	scGuiLine							m_GuiSearchLine1[MAXRESNUM];
	scGuiCross							m_GuiSearchLineCross1[MAXRESNUM];
	scGuiCross							m_GuiSearchCrossAuxilary1_1[MAXRESNUM];
	scGuiCross							m_GuiSearchCrossAuxilary2_1[MAXRESNUM];

	COLORREF                            m_SearchResultColor;		// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;// 搜索结果线宽

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPoint1Array;
	scGuiCoordCross						m_GuiSearchResultFitPoint1[4096];

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPoint1Array_1;
	scGuiCoordCross						m_GuiSearchResultFitPoint1_1[4096];
};

