// LineSearch22Tool.h: interface for the CLineSearch22Tool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LineSearch22TOOL_H__9ABD565D_F642_41E5_9555_B19A1E621E43__INCLUDED_)
#define AFX_LineSearch22TOOL_H__9ABD565D_F642_41E5_9555_B19A1E621E43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseSearchTool.h"
#include "svQuickSearch.h"
#include "svGuiAffineRect.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svFind.h"
#include "svGuiFindLine.h"
#include "svGuiLine.h"
#pragma pack(push,8)

// LineSearch2定位工具训练Data参数类
typedef struct tagLineSearch2TrainDataParam : public CBaseTrainDataParam
{
	double			m_dGrau;							// 颗粒度
	long			m_lNoiseThre;						// 噪声阈值
	long			m_lTotalMag;						// 幅值和
	bool			m_bMaskEnable;						// 是否设置掩模图像
	double			m_dContrastThre;					// 对比度阈值
	long			m_lFilterHalfWidth;					// 滤波器半宽
	int				m_nOutLineNum;						// 局外点
	EdgePolarity	m_edgeProperty;						// 边缘属性
	tagLineSearch2TrainDataParam()
	{
		m_dGrau				= 2.0;
		m_lNoiseThre		= 10;
		m_lTotalMag			= 200;
		m_bMaskEnable		= false;
		m_dContrastThre		= 10.0;
		m_lFilterHalfWidth	= 1;
		m_nOutLineNum		= 0;
		m_edgeProperty		= eDontCare;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eLineSearch2Tool;
	}

}CLineSearch2TrainDataParam;

// LineSearch2定位工具训练Gui参数类
typedef struct tagLineSearch2TrainGuiParam : public CBaseTrainGuiParam
{
	scAffineRect	m_TrainAffineRect;					// 训练区域
	scRect			m_SearchRect;						// 搜索区域
	sc2Vector		m_ModelPoint;						// 参考点

	int				m_nCaliperNum;						// 卡尺个数
	double			m_dStartX;							// 起始点X
	double			m_dStartY;							// 起始点Y
	double			m_dEndX;							// 终止点X
	double			m_dEndY;							// 终止点Y
	double			m_dCaliperProjectLen;				// 投影长度
	double			m_dCaliperSearchLen;				// 搜索长度
	scRadian		m_CaliperSearchRotation;			// 搜索角度

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainAffineRectVisible;			// 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	bool			m_bLineVisible;						// 找线工具是否可见
	tagLineSearch2TrainGuiParam()
	{
		m_nCaliperNum				=	3;
		m_dStartX					=   100.0;
		m_dStartY					=	100.0;
		m_dEndX						=	200.0;
		m_dEndY						=	200.0;
		m_dCaliperProjectLen		=	20.0;
		m_dCaliperSearchLen			=	100;
		m_CaliperSearchRotation		=	scRadian(skPI/2.0);
		m_LineColor					=	RGB(0, 0, 255);
		m_nLineWidth				=	1;
		m_bTrainAffineRectVisible	=	true;
		m_bModelPointVisible		=	true;
		m_bSearchRectVisible		=	true;
		m_bLineVisible				=	true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eLineSearch2Tool;
	}

}CLineSearch2TrainGuiParam;

// LineSearch2定位工具搜索Data参数类
typedef struct tagLineSearch2SearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;	// 搜索个数
	double	m_dAcceptThre;	// 接收阈值
	double  m_dAngleLow;	// 低角度
	double	m_dAngleHigh;	// 高角度
	tagLineSearch2SearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.8;
		m_dAngleLow		= -10.0;
		m_dAngleHigh	= 10.0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eLineSearch2Tool;
	}

}CLineSearch2SearchDataParam;

// LineSearch2定位工具搜索Gui参数类
typedef struct tagLineSearch2GuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;			// 图形颜色
	int				m_nLineWidth;			// 图形线宽
	bool			m_bAffineRectVisible;	// 搜索结果矩形可见
	bool			m_bModelPointVisible;	// 参考点是否可见
	bool			m_bLineVisible;			// 找线工具是否可见

	tagLineSearch2GuiParam()
	{
		m_LineColor				= RGB(0, 255, 0);
		m_nLineWidth			= 1;
		m_bAffineRectVisible	= false;
		m_bModelPointVisible	= false;
		m_bLineVisible			= true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eLineSearch2Tool;
	}

}CLineSearch2SearchGuiParam;

