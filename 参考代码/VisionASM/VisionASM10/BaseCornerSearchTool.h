// BaseCornerSearchTool.h: interface for the CBaseCornerSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "BaseSearchTool.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svPatternSearchRot.h"
#include "svGuiAffineRect.h"
#include "svGuiCross.h"
#include "svFind.h"
#include "svGuiFindLine.h" 
#include "svGuiLine.h"

#define LINE_NUM 2

// BaseCornerSearch定位工具训练Data参数类
typedef struct tagBaseCornerSearchTrainDataParam : public CBaseTrainDataParam
{
	double			m_dContrastThre[LINE_NUM] ;			// 对比度阈值
	long			m_lFilterHalfWidth[LINE_NUM];		// 滤波器半宽
	int				m_nOutLineNum[LINE_NUM];			// 局外点
	EdgePolarity	m_edgeProperty[LINE_NUM];			// 边缘属性
	bool			m_bSortByScore[LINE_NUM];			// 按分数排序
	BOOL			m_bEnableDualEdge[LINE_NUM];		// 启用/禁用找线工具双边缘
	tagBaseCornerSearchTrainDataParam()
	{
		for (int i=0; i<LINE_NUM; i++)
		{
			m_dContrastThre[i]		= 1.0;
			m_lFilterHalfWidth[i]	= 1;
			m_nOutLineNum[i]		= 0;
			m_edgeProperty[i]		= eDontCare;
			m_bSortByScore[i]		= true;
			m_bEnableDualEdge[i]	= false;
		}

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eBaseCornerSearchTool;
	}

}CBaseCornerSearchTrainDataParam;

// BaseCornerSearch定位工具训练Gui参数类
typedef struct tagBaseCornerSearchTrainGuiParam : public CBaseTrainGuiParam
{
	// 找线工具1
	int				m_nCaliperNum[LINE_NUM];			// 卡尺个数
	double			m_dStartX[LINE_NUM];				// 起始点X
	double			m_dStartY[LINE_NUM];				// 起始点Y
	double			m_dEndX[LINE_NUM];					// 终止点X
	double			m_dEndY[LINE_NUM];					// 终止点Y
	double			m_dCaliperProjectLen[LINE_NUM];		// 投影长度
	double			m_dCaliperSearchLen[LINE_NUM];		// 搜索长度
	scRadian		m_CaliperSearchRotation[LINE_NUM];	// 搜索角度
	bool			m_bCaliperVisible[LINE_NUM];		//卡尺是否可见
	// 找线工具掩摸
	svStd vector<double> m_vdCaliperMaskData[LINE_NUM];	// 掩膜数据
	svStd vector<bool>   m_vbCaliperMask[LINE_NUM];		// 卡尺是否被掩膜
	bool			     m_bEnableLineMask[LINE_NUM];	// 是否启用掩膜

	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽

	// 颜色与可见性
// 	COLORREF        m_LineColor;						// 图形颜色
// 	int				m_nLineWidth;						// 图形线宽
//	bool			m_bTrainAffineRectVisible;			// 训练区域是否可见
//	bool			m_bModelPointVisible;				// 模板参考点是否可见
//	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	bool			m_bLineVisible;						// 找线工具是否可见


	tagBaseCornerSearchTrainGuiParam()
	{
		m_nCaliperNum[0]				= 2;
		m_dStartX[0]					= 100.0;
		m_dStartY[0]					= 100.0;
		m_dEndX[0]						= 100.0;
		m_dEndY[0]						= 100.0;
		m_dCaliperProjectLen[0]			= 20.0;
		m_dCaliperSearchLen[0]			= 100.0;
		m_CaliperSearchRotation[0]		= scRadian(skPI/2.0);
		m_bCaliperVisible[0]			= false;
		m_bEnableLineMask[0]			= false;

		m_nCaliperNum[1]				= 2;
		m_dStartX[1]					= 100.0;
		m_dStartY[1]					= 100.0;
		m_dEndX[1]						= 100.0;
		m_dEndY[1]						= 100.0;
		m_dCaliperProjectLen[1]			= 20.0;
		m_dCaliperSearchLen[1]			= 100.0;
		m_CaliperSearchRotation[1]		= scRadian(skPI/2.0);
		m_bCaliperVisible[1]			= false;
		m_bEnableLineMask[1]			= false;

		m_LineColor			 = RGB(0, 0, 255);
		m_nLineWidth		 = 1;
		m_bLineVisible		  = true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eBaseCornerSearchTool;
	}

}CBaseCornerSearchTrainGuiParam;

// BaseCornerSearch定位工具搜索Data参数类
typedef struct tagBaseCornerSearchSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	tagBaseCornerSearchSearchDataParam()
	{
		m_nSearchNum	= 1;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eBaseCornerSearchTool;
	}

}CBaseCornerSearchSearchDataParam;

// BaseCornerSearch定位工具搜索Gui参数类
typedef struct tagBaseCornerSearchGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽

//	COLORREF        m_LineColor;						// 图形颜色
//	int				m_nLineWidth;						// 图形线宽
//	bool			m_bAffineRectVisible;				// 搜索结果矩形可见
//	bool			m_bModelPointVisible;				// 参考点是否可见
	bool			m_bLineVisible;						// 找线工具是否可见
	bool			m_bLineCrossVisible;				// 找线工具是否可见

	tagBaseCornerSearchGuiParam()
	{
		m_LineColor		= RGB(0, 255, 0);
		m_nLineWidth	= 1;

		m_bLineVisible			= true;
		m_bLineCrossVisible		= true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eBaseCornerSearchTool;
	}

}CBaseCornerSearchSearchGuiParam;

// PatternSearch训练结果
typedef struct tagBaseCornerSearchTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y

	double m_dStartX[LINE_NUM]; // 线段起点X
	double m_dStartY[LINE_NUM]; // 线段起点Y
	double m_dEndX[LINE_NUM];	  // 线段终点X
	double m_dEndY[LINE_NUM];	  // 线段终点Y

	tagBaseCornerSearchTrainResult()
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
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eBaseCornerSearchTool;
	}
}CBaseCornerSearchTrainResult;

class CBaseCornerSearchTool :public CBaseSearchTool
{
public:
	CBaseCornerSearchTool(void);
	CBaseCornerSearchTool(CBaseCornerSearchTool &cBaseCornerSearchTool);
	CBaseCornerSearchTool& operator=(CBaseCornerSearchTool &cBaseCornerSearchTool);
	~CBaseCornerSearchTool(void);

public:
// 
// 	//////////////////////////工具属性相关////////////////////////////////
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
	scFindLine							m_FindLineTool[LINE_NUM];		// 找线工具
	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CBaseCornerSearchTrainDataParam		m_tmpTrainDataParam;			// 当前训练Data参数
	CBaseCornerSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CBaseCornerSearchSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CBaseCornerSearchSearchGuiParam		m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CBaseCornerSearchTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	CBaseCornerSearchTrainDataParam		m_TrainDataParam;				// 训练Data参数
	CBaseCornerSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CBaseCornerSearchSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CBaseCornerSearchSearchGuiParam		m_SearchGuiParam;				// 搜索Gui参数	
	CBaseCornerSearchTrainResult			m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;			// 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功

	// 训练GUI
	scGuiFindLine                       m_GuiFindLine[LINE_NUM];		// 两个找线工具
	scGuiLine                           m_GuiTrainResultLine[LINE_NUM];	// 训练结果线段
	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;			// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点	


	scGuiLine							m_GuiSearchLine[MAXRESNUM*2];
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];

	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽
};

