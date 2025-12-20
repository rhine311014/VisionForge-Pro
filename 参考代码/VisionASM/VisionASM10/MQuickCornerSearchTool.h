// MQuickCornerSearchTool.h: interface for the CCornerSearch2Tool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MQUICKCORNERSEARCHTOOL_H__B84841C2_1E6B_4589_A474_5BE4F312043F__INCLUDED_)
#define AFX_MQUICKCORNERSEARCHTOOL_H__B84841C2_1E6B_4589_A474_5BE4F312043F__INCLUDED_

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
#include "svGuiFeaturelet.h"

#define   LINES_NUM 4
#pragma pack(push,8)

// CornerSearch定位工具训练Data参数类
typedef struct tagMQuickCornerSearchTrainDataParam : public CBaseTrainDataParam
{
	double			m_dGrau;							// 颗粒度
	long			m_lNoiseThre;						// 噪声阈值
	long			m_lTotalMag;						// 幅值和
	bool			m_bMaskEnable;						// 是否设置掩模图像

	double			m_dContrastThre[LINES_NUM] ;			// 对比度阈值
	long			m_lFilterHalfWidth[LINES_NUM];		// 滤波器半宽
	int				m_nOutLineNum[LINES_NUM];			// 局外点
	EdgePolarity	m_edgeProperty[LINES_NUM];			// 边缘属性
	BOOL			m_bEnableSortByScore[LINES_NUM];		// 启用/禁用找线工具搜索结果按分数排序
	BOOL			m_bEnableDualEdge[LINES_NUM];		// 启用/禁用找线工具双边缘

	tagMQuickCornerSearchTrainDataParam()
	{
		m_dGrau			= 2.0;
		m_lNoiseThre	= 10;
		m_lTotalMag		= 200;
		m_bMaskEnable	= false;

		for (int i=0; i<LINES_NUM; i++)
		{
			m_dContrastThre[i]		= 10;
			m_lFilterHalfWidth[i]	= 1;
			m_nOutLineNum[i]		= 0;
			m_edgeProperty[i]		= eDarkToLight;
			m_bEnableSortByScore[i] = TRUE;
			m_bEnableDualEdge[i]	= false;
		}
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eMQuickCornerSearchTool;
	}
	
}CMQuickCornerSearchTrainDataParam;

// CornerSearch2定位工具训练Gui参数类
typedef struct tagMQuickCornerSearchTrainGuiParam : public CBaseTrainGuiParam
{
	scAffineRect	m_TrainAffineRect;					// 训练区域
	scRect			m_SearchRect;						// 搜索区域
	sc2Vector		m_ModelPoint;						// 参考点
//	sc2Vector		m_ModelPoint2;						// 参考点
	// 找线工具1
	int				m_nCaliperNum[LINES_NUM];			// 卡尺个数
	double			m_dStartX[LINES_NUM];				// 起始点X
	double			m_dStartY[LINES_NUM];				// 起始点Y
	double			m_dEndX[LINES_NUM];					// 终止点X
	double			m_dEndY[LINES_NUM];					// 终止点Y
	double			m_dCaliperProjectLen[LINES_NUM];		// 投影长度
	double			m_dCaliperSearchLen[LINES_NUM];		// 搜索长度
	scRadian		m_CaliperSearchRotation[LINES_NUM];	// 搜索角度
	bool			m_bCaliperVisible[LINES_NUM];		//卡尺是否可见
	// 找线工具掩摸
	svStd vector<double> m_vdCaliperMaskData[LINES_NUM];		// 掩膜数据
	svStd vector<bool>   m_vbCaliperMask[LINES_NUM];			// 卡尺是否被掩膜
	bool			     m_bEnableLineMask[LINES_NUM];			// 是否启用掩膜

	// 颜色与可见性
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainAffineRectVisible;			// 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bModelPoint2Visible;				// 模板参考点2是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	bool			m_bLineVisible;						// 找线工具是否可见
	tagMQuickCornerSearchTrainGuiParam()
	{
		m_nCaliperNum[0]				= 2;
		m_dStartX[0]					= 100.0;
		m_dStartY[0]					= 100.0;
		m_dEndX[0]						= 100.0;
		m_dEndY[0]						= 100.0;
		m_dCaliperProjectLen[0]			= 20.0;
		m_dCaliperSearchLen[0]			= 100.0;
		m_CaliperSearchRotation[0]		= scRadian(skPI/2.0);
		m_bEnableLineMask[0]			= false;
		m_bCaliperVisible[0]			= false;	

		m_nCaliperNum[1]				= 2;
		m_dStartX[1]					= 100.0;
		m_dStartY[1]					= 100.0;
		m_dEndX[1]						= 100.0;
		m_dEndY[1]						= 100.0;
		m_dCaliperProjectLen[1]			= 20.0;
		m_dCaliperSearchLen[1]			= 100.0;
		m_CaliperSearchRotation[1]		= scRadian(skPI/2.0);
		m_bEnableLineMask[1]			= false;
		m_bCaliperVisible[1]			= false;

		m_nCaliperNum[2]				= 2;
		m_dStartX[2]					= 100.0;
		m_dStartY[2]					= 100.0;
		m_dEndX[2]						= 100.0;
		m_dEndY[2]						= 100.0;
		m_dCaliperProjectLen[2]			= 20.0;
		m_dCaliperSearchLen[2]			= 100.0;
		m_CaliperSearchRotation[2]		= scRadian(skPI/2.0);
		m_bEnableLineMask[2]			= false;
		m_bCaliperVisible[2]			= false;

		m_nCaliperNum[3]				= 2;
		m_dStartX[3]					= 100.0;
		m_dStartY[3]					= 100.0;
		m_dEndX[3]						= 100.0;
		m_dEndY[3]						= 100.0;
		m_dCaliperProjectLen[3]			= 20.0;
		m_dCaliperSearchLen[3]			= 100.0;
		m_CaliperSearchRotation[3]		= scRadian(skPI/2.0);
		m_bEnableLineMask[3]			= false;
		m_bCaliperVisible[3]			= false;

		m_LineColor						= RGB(0, 0, 255);
		m_nLineWidth					= 1;
		m_bTrainAffineRectVisible		= true;
		m_bModelPointVisible			= true;
		m_bModelPoint2Visible           = false;
		m_bSearchRectVisible			= true;
		m_bLineVisible					= true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eMQuickCornerSearchTool;
	}
}CMQuickCornerSearchTrainGuiParam;

