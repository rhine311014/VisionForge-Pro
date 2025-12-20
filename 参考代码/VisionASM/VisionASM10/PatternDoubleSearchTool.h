// PatternDouleSearchTool.h: interface for the CPatternDouleSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATTERNDOUBLESEARCHTOOL_H__1041DB73_FBA4_4118_983F_83DE5917D7D8__INCLUDED_)
#define AFX_PATTERNDOUBLESEARCHTOOL_H__1041DB73_FBA4_4118_983F_83DE5917D7D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseSearchTool.h"
//#include "svQuickSearch.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svPatternSearchRot.h"
#include "svGuiAffineRect.h"
#pragma pack(push,8)

//PatternDouleSearch定位工具训练Data参数类
typedef struct tagPatternDouleSearchTrainDataParam : public CBaseTrainDataParam
{
	//工具1 
	CoarseScale						m_CoarseScale1;			// 最粗定位层次
	Accuracy						m_Accuracy1;			// 定位精度
	double							m_dAngleLow1;			// 低角度
	double							m_dAngleHigh1;			// 高角度
	bool							m_bMaskEnable1;			// 是否设置掩模图像

	//工具2
	CoarseScale						m_CoarseScale2;			// 最粗定位层次
	Accuracy						m_Accuracy2;			// 定位精度
	double							m_dAngleLow2;			// 低角度
	double							m_dAngleHigh2;			// 高角度
	bool							m_bMaskEnable2;			// 是否设置掩模图像
	tagPatternDouleSearchTrainDataParam()
	{
		m_CoarseScale1	=	CoarseScale1;
		m_Accuracy1		=	AccuracyFine;
		m_dAngleLow1	=	-10.0;
		m_dAngleHigh1	=	10.0;
		m_bMaskEnable1	=	false;

		m_CoarseScale2	=	CoarseScale1;
		m_Accuracy2		=	AccuracyFine;
		m_dAngleLow2	=	-10.0;
		m_dAngleHigh2	=	10.0;
		m_bMaskEnable2	=	false;		
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternDouleSearchTool;
	}
}CPatternDoubleSearchTrainDataParam;

//PatternDouleSearch定位工具训练Gui参数类
typedef struct tagPatternDouleSearchTrainGuiParam : public CBaseTrainGuiParam
{
	//工具1
	scRect			m_TrainRect1;						// 训练区域
	sc2Vector		m_ModelPoint1;						// 参考点
	scRect			m_SearchRect1;						// 搜索区域

	//工具2
	scRect			m_TrainRect2;						// 训练区域
	sc2Vector		m_ModelPoint2;						// 参考点
	scRect			m_SearchRect2;						// 搜索区域

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainRectVisible;				// 搜索区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	tagPatternDouleSearchTrainGuiParam()
	{
		m_LineColor			 = RGB(0, 0, 255);
		m_nLineWidth		 = 1;
		m_bModelPointVisible = true;
		m_bTrainRectVisible	 = true;
		m_bSearchRectVisible = true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternDouleSearchTool;
	}
}CPatternDoubleSearchTrainGuiParam;

//PatternDouleSearch定位工具搜索Data参数类
typedef struct tagPatternDouleSearchSearchDataParam : CBaseSearchDataParam
{
	//工具1
	int		m_nSearchNum1;								// 搜索个数
	double	m_dAcceptThre1;								// 接收阈值
	double  m_dConfusionThre1;							// 混淆阈值。混淆阈值不小于接收阈值
// 	bool	m_bEnableSharpScore1;						// 启用清晰度分析						
// 	double	m_dSharpThre1;		 						// 清晰度分析的值
	
	//工具2
	int		m_nSearchNum2;								// 搜索个数
	double	m_dAcceptThre2;								// 接收阈值
	double  m_dConfusionThre2;							// 混淆阈值。混淆阈值不小于接收阈值
// 	bool	m_bEnableSharpScore2;						// 启用清晰度分析
// 	double	m_dSharpThre2;								// 清晰度分析的值

	tagPatternDouleSearchSearchDataParam()
	{
		m_nSearchNum1			= 1;
		m_dAcceptThre1			= 0.6;
		m_dConfusionThre1		= 0.8;

// 		m_bEnableSharpScore1	= false;
// 		m_dSharpThre1			= 0.5; 

		m_nSearchNum2			= 1;
		m_dAcceptThre2			= 0.6;
		m_dConfusionThre2		= 0.8;

// 		m_bEnableSharpScore2	= false;
// 		m_dSharpThre2			= 0.5; 
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternDouleSearchTool;
	}
}CPatternDoubleSearchSearchDataParam;

//PatternDouleSearch定位工具搜索Gui参数类
typedef struct tagPatternDouleSearchSearchGuiParam : CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bRectVisible;						// 搜索结果矩形可见
	bool			m_bPointVisible;					// 搜索结果参考点可见
	tagPatternDouleSearchSearchGuiParam()
	{
		m_LineColor		= RGB(0, 255, 0);
		m_nLineWidth	= 1;
		m_bRectVisible	= true;
		m_bPointVisible = true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternDouleSearchTool;
	}
}CPatternDoubleSearchSearchGuiParam;

