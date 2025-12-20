#pragma once

#include "BaseSearchTool.h"
#include "svQuickSearch.h"
#include "svGuiAffineRect.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svGuiContour.h"


#pragma pack(push,8)

#define  MAXAREATOOLNUM 6

// AreaInspectPick定位工具训练Data参数类
typedef struct tagAreaInspectPickTrainDataParam : public CBaseTrainDataParam
{
	

	std::vector<double>     m_dMinMean; //最小亮度
	std::vector<double>     m_dMaxMean; //最大亮度

	std::vector<double>     m_dMinStdDev; //最小方差
	std::vector<double>     m_dMaxStdDev; //最大方差

	std::vector<BOOL>       m_bMeanInvalid;  //是否使用亮度检测
	std::vector<BOOL>       m_bStdDevInvalid;  //是否使用方差检测

	std::vector<BOOL>       m_bMeanInvert;     //亮度检测结果取反
	std::vector<BOOL>       m_bStdDevInvert;   //方差检测结果取反

	int                     m_nInsAreaNum;   //检测区域个数
	int                     m_nLogicIndex;   //多结果逻辑运算序号

	int m_nColNum;
	int m_nRowNum;

	std::vector<CString>    m_strPositionToolName;
	


	tagAreaInspectPickTrainDataParam()
	{
		m_nInsAreaNum = MAXAREATOOLNUM;
		m_nLogicIndex = 0;         //与运算

		m_nColNum = 1;
		m_nRowNum = 1;
	
		for (int i=0;i<MAXAREATOOLNUM;i++)
		{
// 			m_dCurMean.push_back(-1.0);
// 			m_dCurStdDev.push_back(-1.0);

			m_dMinMean.push_back(0.0);
			m_dMaxMean.push_back(255.0);
			m_dMinStdDev.push_back(0.0);
			m_dMaxStdDev.push_back(255.0);

			m_bMeanInvalid.push_back(TRUE);
			m_bStdDevInvalid.push_back(TRUE);

			m_bMeanInvert.push_back(FALSE);
			m_bStdDevInvert.push_back(FALSE);

          m_strPositionToolName.push_back(_T("")) ;
		}


	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eAreaInspectPickTool;
	}

}CAreaInspectPickTrainDataParam;

// AreaInspectPick定位工具训练Gui参数类
typedef struct tagAreaInspectPickTrainGuiParam : public CBaseTrainGuiParam
{
	std::vector<scRect*>			m_SearchRect;						// 检测区域
	std::vector<COLORREF>        m_LineColor;						// 图形颜色
	std::vector<int>				m_nLineWidth;						// 图形线宽
	std::vector<BOOL>			m_bSearchRectVisible;				// 检测区域是否可见
	scRect			m_Hand1SearchRect;						//抓头1搜索区域
	scRect			m_Hand2SearchRect;						// 抓头2搜索区域
	tagAreaInspectPickTrainGuiParam()
	{
		for (int i=0;i<MAXAREATOOLNUM;i++)
		{ 
			scRect* tempRect = new scRect;
			m_SearchRect.push_back(tempRect);
			m_LineColor.push_back(RGB(0, 0, 255));
			m_nLineWidth.push_back(1);
			m_bSearchRectVisible.push_back(TRUE);
		}
		
	}

	~tagAreaInspectPickTrainGuiParam()
	{
		for (int i=0;i<m_SearchRect.size();i++)
		{
			if (m_SearchRect.at(i)!=NULL)
			{
				delete m_SearchRect.at(i);
				m_SearchRect.at(i)= NULL;
			}
		}
		m_SearchRect.clear();
	}

	tagAreaInspectPickTrainGuiParam& operator=(tagAreaInspectPickTrainGuiParam& cAreaInspectPickToolTrainGuiParam)
	{
		for (int i=0;i<m_SearchRect.size();i++)
		{
			if (m_SearchRect.at(i)!=NULL)
			{
				delete m_SearchRect.at(i);
				m_SearchRect.at(i)= NULL;
			}
		}
		m_SearchRect.clear();

		for(int i=0;i<cAreaInspectPickToolTrainGuiParam.m_SearchRect.size();i++)
		{
			scRect* tempRect = new scRect;
			if(cAreaInspectPickToolTrainGuiParam.m_SearchRect[i])
			{
				*tempRect = *cAreaInspectPickToolTrainGuiParam.m_SearchRect[i];
			}else
			{
				tempRect->Set(250+i*100, 200, 50, 50);
			}
			m_SearchRect.push_back(tempRect);
		}
		m_LineColor = cAreaInspectPickToolTrainGuiParam.m_LineColor;						// 图形颜色
		m_nLineWidth = cAreaInspectPickToolTrainGuiParam.m_nLineWidth;					// 图形线宽
		m_bSearchRectVisible = cAreaInspectPickToolTrainGuiParam.m_bSearchRectVisible;	// 检测区域是否可见
		m_Hand1SearchRect=cAreaInspectPickToolTrainGuiParam.m_Hand1SearchRect;
		m_Hand2SearchRect=cAreaInspectPickToolTrainGuiParam.m_Hand2SearchRect;

		return *this;
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eAreaInspectPickTool;
	}
}CAreaInspectPickTrainGuiParam;

