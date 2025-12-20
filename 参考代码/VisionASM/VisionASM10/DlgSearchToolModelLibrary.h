#pragma once
#include "Label.h"
#include "svGuiDisplay.h"
#include "VisionASMDlg.h"
#include "vsBaseVisionAlign.h"
#include "svGuiRect.h"

#include "svTimer.h"
#include <io.h>
#include "afxwin.h"
// CDlgSearchToolModelLibrary 对话框

//加载产品模板线程的传输参数
typedef struct tagLoadSearchToolTrainImageInfoThreadParam
{
	//private:
	vcBaseVisionAlign *m_pBaseVisionAlign;			// 当前软件信息的指针
	CString m_strPatternsDir;						// 当前模板的路径
	int m_nModelCandidateNum;						// 模板候补数量
	int m_nModelPosIndex;							// 模板位置序号
	std::vector<VpSearchTool> m_vvpSearchToolRepo;	// 工具库
	bool m_bIsLoadCacheModel;						// 是否是加载缓存模板
	bool m_bTimeOut;								// 是否初始化超时
public:



	std::vector<int>		m_vnCandidatePos;		// 存储图片对应的位置
	std::vector<CSearchToolType> m_vToolType;		// 存储图片对应的工具名称

	tagLoadSearchToolTrainImageInfoThreadParam()
	{
		m_pBaseVisionAlign = NULL;
		m_nModelCandidateNum = 0;
		m_nModelPosIndex = 8;
		m_strPatternsDir = "";
		m_bIsLoadCacheModel = false;
		m_vnCandidatePos.resize(0);
		m_vToolType.resize(0);
		m_bTimeOut = true;
	}
	~tagLoadSearchToolTrainImageInfoThreadParam()
	{
	}
	tagLoadSearchToolTrainImageInfoThreadParam(vcBaseVisionAlign *BaseVisionAlign,CString StrPatternsDir,int nModelPosIndex,int nModelCandidateNum,std::vector<VpSearchTool> vvpSearchToolRepo,bool bIsLoadCacheModel)
	{
		m_pBaseVisionAlign		= BaseVisionAlign;
		m_strPatternsDir		= StrPatternsDir.GetBuffer();
		m_nModelPosIndex		= nModelPosIndex;
		m_nModelCandidateNum	= nModelCandidateNum;
		m_vvpSearchToolRepo		= vvpSearchToolRepo;
		m_bIsLoadCacheModel		= bIsLoadCacheModel;
		m_vnCandidatePos.resize(0);
		m_vToolType.resize(0);
		m_bTimeOut = true;
	}
	//设置当前线程是否初始化超时
	void SetIsInitTimeOut(bool bIsTimeOut)
	{
		m_bTimeOut = bIsTimeOut;
	}
	//获取当前线程是否初始化超时
	bool GetIsInitTimeOut()
	{
		return m_bTimeOut;
	}
	//判断模板图片是否存在
	bool IsImageExist(CString strDir)
	{
		// 补充指定路径，指向模板图片
		strDir +=_T("\\Model.bmp");
		//判断模板（图片）是否存在
		int nResult = _access((LPSTR)(LPCTSTR)strDir,0);
		//如果存在
		if(nResult == 0)
			return true;
		return false;
	}

} CLoadSearchToolTrainImageInfoThreadParam;


