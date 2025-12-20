// CornerSearchTool.h: interface for the CCornerSearchTool class.
//
//////////////////////////////////////////////////////////////////////


#pragma once


#include "BaseSearchTool.h"
#include "svQuickSearch.h"
#include "svGuiAffineRect.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svFind.h"
#include "svGuiFindLine.h" 
#include "svGuiLine.h"
#include "svGuiFeaturelet.h"
#pragma pack(push,8)

#ifndef LINE_NUM
#define LINE_NUM  2
#endif

// CornerSearch定位工具训练Data参数类
typedef struct tagQuickCornerCircleSearchTrainDataParam : public CBaseTrainDataParam
{
	double			m_dGrau;							// 颗粒度
	long			m_lNoiseThre;						// 噪声阈值
	long			m_lTotalMag;						// 幅值和
	bool			m_bMaskEnable;						// 是否设置掩模图像
	
	//找线工具
	double			m_dContrastThre[LINE_NUM] ;			// 对比度阈值
	long			m_lFilterHalfWidth[LINE_NUM];		// 滤波器半宽
	int				m_nOutLineNum[LINE_NUM];			// 局外点
	EdgePolarity	m_edgeProperty[LINE_NUM];			// 边缘属性
	bool			m_bSortByScore[LINE_NUM];			// 按分数排序
	bool			m_bEnableLineDualEdge[LINE_NUM];	// 启用/禁用找线工具双边缘
	//找圆工具
	double			m_dCircleContrastThre;					// 对比度阈值
	long			m_lCircleFilterHalfWidth;				// 滤波器半宽
	int				m_nCircleOutLineNum;					// 局外点
	EdgePolarity	m_CircleedgeProperty;					// 边缘属性
	bool			m_bCircleSortByScore;					// 按分数排序
	bool			m_bEnableCircleDualEdge;				// 启用/禁用找圆工具双边缘
	tagQuickCornerCircleSearchTrainDataParam()
	{
		m_dGrau			= 2.0;
		m_lNoiseThre	= 10;
		m_lTotalMag		= 200;
		m_bMaskEnable	= false;
		for (int i=0; i<LINE_NUM; i++)
		{
			m_dContrastThre[i]		= 1.0;
			m_lFilterHalfWidth[i]	= 1;
			m_nOutLineNum[i]		= 0;
			m_edgeProperty[i]		= eDontCare;
			m_bSortByScore[i]		= true;
			m_bEnableLineDualEdge[i] = false;
		}

		m_dCircleContrastThre = 1.0;
		m_lCircleFilterHalfWidth = 1;
		m_nCircleOutLineNum = 10;
		m_CircleedgeProperty = eDontCare;
		m_bCircleSortByScore = true;
		m_bEnableCircleDualEdge = false;

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eQuickCornerCircleSearchTool;
	}

}CQuickCornerCircleSearchTrainDataParam;

