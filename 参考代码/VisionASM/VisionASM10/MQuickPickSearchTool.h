// MQuickSearchTool.h: interface for the CMQuickSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MQUICKSEARCHTOOL_H__8CF0FDDA_2D35_45FE_A3EF_327D8ED73244F__INCLUDED_)
#define AFX_MQUICKSEARCHTOOL_H__8CF0FDDA_2D35_45FE_A3EF_327D8ED73244F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseSearchTool.h"
#include "svQuickSearch.h"
#include "svGuiAffineRect.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#pragma pack(push,8)

// MQuickPickSearch定位工具训练Data参数类
typedef struct tagMQuickPickSearchTrainDataParam : public CBaseTrainDataParam
{
	double	m_dGrau;									// 颗粒度
	long	m_lNoiseThre;								// 噪声阈值
	long	m_lTotalMag;								// 幅值和
	bool	m_bMaskEnable;								// 是否设置掩模图像
	tagMQuickPickSearchTrainDataParam()
	{
		m_dGrau			= 2.0;
		m_lNoiseThre	= 10;
		m_lTotalMag		= 200;
		m_bMaskEnable	= false;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eMQuickPickSearchTool;
	}

}CMQuickPickSearchTrainDataParam;

// QuickSearch定位工具训练Gui参数类
typedef struct tagMQuickPickSearchTrainGuiParam : public CBaseTrainGuiParam
{
	scAffineRect	m_TrainAffineRect;					// 训练区域
	scRect			m_SearchRect;						// 搜索区域
	scRect			m_Hand1SearchRect;						//抓头1搜索区域
	scRect			m_Hand2SearchRect;						// 抓头2搜索区域
	sc2Vector		m_ModelPoint;						// 参考点
	sc2Vector		m_ModelPoint2;						// 参考点2
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainAffineRectVisible;			// 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bModelPoint2Visible;				// 模板参考点2是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见

	bool			m_bSearchHideRectVisible1;			// 夹爪1隐藏区域是否启用
	bool			m_bSearchHideRectVisible2;			// 夹爪2隐藏区域是否启用

	scRect			m_Search1HideRect;					// 抓头1屏蔽区域
	scRect			m_Search2HideRect;					// 抓头2屏蔽区域
	COLORREF        m_HideRectColor;

	tagMQuickPickSearchTrainGuiParam()
	{
		m_LineColor					= RGB(0, 0, 255);
		m_nLineWidth				= 1;
		m_bTrainAffineRectVisible	= true;
		m_bModelPointVisible		= true;
		m_bModelPoint2Visible       = true;
		m_bSearchRectVisible		= true;

		m_bSearchHideRectVisible1		= false;
		m_bSearchHideRectVisible2		= false;

		m_HideRectColor					= RGB(0, 255, 255);
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eMQuickPickSearchTool;
	}
}CMQuickPickSearchTrainGuiParam;

// QuickSearch定位工具搜索Data参数类
typedef struct tagMQuickPickSearchSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double  m_dAngleLow;								// 低角度
	double	m_dAngleHigh;								// 高角度
	double  m_dSizeLow;
	double  m_dSizeHigh;

	bool	m_bCoordSortEnable;
	CoordSortMethod m_nCoordSortMethod;
	SortOrder m_nCoordSortOrderX;
	SortOrder m_nCoordSortOrderY;
	double		m_dCoordSortGap;

	tagMQuickPickSearchSearchDataParam()
	{
		m_nSearchNum	= 2;
		m_dAcceptThre	= 0.8;
		m_dAngleLow		= -5.0;
		m_dAngleHigh	= 5.0;
		m_dSizeLow = 1.0;
		m_dSizeHigh = 1.0;
		m_bCoordSortEnable = false;
		m_nCoordSortMethod = eCoordXOnly;
		m_nCoordSortOrderX = eAscending;
		m_nCoordSortOrderY = eAscending;
		m_dCoordSortGap = 64;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eMQuickPickSearchTool;
	}

}CMQuickPickSearchSearchDataParam;

