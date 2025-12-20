// PatCornerSearchTool.h: interface for the cTwoPatModelSearchTool class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseSearchTool.h"
#include "svQuickSearch.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svPatternSearchRot.h"
#include "svGuiAffineRect.h"
#include "svGuiLine.h"
#include "svGuiLineSeg.h"
#include "svGuiFeaturelet.h"
#pragma pack(push,8)

// PatCornerSearch定位工具训练Data参数类
typedef struct tagTwoQuickModelSearchTrainDataParam : public CBaseTrainDataParam
{
	// 几何1
	double	m_dGrau;									// 颗粒度
	long	m_lNoiseThre;								// 噪声阈值
	long	m_lTotalMag;								// 幅值和
	bool	m_bMaskEnable;								// 是否设置掩模图像


	// 几何2
	double	m_dGrau2;									// 颗粒度
	long	m_lNoiseThre2;								// 噪声阈值
	long	m_lTotalMag2;								// 幅值和
	bool	m_bMaskEnable2;								// 是否设置掩模图像

	tagTwoQuickModelSearchTrainDataParam()
	{
		m_dGrau			= 2.0;
		m_lNoiseThre	= 10;
		m_lTotalMag		= 200;
		m_bMaskEnable	= false;
		m_dGrau2			= 2.0;
		m_lNoiseThre2	= 10;
		m_lTotalMag2		= 200;
		m_bMaskEnable2	= false;

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eTwoQuickModelSearchTool;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		CString strInfo = _T("");
		// ImageChannel
		strInfo.Format(_T("%d"), m_nImageChannel);
		cXMLConfigurator.AddElem(_T("ImageChannel"), strInfo);
		// grau
		strInfo.Format(_T("%f"), m_dGrau);
		cXMLConfigurator.AddElem(_T("Grau"), strInfo);
		// noise thre
		strInfo.Format(_T("%d"), m_lNoiseThre);
		cXMLConfigurator.AddElem(_T("NoiseThre"), strInfo);
		// total mag
		strInfo.Format(_T("%d"), m_lTotalMag);
		cXMLConfigurator.AddElem(_T("TotalMag"), strInfo);
		// mask enable
		strInfo.Format(_T("%d"), m_bMaskEnable);
		cXMLConfigurator.AddElem(_T("MaskEnable2"), strInfo);

		// grau2
		strInfo.Format(_T("%f"), m_dGrau2);
		cXMLConfigurator.AddElem(_T("Grau2"), strInfo);
		// noise thre2
		strInfo.Format(_T("%d"), m_lNoiseThre2);
		cXMLConfigurator.AddElem(_T("NoiseThre2"), strInfo);
		// total mag2
		strInfo.Format(_T("%d"), m_lTotalMag2);
		cXMLConfigurator.AddElem(_T("TotalMag2"), strInfo);
		// mask enable2
		strInfo.Format(_T("%d"), m_bMaskEnable2);
		cXMLConfigurator.AddElem(_T("MaskEnable22"), strInfo);
	}

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		bool bRet = false;
		// coarse scale
		// ImageChannel
		bRet = cXMLConfigurator.FindElem(_T("ImageChannel"));				
		if (!bRet)
		{
			m_nImageChannel = 0;
		}
		else
		{
			m_nImageChannel = _ttol(cXMLConfigurator.GetElemData());
		}
		// grau
		bRet = cXMLConfigurator.FindElem(_T("Grau"));
		if (bRet)
		{
			m_dGrau = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		// noise thre
		bRet = cXMLConfigurator.FindElem(_T("NoiseThre"));
		if (bRet)
		{
			m_lNoiseThre = _ttol(cXMLConfigurator.GetElemData());
		}

		// total mag
		bRet = cXMLConfigurator.FindElem(_T("TotalMag"));
		if (bRet)
		{
			m_lTotalMag = _ttol(cXMLConfigurator.GetElemData());
		}
		// mask enable
		bRet = cXMLConfigurator.FindElem(_T("MaskEnable2"));
		if (bRet)
		{
			m_bMaskEnable = _ttoi(cXMLConfigurator.GetElemData());
		}

		// grau2
		bRet = cXMLConfigurator.FindElem(_T("Grau2"));
		if (bRet)
		{
			m_dGrau2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		// noise thre2
		bRet = cXMLConfigurator.FindElem(_T("NoiseThre2"));
		if (bRet)
		{
			m_lNoiseThre2 = _ttol(cXMLConfigurator.GetElemData());
		}

		// total mag2
		bRet = cXMLConfigurator.FindElem(_T("TotalMag2"));
		if (bRet)
		{
			m_lTotalMag2 = _ttol(cXMLConfigurator.GetElemData());
		}
		// mask enable2
		bRet = cXMLConfigurator.FindElem(_T("MaskEnable22"));
		if (bRet)
		{
			m_bMaskEnable2 = _ttoi(cXMLConfigurator.GetElemData());
		}
	}


}CTwoQuickModelSearchTrainDataParam;

// PatCornerSearch定位工具训练Gui参数类
typedef struct tagTwoQuickModelSearchTrainGuiParam : public CBaseTrainGuiParam
{
	scAffineRect	m_TrainAffineRect;					// 训练区域
	sc2Vector		m_ModelPoint;						// 参考点
	scRect			m_SearchRect;						// 搜索区域


	scAffineRect	m_TrainAffineRect2;					// 训练区域2
	sc2Vector		m_ModelPoint2;						// 参考点
	scRect			m_SearchRect2;						// 搜索区域


	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bTrainAffineRectVisible;				// 搜索区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	tagTwoQuickModelSearchTrainGuiParam()
	{
		m_LineColor			 = RGB(0, 0, 255);
		m_nLineWidth		 = 1;
		m_bModelPointVisible = true;
		m_bTrainAffineRectVisible	 = true;
		m_bSearchRectVisible = true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eTwoQuickModelSearchTool;
	}

}CTwoQuickModelSearchTrainGuiParam;

// PatCornerSearch定位工具搜索Data参数类
typedef struct tagTwoQuickModelSearchSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double	m_dAcceptThre2;								// 接收阈值

	double  m_dAngleLow;								// 低角度
	double	m_dAngleHigh;								// 高角度
	double  m_dSizeLow;
	double  m_dSizeHigh;

	double  m_dAngleLow1;								// 低角度
	double	m_dAngleHigh1;								// 高角度
	double  m_dSizeLow1;
	double  m_dSizeHigh1;


	double	m_dPixelRatio;								// 像素当量
	bool	m_bEnableSizeCheck;							// 启用尺寸检查
	bool	m_bEnableMax;								// 是否启用最大值判断
	bool	m_bEnableMin;								// 是否启用最小值判断
	double	m_dDistanceMax;								// 距离最大值（毫米）
	double	m_dDistanceMin;								// 距离最小值（毫米）

	tagTwoQuickModelSearchSearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.8;
		m_dAcceptThre2	= 0.8;

		m_dAngleLow		= -5.0;
		m_dAngleHigh	= 5.0;
		m_dSizeLow		= 1.0;
		m_dSizeHigh		= 1.0;

		m_dAngleLow1	= -5.0;
		m_dAngleHigh1	= 5.0;
		m_dSizeLow1		= 1.0;
		m_dSizeHigh1	= 1.0;

		m_dPixelRatio = 0.01;
		m_bEnableSizeCheck = false;
		m_bEnableMax = false;
		m_bEnableMin = false;
		m_dDistanceMax = 1.0;
		m_dDistanceMin = 0.0;

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eTwoQuickModelSearchTool;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		CString strInfo = _T("");
		strInfo.Format(_T("%d"), m_nSearchNum);
		cXMLConfigurator.AddElem(_T("SearchNum"), strInfo);


		strInfo.Format(_T("%f"), m_dAcceptThre);
		cXMLConfigurator.AddElem(_T("AcceptThre"), strInfo);

		strInfo.Format(_T("%f"), m_dAcceptThre2);
		cXMLConfigurator.AddElem(_T("AcceptThre2"), strInfo);

		strInfo.Format(_T("%f"), m_dAngleLow);
		cXMLConfigurator.AddElem(_T("AngleLow"), strInfo);
		strInfo.Format(_T("%f"), m_dAngleHigh);
		cXMLConfigurator.AddElem(_T("AngleHigh"), strInfo);

		strInfo.Format(_T("%f"), m_dSizeLow);
		cXMLConfigurator.AddElem(_T("SizeLow"), strInfo);
		strInfo.Format(_T("%f"), m_dSizeHigh);
		cXMLConfigurator.AddElem(_T("SizeHigh"), strInfo);

		// m_bEnableSizeCheck
		strInfo.Format(_T("%d"), m_bEnableSizeCheck);
		cXMLConfigurator.AddElem(_T("EnableSizeCheck"), strInfo);

		// m_dPixelRatio
		strInfo.Format(_T("%f"), m_dPixelRatio);
		cXMLConfigurator.AddElem(_T("dPixelRatio"), strInfo);

		// m_bEnableMax
		strInfo.Format(_T("%d"), m_bEnableMax);
		cXMLConfigurator.AddElem(_T("bEnableMax"), strInfo);

		// m_bEnableMin
		strInfo.Format(_T("%d"), m_bEnableMin);
		cXMLConfigurator.AddElem(_T("bEnableMin"), strInfo);

		// m_dDistanceMax
		strInfo.Format(_T("%f"), m_dDistanceMax);
		cXMLConfigurator.AddElem(_T("dDistanceMax"), strInfo);

		// m_dDistanceMin
		strInfo.Format(_T("%f"), m_dDistanceMin);
		cXMLConfigurator.AddElem(_T("dDistanceMin"), strInfo);
	}

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		bool bRet = false;
		bRet = cXMLConfigurator.FindElem(_T("SearchNum"));
		if (bRet)
		{
			m_nSearchNum = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("AcceptThre"));
		if (bRet)
		{
			m_dAcceptThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("AngleLow"));
		if (bRet)
		{
			m_dAngleLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("AngleHigh"));
		if (bRet)
		{
			m_dAngleHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}


		bRet = cXMLConfigurator.FindElem(_T("SizeLow"));
		if (bRet)
		{
			m_dSizeLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		else
		{
			m_dSizeLow = 1.0;
		}

		bRet = cXMLConfigurator.FindElem(_T("SizeHigh"));
		if (bRet)
		{
			m_dSizeHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		else
		{
			m_dSizeHigh = 1.0;
		}

		// m_bEnableSizeCheck
		bRet = cXMLConfigurator.FindElem(_T("EnableSizeCheck"));
		if (bRet)
		{
			m_bEnableSizeCheck = _ttoi(cXMLConfigurator.GetElemData());
		}

		// m_dPixelRatio
		bRet = cXMLConfigurator.FindElem(_T("dPixelRatio"));
		if (bRet)
		{
			m_dPixelRatio = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}


		// m_bEnableMax
		bRet = cXMLConfigurator.FindElem(_T("bEnableMax"));
		if (bRet)
		{
			m_bEnableMax = _ttoi(cXMLConfigurator.GetElemData());
		}


		// m_bEnableMin
		bRet = cXMLConfigurator.FindElem(_T("bEnableMin"));
		if (bRet)
		{
			m_bEnableMin = _ttoi(cXMLConfigurator.GetElemData());
		}

		// m_dDistanceMax
		bRet = cXMLConfigurator.FindElem(_T("dDistanceMax"));
		if (bRet)
		{
			m_dDistanceMax = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		// m_dDistanceMin
		bRet = cXMLConfigurator.FindElem(_T("dDistanceMin"));
		if (bRet)
		{
			m_dDistanceMin = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
	}


}CTwoQuickModelSearchSearchDataParam;

// PatCornerSearch定位工具搜索Gui参数类
typedef struct tagTwoQuickModelSearchGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bAffineRectVisible;				// 搜索结果矩形可见
	bool			m_bModelPointVisible;				// 搜索结果矩形可见

	bool			m_bLineVisible;						// 找线工具是否可见
	bool			m_bLineCrossVisible;				// 找线工具是否可见
	tagTwoQuickModelSearchGuiParam()
	{
		m_LineColor		= RGB(0, 255, 0);
		m_nLineWidth	= 1;
		m_bAffineRectVisible	= true;
		m_bModelPointVisible = true;
		m_bLineVisible = true;
		m_bLineCrossVisible = true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eTwoQuickModelSearchTool;
	}

}CTwoQuickModelSearchSearchGuiParam;

// QuickSearch训练结果
typedef struct tagTwoQuickModelSearchTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y

	double m_dModelX1; // 参考点X					
	double m_dModelY1; // 参考点Y

	double m_dModelX2; // 参考点X					
	double m_dModelY2; // 参考点Y

	double m_dModelX3; // 参考点X					
	double m_dModelY3; // 参考点Y

	tagTwoQuickModelSearchTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;

		m_dModelX1 = 0;
		m_dModelY1 = 0;

		m_dModelX2 = 0;
		m_dModelY2 = 0;

		m_dModelX3 = 0;
		m_dModelY3 = 0;
	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eTwoQuickModelSearchTool;
	}
}CTwoQuickModelSearchTrainResult;

