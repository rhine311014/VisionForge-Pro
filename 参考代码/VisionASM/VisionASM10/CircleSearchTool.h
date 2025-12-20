// CircleSearchTool.h: interface for the CCircleSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIRCLESEARCHTOOL_H__178C6CE1_A745_4F8E_BAD9_E7F1C5D03EC9__INCLUDED_)
#define AFX_CIRCLESEARCHTOOL_H__178C6CE1_A745_4F8E_BAD9_E7F1C5D03EC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseSearchTool.h"
#include "svQuickSearch.h"
#include "svFind.h"
#include "svGuiFindCircle.h"
#include "svGuiAffineRect.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svGuiFindCircle.h"
#include "svGuiCircle.h"
#include "svGuiCoordCross.h"
#include "svGuiFeaturelet.h"
#include "svGuiLineSeg.h"

#pragma pack(push,8)

// CircleSearch定位工具训练Data参数类
typedef struct tagCircleSearchTrainDataParam : public CBaseTrainDataParam
{
	double			m_dGrau;							// 颗粒度
	long			m_lNoiseThre;						// 噪声阈值
	long			m_lTotalMag;						// 幅值和
	bool			m_bMaskEnable;						// 是否设置掩模图像

	double			m_dContrastThre;					// 对比度阈值
	long			m_lFilterHalfWidth;					// 滤波器半宽
	int				m_nOutLineNum;						// 局外点
	EdgePolarity	m_edgeProperty;						// 边缘属性
	bool			m_bSortByScore;						// 按分数排序
	bool			m_bEnableDualEdge;					// 启用/禁用找圆工具双边缘
	int				m_nAuxiliaryPosType;
	
	
	tagCircleSearchTrainDataParam()
	{
		m_dGrau				= 2.0;
		m_lNoiseThre		= 10;
		m_lTotalMag			= 200;
		m_bMaskEnable		= false;

		m_dContrastThre		= 10.0;
		m_lFilterHalfWidth	= 1;
		m_nOutLineNum		= 10;
		m_edgeProperty		= eDarkToLight;
		m_bSortByScore		= true;
		m_bEnableDualEdge	= false;
		m_nAuxiliaryPosType = 0;
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eCircleSearchTool;
	}
}CCircleSearchTrainDataParam;

// CircleSearch定位工具训练Gui参数类
typedef struct tagCircleSearchTrainGuiParam : public CBaseTrainGuiParam
{
	scAffineRect	m_TrainAffineRect;					// 训练区域
	scRect			m_SearchRect;						// 搜索区域
	sc2Vector		m_ModelPoint;						// 参考点

	sc2Vector		m_CircleCenter;						// 圆心X
	double			m_dRadius;							// 半径
	scRadian		m_ArcStart;							// 起始弧度
	scRadian        m_ArcSpan;							// 弧度跨度
	int				m_nCaliperNum;						// 卡尺个数
	double			m_dCaliperProjectLen;				// 投影长度
	double			m_dCaliperSearchLen;				// 搜索长度
	CaliperSearchDirection m_CaliperSearchDirection;	// 搜索方向

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainAffineRectVisible;			// 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	bool			m_bCircleVisible;					// 找圆工具是否可见
	bool			m_bCircleCaliperVisible;			//找圆卡尺是否可见

	// 找圆工具掩摸
	svStd vector<double> m_vdCaliperMaskData;			// 掩膜数据
	svStd vector<bool>   m_vbCaliperMask;				// 卡尺是否被掩膜
	bool			     m_bEnableCircleMask;			// 是否启用掩膜
	tagCircleSearchTrainGuiParam()
	{
		m_CircleCenter				= sc2Vector(200,200);
		m_dRadius					= 100.0;
		m_ArcStart					= scRadian(0);
		m_ArcSpan					= scRadian(sk2PI);
		m_nCaliperNum				= 20;
		m_dCaliperProjectLen		= 10.0;
		m_dCaliperSearchLen			= 50.0;
		m_CaliperSearchDirection	= eInward;
		m_LineColor					= RGB(0, 0, 255);
		m_nLineWidth				= 1;
		m_bTrainAffineRectVisible	= true;
		m_bModelPointVisible		= true;
		m_bSearchRectVisible		= true;
		m_bCircleVisible			= true;
		m_bCircleCaliperVisible		= false;
		m_bEnableCircleMask			= false;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCircleSearchTool;
	}
}CCircleSearchTrainGuiParam;

// CircleSearch定位工具搜索Data参数类
typedef struct tagCircleSearchSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double  m_dAngleLow;								// 低角度
	double	m_dAngleHigh;								// 高角度

	bool	m_bCoordSortEnable;
	CoordSortMethod m_nCoordSortMethod;
	SortOrder m_nCoordSortOrderX;
	SortOrder m_nCoordSortOrderY;
	double		m_dCoordSortGap;
	
	bool    m_bEnableRadiusControl;						// 启用半径管控
	double  m_dMinRadius;								// 最小的半径
	double  m_dMaxRadius;								// 最大的半径
	
    //MoveCircleCenter
    BOOL	m_bEnableCircleCenterMove;
    int		m_nCircleCenterMoveMode;
    int		m_nCircleCenterMoveModeX;
    int		m_nCircleCenterMoveModeY;
    double	m_dCircleCenterMoveLengthX;
    double	m_dCircleCenterMoveRadiusMulX;
    double	m_dCircleCenterMoveLengthY;
    double	m_dCircleCenterMoveRadiusMulY;
    double	m_dCircleCenterMoveAngle;
    double	m_dCircleCenterMoveRadiusMul;

	tagCircleSearchSearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.8;
		m_dAngleLow		= -5.0;
		m_dAngleHigh	= 5.0;

		m_bCoordSortEnable = false;
		m_nCoordSortMethod = eCoordXOnly;
		m_nCoordSortOrderX = eAscending;
		m_nCoordSortOrderY = eAscending;
		m_dCoordSortGap = 64;

		m_bEnableRadiusControl = false;
		m_dMinRadius    = 0;
		m_dMaxRadius    = 0.5;
		
        m_bEnableCircleCenterMove = FALSE;
        m_nCircleCenterMoveMode = 0;
        m_nCircleCenterMoveModeX = 0;
        m_nCircleCenterMoveModeY = 0;
        m_dCircleCenterMoveLengthX = 0.0;
        m_dCircleCenterMoveRadiusMulX = 0.0;
        m_dCircleCenterMoveLengthY = 0.0;
        m_dCircleCenterMoveRadiusMulY = 0.0;
        m_dCircleCenterMoveAngle  = 0.0;
        m_dCircleCenterMoveRadiusMul = 0.0;
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eCircleSearchTool;
	}

}CCircleSearchSearchDataParam;

