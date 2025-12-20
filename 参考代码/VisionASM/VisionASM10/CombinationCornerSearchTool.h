
#pragma once
#include "BaseSearchTool.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svPatternSearchRot.h"
#include "svGuiAffineRect.h"

#include "svFind.h"
#include "svGuiFindLine.h" 
#include "svGuiLine.h"
#include "svGuiCoordCross.h"
#include "svGuiLineSeg.h"
#include "svGuiText.h"
#include "svGuiFeaturelet.h"
#define LINE_NUM 2

#define INI_SEARCH_ANGLE 2
#define INI_CONTRASTTHRE 5
#define INI_ACCEPTTHRE	0.4
#define INI_CONFUSIONTHRE 0.6
//训练Data参数类
typedef struct tagCombinationCornerSearchTrainDataParam : public CBaseTrainDataParam
{
	CoarseScale		m_CoarseScale;						// 最粗定位层次
	Accuracy		m_Accuracy;							// 定位精度
	double			m_dAngleLow;						// 低角度
	double			m_dAngleHigh;						// 高角度
	bool			m_bMaskEnable;						// 是否设置掩模图像

	double			m_dGrau;							// 颗粒度
	long			m_lNoiseThre;						// 噪声阈值
	long			m_lTotalMag;						// 幅值和

	double			m_dContrastThre[LINE_NUM] ;			// 对比度阈值
	long			m_lFilterHalfWidth[LINE_NUM];		// 滤波器半宽
	int				m_nOutLineNum[LINE_NUM];			// 局外点
	EdgePolarity	m_edgeProperty[LINE_NUM];			// 边缘属性
	bool			m_bSortByScore[LINE_NUM];			// 按分数排序
	BOOL			m_bEnableDualEdge[LINE_NUM];		// 启用/禁用找线工具双边缘

	CoarseScale		m_CoarseScale1;						// 最粗定位层次
	Accuracy		m_Accuracy1;						// 定位精度
	double			m_dAngleLow1;						// 低角度
	double			m_dAngleHigh1;						// 高角度
	bool			m_bMaskEnable1;						// 是否设置掩模图像

	double			m_dGrau1;							// 颗粒度
	long			m_lNoiseThre1;						// 噪声阈值
	long			m_lTotalMag1;						// 幅值和


	//bool m_bEnableCheck;								//启用检测
	CSearchToolType m_SearchToolType;					//定位工具类型
	CSearchToolType m_InspectToolType;					//检测工具类型

	BOOL		    m_bEnableCorner;					//启用角定位





	tagCombinationCornerSearchTrainDataParam()
	{
		m_CoarseScale	=	CoarseScale32;
		m_Accuracy		=	AccuracyCoarse;
		m_dAngleLow		=	-INI_SEARCH_ANGLE;
		m_dAngleHigh	=	INI_SEARCH_ANGLE;
		m_bMaskEnable	=	false;

		m_dGrau			= 2.0;
		m_lNoiseThre	= 10;
		m_lTotalMag		= 200;

		m_CoarseScale1	=	CoarseScale32;
		m_Accuracy1		=	AccuracyCoarse;
		m_dAngleLow1		=	-10.0;
		m_dAngleHigh1	=	10.0;
		m_bMaskEnable1	=	false;

		m_dGrau1		= 2.0;
		m_lNoiseThre1	= 10;
		m_lTotalMag1	= 200;




		for (int i=0; i<LINE_NUM; i++)
		{
			m_dContrastThre[i]		= 1.0;
			m_lFilterHalfWidth[i]	= 1;
			m_nOutLineNum[i]		= 0;
			m_edgeProperty[i]		= eDontCare;
			m_bSortByScore[i]		= true;
			m_bEnableDualEdge[i]	= false;
		}

		//m_bEnableCheck = false;
		m_SearchToolType = ePatternCornerSearchTool;
		m_InspectToolType = ePatternSearchTool;

        m_bEnableCorner=true;

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCombinationCornerSearchTool;
	}

	virtual CSearchToolType GetSubSearchToolType()
	{
		return m_SearchToolType;
	}

	virtual bool SetSubSearchToolType(CSearchToolType mSearchToolType)
	{
		m_SearchToolType = mSearchToolType;
		return true;
	}

	virtual CSearchToolType GetInspectToolType()
	{
		return m_InspectToolType;
	}

	virtual bool SetSubInspectToolType(CSearchToolType mInspectToolType)
	{
		m_InspectToolType = mInspectToolType;
		return true;
	}

}CCombinationCornerSearchTrainDataParam;