//
// PatCornerSearch定位工具
//
class /*SV_SEARCHTOOL_API*/ CTwoQuickModelSearchTool : public CBaseSearchTool    
{
public:
	CTwoQuickModelSearchTool();
	CTwoQuickModelSearchTool(CTwoQuickModelSearchTool &cTwoQuickModelSearchTool);
	CTwoQuickModelSearchTool& operator=(CTwoQuickModelSearchTool &cTwoQuickModelSearchTool);
	virtual ~CTwoQuickModelSearchTool();

public:
	
	//////////////////////////工具属性相关////////////////////////////////
	virtual CSearchToolType GetSearchToolType();										    // 获取工具类型
	
	//////////////////////////显示更新相关////////////////////////////////
	virtual bool SetDisplay(scGuiDisplay *pGuiDisplay);										// 设置显示控件指针				
	virtual scGuiDisplay* GetDisplay();
	virtual bool SetTrainResultDisplay(scGuiDisplay *pGuiDisplay);							// 设置训练结果显示控件指针
	virtual bool SetTrainResultDisplay2(scGuiDisplay *pGuiDisplay);							// 设置训练结果显示控件指针
	virtual scGuiDisplay* GetTrainResultDisplay();											// 获取训练结果显示控件指针														// 获取显示空间指针
	virtual scGuiDisplay* GetTrainResultDisplay2();											// 获取训练结果显示控件指针
	virtual bool SetGuiKeyText(CString strGuiKeyText);										// 设置Gui图形的关键字
	virtual bool GetGuiKeyText(CString &strGuiKeyText);										// 获取Gui图形的关键字
	virtual bool ShowTrainGui(bool bShow) ;													// 是否显示训练界面
	virtual bool ShowTrainResultGui(bool bShow) ;											// 是否显示训练结果界面
	virtual bool ShowSearchResultGui(bool bShow);											// 是否显示搜索界面
	virtual bool UpdateGuiDisplay();														// 更新界面显示
	virtual bool ClearAllGui();															    // 清除定位工具在Display上的控件
	virtual bool ClearResult();																// 清除搜索结果
	virtual bool SetGuiByCam(double nWidth,double nHeight);									// 设置根据相机设置GUI大小

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
	virtual bool GetMaskImage2(cpImage &ImageMask);											// 获取训练原始图像