// AreaInspectPick定位工具搜索Data参数类
typedef struct tagAreaInspectPickSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double  m_dAngleLow;								// 低角度
	double	m_dAngleHigh;								// 高角度
	tagAreaInspectPickSearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.8;
		m_dAngleLow		= -5.0;
		m_dAngleHigh	= 5.0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eAreaInspectPickTool;
	}

}CAreaInspectPickSearchDataParam;

// AreaInspectPick定位工具搜索Gui参数类
typedef struct tagAreaInspectPickSearchGuiParam : public CBaseSearchGuiParam
{
	std::vector<COLORREF>        m_LineColor;						// 图形颜色
	std::vector<int>				m_nLineWidth;						// 图形线宽
	std::vector<BOOL>			m_bAffineRectVisible;				// 搜索结果矩形可见
	std::vector<BOOL>			m_bModelPointVisible;				// 参考点是够可见
	tagAreaInspectPickSearchGuiParam()
	{
		for (int i=0;i<MAXAREATOOLNUM;i++)
		{
			m_LineColor.push_back(RGB(0, 0, 255));
			m_nLineWidth.push_back(1);
			m_bAffineRectVisible.push_back(TRUE);
			m_bModelPointVisible.push_back(TRUE);
		}

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eAreaInspectPickTool;
	}

}CAreaInspectPickSearchGuiParam;

// AreaInspectPick训练结果
typedef struct tagAreaInspectPickTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y

	tagAreaInspectPickTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eAreaInspectPickTool;
	}
}CAreaInspectPickTrainResult;

//
//	AreaInspectPick定位工具
//
class /*SV_SEARCHTOOL_API*/ CAreaInspectPickTool : public CBaseSearchTool  
{
public:
	CAreaInspectPickTool();
	CAreaInspectPickTool(CAreaInspectPickTool &cAreaInspectPickTool);
	CAreaInspectPickTool& operator=(CAreaInspectPickTool &cAreaInspectPickTool);
	virtual ~CAreaInspectPickTool();
public:
	bool	m_bInspectFinish;
	bool	m_bInspectOK;

// 	double	m_dDefectArea;
// 	int 	m_nMaxGreyVal;
	std::vector<double>  	m_dCurMean;   //当前亮度（平均值）
	std::vector<double>     m_dCurStdDev; //当前均匀度（方差）
	std::vector<double>     m_dCurDefArea; //当前缺陷面积（方差）
	//scGuiContour m_GuiContour;                 // Blob交互控件
public:

	//////////////////////////工具属性相关////////////////////////////////
	virtual CSearchToolType GetSearchToolType();										    // 获取工具类型
	virtual void SetSearchToolName(CString strName);									    // 设置工具名称
	virtual CString GetSearchToolName();												    // 获取工具名称

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