class CDlgSearchToolModelLibrary : public CDialog
{
	DECLARE_DYNAMIC(CDlgSearchToolModelLibrary)

public:
	CDlgSearchToolModelLibrary(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSearchToolModelLibrary();

	// 对话框数据
	enum { IDD = IDD_DLG_SEARCH_TOOL_MODEL_LIBRARY };
private:
	// 模板库初始化时：获取总的模板信息编号
	// 总的模板（当前定位工具全部的模板信息）信息编号（连续编号）： 1 2 3 4 .... 99 100 ...（“模板库序号”）
	// 通过平台、产品、工具类型通道筛选后
	// 需要预览的信息 m_vnPreviewImageIndex（存储需要预览的信息）（非连续性编号，对应上述连续编号）：1 3 6 7 8 99....（“图像预览序号”）
	// 每一页显示的预压图像与预览信息，通过获取m_vnPreviewImageIndex编号获取模板所有信息，如预览第一页，则需要获取m_vnPreviewImageIndex中前16个编号对用的模板信息
	// m_vcpImage每次翻页实时刷新，仅保存当前预览页的16幅图像

	// 模板加载逻辑：
	// 通过判断大小产品、是否加载单个大产品、单个小产品条件判断，
	// 产品是否当前运行产品，或者已经加载过模板了，如果是，则直接在m_vvpCalibObjectSearchToolRepo等变量中获取模板信息
	// 否则去根目录下加载xml与bmp模板信息

	VpGuiDisplay			m_vpGuiDisplay;						// 所有Gui
	scGuiDisplay*			m_pGuiDisplayPreview;				// 预览Gui
	std::vector<cpImage>	m_vcpImage;							// 存储当前预览页的所有模板图像（最多16幅图像）
	std::vector<CLabel*>	m_vpLabelImageContent;				// 存储当前预览页的模板图像对应的文本信息
	std::vector<scGuiText*> m_vpGuiText;						// Gui内部显示的文本信息（主要显示工具类型）

	std::vector<int>		m_vnPlatformIndex;					// 存储所有模板对应的平台序号
	std::vector<int>		m_vnProductIndex;					// 存储所有模板对应的产品序号
	std::vector<int>		m_vnPos;							// 存储所有模板对应的位置
	std::vector<int>		m_vnCandidatePos;					// 存储所有模板对应的候补位置
	std::vector<SearchMode>		m_vNSearchMode;					// 存储所有模板对应的工具类型
	std::vector<CSearchToolType> m_vToolType;					// 存储所有模板对应的工具名称
	// 因为以上vector都是存放所有模板平台序号，产品序号等的信息，但是实际预览图片m_vcpImage并不是所有图片
	// 所以将所有需要预览图片的序号存入当前vector内，方便其他操作
	std::vector<int>		m_vnPreviewImageIndex;				// 存储预览图像的编号

	BOOL						m_bIsLoadAllProduct;			// 当前选择平台是否加载全部产品模板
	// 每个平台是否选择加载全部产品模板，true选择加载全部模板，false加载单个模板
	std::vector<bool>			m_vbLoadAllProduct;				
	int							m_nComboSelectPlatformIndex;	// 当前选择的平台序号（下拉列表）
	// 记录当前选择的大小产品序号：
	// m_vvnComboSelectProductIndex[nPlatformIndex][0]工位nPlatformIndex选择的大产品序号
	// m_vvnComboSelectProductIndex[nPlatformIndex][1]工位nPlatformIndex选择的小产品序号
	std::vector<vector<int>>	m_vvnComboSelectProductIndex;	
	scTimer					m_time;								// 全局时间变量
	CString					m_strtime;							// 全局记录时间变量

	CString					m_strModuleDir;						// 模板地址
	int						m_nCurPageIndex;					// 当前页码
	int						m_nMaxPageNum;						// 最大页码（总页码）
	int						m_nCurSelectImageIndex;				// 当前选择的图片预览序号 0代表第一幅图
	int						m_nImageNum;						// 图片的最大总数
	bool					m_bIsLoadPlatformTool[6];			// 标记平台工具是否已经加载完成（最多6个平台）
	
	// 显示当前选择模板的预览信息，包含：工具类型、平台序号、产品序号、产品名称、模板类型、模板位置
	CString m_strPreToolType;									// 预览工具类型
	CString m_strPrePlatformIndex;								// 预览平台序号
	CString m_strPreProductIndex;								// 预览产品序号
	CString m_strPreProductName;								// 预览产品名称
	CString m_strPreModelType;									// 预览模板类型
	CString m_strPreModelPosition;								// 预览模板位置
	// 显示当前每一页（16个模板）模板的模板信息：平台序号+平台序号+模板位置
	CLabel					m_labelGuiContent0;					
	CLabel					m_labelGuiContent1;
	CLabel					m_labelGuiContent2;
	CLabel					m_labelGuiContent3;
	CLabel					m_labelGuiContent4;
	CLabel					m_labelGuiContent5;
	CLabel					m_labelGuiContent6;
	CLabel					m_labelGuiContent7;
	CLabel					m_labelGuiContent8;
	CLabel					m_labelGuiContent9;
	CLabel					m_labelGuiContent10;
	CLabel					m_labelGuiContent11;
	CLabel					m_labelGuiContent12;
	CLabel					m_labelGuiContent13;
	CLabel					m_labelGuiContent14;
	CLabel					m_labelGuiContent15;
	COLORREF				colorNormal;			// CLabel的一般背景颜色
	COLORREF				colorSelect;			// CLabel的被选中背景颜色

	BOOL m_bUsingCalib;											//是否显示标定模板图像
	BOOL m_bUsingTarget;										//是否显示目标模板图像
	BOOL m_bUsingObject;										//是否显示对象模板图像
	BOOL m_bUsingCalibTarget;									//是否显示关联模板图像
	BOOL m_bUsingInspect;										//是否显示检测模板图像
	BOOL m_bUsingVirtual;										//是否显示虚拟模板图像

	CBaseSearchTool*		m_pBaseSearchTool;		// 最终输出模板喜喜（最终拷贝的模板）（当前析构以后，会释放当前变量内存）

	void InitDlgItem();								// 初始Gui化界面
	bool UpdateDlgData();							// 刷新界面（预览模板信息）数据

	bool LoadAllToolTrainImage();					// 加载软件所有工具信息
	bool LoadSearchToolTrainImageInfo(int nPlatformIndex,int nProductIndex); // 根据平台序号、产品序号加载所有模板图片信息（不加载图像，仅加载图像对应的所有信息）
	bool LoadSearchToolTrainImage(int nCurPageIndex);	// 根据当前页面编号加载指定图片（仅当前页面对应的16幅模板图像）

	bool ClearGuiDisplay();							// 释放所有Gui
	bool ClearcpImage();							// 释放所有Image
	bool ClearModelLibraryVector();					// 清除所有模板库信息（暂时没有函数调用）

	void UpdateToolPreviewInfo(int nImageIndex);	// 刷新“模板信息预览”信息根据图片的图像库的序号
	bool UpdatePreviewImageIndex();					// 计算所有需要预览的图片编号，存放入m_vnPreviewImageIndex内,并重新计算最大页数，以及当前页码

	void UpdateDialogLanguage();					// 中英文显示
	void UpdateStatusBar();							// 状态栏显示

	void UpdateComboLoadPlatform();					// 刷新“加载产品模板设置”中，平台下拉框内容
	void UpdateComboLoadProduct(int nPlatform);		// 根据选中平台序号，刷新“加载产品模板设置”中，大小产品下拉框内容

	BOOL CheckSearchToolIsValid(CSearchToolType csType);

public:
	std::vector<vcBaseVisionAlign*> m_vpVisionASM;				// 视觉对位工具
	int						m_nCurPlatformIndex;				// 当前平台序号

	CXPStyleButtonST		m_btnOK;							// 确定按钮
	CXPStyleButtonST		m_btnCancel;						// 取消按钮
	CXPStyleButtonST		m_btnPreviouPage;					// 上一页按钮
	CXPStyleButtonST		m_btnNextPage;						// 下一页按钮牛

	CString GetSearchToolType(CSearchToolType csType);			//  根据工具类型获取工具类新对用的名称信息，如：“区域定位工具”名称

	int		GetSelectImageIndex();								// // 获取选择的模板在模板库中的序号（并非预览序号）
	// 根据模板序号(模板库序号，并非预览序号)返回图片信息
	// bIsCompleteInfo = true返回完整信息用于状态栏
	// bIsCompleteInfo = false否则返回不完整信息用于工具说明Label
	bool	GetImageContent(int nImageIndex,CString &strImageContent,bool bIsCompleteInfo = false); 
	int		GetSelectPreviewImageIndex();						// 获取选择的模板的预览序号（并非模板库序号）
	bool	GetSelectedImage(cpImage &ImagePattern);			// 获取选中的模板图像
	bool	GetSelectedPlatformIndex(int &nPlatformIndex);		// 获取选中的平台序号
	bool	GetSelectedProduceIndex(int &nProductIndex);		// 获取选中的产品序号
	bool	GetSelectedSearchToolMode(SearchMode &sSelectMode);	// 获取选中的工具类型
	bool	GetSelectedSearchToolPosIndex(int &nPosIndex);		// 获取选中的工具位置
	bool	GetSelectedSearchToolCandidatePosIndex(int &nCandidatePosIndex); //获取选中的工具候补位置
	bool	GetSelectedSearchToolType(CSearchToolType &SearchToolType);//获取选中的工具模板类型
	CBaseSearchTool* GetSelectedSearchTool();					// 获取选中的定位工具

	int		GetImagePlatformIndex(int nImageIndex);				// 根据模板在模板库的序号获取图片的平台序号
	int		GetImageProduceIndex(int nImageIndex);				// 根据模板在模板库的序号获取图片的产品序号
	SearchMode	GetImageSearchToolMode(int nImageIndex);		// 根据模板在模板库的序获取图片的工具类型
	int		GetImageSearchToolPosIndex(int nImageIndex);		// 根据模板在模板库的序号获取图片的工具位置
	int		GetImageSearchToolCandidatePosIndex(int nImageIndex); //根据模板在模板库的序号获取图片的工具候补位置
	CSearchToolType	GetImageSearchToolType(int nImageIndex);	//根据模板在模板库的序号获取图片的工具模板类新

	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	static DWORD WINAPI LoadSearchToolTrainImageInfoThread(LPVOID lpParam);	// 加载模板信息的线程函数（模板信息不包括图像）
	static DWORD WINAPI LoadSearchToolTrainImageThread(LPVOID lpParam);		// 加载模板图像的线程函数
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CThemeHelperST		m_themeHelper;

	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	int	m_nlgidLanguage;
	BOOL			m_bSysKeyboardEnabled;
	/*CKeyBoard		m_ctrlKeyboard;*/
	CStringArray	*m_psaSysInfoStrings;

	CComboBox m_ComboLoadPlatform;			//“加载产品模板设置”中的平台下拉框
	CComboBox m_ComboLoadBigProduct;		//“加载产品模板设置”中的大产品下拉框
	CComboBox m_ComboLoadSmallProduct;		//“加载产品模板设置”中的小产品下拉框

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam);	// 双击GuiDisplay的函数，用于显示“定位工具模板预览”界面，仅“模板信息预览”的GuiDisplay响应
	afx_msg LRESULT OnGuiDisplayClk(WPARAM wParam, LPARAM lParam);		// 单机GuiDisplay的函数，用于显示选择的预览模板信息
	afx_msg void OnBnClickedBtnPreviousPage();
	afx_msg void OnBnClickedBtnNextPage();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckUpdateDlgData();			//	选择工具通道
	afx_msg void OnSelchangeCmbLoadProductPlatform();		// 点击平台下拉框响应函数
	afx_msg void OnSelchangeCmbLoadProductChProduct();		// 点击小产品下拉框响应函数
	afx_msg void OnSelchangeCmbLoadProductFaProduct();		// 点击大产品下拉框响应函数
	afx_msg void OnBnClickedRadioLoadAllproduct();			// 点击加载全部产品响应函数