//PatternDouleSearch训练结果
typedef struct tagPatternDouleSearchTrainResult
{
	//工具1
	double m_dModelX1;		// 参考点X					
	double m_dModelY1;		// 参考点Y
	double m_dSharpness1;	//模板清晰度值
	//工具2
	double m_dModelX2;		// 参考点X					
	double m_dModelY2;		// 参考点Y
	double m_dSharpness2;	//模板清晰度值

	double m_dMarkToSearch2X;		//第一个工具定位Mark点距离第二个工具搜索中心的距离X
	double m_dMarkToSearch2Y;		//第一个工具定位Mark点距离第二个工具搜索中心的距离Y
	sc2Vector m_scRectSearchSize;	//第二个工具搜索的尺寸
	tagPatternDouleSearchTrainResult()
	{
		m_dModelX1		 = 0;
		m_dModelY1		 = 0;
		m_dSharpness1	 = 1.0;

		m_dModelX2		 = 0;
		m_dModelY2		 = 0;
		m_dSharpness2	 = 1.0;
		m_dMarkToSearch2X	= 200.0;
		m_dMarkToSearch2Y	= 200.0;
		m_scRectSearchSize	= sc2Vector(400.0,300.0);
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternDouleSearchTool;
	}
}CPatternDouleSearchTrainResult;

/*PatternDouleSearch定位工具*/
class CPatternDoubleSearchTool :
	public CBaseSearchTool
{
public:
	CPatternDoubleSearchTool();
	CPatternDoubleSearchTool(CPatternDoubleSearchTool &CPatternDoubleSearchTool);
	CPatternDoubleSearchTool& operator=(CPatternDoubleSearchTool &CPatternDoubleSearchTool);
	virtual ~CPatternDoubleSearchTool(void);

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

	///////////////////////////训练相关/////////////////////////////////////
	virtual bool SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam);					// 设置训练Data参数				
	virtual bool GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam);					// 获取训练Data参数
	virtual bool SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam);						// 获取搜索Gui参数
	virtual bool GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam);						// 获取搜索Gui参数
	virtual bool SetTrainImage(const cpImage &ImageTrain);									// 设置训练原始图像
	virtual bool GetTrainImage(cpImage &ImageTrain);										// 获取训练原始图像
	virtual bool SetMaskImage(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool GetMaskImage(cpImage &ImageMask);										// 获取训练原始图像
	virtual bool SetMaskImage2(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool GetMaskImage2(cpImage &ImageMask);										// 获取训练原始图像
	
	virtual bool Train();																	// 执行训练
	virtual bool IsTrained();																// 是否训练成功
	virtual bool GetTrainResultImage(cpImage &ImageTrainResult);							// 获取训练结果图像
	virtual bool GetPatternImage(cpImage &ImagePattern);									// 获取训练模板原始图像													// 恢复训练参数
	virtual bool GetTrainResultImage2(cpImage &ImageTrainResult);							// 获取训练结果图像
	virtual bool GetPatternImage2(cpImage &ImagePattern);									// 获取训练模板原始图像
	
	virtual bool GetTrainResult(CBaseTrainResult *pTrainResult);							// 获取训练结果
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
	virtual void SetLanguage(int nLanguage);									// 设置中英文菜单 0：eng；1：chs
	//////////////////////错误信息///////////////////////////////////////
	virtual CString	GetResultErrInfo();

private:
	/////////////////////////Gui控件相关////////////////////////////////	
	bool UpdateTrainGuiData(bool bSaveAndValidate);											// 更新训练控件数据信息	
	bool UpdateSearchGuiData(bool bSaveAndValidate);										// 更新搜索结果控件数据信息	
	bool ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray);					// 清除静态控件及其显示													
	bool UpdateTrainGuiDisplay();															// 更新训练Gui控件显示
	bool UpdateSearchResultGuiDisplay();													// 更新搜索Gui控件显示
	bool ClearTrainGui();																	// 清除定位工具在Display上的训练控件
	bool ClearResultGui();


private:
	scPatternSearchRot					m_PatternSearchTool1;			// PatternSearch搜索工具
	scPatternSearchRot					m_PatternSearchTool2;			// PatternSearch搜索工具
	
	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀

	CPatternDoubleSearchTrainDataParam	m_tmpTrainDataParam;			// 当前训练Data参数
	CPatternDoubleSearchTrainGuiParam	m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CPatternDoubleSearchSearchDataParam	m_tmpSearchDataParam;			// 当前搜索Data参数	
	CPatternDoubleSearchSearchGuiParam	m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CPatternDouleSearchTrainResult		m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask1;				// 当前掩模图像
	cpImage							m_tmpImageMask2;				// 当前掩模图像
	CPatternDoubleSearchTrainDataParam	m_TrainDataParam;				// 训练Data参数
	CPatternDoubleSearchTrainGuiParam		m_TrainGuiParam;				// 训练Gui参数	
	CPatternDoubleSearchSearchDataParam	m_SearchDataParam;				// 搜索Data参数	
	CPatternDoubleSearchSearchGuiParam	m_SearchGuiParam;				// 搜索Gui参数	
	CPatternDouleSearchTrainResult		m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask1;					// 掩模图像
	cpImage							m_ImageTrainResult1;				// 训练结果图像
	cpImage							m_ImageMask2;					// 掩模图像
	cpImage							m_ImageTrainResult2;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功

	// 训练GUI
	scGuiRect							m_GuiTrainRect1;				// 训练矩形			
	scGuiRect							m_GuiSearchRect1;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint1;				// 参考点

	scGuiRect							m_GuiTrainRect2;				// 训练矩形			
	scGuiRect							m_GuiSearchRect2;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint2;				// 参考点

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchRectArray;			// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点


	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM*2];
	scGuiCross							m_GuiSearchCross[MAXRESNUM*2];

// 	scGuiAffineRect						m_GuiSearchAffineRect1[MAXRESNUM];
// 	scGuiCross							m_GuiSearchCross1[MAXRESNUM];
// 	scGuiAffineRect						m_GuiSearchAffineRect2[MAXRESNUM];
// 	scGuiCross							m_GuiSearchCross2[MAXRESNUM];
};

#pragma pack(pop)


#endif // !defined(AFX_PATTERNDOUBLESEARCHTOOL_H__1041DB73_FBA4_4118_983F_83DE5917D7D8__INCLUDED_)