// QuickSearch训练结果
typedef struct tagLineSearch2TrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y
	double m_dStartX; // 线段起点X
	double m_dStartY; // 线段起点Y
	double m_dEndX;	  // 线段终点X
	double m_dEndY;	  // 线段终点Y

	tagLineSearch2TrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
		m_dStartX = 0;
		m_dStartY = 0;
		m_dEndX   = 0;
		m_dEndY   = 0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eLineSearch2Tool;
	}
}CLineSearch2TrainResult;


//
//	线定位工具
//
class /*SV_SEARCHTOOL_API*/ CLineSearch2Tool : public CBaseSearchTool    
{
public:
	CLineSearch2Tool();
	CLineSearch2Tool(CLineSearch2Tool &cLineSearch2Tool);
	CLineSearch2Tool& operator=(CLineSearch2Tool &cLineSearch2Tool);
	virtual ~CLineSearch2Tool();

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
	virtual bool LoadModelFromFile(CString strFilePath);									// 加载模板文件, strFilePath为文件夹路径
	virtual bool SaveModelToFile(CString strFilePath);										// 保存模板文件, strFilePath为文件夹路径
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
	scQuickSearchTool                   m_QuickSearchTool;			// QuickSearch搜索工具
	scFindLine							m_FindLineTool;				// 找线工具
	scGuiDisplay					   *m_pGuiDisplay;				// 显示对象指针
	HWND                                m_hDispWnd;					// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer; // 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;		// 静态容器
	CString                             m_strKeyText;				// 关键字前缀
	CLineSearch2TrainDataParam			m_tmpTrainDataParam;		// 当前训练Data参数
	CLineSearch2TrainGuiParam			m_tmpTrainGuiParam;			// 当前训练Gui参数	
	CLineSearch2SearchDataParam			m_tmpSearchDataParam;		// 当前搜索Data参数	
	CLineSearch2SearchGuiParam			m_tmpSearchGuiParam;		// 当前搜索Gui参数
	CLineSearch2TrainResult				m_tmpTrainResult;			// 当前训练结果
	cpImage							m_tmpImageTrain;			// 当前训练图像
	cpImage							m_tmpImageMask;				// 当前掩模图像
	CLineSearch2TrainDataParam			m_TrainDataParam;			// 训练Data参数
	CLineSearch2TrainGuiParam			m_TrainGuiParam;			// 训练Gui参数	
	CLineSearch2SearchDataParam			m_SearchDataParam;			// 搜索Data参数	
	CLineSearch2SearchGuiParam			m_SearchGuiParam;			// 搜索Gui参数
	CLineSearch2TrainResult				m_TrainResult;				// 训练结果
	cpImage							m_ImageTrain;				// 训练图像
	cpImage							m_ImageMask;				// 掩模图像
	cpImage							m_ImageTrainResult;			// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;		// 结果保存
	bool								m_bTrainGuiShow;			// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;      // 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;		// 是否显示搜索结果Gui
	bool								m_bTrainOK;					// 是否训练成功

	// 训练GUI
	scGuiAffineRect                     m_GuiTrainRect;				// 训练矩形			
	scGuiRect							m_GuiSearchRect;			// 搜索矩形
	scGuiCross                          m_GuiModelPoint;			// 参考点
	scGuiFindLine                       m_GuiFindLine;				// 两个找线工具
	scGuiLine                           m_GuiTrainResultLine;       // 训练结果线段
	COLORREF                            m_TrainGUIColor;            // 训练图形颜色
	int                                 m_nTrainGuiLineWidth;		// 训练图形线宽
	
	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchAffineRectArray;// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;		// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;		// 搜索结果直线

	
	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];
	scGuiLine							m_GuiSearchLine[MAXRESNUM];


	COLORREF                            m_SearchResultColor;		// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;// 搜索结果线宽
};
#pragma pack(pop)

#endif // !defined(AFX_LineSearch22TOOL_H__9ABD565D_F642_41E5_9555_B19A1E621E43__INCLUDED_)