// CombinationCornerSearch定位工具训练Gui参数类
typedef struct tagCombinationCornerSearchTrainGuiParam : public CBaseTrainGuiParam
{
	//角定位工具GUI训练参数 
	scRect			m_TrainRect;						// 训练区域
	scAffineRect	m_TrainAffineRect;					// 几何训练区域
	sc2Vector		m_ModelPoint;						// 参考点
	scRect			m_SearchRect;						// 搜索区域

	// 找线工具
	int				m_nCaliperNum[LINE_NUM];			// 卡尺个数
	double			m_dStartX[LINE_NUM];				// 起始点X
	double			m_dStartY[LINE_NUM];				// 起始点Y
	double			m_dEndX[LINE_NUM];					// 终止点X
	double			m_dEndY[LINE_NUM];					// 终止点Y
	double			m_dCaliperProjectLen[LINE_NUM];		// 投影长度
	double			m_dCaliperSearchLen[LINE_NUM];		// 搜索长度
	scRadian		m_CaliperSearchRotation[LINE_NUM];	// 搜索角度
	bool			m_bCaliperVisible[LINE_NUM];		// 是否显示卡尺

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainRectVisible;				// 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	bool			m_bLineVisible;						// 线是否可见
    bool			m_bEnableCornerLineMask[LINE_NUM];
	svStd vector<double> m_vdMaskData_FindLine[LINE_NUM];// 掩膜数据
	svStd vector<bool> m_vbCaliperMask_FindLine[LINE_NUM];// 卡尺是否被掩膜

	scRect			m_TrainRect1;						// 训练区域
	scAffineRect	m_TrainAffineRect1;					// 几何训练区域
	sc2Vector		m_ModelPoint1;						// 参考点
	scRect			m_SearchRect1;						// 搜索区域
	COLORREF        m_LineColor1;						// 图形颜色
	int				m_nLineWidth1;						// 图形线宽
	bool			m_bTrainRectVisible1;				// 训练区域是否可见
	bool			m_bModelPointVisible1;				// 模板参考点是否可见
	bool			m_bSearchRectVisible1;				// 搜索区域是否可见



	tagCombinationCornerSearchTrainGuiParam()
	{
	
		

	
	
		m_nCaliperNum[0]				= 50;
		m_dStartX[0]					= 1800.0;
		m_dStartY[0]					= 1700.0;
		m_dEndX[0]						= 1800.0;
		m_dEndY[0]						= 2300.0;
		m_dCaliperProjectLen[0]			= 30.0;
		m_dCaliperSearchLen[0]			= 250.0;
		m_CaliperSearchRotation[0]		= scRadian(skPI/2.0);
        m_bEnableCornerLineMask[0]      = false;
		m_bCaliperVisible[0]			= false;

		m_nCaliperNum[1]				= 50;
		m_dStartX[1]					= 2600.0;
		m_dStartY[1]					= 800.0;
		m_dEndX[1]						= 3500.0;
		m_dEndY[1]						= 800.0;
		m_dCaliperProjectLen[1]			= 30.0;
		m_dCaliperSearchLen[1]			= 250.0;
		m_CaliperSearchRotation[1]		= scRadian(skPI/2.0);
        m_bEnableCornerLineMask[1]      = false;
		m_bCaliperVisible[1]			= false;

		m_LineColor						= RGB(0, 0, 255);
		m_nLineWidth					= 1;
		m_bModelPointVisible			= true;
		m_bTrainRectVisible				= true;
		m_bSearchRectVisible			= true;
		m_bLineVisible					= true;

		m_LineColor1				    = RGB(0, 0, 255);
		m_nLineWidth1					= 1;
		m_bModelPointVisible1			= true;
		m_bTrainRectVisible1			= true;
		m_bSearchRectVisible1			= true;




	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCombinationCornerSearchTool;
	}

}CCombinationCornerSearchTrainGuiParam;

