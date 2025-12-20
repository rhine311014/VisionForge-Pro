// PatternSearchObroundTool.h: interface for the CPatternSearchObroundTool class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseSearchTool.h"
#include "svPatternSearchRot.h"
#include "svFind.h"
#include "svGuiFindCircle.h"
#include "svGuiAffineRect.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svGuiFindCircle.h"
#include "svGuiCircle.h"
#include "svGuiFindObround.h"
#include "svGuiObround.h"
#include "svObround.h"
#include "svFindObround.h"
#pragma pack(push,8)

// PatternSearchObround定位工具训练Data参数类
typedef struct tagPatternSearchObroundTrainDataParam : public CBaseTrainDataParam
{
	CoarseScale						m_CoarseScale;			// 最粗定位层次
	Accuracy						m_Accuracy;				// 定位精度
	double							m_dAngleLow;			// 低角度
	double							m_dAngleHigh;			// 高角度
	bool							m_bMaskEnable;			// 是否设置掩模图像

	double			m_dContrastThre;					// 对比度阈值
	long			m_lFilterHalfWidth;					// 滤波器半宽
	int				m_nOutLineNum;						// 局外点
	EdgePolarity	m_edgeProperty;						// 边缘属性
	bool			m_bSortByScore;						// 按分数排序

	tagPatternSearchObroundTrainDataParam()
	{
		m_CoarseScale	=	CoarseScale1;
		m_Accuracy		=	AccuracyFine;
		m_dAngleLow		=	-10.0;
		m_dAngleHigh	=	10.0;
		m_bMaskEnable		= false;

		m_dContrastThre		= 10.0;
		m_lFilterHalfWidth	= 1;
		m_nOutLineNum		= 10;
		m_edgeProperty		= eDarkToLight;
		m_bSortByScore		= true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternSearchObroundTool;
	}
}CPatternSearchObroundTrainDataParam;

// PatternSearchObround定位工具训练Gui参数类
typedef struct tagPatternSearchObroundTrainGuiParam : public CBaseTrainGuiParam
{
	scRect			m_TrainRect;						// 训练区域
	scRect			m_SearchRect;						// 搜索区域
	sc2Vector		m_ModelPoint;						// 参考点

	scObround		m_Obround;							// 长圆形

	int				m_nCaliperNum;						// 卡尺个数
	double			m_dCaliperProjectLen;				// 投影长度
	double			m_dCaliperSearchLen;				// 搜索长度
	CaliperSearchDirection m_CaliperSearchDirection;	// 搜索方向

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainRectVisible;				// 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	bool			m_bObroundVisible;					// 找圆工具是否可见
	tagPatternSearchObroundTrainGuiParam()
	{
		m_Obround = scObround(sc2Vector(200,200),100,100,scRadian(0));

		m_nCaliperNum				= 20;
		m_dCaliperProjectLen		= 10.0;
		m_dCaliperSearchLen			= 50.0;
		m_CaliperSearchDirection	= eInward;

		m_LineColor					= RGB(0, 0, 255);
		m_nLineWidth				= 1;
		m_bTrainRectVisible			= true;
		m_bModelPointVisible		= true;
		m_bSearchRectVisible		= true;
		m_bObroundVisible			= true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternSearchObroundTool;
	}
}CPatternSearchObroundTrainGuiParam;

// PatternSearchObround定位工具搜索Data参数类
typedef struct tagPatternSearchObroundSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double  m_dConfusionThre;							// 混淆阈值。混淆阈值不小于接收阈值

	tagPatternSearchObroundSearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.6;
		m_dConfusionThre = 0.8;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternSearchObroundTool;
	}

}CPatternSearchObroundSearchDataParam;

// PatternSearchObround定位工具搜索Gui参数类
typedef struct tagPatternSearchObroundGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bAffineRectVisible;				// 搜索结果矩形可见
	bool			m_bModelPointVisible;				// 参考点是否可见
	bool			m_bObroundVisible;					// 圆形是否可见
	bool			m_bObroundCenterVisible;			// 圆心是否可见
	tagPatternSearchObroundGuiParam()
	{
		m_LineColor				= RGB(0, 255, 0);
		m_nLineWidth			= 1;
		m_bAffineRectVisible	= true;
		m_bModelPointVisible	= false;
		m_bObroundVisible		= true;
		m_bObroundCenterVisible	= true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternSearchObroundTool;
	}

}CPatternSearchObroundSearchGuiParam;

// PatternSearch训练结果
typedef struct tagPatternSearchObroundTrainResult
{
	double m_dModelX;
	double m_dModelY;
	double m_dCenterX;
	double m_dCenterY;
	tagPatternSearchObroundTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
		m_dCenterX = 0;
		m_dCenterY = 0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return ePatternSearchObroundTool;
	}
}CPatternSearchObroundTrainResult;

//
//	圆定位工具
//
class /*SV_SEARCHTOOL_API*/ CPatternSearchObroundTool : public CBaseSearchTool    
{
public:
	CPatternSearchObroundTool();
	CPatternSearchObroundTool(CPatternSearchObroundTool &cPatternSearchObroundTool);
	CPatternSearchObroundTool& operator=(CPatternSearchObroundTool &cPatternSearchObroundTool);
	virtual ~CPatternSearchObroundTool();

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
	virtual bool LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern);			// 加载模板训区域图片， strDir为文件夹路径,ImagePattern为获取得到的训练区域图片
	//////////////////////语言类型//////////////////////////////////////
	virtual void	SetLanguage(int nLanguage);									// 设置中英文菜单 0：eng；1：chs
	//////////////////////错误信息///////////////////////////////////////
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
	scPatternSearchRot                  m_PatternSearchTool;				// PatternSearch搜索工具
	scFindObround						m_FindObroundTool;				// 找线工具
	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CPatternSearchObroundTrainDataParam			m_tmpTrainDataParam;			// 当前训练Data参数
	CPatternSearchObroundTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CPatternSearchObroundSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CPatternSearchObroundSearchGuiParam			m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CPatternSearchObroundTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
 	CPatternSearchObroundTrainDataParam			m_TrainDataParam;				// 训练Data参数
 	CPatternSearchObroundTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
 	CPatternSearchObroundSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
 	CPatternSearchObroundSearchGuiParam			m_SearchGuiParam;				// 搜索Gui参数
	CPatternSearchObroundTrainResult			m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
	cpImage							m_ImageTrainResult;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;      // 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功
	

	// 训练GUI
	scGuiRect							m_GuiTrainRect;					// 训练矩形			
	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint;				// 参考点
	scGuiFindObround                    m_GuiFindObround;				// 找圆工具
	scGuiObround                        m_GuiTrainResultObround;         // 训练结果圆
	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽
	
	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchAffineRectArray;	// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchObroundArray;		// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchObroundCenterArray;	// 搜索结果直线

	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];
	scGuiObround						m_GuiSearchObround[MAXRESNUM];
	scGuiCross							m_GuiSearchObroundCenter[MAXRESNUM];

	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽
};
#pragma pack(pop)

