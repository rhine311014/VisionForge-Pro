#pragma once
#include "basesearchtool.h"
#include "svCodeCalibrate.h"
#include "svGuiPolyline.h"
#include "svPolyline.h"
#include "svGuiText.h"
#include "vsBaseAlignTool.h"
#include "svDIB.h"
// DmCodeCalibrate定位工具训练Data参数类
typedef struct tagDmCodeCalibrateTrainDataParam : public CBaseTrainDataParam
{

	tagDmCodeCalibrateTrainDataParam()
	{

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eDmCodeCalibrateTool;
	}

}CDmCodeCalibrateTrainDataParam;

// DmCodeCalibrate定位工具训练Gui参数类
typedef struct tagDmCodeCalibrateTrainGuiParam : public CBaseTrainGuiParam
{
	scRect m_DmCodeSearchRect;
	scRect m_CornersSearchRect;
	BOOL m_bDmCodeSearchRectVisible;
	BOOL m_bCornersSearchRectVisible;
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	tagDmCodeCalibrateTrainGuiParam()
	{
		m_LineColor			 = RGB(0, 0, 255);
		m_nLineWidth		 = 1;
		m_bCornersSearchRectVisible =TRUE;
		m_bDmCodeSearchRectVisible = TRUE;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eDmCodeCalibrateTool;
	}

}CDmCodeCalibrateTrainGuiParam;

// DmCodeCalibrate定位工具搜索Data参数类
typedef struct tagDmCodeCalibrateSearchDataParam : public CBaseSearchDataParam
{
	scDMCodeParam m_DmCodeSearchParam;

	tagDmCodeCalibrateSearchDataParam()
	{

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eDmCodeCalibrateTool;
	}

}CDmCodeCalibrateSearchDataParam;

// PatternSearch定位工具搜索Gui参数类
typedef struct tagDmCodeCalibrateSearchGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	BOOL            m_DmCodePolylineVisible;
	BOOL            m_bCornersCoordCrossVisible;
	tagDmCodeCalibrateSearchGuiParam()
	{
		m_LineColor		= RGB(0, 255, 0);
		m_nLineWidth	= 1;
		m_bCornersCoordCrossVisible =TRUE;
		m_DmCodePolylineVisible =TRUE;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eDmCodeCalibrateTool;
	}

}CDmCodeCalibrateSearchGuiParam;

// QuickSearch训练结果
typedef struct tagDmCodeCalibrateTrainResult
{


	tagDmCodeCalibrateTrainResult()
	{

	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eDmCodeCalibrateTool;
	}
}CDmCodeCalibrateTrainResult;


#define MAXCORNERSNUM 500
class CDmCodeCalibrateTool :
	public CBaseSearchTool
{
public:
	CDmCodeCalibrateTool(void);
	CDmCodeCalibrateTool(CDmCodeCalibrateTool& cDmCodeCalibrateTool);
	CDmCodeCalibrateTool& operator=(CDmCodeCalibrateTool& cDmCodeCalibrateTool);
	virtual ~CDmCodeCalibrateTool(void);
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
	virtual bool SetGuiByCam(double nWidth,double nHeight);																// 设置根据相机设置GUI大小

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
	virtual bool RestoreSearchParam();														// 恢复搜索参数														// 恢复搜索参数	

	/////////////////////////参数加载与保存////////////////////////////////	
	virtual bool LoadModelFromFile(CString strDir);											// 加载模板文件, strDir为文件夹路径
	virtual bool SaveModelToFile(CString strDir);											// 保存模板文件, strDir为文件夹路径
	virtual bool LoadModelTrainPatternImage(CString strDir , cpImage &ImagePattern);		// 加载模板训区域图片， strDir为文件夹路径,ImagePattern为获取得到的训练区域图片
	//////////////////////语言类型//////////////////////////////////////
	virtual void	SetLanguage(int nLanguage);									// 设置中英文菜单 0：eng；1：chs
	virtual CString GetResultErrInfo();
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
	scDMCodeCalibrate                   m_DmCodeCalibrateTool;
	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CDmCodeCalibrateTrainDataParam		m_tmpTrainDataParam;			// 当前训练Data参数
	CDmCodeCalibrateTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CDmCodeCalibrateSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CDmCodeCalibrateSearchGuiParam		m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CDmCodeCalibrateTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
	CDmCodeCalibrateTrainDataParam		m_TrainDataParam;				// 训练Data参数
	CDmCodeCalibrateTrainGuiParam		m_TrainGuiParam;				// 训练Gui参数	
	CDmCodeCalibrateSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CDmCodeCalibrateSearchGuiParam		m_SearchGuiParam;				// 搜索Gui参数	
	CDmCodeCalibrateTrainResult			m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
	cpImage							m_ImageTrainResult;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功
	CString                             m_strError;

	// 训练GUI
	scGuiRect							m_GuiDmCodeSearchRect;			// 二维码搜索矩形			
	scGuiRect							m_GuiCornersSearchRect;			// 角点搜索矩形

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchPolylineArray;			// 搜索结果多边形
	std::vector<scGuiGraphic*>          m_pGuiSearchCoordCrossArray;		// 搜索结果角点

	std::vector<sc2Vector>   m_vImgDecodeCorners;//二维码角点图像坐标
	std::vector<sc2Vector>   m_vImgDecodeCornersLabel;//二维码角点位置数字标记
	std::vector<Code2DResult> m_vArrayDMCodesResult;//二维码结果

	scGuiPolyline					    m_GuiSearchDmCodePolyline[MAXRESNUM];
	scGuiCoordCross					    m_GuiSearchCornersCoordCross[MAXCORNERSNUM];
};

