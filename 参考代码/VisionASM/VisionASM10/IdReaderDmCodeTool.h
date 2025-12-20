// IdReaderDmCodeTool.h: interface for the CIdReaderDmCodeTool class.
//
//////////////////////////////////////////////////////////////////////


#pragma once
#include "BaseSearchTool.h"
#include "svGuiRect.h"
#include "svGuiText.h"
#include "svGuiContour.h"
#include "svAffsampl.h"
#define DmCodeTool_MaxResultNum 20
#pragma pack(push,8)

// IdReaderDmCode定位工具训练Data参数类
typedef struct tagIdReaderDmCodeTrainDataParam : public CBaseTrainDataParam
{

	FindDMCodePara	m_FindDMCodePara;		// 读码参数
	CoarseScale		m_CoarseScale;			// 采样缩放系数
	FindQRCodePara  m_FindQRCodePara;		// 读码参数
	
	int m_nCodeType;						// 读码类型 0-DM；1-QR

	tagIdReaderDmCodeTrainDataParam()
	{
		m_CoarseScale = CoarseScale1;
		m_nCodeType = 0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eIdReaderDmCodeTool;
	}
	
}CIdReaderDmCodeTrainDataParam;

// IdReaderDmCode定位工具训练Gui参数类
typedef struct tagIdReaderDmCodeTrainGuiParam : public CBaseTrainGuiParam
{
	scRect    m_TrainRect;
	COLORREF  m_LineColor;
	int       m_nLineWidth;
	bool      m_bTrainRectVisible;			// 检测区域是否可见

	tagIdReaderDmCodeTrainGuiParam()
	{
		m_TrainRect = scRect(100,100,205,205);
		m_LineColor = RGB(0, 0, 255);
		m_nLineWidth = 1;
		m_bTrainRectVisible = true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eIdReaderDmCodeTool;
	}
}CIdReaderDmCodeTrainGuiParam;

// IdReaderDmCode定位工具搜索Data参数类
typedef struct tagIdReaderDmCodeSearchDataParam : public CBaseSearchDataParam
{
	
	tagIdReaderDmCodeSearchDataParam()
	{

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eIdReaderDmCodeTool;
	}
}CIdReaderDmCodeSearchDataParam;

// IdReaderDmCode定位工具搜索Gui参数类
typedef struct tagIdReaderDmCodeGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColorOK;					    // 图形OK颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bVisible;							// 搜索结果可见

	tagIdReaderDmCodeGuiParam()
	{
		m_LineColorOK = RGB(0,255,0);
		m_nLineWidth = 1;
		m_bVisible = true;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eIdReaderDmCodeTool;
	}
}CIdReaderDmCodeSearchGuiParam;

// IdReaderDmCode训练结果
typedef struct tagIdReaderDmCodeTrainResult
{

	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y

	
	tagIdReaderDmCodeTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eIdReaderDmCodeTool;
	}
}CIdReaderDmCodeTrainResult;


//
//	DM读码工具
//
class CIdReaderDmCodeTool : public CBaseSearchTool    
{
public:
	CIdReaderDmCodeTool();
	CIdReaderDmCodeTool(CIdReaderDmCodeTool &cIdReaderDmCodeTool);
	CIdReaderDmCodeTool& operator=(CIdReaderDmCodeTool &cIdReaderDmCodeTool);
	virtual ~CIdReaderDmCodeTool();

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
	virtual bool Search(cpImage &ImageSearch);												// 执行搜索 
	virtual int  GetResultNum();															// 获取搜索结果个数
	virtual bool GetResult(int nIndex, CSearchResult &searchResult);						// 获取搜索结果
	virtual bool RestoreSearchParam();														// 恢复搜索参数	

	/////////////////////////参数加载与保存////////////////////////////////	
	virtual bool LoadModelFromFile(CString strDir);											// 加载模板文件, strDir为文件夹路径
	virtual bool SaveModelToFile(CString strDir);											// 保存模板文件, strDir为文件夹路径
	//////////////////////语言类型//////////////////////////////////////
	virtual void	SetLanguage(int nLanguage);									// 设置中英文菜单 0：eng；1：chs
	virtual CString	GetResultErrInfo();											// 获取错误序列号

	void ConvertImagePostion(svStd vector<DecodeMatrixResult> &m_DMcodeResultArray,CIdReaderDmCodeTrainDataParam m_TrainDataParam);
	void ConvertImagePostion(svStd vector<DecodeQRResult> &m_QRcodeResultArray,CIdReaderDmCodeTrainDataParam m_TrainDataParam);

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
	scDMCodeTool						m_toolDMCode;					// DM读码工具

	scQRCodeTool						m_toolQRCode;					// QR读码工具

	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CIdReaderDmCodeTrainDataParam		m_tmpTrainDataParam;			// 当前训练Data参数
	CIdReaderDmCodeTrainGuiParam		m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CIdReaderDmCodeSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CIdReaderDmCodeSearchGuiParam		m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CIdReaderDmCodeTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage								m_tmpImageTrain;				// 当前训练图像
	cpImage								m_tmpImageMask;					// 当前掩模图像
	CIdReaderDmCodeTrainDataParam		m_TrainDataParam;				// 训练Data参数
	CIdReaderDmCodeTrainGuiParam		m_TrainGuiParam;				// 训练Gui参数	
	CIdReaderDmCodeSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CIdReaderDmCodeSearchGuiParam		m_SearchGuiParam;				// 搜索Gui参数
	CIdReaderDmCodeTrainResult			m_TrainResult;					// 训练结果
	cpImage								m_ImageTrain;					// 训练图像
	cpImage								m_ImageMask;					// 掩模图像
	cpImage								m_ImageTrainResult;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;			// 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功
	LOGFONT								m_LogFont;

	// 检测GUI
	scGuiRect							m_GuiTrainRect;					// 检测矩形			

	// 检测结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchResultArray;		// 搜索结果参考点

	scGuiPolyline						m_pGuiPolylineArray[DmCodeTool_MaxResultNum];
	scGuiText							m_pGuiTextArray[DmCodeTool_MaxResultNum];
	scGuiCircle							m_pGuiCircleArray[DmCodeTool_MaxResultNum][4];

	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽
	
	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽

	CString m_StrInspectInfor;
};
#pragma pack(pop)