// CenterSearch定位工具搜索Data参数类
typedef struct tagMQuickCornerSearchDataParam : public CBaseSearchDataParam
{
	int				m_nSearchNum;		// 搜索个数
	double			m_dAcceptThre;		// 接收阈值
	double			m_dAngleLow;		// 低角度
	double			m_dAngleHigh;		// 高角度
	double  m_dSizeLow;
	double  m_dSizeHigh;

	bool	m_bCoordSortEnable;
	CoordSortMethod m_nCoordSortMethod;
	SortOrder m_nCoordSortOrderX;
	SortOrder m_nCoordSortOrderY;
	double		m_dCoordSortGap;
	bool	m_bGetFourResultEnable;

	bool	m_bEnable24CorLineCombine;//23共线综合
	bool	m_bEnable13ProjectTo24Combine;//14投影到23综合
	bool	m_bEnableOutput13MidCombine;//以四边形1234(14相对，23相对)的边14的中点作为结果输出

	tagMQuickCornerSearchDataParam()
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
		m_bGetFourResultEnable = false;

		m_bEnable24CorLineCombine = false;
		m_bEnable13ProjectTo24Combine = false;
		m_bEnableOutput13MidCombine = false; 
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eMQuickCornerSearchTool;
	}
}CMQuickCornerSearchDataParam;

// CenterSearch2定位工具搜索Gui参数类
typedef struct tagMQuickCornerSearchGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bAffineRectVisible;				// 搜索结果矩形可见
	bool			m_bModelPointVisible;				// 参考点是否可见
	bool			m_bLineVisible;						// 找线工具是否可见
	bool			m_bLineCrossVisible;				// 找线工具是否可见

	bool			m_bEnableShowLinePoint;				// 拟合数据点是否可见
	bool			m_bEnableShowLinePoint1;				// 拟合数据点是否可见
	bool			m_bEnableShowLinePoint2;				// 拟合数据点是否可见
	bool			m_bEnableShowLinePoint3;				// 拟合数据点是否可见
	COLORREF        m_cLineColorOKFitPoint;			// 拟合数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint;			// 拟合数据点NG颜色


	tagMQuickCornerSearchGuiParam()
	{
		m_LineColor				= RGB(0, 255, 0);
		m_nLineWidth			= 1;
		m_bAffineRectVisible	= true;
		m_bModelPointVisible	= true;
		m_bLineVisible			= true;
		m_bLineCrossVisible		= true;

		m_bEnableShowLinePoint  = false;
		m_bEnableShowLinePoint1 = false;
		m_bEnableShowLinePoint2 = false;
		m_bEnableShowLinePoint3 = false;
		m_cLineColorOKFitPoint = RGB(255,255,0);
		m_cLineColorNGFitPoint = RGB(255,0,0);
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eMQuickCornerSearchTool;
	}
}CMQuickCornerSearchGuiParam;

