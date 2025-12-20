#pragma once

#include "BaseSearchTool.h"

#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svPatternSearchRot.h"
#include "svGuiAffineRect.h"

// PatternSearch定位工具训练Data参数类
typedef struct tagPatternSearchCheckTrainDataParam : public CBaseTrainDataParam
{
	CoarseScale						m_CoarseScale;			// 最粗定位层次
	Accuracy						m_Accuracy;				// 定位精度
	double							m_dAngleLow;			// 低角度
	double							m_dAngleHigh;			// 高角度
	bool							m_bMaskEnable;			// 是否设置掩模图像
	bool							m_bEnableCheckRegion1;	// 是否启用检查区域1
	bool							m_bEnableCheckRegion2;	// 是否启用检查区域2

	tagPatternSearchCheckTrainDataParam()
	{
		m_CoarseScale	=	CoarseScale1;
		m_Accuracy		=	AccuracyFine;
		m_dAngleLow		=	-10.0;
		m_dAngleHigh	=	10.0;
		m_bMaskEnable	=	false;
		m_bEnableCheckRegion1  =   true;
		m_bEnableCheckRegion2  =   true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternSearchCheckTool;
	}

}CPatternSearchCheckTrainDataParam;


// PatternSearch定位工具训练Gui参数类
typedef struct tagPatternSearchCheckTrainGuiParam : public CBaseTrainGuiParam
{
	scRect			m_TrainRect;						// 训练区域
	scRect			m_TrainRect1;						// 训练区域
	scRect			m_TrainRect2;						// 训练区域

	sc2Vector		m_ModelPoint;						// 参考点
	sc2Vector		m_ModelPoint1;						// 参考点
	sc2Vector		m_ModelPoint2;						// 参考点

	scRect			m_SearchRect;						// 搜索区域
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainRectVisible;				// 搜索区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见

	tagPatternSearchCheckTrainGuiParam()
	{
		m_LineColor			 = RGB(0, 0, 255);
		m_nLineWidth		 = 1;
		m_bModelPointVisible = true;
		m_bTrainRectVisible	 = true;
		m_bSearchRectVisible = true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternSearchCheckTool;
	}

}CPatternSearchCheckTrainGuiParam;

// PatternSearch定位工具搜索Data参数类
typedef struct tagPatternSearchCheckSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double  m_dConfusionThre;							// 混淆阈值。混淆阈值不小于接收阈值
	double	m_dAcceptThre2;								// 接收阈值
	double  m_dConfusionThre2;							// 混淆阈值。混淆阈值不小于接收阈值

	tagPatternSearchCheckSearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.6;
		m_dConfusionThre = 0.8;
		m_dAcceptThre2	= 0.6;
		m_dConfusionThre2 = 0.8;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternSearchCheckTool;
	}

}CPatternSearchCheckSearchDataParam;

// PatternSearch定位工具搜索Gui参数类
typedef struct tagPatternSearchCheckGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bRectVisible;						// 搜索结果矩形可见
	bool			m_bPointVisible;					// 搜索结果矩形可见

	tagPatternSearchCheckGuiParam()
	{
		m_LineColor		= RGB(0, 255, 0);
		m_nLineWidth	= 1;
		m_bRectVisible	= true;
		m_bPointVisible = true;
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternSearchCheckTool;
	}

}CPatternSearchCheckSearchGuiParam;


// QuickSearch训练结果
typedef struct tagPatternSearchCheckTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y

	tagPatternSearchCheckTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternSearchCheckTool;
	}
}CPatternSearchCheckTrainResult;


class CPatternSearchCheckTool : public CBaseSearchTool
{
public:

	CPatternSearchCheckTool();
	CPatternSearchCheckTool(CPatternSearchCheckTool &cPatternSearchTool);
	CPatternSearchCheckTool& operator=(CPatternSearchCheckTool &cPatternSearchTool);
	virtual ~CPatternSearchCheckTool();

public:

	//////////////////////////工具属性相关////////////////////////////////
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
	bool UpdateSearchResultGuiDisplay();													// 更新搜索Gui控件显示
	bool ClearTrainGui();																	// 清除定位工具在Display上的训练控件
	bool ClearResultGui();																	// 清除定位工具在Display上的结果控件


private:
	scPatternSearchRot					m_PatternSearchTool;			// PatternSearch搜索工具
	scPatternSearchRot					m_PatternSearchTool1;			// PatternSearch搜索工具
	scPatternSearchRot					m_PatternSearchTool2;			// PatternSearch搜索工具

	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀

	CPatternSearchCheckTrainDataParam		m_tmpTrainDataParam;			// 当前训练Data参数
	CPatternSearchCheckTrainGuiParam		m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CPatternSearchCheckSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CPatternSearchCheckSearchGuiParam		m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CPatternSearchCheckTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage								m_tmpImageTrain;				// 当前训练图像
	cpImage								m_tmpImageMask;					// 当前掩模图像

	CPatternSearchCheckTrainDataParam		m_TrainDataParam;				// 训练Data参数
	CPatternSearchCheckTrainGuiParam		m_TrainGuiParam;				// 训练Gui参数	
	CPatternSearchCheckSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CPatternSearchCheckSearchGuiParam		m_SearchGuiParam;				// 搜索Gui参数	
	CPatternSearchCheckTrainResult			m_TrainResult;					// 训练结果

	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
	cpImage							m_ImageTrainResult;				// 训练结果图像
	cpImage							m_ImageTrainResult1;			// 训练结果图像
	cpImage							m_ImageTrainResult2;			// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存

	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功

	// 训练GUI
	scGuiRect							m_GuiTrainRect;					// 训练矩形
	scGuiRect							m_GuiTrainRect1;				// 训练矩形		
	scGuiRect							m_GuiTrainRect2;				// 训练矩形		
	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint;				// 参考点

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchRectArray;			// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点


	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];

	scGuiRect							m_GuiSearchRect1[MAXRESNUM];
	scGuiAffineRect						m_GuiSearchAffineRect1[MAXRESNUM];

	scGuiRect							m_GuiSearchRect2[MAXRESNUM];
	scGuiAffineRect						m_GuiSearchAffineRect2[MAXRESNUM];
};