// CornerSearch定位工具训练Gui参数类
typedef struct tagQuickCornerCircleSearchTrainGuiParam : public CBaseTrainGuiParam
{
	scAffineRect	m_TrainAffineRect;					// 训练区域
	scRect			m_SearchRect;						// 搜索区域
	sc2Vector		m_ModelPoint;						// 参考点

	// 找线工具
	int				m_nCaliperNum[LINE_NUM];			// 卡尺个数
	double			m_dStartX[LINE_NUM];				// 起始点X
	double			m_dStartY[LINE_NUM];				// 起始点Y
	double			m_dEndX[LINE_NUM];					// 终止点X
	double			m_dEndY[LINE_NUM];					// 终止点Y
	double			m_dCaliperProjectLen[LINE_NUM];		// 投影长度
	double			m_dCaliperSearchLen[LINE_NUM];		// 搜索长度
	scRadian		m_CaliperSearchRotation[LINE_NUM];	// 搜索角度
	bool			m_bLineCaliperVisible[LINE_NUM];	//找线卡尺是否可见			
	// 找圆工具掩摸
	svStd vector<double> m_vdCircleCaliperMaskData;				// 掩膜数据
	svStd vector<bool>   m_vbCircleCaliperMask;					// 卡尺是否被掩膜
	bool			     m_bEnableCircleMask;					// 是否启用掩膜

	//找圆工具
	sc2Vector		m_CircleCenter;								// 圆心X
	double			m_dCircleRadius;							// 半径
	scRadian		m_CircleArcStart;							// 起始弧度
	scRadian        m_CircleArcSpan;							// 弧度跨度
	int				m_nCircleCaliperNum;						// 卡尺个数
	double			m_dCircleCaliperProjectLen;					// 投影长度
	double			m_dCircleCaliperSearchLen;					// 搜索长度
	CaliperSearchDirection m_CircleCaliperSearchDirection;		// 搜索方向
	bool			m_bCircleCaliperVisible;					//找圆卡尺是否可见

	// 找线工具掩摸
	svStd vector<double> m_vdLineCaliperMaskData[LINE_NUM];			// 掩膜数据
	svStd vector<bool>   m_vbLineCaliperMask[LINE_NUM];				// 卡尺是否被掩膜
	bool			     m_bEnableLineMask[LINE_NUM];			// 是否启用掩膜

	// 颜色与可见性
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainAffineRectVisible;			// 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	bool			m_bLineVisible;						// 找线工具是否可见
	bool            m_bCircleVisible;                   // 找圆工具是否可见
	tagQuickCornerCircleSearchTrainGuiParam()
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
		m_bLineCaliperVisible[0]		= false;

		m_nCaliperNum[1]				= 2;
		m_dStartX[1]					= 100.0;
		m_dStartY[1]					= 100.0;
		m_dEndX[1]						= 100.0;
		m_dEndY[1]						= 100.0;
		m_dCaliperProjectLen[1]			= 20.0;
		m_dCaliperSearchLen[1]			= 100.0;
		m_CaliperSearchRotation[1]		= scRadian(skPI/2.0);
		m_bEnableLineMask[1]			= false;
		m_bLineCaliperVisible[1]		= false;
	
		m_CircleCenter						= sc2Vector(200,200);
		m_dCircleRadius						= 100.0;
		m_CircleArcStart					= scRadian(0);
		m_CircleArcSpan						= scRadian(sk2PI);
		m_nCircleCaliperNum					= 20;
		m_dCircleCaliperProjectLen			= 10.0;
		m_dCircleCaliperSearchLen			= 50.0;
		m_CircleCaliperSearchDirection		= eInward;
		m_bCircleCaliperVisible				= false;
		m_bEnableCircleMask					= false;

		m_LineColor						= RGB(0, 0, 255);
		m_nLineWidth					= 1;
		m_bTrainAffineRectVisible		= true;
		m_bModelPointVisible			= true;
		m_bSearchRectVisible			= true;
		m_bLineVisible					= true;
		m_bCircleVisible                = true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eQuickCornerCircleSearchTool;
	}
}CQuickCornerCircleSearchTrainGuiParam;

// CornerSearch定位工具搜索Data参数类
typedef struct tagQuickCornerCircleSearchSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	//double  m_dConfusionThre;							// 混淆阈值。混淆阈值不小于接收阈值
	double  m_dAngleLow;								// 低角度
	double	m_dAngleHigh;								// 高角度

	tagQuickCornerCircleSearchSearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.6;
    //	m_dConfusionThre = 0.8;
		m_dAngleLow		= -5.0;
		m_dAngleHigh	= 5.0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eQuickCornerCircleSearchTool;
	}
}CQuickCornerCircleSearchSearchDataParam;

// CornerSearch定位工具搜索Gui参数类
typedef struct tagQuickCornerCircleSearchSearchGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bAffineRectVisible;				// 搜索结果矩形可见
	bool			m_bModelPointVisible;				// 参考点是否可见

	bool			m_bLineVisible;						// 找线工具是否可见
	bool			m_bLineCrossVisible;				// 找线工具是否可见

	bool			m_bCircleVisible;					// 找圆工具是否可见
	bool			m_bCircleCenterVisible;				// 找圆工具是否可见

	bool			m_bEnableShowLinePoint;				// 拟合数据点是否可见(线1)
	bool			m_bEnableShowLinePoint1;			// 拟合数据点是否可见（线2）
	bool            m_bEnableShowCirclePoint;           // 拟合数据点是否可见(圆)

	COLORREF        m_cLineColorOKFitPoint;			// 拟合数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint;			// 拟合数据点NG颜色

	tagQuickCornerCircleSearchSearchGuiParam()
	{
		m_LineColor				= RGB(0, 255, 0);
		m_nLineWidth			= 1;
		m_bAffineRectVisible	= true;
		m_bModelPointVisible	= true;
		m_bLineVisible			= true;
		m_bLineCrossVisible		= true;

		m_bCircleVisible		= true;
		m_bCircleCenterVisible  = true;

		m_bEnableShowLinePoint = false;
		m_bEnableShowLinePoint1 = false;
		m_bEnableShowCirclePoint=false;
		m_cLineColorOKFitPoint = RGB(255,255,0);
		m_cLineColorNGFitPoint = RGB(255,0,0);
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eQuickCornerCircleSearchTool;
	}
}CQuickCornerCircleSearchSearchGuiParam;