// PatternnCornerSearch定位工具搜索Data参数类
typedef struct tagCombinationCornerSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double  m_dConfusionThre;							// 混淆阈值。混淆阈值不小于接收阈值
	double  m_dAngleLow;								// 低角度
	double	m_dAngleHigh;								// 高角度

	int		m_nSearchNum1;								// 搜索个数
	double	m_dAcceptThre1;								// 接收阈值
	double  m_dConfusionThre1;							// 混淆阈值。混淆阈值不小于接收阈值
	double  m_dAngleLow1;								// 低角度
	double	m_dAngleHigh1;								// 高角度
	double  m_dSizeLow1;
	double  m_dSizeHigh1;
	bool m_bLine2ForceVerticalLine1;					//线2强制垂直线1
	//bool	m_bCheckAfterSearch;						// true 检查在搜索之后



	tagCombinationCornerSearchDataParam()
	{
		m_nSearchNum				= 1;
		m_dAcceptThre				= INI_ACCEPTTHRE;
		m_dConfusionThre			= INI_CONTRASTTHRE;
		m_dAngleLow					= -INI_SEARCH_ANGLE;
		m_dAngleHigh				= INI_SEARCH_ANGLE;

		m_nSearchNum1				= 1;
		m_dAcceptThre1				= INI_ACCEPTTHRE;
		m_dConfusionThre1			= INI_CONTRASTTHRE;
		m_dAngleLow1				= -INI_SEARCH_ANGLE;
		m_dAngleHigh1				= INI_SEARCH_ANGLE;
		m_dSizeLow1					= 1.0;
		m_dSizeHigh1				= 1.0;
		m_bLine2ForceVerticalLine1=FALSE;
		//m_bCheckAfterSearch			=true;

	}

	virtual void InspectOKNG_AndResultInfo()
	{
		
	}


	virtual CSearchToolType GetSearchToolType()
	{
		return eCombinationCornerSearchTool;
	}

}CCombinationCornerSearchSearchDataParam;

// CombinationCornerSearch定位工具搜索Gui参数类
typedef struct tagCombinationCornerSearchGuiParam : public CBaseSearchGuiParam
{

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bRectVisible;						// 搜索结果矩形可见
	bool			m_bPointVisible;					// 参考点是否可见

	bool			m_bLineVisible;						// 找线工具是否可见
	bool			m_bLineCrossVisible;				// 找线工具是否可见

	COLORREF        m_LineColor1;						// 图形颜色
	int				m_nLineWidth1;						// 图形线宽
	bool			m_bRectVisible1;					// 搜索结果矩形可见
	bool			m_bPointVisible1;					// 搜索结果矩形可见

	bool			m_bVisibleFitPoint1;				// 拟合1数据点是否可见
	COLORREF        m_cLineColorOKFitPoint1;			// 拟合1数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint1;			// 拟合1数据点NG颜色

	bool			m_bVisibleFitPoint2;				// 拟合2数据点是否可见
	COLORREF        m_cLineColorOKFitPoint2;			// 拟合2数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint2;			// 拟合2数据点NG颜色

	

	tagCombinationCornerSearchGuiParam()
	{
		m_LineColor					= RGB(0, 255, 0);
		m_nLineWidth				= 1;
		m_bRectVisible				= true;
		m_bPointVisible				= true;
		m_bLineVisible				= true;
		m_bLineCrossVisible			= true;
		

		m_LineColor1				= RGB(0, 255, 0);
		m_nLineWidth1				= 1;
		m_bRectVisible1				= true;
		m_bPointVisible1			= true;

		m_bVisibleFitPoint1			= false;
		m_cLineColorOKFitPoint1		= RGB(255,255,0);
		m_cLineColorNGFitPoint1		= RGB(255,0,0);

		m_bVisibleFitPoint2			= false;
		m_cLineColorOKFitPoint2		= RGB(255,255,0);
		m_cLineColorNGFitPoint2		= RGB(255,0,0);

	

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCombinationCornerSearchTool;
	}

}CCombinationCornerSearchSearchGuiParam;