// QuickSearch定位工具搜索Gui参数类
typedef struct tagMQuickPickSearchSearchGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bAffineRectVisible;				// 搜索结果矩形可见
	bool			m_bModelPointVisible;				// 参考点是够可见
	tagMQuickPickSearchSearchGuiParam()
	{
		m_LineColor			 = RGB(0, 255, 0);
		m_nLineWidth		 = 1;
		m_bAffineRectVisible = true;
		m_bModelPointVisible = true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eQuickSearchTool;
	}

}CMQuickPickSearchSearchGuiParam;

// QuickSearch训练结果
typedef struct tagMQuickPickSearchTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y
	double m_dModelX2; // 参考点X					
	double m_dModelY2; // 参考点Y

	tagMQuickPickSearchTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
		m_dModelX2 = 0;
		m_dModelY2 = 0;	
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eMQuickPickSearchTool;
	}
}CMQuickPickSearchTrainResult;

//
//	QuickSearch定位工具
//
class /*SV_SEARCHTOOL_API*/ CMQuickPickSearchTool : public CBaseSearchTool  
{
public:
	CMQuickPickSearchTool();
	CMQuickPickSearchTool(CMQuickPickSearchTool &cMQuickPickSearchTool);
	CMQuickPickSearchTool& operator=(CMQuickPickSearchTool &cMQuickPickSearchTool);
	virtual ~CMQuickPickSearchTool();

public:

	//////////////////////////工具属性相关////////////////////////////////
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
	BOOL PointInRect(sc2Vector point,scRect rect);
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
	scQuickSearchTool                   m_QuickSearchTool;				// QuickSearch搜索工具
	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd;			// 显示训练结果对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CMQuickPickSearchTrainDataParam			m_tmpTrainDataParam;			// 当前训练Data参数
	CMQuickPickSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CMQuickPickSearchSearchDataParam			m_tmpSearchDataParam;			// 当前搜索Data参数	
	CMQuickPickSearchSearchGuiParam			m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CMQuickPickSearchTrainResult				m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
	CMQuickPickSearchTrainDataParam			m_TrainDataParam;				// 训练Data参数
	CMQuickPickSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CMQuickPickSearchSearchDataParam			m_SearchDataParam;				// 搜索Data参数	
	CMQuickPickSearchSearchGuiParam			m_SearchGuiParam;				// 搜索Gui参数
	CMQuickPickSearchTrainResult				m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
 	cpImage							m_ImageTrainResult;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功
	
	// 训练GUI
	scGuiAffineRect                     m_GuiTrainRect;					// 训练矩形			
	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiRect							m_GuiHand1SearchRect;				//吸头1 搜索矩形
	scGuiRect							m_GuiHand2SearchRect;				//吸头2搜索矩形
	scGuiRect							m_GuiSearch1HideRect;			// 搜索抓1隐藏矩形
	scGuiRect							m_GuiSearch2HideRect;			// 搜索抓2隐藏矩形
	scGuiCross                          m_GuiModelPoint;				// 参考点
	scGuiCross                          m_GuiModelPoint2;				// 参考点2
	scGuiFeaturelet						m_GuiTrainFeaturelet;			// 训练特征链集
	
	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchAffineRectArray;	// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchCross2Array;		// 搜索结果参考点2
	std::vector<scGuiGraphic*>          m_pGuiSearchCross3Array;		// 搜索结果都有参考点

	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];
	scGuiCross							m_GuiSearchCross2[MAXRESNUM];
	scGuiCross							m_GuiSearchCross3[MAXRESNUM];

};
#pragma pack(pop)

#endif // !defined(AFX_QUICKSEARCHTOOL_H__8CF0FDDA_2D35_45FE_A3EF_327D8D73244F__INCLUDED_)