	virtual bool ApplyStatisticData();
	virtual bool ResetStatisticData();

	///////////////////////////搜索相关/////////////////////////////////////	
	virtual bool SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam);				// 设置搜索Data参数
	virtual bool GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam);				// 获取搜索Data参数
	virtual bool SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam);					// 设置搜索Gui参数
	virtual bool GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam);					// 获取搜索Gui参数
	virtual bool Search(cpImage &ImageSearch);										// 执行搜索 
	virtual bool SearchStatistic(cpImage &ImageSearch);								// 执行统计搜索 
	virtual int  GetResultNum();															// 获取搜索结果个数
	virtual bool GetResult(int nIndex, CSearchResult &searchResult);						// 获取搜索结果
	virtual bool RestoreSearchParam();														// 恢复搜索参数	

	/////////////////////////参数加载与保存////////////////////////////////	
	virtual bool LoadModelFromFile(CString strDir);											// 加载模板文件, strDir为文件夹路径
	virtual bool SaveModelToFile(CString strDir);											// 保存模板文件, strDir为文件夹路径
	virtual bool LoadModelTrainPatternImage(CString strDir , cpImage &ImagePattern);		// 加载模板训区域图片， strDir为文件夹路径,ImagePattern为获取得到的训练区域图片

	virtual void	SetLanguage(int nLanguage);									// 设置中英文菜单 0：eng；1：chs
	//////////////////////错误信息///////////////////////////////////////
	virtual CString	GetResultErrInfo();
	BOOL	PointInRect(sc2Vector point,scRect rect);

public:
	bool SaveTrainGuiData();		
	virtual void  ResizeToolsAllParam(int nNewToolNum);
private:

	/////////////////////////Gui控件相关////////////////////////////////	
	bool UpdateTrainGuiData(bool bSaveAndValidate);											// 更新训练控件数据信息	
	bool UpdateSearchGuiData(bool bSaveAndValidate);										// 更新搜索结果控件数据信息	
	bool ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray);					// 清除静态控件及其显示													
	bool UpdateTrainGuiDisplay();															// 更新训练Gui控件显示
	bool UpdateSearchResultGuiDisplay();													// 更新搜索Gui控件显示
	bool ClearTrainGui();																	// 清除定位工具在Display上的训练控件
	bool ClearResultGui();																	// 清除定位工具在Display上的结果控件
	bool ClearTrainVectorGui();																// 清除vector中的Gui

private:
	CString m_strName;

	scQuickSearchTool                   m_AreaInspectTool;				// AreaInspectPick搜索工具
	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CAreaInspectPickTrainDataParam			m_tmpTrainDataParam;			// 当前训练Data参数
	CAreaInspectPickTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CAreaInspectPickSearchDataParam			m_tmpSearchDataParam;			// 当前搜索Data参数	
	CAreaInspectPickSearchGuiParam			m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CAreaInspectPickTrainResult				m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
	CAreaInspectPickTrainDataParam			m_TrainDataParam;				// 训练Data参数
	CAreaInspectPickTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CAreaInspectPickSearchDataParam			m_SearchDataParam;				// 搜索Data参数	
	CAreaInspectPickSearchGuiParam			m_SearchGuiParam;				// 搜索Gui参数
	CAreaInspectPickTrainResult				m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
 	cpImage							m_ImageTrainResult;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功
	
	// 训练GUI
	std::vector<scGuiAffineRect*>                     m_GuiTrainRect;					// 训练矩形			
	std::vector<scGuiRect*>							m_GuiSearchRect;				// 搜索矩形
	std::vector<scGuiCross*>                         m_GuiModelPoint;				// 参考点
	scGuiRect							m_GuiHand1SearchRect;				//吸头1 搜索矩形
	scGuiRect							m_GuiHand2SearchRect;				//吸头2搜索矩形
	
	
	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchResultRectArray;	// 搜索结果矩形
	scGuiRect							m_GuiSearchResultRect[100];
};

#pragma pack(pop)