// 训练结果
typedef struct tagCombinationCornerSearchTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y
	double m_dModelX1; // 参考点X1					
	double m_dModelY1; // 参考点Y1

	double m_dStartX[LINE_NUM]; // 线段起点X
	double m_dStartY[LINE_NUM]; // 线段起点Y
	double m_dEndX[LINE_NUM];	  // 线段终点X
	double m_dEndY[LINE_NUM];	  // 线段终点Y





	tagCombinationCornerSearchTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
		m_dModelX1 = 0;
		m_dModelY1 = 0;

		for (int i=0; i<LINE_NUM; i++)
		{
			m_dStartX[i] = 0;
			m_dStartY[i] = 0;
			m_dEndX[i]   = 0;
			m_dEndY[i]   = 0;
		}



	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eCombinationCornerSearchTool;
	}
}CCombinationCornerSearchTrainResult;


typedef struct tagFindLineTrainDataEx
{
	double			m_dContrastThre;			// 对比度阈值
	long			m_lFilterHalfWidth;			// 滤波器半宽
	int				m_nOutLineNum;				// 局外点
	EdgePolarity	m_edgeProperty;				// 边缘属性
	bool			m_bSortByScore;				// 按分数排序

	int				m_nCaliperNum;				// 卡尺个数
	double			m_dStartX;					// 起始点X
	double			m_dStartY;					// 起始点Y
	double			m_dEndX;					// 终止点X
	double			m_dEndY;					// 终止点Y
	double			m_dCaliperProjectLen;		// 投影长度
	double			m_dCaliperSearchLen;		// 搜索长度
	scRadian		m_CaliperSearchRotation;	// 搜索角度
	svStd vector<double> m_vdMaskData_FindLine;	// 掩膜数据
	svStd vector<bool> m_vbCaliperMask_FindLine;// 卡尺是否被掩膜

	bool			m_bEnableDualEdge;			//双极性启用

	tagFindLineTrainDataEx()
	{
		m_dContrastThre				= 5.0;
		m_lFilterHalfWidth			= 1;
		m_nOutLineNum				= 0;
		m_edgeProperty				= eDontCare;
		m_bSortByScore				= true;

		m_nCaliperNum				= 2;
		m_dStartX					= 200.0;
		m_dStartY					= 100.0;
		m_dEndX						= 200.0;
		m_dEndY						= 500.0;
		m_dCaliperProjectLen		= 20.0;
		m_dCaliperSearchLen			= 100.0;
		m_CaliperSearchRotation		= scRadian(skPI/2.0);

		m_bEnableDualEdge			= false;

	}

}StructFindLineTrainDataEx;

class CCombinationCornerSearchTool :public CBaseSearchTool
{
public:
	CCombinationCornerSearchTool(void);
	CCombinationCornerSearchTool(CCombinationCornerSearchTool &cCombinationCornerSearchTool);
	CCombinationCornerSearchTool& operator=(CCombinationCornerSearchTool &cCombinationCornerSearchTool);
	~CCombinationCornerSearchTool(void);
private:
	// 初始化成员
	void Initialize_Member_Variables();
	
	// 执行区域粗定位搜索
	bool ExecutePatternSerach(cpImage &ImageSearch,scPatternSearchRot* m_PatternSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dConfusionThre,double dTransX,double dTransY,double dRotation);
	
	// 执行几何粗定位搜索
	bool ExecuteQuickSearch(cpImage &ImageSearch,scQuickSearchTool* m_QuickSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dAngleLow,double m_dAngleHigh,double m_dSizeLow1,double m_dSizeHigh1,double dTransX,double dTransY,double dRotation);
	
	// 执行找线
	bool ExecuteFindLine(cpImage &ImageSearch,scFindLine* m_FindLineTool,StructFindLineTrainData  m_sFindLineTrainData,double dTransX,double dTransY,double dRotation,scLineSeg &ResultLine);