	SearchMode m_nSearchModeFrom; 
	int m_nPosIndexFrom;
	int m_nCandidatePosFrom;
};
//加载产品模板线程的传输参数
typedef struct tagLoadSearchToolTrainImageThreadParam
{						
	vector<CString> m_vstrPatternDir;				//当前模板的路径
	vector<bool> m_vbLoadTrainImage;				// 是否Load模板,是则直接加载根目录下的模板，否则直接读取缓存模板
	bool m_bTimeOut;								// 是否初始化超时
	vector<bool> m_vbLoadCacheModel;				// 是否加载缓存模板，否则加载根目录下的模板
	VpSearchTool m_vpSearchTool;
public:
	std::vector<cpImage>	m_vcpImage;					// 存储所有图像
	tagLoadSearchToolTrainImageThreadParam()
	{
		m_vbLoadTrainImage.resize(4);
		m_vstrPatternDir.resize(4);
		m_vpSearchTool.resize(4);
		m_vcpImage.resize(0);
		m_vbLoadCacheModel.resize(4);
		m_bTimeOut = true;
	}
	~tagLoadSearchToolTrainImageThreadParam()
	{
		for(int i = 0; i < m_vcpImage.size(); i++)
		{
			m_vcpImage[i].Release();
		}
	}
	tagLoadSearchToolTrainImageThreadParam(VpSearchTool vpSearchTool,vector<CString> vstrPatternDir,vector<bool> vbLoadTrainImage,vector<bool> vbLoadCacheModel)
	{

		m_vpSearchTool = vpSearchTool;
		m_vstrPatternDir = vstrPatternDir;
		m_vbLoadTrainImage = vbLoadTrainImage;
		m_vbLoadCacheModel = vbLoadCacheModel;
		m_bTimeOut = true;
		m_vcpImage.resize(0);
	}
	//设置当前线程是否初始化超时
	void SetIsInitTimeOut(bool bIsTimeOut)
	{
		m_bTimeOut = bIsTimeOut;
	}
	//获取当前线程是否初始化超时
	bool GetIsInitTimeOut()
	{
		return m_bTimeOut;
	}


} CLoadSearchToolTrainImageThreadParam;