// CornerSearch2训练结果
typedef struct tagMQuickCornerSearchTrainResult
{
	double m_dModelX;   // 参考点X					
	double m_dModelY;   // 参考点Y
// 	double m_dModelX2;  // 参考点X					
// 	double m_dModelY2;  // 参考点Y



	double m_dModelX0; // 交点X0					
	double m_dModelY0; // 交点Y0
	double m_dModelX1; // 交点X1					
	double m_dModelY1; // 交点Y1
	double m_dModelX2; // 交点X2					
	double m_dModelY2; // 交点Y2
	double m_dModelX3; // 交点X3					
	double m_dModelY3; // 交点Y3

	double m_dStartX[LINES_NUM]; // 线段起点X
	double m_dStartY[LINES_NUM]; // 线段起点Y
	double m_dEndX[LINES_NUM];	  // 线段终点X
	double m_dEndY[LINES_NUM];	  // 线段终点Y
	
	tagMQuickCornerSearchTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;

		m_dModelX0 = 0;
		m_dModelY0 = 0;

		m_dModelX1 = 0;
		m_dModelY1 = 0;

		m_dModelX2 = 0;
		m_dModelY2 = 0;

		m_dModelX3 = 0;
		m_dModelY3 = 0;


		for (int i=0; i<LINES_NUM; i++)
		{
			m_dStartX[i] = 0;
			m_dStartY[i] = 0;
			m_dEndX[i]   = 0;
			m_dEndY[i]   = 0;
		}
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eMQuickCornerSearchTool;
	}
}CMQuickCornerSearchTrainResult;

//
//	角定位工具2
//

class /*SV_SEARCHTOOL_API*/ CMQuickCornerSearchTool : public CBaseSearchTool  
{
public:
	CMQuickCornerSearchTool();
	virtual ~CMQuickCornerSearchTool();

	CMQuickCornerSearchTool(CMQuickCornerSearchTool &cMQuickCornerSearchTool);
	CMQuickCornerSearchTool& operator=(CMQuickCornerSearchTool &cMQuickCornerSearchTool);

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
	virtual bool SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam);	// 设置搜索Data参数
	virtual bool GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam);				// 获取搜索Data参数
	virtual bool SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam);					// 设置搜索Gui参数
	virtual bool GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam);					// 获取搜索Gui参数
	virtual bool Search(cpImage &ImageSearch);										// 执行搜索 
	virtual int  GetResultNum();															// 获取搜索结果个数
	virtual bool GetResult(int nIndex, CSearchResult &searchResult);						// 获取搜索结果
//	virtual bool GetResultRelative(int nIndex, CSearchResult &searchResult);				// 获取搜索结果（相对坐标）
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
	scFindLine							m_FindLineTool[LINES_NUM];		// 找线工具
	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd;			// 显示训练结果对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CMQuickCornerSearchTrainDataParam			m_tmpTrainDataParam;			// 当前训练Data参数
	CMQuickCornerSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CMQuickCornerSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CMQuickCornerSearchGuiParam			m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CMQuickCornerSearchTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
	CMQuickCornerSearchTrainDataParam			m_TrainDataParam;				// 训练Data参数
	CMQuickCornerSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CMQuickCornerSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CMQuickCornerSearchGuiParam			m_SearchGuiParam;				// 搜索Gui参数
	CMQuickCornerSearchTrainResult			m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
	cpImage							m_ImageTrainResult;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
//	std::vector<CSearchResult>			m_SearchResultArrayRelative;	// 结果保存（相对图像坐标系）
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;			// 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功

	// 训练GUI
	scGuiAffineRect                     m_GuiTrainRect;					// 训练矩形			
	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint;				// 参考点
	scGuiCross                          m_GuiModelPoint2;				// 参考点

	scGuiFindLine                       m_GuiFindLine[LINES_NUM];		// 两个找线工具
	scGuiLine                           m_GuiTrainResultLine[LINES_NUM];	// 训练结果线段
	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽
	scGuiFeaturelet						m_GuiTrainFeaturelet;			// 训练特征链集
	
	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchAffineRectArray;	// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点	
	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;			// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点	


	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽

	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];
	scGuiCross							m_GuiSearchCross1[MAXRESNUM];
	scGuiCross							m_GuiSearchCross2[MAXRESNUM];

	scGuiCross							m_GuiSearchCross3[MAXRESNUM];
	scGuiCross							m_GuiSearchCross4[MAXRESNUM];

	scGuiLine							m_GuiSearchLine[MAXRESNUM*4];
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];


	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPointArray_1;
	scGuiCoordCross						m_GuiSearchResultFitPoint_1[4096];

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPointArray_2;
	scGuiCoordCross						m_GuiSearchResultFitPoint_2[4096];

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPointArray_3;
	scGuiCoordCross						m_GuiSearchResultFitPoint_3[4096];

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPointArray_4;
	scGuiCoordCross						m_GuiSearchResultFitPoint_4[4096];

};
#pragma pack(pop)

#endif // !defined(AFX_MQuickCornerSearchTool_H__B84841C2_1E6B_4589_A474_5BE4F312043F__INCLUDED_)