	virtual bool Train();																	// 执行训练
	virtual bool IsTrained();																// 是否训练成功
	virtual bool GetTrainResultImage(cpImage &ImageTrainResult);							// 获取训练结果图像
	virtual bool GetTrainResultImage2(cpImage &ImageTrainResult);							// 获取训练结果图像
	virtual bool GetTrainResult(CBaseTrainResult *pTrainResult);							// 获取训练结果
	virtual bool GetPatternImage(cpImage &ImagePattern);									// 获取训练模板原始图像
	virtual bool GetPatternImage2(cpImage &ImagePattern);									// 获取训练模板原始图像

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
	//////////////////////语言类型//////////////////////////////////////
	virtual void	SetLanguage(int nLanguage);									// 设置中英文菜单 0：eng；1：chs
	virtual CString	GetResultErrInfo();											// 获取错误序列号

	virtual bool LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern);			// 加载模板训区域图片， strDir为文件夹路径,ImagePattern为获取得到的训练区域图片

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
	scQuickSearchTool					m_QuickSearchTool;			// QuickSearchTool搜索工具
	scQuickSearchTool					m_QuickSearchTool2;			// QuickSearchTool搜索工具

	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd;			// 显示训练结果对象句柄
	
	scGuiDisplay					   *m_pGuiTrainResultDisplay2;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd2;			// 显示训练结果对象句柄
	
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CTwoQuickModelSearchTrainDataParam		m_tmpTrainDataParam;			// 当前训练Data参数
	CTwoQuickModelSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CTwoQuickModelSearchSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CTwoQuickModelSearchSearchGuiParam		m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CTwoQuickModelSearchTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
	cpImage							m_tmpImageMask2;					// 当前掩模图像
	CTwoQuickModelSearchTrainDataParam		m_TrainDataParam;				// 训练Data参数
	CTwoQuickModelSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CTwoQuickModelSearchSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CTwoQuickModelSearchSearchGuiParam		m_SearchGuiParam;				// 搜索Gui参数	
	CTwoQuickModelSearchTrainResult			m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
	cpImage							m_ImageTrainResult;				// 训练结果图像
	cpImage							m_ImageMask2;					// 掩模图像
	cpImage							m_ImageTrainResult2;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功

	// 训练GUI
	scGuiAffineRect						m_GuiTrainRect;					// 训练矩形			
	scGuiRect							m_GuiSearchRect;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint;				// 参考点
	scGuiFeaturelet						m_GuiTrainFeaturelet;			// 训练特征链集
	
	
	scGuiAffineRect						m_GuiTrainRect2;				// 训练矩形			
	scGuiRect							m_GuiSearchRect2;				// 搜索矩形
	scGuiCross                          m_GuiModelPoint2;				// 参考点
	scGuiFeaturelet						m_GuiTrainFeaturelet2;			// 训练特征链集
	
	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchAffineRectArray;			// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;			// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点	

	
	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM];
	scGuiCross							m_GuiSearchCross[MAXRESNUM*2];
	scGuiLine							m_GuiSearchLine[MAXRESNUM];
	scGuiAffineRect						m_GuiSearchAffineRect2[MAXRESNUM];
	scGuiCross							m_GuiSearchCross2[MAXRESNUM*2];
	scGuiLineSeg						m_GuiSearchLineSeg2[MAXRESNUM];
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];

};
#pragma pack(pop)