	// GUI 显示矩形
	void ShowResultRect(std::vector<scGuiGraphic*>& m_pGuiSearchRectArray,scGuiAffineRect m_GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* m_AffineRect,const CString strRect,COLORREF color,int LineWidth);
	// GUI 显示交叉点
	void ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,COLORREF color,int LineWidth);
	void ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,const CString strCrossLabel,COLORREF color,int LineWidth);
	// GUI 显示线
	void ShowResultLine(std::vector<scGuiGraphic*>& m_pGuiSearchLineArray,scGuiLine m_GuiSearchLine[MAXRESNUM],const int nOffset,scLine *m_scLine,const CString strLine,COLORREF color,int LineWidth);

public:
	// 
	// 	//////////////////////////工具属性相关////////////////////////////////
	virtual CSearchToolType GetSearchToolType();										    // 获取工具类型

	//////////////////////////显示更新相关////////////////////////////////
	virtual bool SetDisplay(scGuiDisplay *pGuiDisplay);										// 设置显示控件指针				
	virtual scGuiDisplay* GetDisplay();														// 获取显示空间指针
	virtual bool SetTrainResultDisplay(scGuiDisplay *pGuiDisplay);							// 设置训练结果显示控件指针
	virtual scGuiDisplay* GetTrainResultDisplay();											// 获取训练结果显示控件指针
	virtual bool SetTrainResultDisplay2(scGuiDisplay *pGuiDisplay);							// 设置训练结果显示控件指针
	virtual scGuiDisplay* GetTrainResultDisplay2();											// 获取训练结果显示控件指针
	virtual bool SetGuiKeyText(CString strGuiKeyText);										// 设置Gui图形的关键字
	virtual bool GetGuiKeyText(CString &strGuiKeyText);										// 获取Gui图形的关键字
	virtual bool ShowTrainGui(bool bShow) ;													// 是否显示训练界面
	virtual bool ShowTrainResultGui(bool bShow) ;											// 是否显示训练结果界面
	virtual bool ShowSearchResultGui(bool bShow);											// 是否显示搜索界面
	virtual bool UpdateGuiDisplay();														// 更新界面显示
	virtual bool ClearAllGui();															    // 清除定位工具在Display上的控件
	virtual bool ClearResult();																// 清除搜索结果
    virtual bool SetGuiByCam(double nWidth,double nHeight);								// 根据相机宽高设置GUI大小
	///////////////////////////训练相关/////////////////////////////////////
	virtual bool SetTrainDataParamForce(CBaseTrainDataParam *pTrainDataParam);				// 强制 调整m_TrainDataParam 值
	virtual bool SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam);					// 设置训练Data参数				
	virtual bool GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam);					// 获取训练Data参数
	virtual bool SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam);						// 获取搜索Gui参数
	virtual bool GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam);						// 获取搜索Gui参数
	virtual bool SetTrainImage(const cpImage &ImageTrain);									// 设置训练原始图像
	virtual bool GetTrainImage(cpImage &ImageTrain);										// 获取训练原始图像
	virtual bool SetMaskImage(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool SetMaskImage2(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool GetMaskImage(cpImage &ImageMask);											// 获取训练原始图像
	virtual bool GetMaskImage2(cpImage &ImageMask);											// 获取训练原始图像
	virtual bool Train();																	// 执行训练
	virtual bool IsTrained();																// 是否训练成功
	virtual bool GetTrainResultImage(cpImage &ImageTrainResult);							// 获取训练结果图像
	virtual bool GetTrainResultImage2(cpImage &ImageTrainResult);							// 获取训练结果图像
	virtual bool GetTrainResult(CBaseTrainResult *pTrainResult);							// 获取训练结果
	virtual bool GetPatternImage(cpImage &ImagePattern);	// 获取训练模板原始图像
	virtual bool GetPatternImage2(cpImage &ImagePattern);
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
	virtual CString GetResultErrInfo();

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

	
	void ShowFitPoint(std::vector<scGuiGraphic*>& m_pGuiInspectFitPointArray,scGuiCoordCross m_GuiInspectFitPoint[MAXPTSNUM],scFindLine* m_FindLineTool,const CString strFitPoint);
	void ShowFitPoint(std::vector<scGuiGraphic*>& m_pGuiInspectFitPointArray,scGuiCoordCross m_GuiInspectFitPoint[MAXPTSNUM],scFindCircle* m_FindLineTool,const CString strFitPoint);
	

	bool GetFindLineCenterPoint(scFindLine* m_FindLineTool,sc2Vector &pointCenter2);

	

private:
	scPatternSearchRot					m_PatternSearchTool;			// 区域角定位的粗定位搜索工具
	scPatternSearchRot					m_PatternSearchTool1;			// 检查工具的区域搜索工具
	scQuickSearchTool                   m_QuickSearchTool;				// 几何角定位QuickSearch搜索工具
	scQuickSearchTool                   m_QuickSearchTool1;				// 检查工具QuickSearch搜索工具

	scFindLine							m_FindLineTool[LINE_NUM];		// 找线工具

	

	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd;			// 显示训练结果对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay2;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd2;			// 显示训练结果对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CCombinationCornerSearchTrainDataParam		m_tmpTrainDataParam;			// 当前训练Data参数
	CCombinationCornerSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CCombinationCornerSearchSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CCombinationCornerSearchSearchGuiParam		m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CCombinationCornerSearchTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像

	cpImage							m_tmpImageTrain1;				// 当前训练图像
	cpImage							m_tmpImageMask1;					// 当前掩模图像

	CCombinationCornerSearchTrainDataParam		m_TrainDataParam;				// 训练Data参数
	CCombinationCornerSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CCombinationCornerSearchSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CCombinationCornerSearchSearchGuiParam		m_SearchGuiParam;				// 搜索Gui参数	
	CCombinationCornerSearchTrainResult			m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像

	cpImage							m_ImageTrain1;					// 训练图像
	cpImage							m_ImageMask1;				// 掩模图像

	cpImage							m_ImageTrainResult;				// 训练结果图像
	cpImage							m_ImageTrainResult1;			// 检查工具训练结果图像

	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;			// 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功

	// 训练GUI
	scGuiRect							m_GuiTrainRect;					// 训练矩形
	scGuiRect							m_GuiTrainRect1;				// 训练矩形

	scGuiAffineRect						m_GuiTrainAffineRect;			//几何训练区
	scGuiAffineRect						m_GuiTrainAffineRect1;

	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiRect							m_GuiSearchRect1;				// 搜索矩形

	scGuiCross                          m_GuiModelPoint;				// 参考点
	scGuiCross                          m_GuiModelPoint1;				// 参考点
	scFindCircle						m_FindCircleTool;				// 找圆工具
	scGuiFindCircle                     m_GuiFindCircle;				// 找圆工具
	scGuiCircle                         m_GuiTrainResultCircle;         // 训练结果圆
	scGuiFindLine                       m_GuiFindLine[LINE_NUM];		// 两个找线工具
	scGuiLine                           m_GuiTrainResultLine[LINE_NUM];	// 训练结果线段
	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽

	scGuiFeaturelet						m_GuiTrainFeaturelet;			// 训练特征链集
	scGuiFeaturelet						m_GuiTrainFeaturelet2;			// 训练特征链集

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchRectArray;			// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;			// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineTextArray;		// 搜索结果文本




	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM];
	scGuiCross							m_GuiSearchCross2[MAXRESNUM];
	scGuiCross							m_GuiSearchCross3[MAXRESNUM];
	scGuiLine							m_GuiSearchLine[MAXRESNUM*2];
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];
	scGuiLineSeg						m_GuiSearchLineSeg[MAXRESNUM*2];
	scGuiAffineRect						m_GuiSearchAffineRect1[MAXRESNUM];
    scGuiText							m_GuiSearchLineText[MAXRESNUM];

	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPoint1Array;
	scGuiCoordCross						m_GuiSearchResultFitPoint1[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPoint2Array;
	scGuiCoordCross						m_GuiSearchResultFitPoint2[MAXPTSNUM];
	
};