// CircleSearch定位工具搜索Gui参数类
typedef struct tagCircleSearchGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bAffineRectVisible;				// 搜索结果矩形可见
	bool			m_bModelPointVisible;				// 参考点是否可见
	bool			m_bCircleVisible;					// 圆形是否可见
	bool			m_bCircleCenterVisible;				// 圆心是否可见

	bool			m_bEnableShowLinePoint;				// 拟合数据点是否可见
	COLORREF        m_cLineColorOKFitPoint1;			// 拟合数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint1;			// 拟合数据点NG颜色

	tagCircleSearchGuiParam()
	{
		m_LineColor				= RGB(0, 255, 0);
		m_nLineWidth			= 1;
		m_bAffineRectVisible	= true;
		m_bModelPointVisible	= true;
		m_bCircleVisible		= true;
		m_bCircleCenterVisible	= false;

		m_bEnableShowLinePoint = false;
		m_cLineColorOKFitPoint1 = RGB(255,255,0);
		m_cLineColorNGFitPoint1 = RGB(255,0,0);
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCircleSearchTool;
	}

}CCircleSearchSearchGuiParam;

// QuickSearch训练结果
typedef struct tagCircleSearchTrainResult
{
	double m_dModelX;
	double m_dModelY;
	double m_dCenterX;
	double m_dCenterY;
	double m_dRadius;
	tagCircleSearchTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
		m_dCenterX = 0;
		m_dCenterY = 0;
		m_dRadius = 0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCircleSearchTool;
	}
}CCircleSearchTrainResult;

//
//	圆定位工具
//
class /*SV_SEARCHTOOL_API*/ CCircleSearchTool : public CBaseSearchTool    
{
public:
	CCircleSearchTool();
	CCircleSearchTool(CCircleSearchTool &cCircleSearchTool);
	CCircleSearchTool& operator=(CCircleSearchTool &cCircleSearchTool);
	virtual ~CCircleSearchTool();

public:
	
	//////////////////////////工具属性相关////////////////////////////////
	virtual CSearchToolType GetSearchToolType();										    // 获取工具类型
	
	//////////////////////////显示更新相关////////////////////////////////
	virtual bool SetDisplay(scGuiDisplay *pGuiDisplay);										// 设置显示控件指针	
	virtual scGuiDisplay* GetDisplay();														// 获取显示控件指针
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
	virtual bool LoadModelTrainPatternImage(CString strDir , cpImage &ImagePattern);		// 加载模板训区域图片， strDir为文件夹路径,ImagePattern为获取得到的训练区域图片
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

private:	
	scQuickSearchTool                   m_QuickSearchTool;				// QuickSearch搜索工具
	scFindCircle						m_FindCircleTool;				// 找线工具
	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd;			// 显示训练结果对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CCircleSearchTrainDataParam			m_tmpTrainDataParam;			// 当前训练Data参数
	CCircleSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CCircleSearchSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CCircleSearchSearchGuiParam			m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CCircleSearchTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
 	CCircleSearchTrainDataParam			m_TrainDataParam;				// 训练Data参数
 	CCircleSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
 	CCircleSearchSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
 	CCircleSearchSearchGuiParam			m_SearchGuiParam;				// 搜索Gui参数
	CCircleSearchTrainResult			m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
	cpImage							m_ImageTrainResult;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;      // 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功
	

	// 训练GUI
	scGuiAffineRect                     m_GuiTrainRect;					// 训练矩形			
	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint;				// 参考点
	scGuiFindCircle                     m_GuiFindCircle;				// 找圆工具
	scGuiCircle                         m_GuiTrainResultCircle;         // 训练结果圆
	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽
	scGuiFeaturelet						m_GuiTrainFeaturelet;			// 训练特征链集

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchAffineRectArray;	// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleArray;		// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleCenterArray;	// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchCirclePointArray1;	// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchCirclePointArray2;	// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineSegArray;			// 搜索结果直线

	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];
	scGuiCircle							m_GuiSearchCircle[MAXRESNUM];
	scGuiCross							m_GuiSearchCircleCenter[MAXRESNUM];
	scGuiCross							m_GuiSearchCirclePoint1[MAXRESNUM];
	scGuiCross							m_GuiSearchCirclePoint2[MAXRESNUM];
	scGuiLineSeg						m_GuiSearchLineSeg[MAXRESNUM];

	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPoint1Array;
	scGuiCoordCross						m_GuiSearchResultFitPoint1[4096];
};
#pragma pack(pop)

#endif // !defined(AFX_CIRCLESEARCHTOOL_H__178C6CE1_A745_4F8E_BAD9_E7F1C5D03EC9__INCLUDED_)