// CornerSearch训练结果
typedef struct tagQuickCornerCircleSearchTrainResult
{

	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y
	double m_dStartX[LINE_NUM]; // 线段起点X
	double m_dStartY[LINE_NUM]; // 线段起点Y
	double m_dEndX[LINE_NUM];	  // 线段终点X
	double m_dEndY[LINE_NUM];	  // 线段终点Y

	double m_dCircleCenterX;
	double m_dCircleCenterY;
	double m_dCircleRadius;

	tagQuickCornerCircleSearchTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
		for (int i=0; i<LINE_NUM; i++)
		{
			m_dStartX[i] = 0;
			m_dStartY[i] = 0;
			m_dEndX[i]   = 0;
			m_dEndY[i]   = 0;
		}

		m_dCircleCenterX = 0;
		m_dCircleCenterY = 0;
		m_dCircleRadius = 0;

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eQuickCornerCircleSearchTool;
	}
}CQuickCornerCircleSearchTrainResult;


//

//
class /*SV_SEARCHTOOL_API*/ CQuickCornerCircleSearchTool : public CBaseSearchTool    
{
public:
	CQuickCornerCircleSearchTool();
	CQuickCornerCircleSearchTool(CQuickCornerCircleSearchTool &cCornerSearchTool);
	CQuickCornerCircleSearchTool& operator=(CQuickCornerCircleSearchTool &cCornerSearchTool);
	virtual ~CQuickCornerCircleSearchTool();

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
	scFindLine							m_FindLineTool[LINE_NUM];		// 找线工具
	scFindCircle                        m_FindCircleTool;               // 找圆工具
	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd;			// 显示训练结果对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CQuickCornerCircleSearchTrainDataParam			m_tmpTrainDataParam;			// 当前训练Data参数
	CQuickCornerCircleSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CQuickCornerCircleSearchSearchDataParam		    m_tmpSearchDataParam;			// 当前搜索Data参数	
	CQuickCornerCircleSearchSearchGuiParam			m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CQuickCornerCircleSearchTrainResult			    m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
	CQuickCornerCircleSearchTrainDataParam			m_TrainDataParam;				// 训练Data参数
	CQuickCornerCircleSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CQuickCornerCircleSearchSearchDataParam		    m_SearchDataParam;				// 搜索Data参数	
	CQuickCornerCircleSearchSearchGuiParam			m_SearchGuiParam;				// 搜索Gui参数
	CQuickCornerCircleSearchTrainResult			    m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
	cpImage							m_ImageTrainResult;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;			// 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功

	// 训练GUI
	scGuiAffineRect                     m_GuiTrainRect;					// 训练矩形			
	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint;				// 参考点
	scGuiFindLine                       m_GuiFindLine[LINE_NUM];		// 两个找线工具
	scGuiFindCircle                     m_GuiFindCircle;                // 找圆工具
	scGuiLine                           m_GuiTrainResultLine[LINE_NUM];	// 训练结果线段
	scGuiCircle                         m_GuiTrainResultCircle;         // 训练结果圆
	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽
	scGuiFeaturelet						m_GuiTrainFeaturelet;			// 训练特征链集

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchAffineRectArray;	// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;			// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点	
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleArray;        // 搜索结果圆
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleCenterArray;  // 搜索结果圆心

	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];
	scGuiCross							m_GuiSearchCross2[MAXRESNUM*2];
	scGuiLine							m_GuiSearchLine[MAXRESNUM*2];
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];
	scGuiCircle                         m_GuiSearchCircle[MAXRESNUM];
	scGuiCross                          m_GuiSearchCircleCenter[MAXRESNUM];

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPointArray_1;  //线1拟合点
	scGuiCoordCross						m_GuiSearchResultFitPoint_1[4096];

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPointArray_2;
	scGuiCoordCross						m_GuiSearchResultFitPoint_2[4096];  // 线2拟合点

	std::vector<scGuiGraphic*>          m_pGuiSearchResultCircleFitPointArray;  // 圆拟合点
	scGuiCoordCross                     m_GuiSearchResultCircleFitPoint[4096];
	
	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽
};
#pragma pack(pop)