// vsVisionAlignDef.h
#pragma once


#include <vector>
#include <afxtempl.h>
#include "svVector.h"
#include "vsComm.h"
//#include "vsGrab.h"
#include "vsCamera.h"
#include "svGuiPolyline.h"
#include "QuickSearchTool.h"
#include "PatternSearchTool.h"
#include "CircleSearchTool.h"
#include "LineSearchTool.h"
#include "CornerSearchTool.h"
#include "FixedPosSearchTool.h"
#include "CrossSearchTool.h"
#include "PatCornerSearchTool.h"
#include "ProCircleSearchTool.h"
#include "PatternCornerSearchTool.h"
#include "CombinedQuickSearch.h"
#include "CombinedPatternSearchTool.h"
#include "BaseCornerSearchTool.h"
#include "PatternSearchCheckTool.h"
#include "QuickSearchCheckTool.h"
#include "QuickSearchObroundTool.h"
#include "PatternSearchObroundTool.h"
#include "MPatternSearchTool.h"
#include "MQuickSearchTool.h"
#include "MQuickCornerSearchTool.h"
#include "MQuickPickCornerSearchTool.h"
#include "QuickSearchGenRectTool.h"
#include "PatternSearchGenRectTool.h"
#include "PatternLineSearchTool.h"
#include "2PatternSearchTool.h"
#include "QuickLineCircleTool.h"
#include "PatternLineCircleTool.h"
#include "PatternCircleSearchTool.h"
#include "QuickSearchCheckLineTool.h"
#include "PatternSearchCheckLineTool.h"
#include "PatternBlobSearchTool.h"
#include "CornerSearchCheckTool.h"
#include "RectSearchTool.h"
#include "PatternRectSearchTool.h"
#include "AreaInspectTool.h"
#include "AreaInspectToolEx.h"
#include "ChordSearchTool.h"
#include "TwoPatModelSearchTool.h"
#include "MPatternCornerSearchTool.h"
#include "MPatternPickCornerSearchTool.h"
#include "PatternCornerCircleSearchTool.h"
#include "QuickCornerCircleSearchTool.h"
#include "PatternPosLineDistanceTool.h"
#include "CornerSearchCheckInspectTool.h"
#include "PatternDoubleSearchTool.h"
#include "PatternChordSearchTool.h"
#include "QuickMulCircleSearchTool.h"
#include "DmCodeCalibrateTool.h"
#include "PatternCircle4LineSearchTool.h"
#include "QuickCircle4LineSearchTool.h"
#include "SpecialPickSearchTool.h"
#include "PatternCornerLineSearchTool.h"
#include "PatternMulCircleSearchTool.h"
#include "PatternCornerMulCircleSearchTool.h"
#include "PatternCornerMulObroundTool.h"
#include "QuickCornerMulObroundTool.h"
#include "CircleCornerSearchCheckInspectTool.h"
#include "ACFInspectTool.h"
#include "VirtualRectSearchTool.h"
#include "PatternVirtualRectSearchTool.h"
#include "TwoQuickModelSearchTool.h"
#include "IdReaderDmCodeTool.h"
#include "IdReaderBarCodeTool.h"
#include "vsBaseAlignTool.h"
#include "SysPlatformInfo.h"
#include "vsAlignToolParam.h"
#include "svGuiDisplay.h"
#include "svGuiText.h"
#include "CommProtocolXYDFast.h"
#include "vsBaseAlignTool.h"
#include "QuickPosLineDistanceTool.h"
#include "DlgQuickPosLineDistanceTool.h"
#include "CombinationCornerSearchTool.h"
#include "DlgCombinedCornerSearchTool.h"
#include "MQuickPickSearchTool.h"
#include "DlgMQuickPickSearchTool.h"
#include "MPatternPickSearchTool.h"
#include "DlgMPatternPickSearchTool.h"
#include "AreaInspectPickTool.h"
#include "DlgAreaInspectPickTool.h"

#define PRECISIONXYUNIT 1000.0
#define PRECISIONDUNIT 1.0
#define PRECISIONXYDOTNUM 2
#define PRECISIONDDOTNUM 4
#define GUANLIANBOARDNUM 9
typedef std::vector<unsigned int> VecUINT;
typedef std::vector<CBaseSearchTool*> VpSearchTool;
typedef std::vector<scGuiText*>	VpGuiText;

//////////////////////////////////////////////////////////////////////////
//	平台系统级数据结构
//	单个平台系统级数据结构包括：
//	1. 平台信息：平台类型信息、平台详细信息（XYD、UVW等坐标轴方向信息、D的类型（直线、直接）、XYD、UVW等的尺寸信息）、
//				 平台产品信息（平台标定信息、平台产品对位信息）
//				 1.1 平台标定信息（平台标定信息数量、平台标定信息列表、当前使用的平台标定信息、
//									单项（标定参数：运动量、定位模板；标定结果：关系））
//				 1.2 平台产品对位信息（平台信息数量、平台对位信息列表、当前使用的平台对位信息、
//									单项（对位参数：精度、次数、补偿量、目标定位模板、对象定位模板、虚拟对象定位模板））
//				 1.3 换产品不更改平台参数，不需要重新标定；
//					 每个平台只有一份平台标定参数，但又N多个平台标定结果，平台标定结果保存在产品对位信息中？
//				 
//	2. 相机信息：相机个数、相机安装模式、相机序列号、相机采集信息、相机图像显示信息
//  3. 通信接口信息：接口类型（串口、网口），接口参数（串口通信参数、网口通信参数）
//  4. 
//////////////////////////////////////////////////////////////////////////

// 单个产品标定参数、标定结果信息
class CCalibData
{
public:	
	CCalibData();
	~CCalibData();
	CCalibData(const SysPlatformInfo& platformInfo);
	CCalibData(const CCalibData& lCData);
	CCalibData& operator = (const CCalibData& lCData);

	BOOL IsValid();
	BOOL IsCalibrateInfoValid();
	int	GetCalibrateParamNum();
	int GetpCalibratedInfoNum();

	void SetCalibIndex(int nIndex);										// 设置标定参数的索引值
	int GetCalibIndex();												// 获得标定参数的索引值

// 	void SetCalibUserName(BSTR bstrUserName);							// 设置标定参数的名称
// 	BSTR  GetCalibUserName();											// 获取标定模板的名称

	void SetCalibUserName(CString strUserName);							// 设置标定参数的名称
	CString  GetCalibUserName();										// 获取标定模板的名称

	void DeleteObjectSearchTools();										// 释放所有标定对象定位工具
	void ResetObjectSearchTools();										// 赋空所有对象定位工具

	// 视觉流程中会调用
	void SetCalibMarkPatternMode(CalibMarkPatternMode mode);
	CalibMarkPatternMode GetCalibMarkPatternMode();

	void SetCalibHomographyDOF(HomographyDOF CalibHomographyDOF);       // 设置求矩阵算法
	HomographyDOF GetCalibHomographyDOF();	

	void SetWholeCalibCacuPos(int nPosIndex);							// 设置、获取全局二维码靶标标定旋转中心计算位置
	int GetWholeCalibCacuPos();

	void SetWholeCalibCacuPosEnable(BOOL bEnable);
	BOOL GetWholeCalibCacuPosEnable();


public:
	std::vector<CCalibrateParam*>	m_vpCalibrateParam;						// 产品的标定信息
	std::vector<CCalibratedInfo*>	m_vpCalibratedInfo;						// 产品对应的相机标定参数信息

	int m_nCalibIndex;
	CString m_strUserName;

	CalibMarkPatternMode m_eCalibMarkPatternMode;							// 标定模式（模板模式、二维码模式）
	BOOL m_bEnableWholeCalibCacuPos;										// 是否启用全局二维码标定计算旋转中心位置
	int m_nWholeCalibCacuPos;												// 全局二维码标定计算旋转中心的位置


	HomographyDOF	m_eCalibHomographyDOF;//标定算法类型

	BOOL							m_bCalibObjectSearchToolInited;			// 标定定位工具是否初始化
	std::vector<VpSearchTool>		m_vvpCalibObjectSearchToolRepo;			// 标定定位工具库

};

// 单个平台汇总所有产品标定类型信息
class CTotalCalibData
{
public:
	CTotalCalibData();	
	~CTotalCalibData();

public:
	int		GetCount() const;										// 获取产品标定数量

	BOOL	SetItem(int nIndex, const CCalibData& item);			// 设置第nIndex个产品标定的信息
	BOOL	SetItemGrow(int nIndex, const CCalibData& item);		// 设置第nIndex个产品标定的信息,如果有必要，则添加产品标定

	CCalibData	GetItem(int nIndex) const;							// 获取产品标定信息
	CCalibData*	GetItemPtr(int nIndex) const;						// 获取产品标定信息指针

	BOOL	AddItem(const CCalibData& item);						// 添加产品标定
	BOOL	RemoveItem(int nIndex);									// 删除产品标定
	BOOL	ClearItem(int nIndex);									// 清空产品标定
	BOOL	CopyItem(int nFromIndex, int nToIndex);					// 将第nFromIndex个模板拷贝到第nToIndex
	void	Clear();												// 清空产品标定

	int GetCurCalibIndex() const { return m_nCurCalibIndex; }		// 获取当前产品标定索引值
	void SetCurCalibIndex(int nIdx) { m_nCurCalibIndex = nIdx; }	// 设置当前产品标定索引值

	BOOL SetItemName(int nIdx, LPCTSTR lpszNewName);				// 设置指定产品标定的名称
	CString GetItemName(int nIdx);									// 获取指定产品标定的名称

	void RemoveInvalidItems();										//　删除无效产品标定

protected:
	int							m_nCurCalibIndex;					// 当前产品标定索引值：0、1、2、3、4... ...
	std::vector<CCalibData*>	m_vCalibData;

public:
	bool SetCalibIndex(int nCalibIndexFa, int nCalibIndexChd, int nCalibIndex);
	int GetCalibIndex(int nCalibIndexFa, int nCalibIndexChd);

	int GetCalibCountFa(){return m_vCalibIndexFaChd.size();}
	int GetCalibCountChd(int nFaCalibIndex);

	bool GetDlgCalibIndex(int nCalibIndex, int& nCalibIndexFa, int& nCalibIndexChd);
	void GetCurDlgCalibIndex(int& nCalibIndexFa, int& nCalibIndexChd){GetDlgCalibIndex(m_nCurCalibIndex, nCalibIndexFa, nCalibIndexChd);}

	bool SetCalibNameFa(int nCalibIndexFa, LPCTSTR lpszNewName);
	CString GetCalibNameFa(int nCalibIndexFa);

	void AddCalibLastChdIndex(int nLastChdIndex){m_vCalibLastChdIndex.push_back(nLastChdIndex);}
	void UpdateCalibLastChdIndex(int nCalibIndexFa, int nLastChdIndex);
	int GetCalibLastChdIndex(int nCalibIndexFa);

protected:
	std::vector<std::vector<int>> m_vCalibIndexFaChd;	// m_vCalibIndexFaChd[i][j]表示第i个大产品第j个小产品在全部产品的索引值（从0开始）
	std::vector<CString> m_vCalibNameFa;				// m_vCalibNameFa[i]表示第i个大产品的名字
	std::vector<int> m_vCalibLastChdIndex;				// m_vCalibLastChdIndex[i]表示第i个大产品最后一次选中小产品在该大产品的索引值（从0开始）	
};
// 
// // 标定定位工具
// class CCalibSearchTool
// {
// public:	
// 	CCalibSearchTool();
// 	~CCalibSearchTool();
// 	CCalibSearchTool(const SysPlatformInfo& platformInfo);
// 	CCalibSearchTool(const CCalibSearchTool& lPData);
// 	CCalibSearchTool& operator = (const CCalibSearchTool& lPData);
// 	BOOL IsValid();
// public:
// 	std::vector<VpSearchTool>			m_vvpAlnTargetSearchToolRepo;		// 标定定位工具库
// };
// 
// 
// // 产品定位工具
// class CAlnSearchTool
// {
// public:	
// 	CAlnSearchTool();
// 	~CAlnSearchTool();
// 	CAlnSearchTool(const SysPlatformInfo& platformInfo);
// // 	CAlnSearchTool(const CAlnSearchTool& lPData);
// // 	CAlnSearchTool& operator = (const CAlnSearchTool& lPData);;
// 	BOOL DeleteAllSearchTools();
// 	BOOL DeleteTargetSearchTools();
// 	BOOL DeleteObjectSearchTools();
// public:
// 	BOOL								m_bAlnTargetSearchToolInited;
// 	BOOL								m_bAlnObjectSearchToolInited;
// 	std::vector<VpSearchTool>			m_vvpAlnTargetSearchToolRepo;		// 目标定位工具库
// 	std::vector<VpSearchTool>			m_vvpAlnObjectSearchToolRepo;		// 对象定位工具库
// };
// 
// 

// 单个产品信息
// 注:对于定位工具，拷贝构造、赋值函数都是浅拷贝，需要注意内存泄露、引用等问题，需要释放的时候需要调用相应的Delete函数
class CProductData
{
public:	
	CProductData();
	~CProductData();
	CProductData(const SysPlatformInfo& platformInfo);
	CProductData(const CProductData& lPData);
	CProductData& operator = (const CProductData& lPData);

	BOOL IsValid();

	void DeleteTargetSearchTools();											// 释放所有目标定位工具
	void DeleteObjectSearchTools();											// 释放所有对象定位工具
	void DeleteVirtualObjectSearchTools();									// 释放所有虚拟对象定位工具
	void DeleteCalibTargetSearchTools();									// 释放所有关联标定定位工具
	void DeleteInspectSearchTools();										// 释放所有检测定位工具

	void ResetTargetSearchTools();											// 赋空所有目标定位工具
	void ResetObjectSearchTools();											// 赋空所有对象定位工具
	void ResetVirtualObjectSearchTools();									// 赋空所有虚拟对象定位工具
	void ResetCalibTargetSearchTools();										// 赋空所有目标定位工具
	void ResetInspectSearchTools();											// 赋空所有检测定位工具

public:
	std::vector<scDMCodeParam> m_vDMcodeCalib;								// 二维码标定参数   iyo

	CAlignerParam*					m_pAlignerParam;						// 产品的对位信息
	std::vector<CCalibratedInfo*>	m_vpCalibratedInfo;						// 产品对应的相机标定参数信息

	std::vector<CPlatformAxisPos*>  m_vpTargetPlatformAxisPos;
	std::vector<CPlatformAxisPos*>	m_vpObjectPlatformAxisPos;
	CMarkImagePos					m_mpTargetMarkImagePos;
	CMarkImagePos					m_mpObjectMarkImagePos;

	BOOL							m_bAlnTargetSearchToolInited;			// 目标定位工具是否初始化
	BOOL							m_bAlnObjectSearchToolInited;			// 对象定位工具是否初始化
	BOOL							m_bAlnVirtualObjectSearchToolInited;	// 虚拟对象定位工具是否初始化
	BOOL							m_bAlnCalibTargetSearchToolInited;		// 关联标定定位工具是否初始化
	BOOL							m_bAlnInspectSearchToolInited;			// 检测定位工具是否初始化


	std::vector<VpSearchTool>		m_vvpAlnTargetSearchToolRepo;			// 目标定位工具库
	std::vector<VpSearchTool>		m_vvpAlnObjectSearchToolRepo;			// 对象定位工具库
	std::vector<VpSearchTool>		m_vvpAlnVirtualObjectSearchToolRepo;	// 虚拟对象定位工具库
	std::vector<VpSearchTool>		m_vvpAlnCalibTargetSearchToolRepo;		// 关联标定定位工具库
	std::vector<VpSearchTool>		m_vvpAlnInspectSearchToolRepo;			// 检测定位工具库

	CRobotTeachInfo					m_RobotTeachInfo;						// 机器人示教信息
};



// 单个平台汇总所有产品类型信息
class CTotalProductData
{
public:
	CTotalProductData();	
	~CTotalProductData();

public:
	int		GetCount() const;								// 获取产品数量

	BOOL	SetItem(int nIndex, const CProductData& item);		// 设置第nIndex个产品的信息
	BOOL	SetItemGrow(int nIndex, const CProductData& item);	// 设置第nIndex个产品的信息,如果有必要，则添加产品
	CProductData	GetItem(int nIndex) const;					// 获取产品信息
	CProductData*	GetItemPtr(int nIndex) const;				// 获取产品信息指针
	
	BOOL	AddItem(const CProductData& item);				// 添加产品
	BOOL	RemoveItem(int nIndex);					        // 删除产品
	BOOL	ClearItem(int nIndex);					        // 清空产品
	BOOL	CopyItem(int nFromIndex, int nToIndex);			// 将第nFromIndex个模板拷贝到第nToIndex
	void	Clear();										// 清空产品

	int GetCurProductIndex() const { return m_nCurProductIndex; }	// 获取当前产品索引值
	void SetCurProductIndex(int nIdx) { m_nCurProductIndex = nIdx; }	// 设置当前产品索引值

	BOOL SetItemName(int nIdx, LPCTSTR lpszNewName);			// 设置指定产品的名称
	CString GetItemName(int nIdx);								// 获取指定产品的名称

	void RemoveInvalidItems();								//　删除无效产品

	

protected:
	int				 m_nCurProductIndex;					// 当前产品索引值：0、1、2、3、4... ...
	std::vector<CProductData*> m_vProductData;

public:
	bool SetProductIndex(int nProductIndexFa, int nProductIndexChd, int nProductIndex);
	int GetProductIndex(int nProductIndexFa, int nProductIndexChd);

	int GetProductCountFa(){return m_vProductIndexFaChd.size();}
	int GetProductCountChd(int nProductIndexFa);

	bool GetDlgProductIndex(int nProductIndex, int& nProductIndexFa, int& nProductIndexChd);
	void GetCurDlgProductIndex(int& nProductIndexFa, int& nProductIndexChd){GetDlgProductIndex(m_nCurProductIndex, nProductIndexFa, nProductIndexChd);}

	bool SetProductNameFa(int nProductIndexFa, LPCTSTR lpszNewName);
	CString GetProductNameFa(int nProductIndexFa);

	void AddLastChdIndex(int nLastChdIndex){m_vProductLastChdIndex.push_back(nLastChdIndex);}
	void UpdateLastChdIndex(int nProductIndexFa, int nLastChdIndex);
	int GetLastChdIndex(int nProductIndexFa);

protected:
	std::vector<std::vector<int>> m_vProductIndexFaChd;	// m_vProductIndexFaChd[i][j]表示第i个大产品第j个小产品在全部产品的索引值（从0开始）
	std::vector<CString> m_vProductNameFa;				// m_vProductNameFa[i]表示第i个大产品的名字
	std::vector<int> m_vProductLastChdIndex;			// m_vProductLastChdIndex[i]表示第i个大产品最后一次选中小产品在该大产品的索引值（从0开始）
};

// 位置信息
typedef struct tagPosInfo
{
	BOOL	m_bPosEnable;			// 该位置是否有效
	int		m_nCamIndex;			// 相机索引号
	tagPosInfo()
	{
		m_bPosEnable = FALSE;
		m_nCamIndex = 0;
	}
} CPosInfo;

//相机类型
typedef enum tagShareCamType
{
	eNoShare=0,//不共享
	eShareSrc=1,//主相机
	eShareTar=2,//副相机
}CShareCamType;

//共享相机信息
typedef struct tagShareCamInfo
{
	int m_nPlatformIndex;
	int m_nCamIndex;
	tagShareCamInfo()
	{
		m_nPlatformIndex=0;
		m_nCamIndex=0;
	}
}CShareCamInfo;

//存储图像类型
typedef enum tageImageType
{
	eBmp = 0,
	eJpeg = 1,
	ePng = 2,
}CImageType;
// 相机控制参数
typedef struct tagCameraParam
{
	CString			m_strDeviceID;
	unsigned long	m_lSerialNum;		// 相机序列号
	int				m_nVideoMode;		// 相机视频模式：0：640*480，1：800*600，2:1024*768,3:1280*960,4:1600*1200,5：自定义
	int				m_nRawWidth;		// 相机宽
	int				m_nRawHeight;		// 相机高
	int				m_nCustemStartX;	// 自定义起始X
	int				m_nCustemStartY;	// 自定义起始Y
	int				m_nCustemWidth;		// 自定义宽
	int				m_nCustemHeight;	// 自定义高			
	double			m_dFrameRate;		// 帧率
	double			m_dShutter;			// 曝光时间
	double			m_dGain;			// 增益
	bool		    m_bGammaEnable;		// Gamma启用
	double			m_dGamma;			// Gamma
	CShareCamType   m_eShareCamType;    // 相机共享类型
	int             m_nShareCamNum;     // 相机共享数目
	std::vector<CShareCamInfo> m_vShareCamInfo;//相机共享信息

    /* 说明：若本相机为共享主相机（eShareSrc），则m_nShareCamNum为它的
	副相机（eShareTar）数目，m_vShareCamInfo里存入它的副相机信息；
	若本相机为副相机，则m_nShareCamNum为1，m_vShareCamInfo里存入它的主相机信息*/

	double			m_dWhiteBalanceRatioR;//白平衡R系数R
	double			m_dWhiteBalanceRatioG;//白平衡R系数G
	double			m_dWhiteBalanceRatioB;//白平衡R系数B

	double			m_dCalibTargetShutter;// 关联时曝光

	tagCameraParam()
	{
		m_strDeviceID = _T("");
        m_lSerialNum    = 0;
		m_nVideoMode    = 0;
		m_nRawWidth     = 640;
		m_nRawHeight    = 480;
		m_nCustemStartX = 0;
		m_nCustemStartY = 0;
		m_nCustemWidth  = 640;
		m_nCustemHeight = 480;
		m_dFrameRate    = 7.5;
		m_dShutter      = 10;
		m_dGain         = 0;
		m_bGammaEnable	= false;		// Gamma启用
		m_dGamma		= 1;			// Gamma
		m_eShareCamType=eNoShare;
		m_nShareCamNum  = 0;
		m_dWhiteBalanceRatioR = 1.0;
		m_dWhiteBalanceRatioG = 1.0;
		m_dWhiteBalanceRatioB = 1.0;

		m_dCalibTargetShutter = 0.01;
	}
	
	tagCameraParam& operator= (const tagCameraParam& lCameraParam)
	{
		m_strDeviceID = lCameraParam.m_strDeviceID;
		m_lSerialNum    = lCameraParam.m_lSerialNum;
		m_nVideoMode    = lCameraParam.m_nVideoMode;
		m_nRawWidth     = lCameraParam.m_nRawWidth;
		m_nRawHeight    = lCameraParam.m_nRawHeight;
		m_nCustemStartX = lCameraParam.m_nCustemStartX;
		m_nCustemStartY = lCameraParam.m_nCustemStartY;
		m_nCustemWidth  = lCameraParam.m_nCustemWidth;
		m_nCustemHeight = lCameraParam.m_nCustemHeight;
		m_dFrameRate    = lCameraParam.m_dFrameRate;
		m_dShutter      = lCameraParam.m_dShutter;
		m_dGain         = lCameraParam.m_dGain;
		m_bGammaEnable	= lCameraParam.m_bGammaEnable;		// Gamma启用
		m_dGamma		= lCameraParam.m_dGamma;			// Gamma
		m_eShareCamType = lCameraParam.m_eShareCamType;
		m_vShareCamInfo = lCameraParam.m_vShareCamInfo;
		m_nShareCamNum  = lCameraParam.m_nShareCamNum;
		m_dWhiteBalanceRatioR         = lCameraParam.m_dWhiteBalanceRatioR;
		m_dWhiteBalanceRatioG         = lCameraParam.m_dWhiteBalanceRatioG;
		m_dWhiteBalanceRatioB         = lCameraParam.m_dWhiteBalanceRatioB;
		
		m_dCalibTargetShutter =		lCameraParam.m_dCalibTargetShutter;
		return *this;
	}

}CameraParam, *pCameraParam;

// 显示设置参数
typedef struct tagImageRoi
{
	BOOL   m_bMirrorHoriz;		//
	BOOL   m_bMirrorVert;
	int    m_nRotation;        // 特殊角度旋转：0为0°，1为90°，2为180°，3为270°

	double m_dCrossX;			// 界面显示的十字线
	double m_dCrossY;			// 界面显示的十字线
	BOOL   m_bCrossVisible;		// 是否显示界面的十字线

	BOOL   m_bGammaCorrectionEnable;	// Gamma校正是否有效
	double m_dGamma;					// Gamma校正值
	
	tagImageRoi()
	{
		m_bMirrorHoriz = FALSE;
		m_bMirrorVert  = FALSE;
		m_nRotation    = 0;
		m_dCrossX		= 100;
		m_dCrossY		= 100;
		m_bCrossVisible = FALSE; // 默认不启用十字线

		m_bGammaCorrectionEnable = FALSE;	// Gamma校正是否有效
		m_dGamma = 1.0;						// Gamma校正值
	}
	
	tagImageRoi& operator = (const tagImageRoi& lImgRoi)
	{
		m_bMirrorHoriz = lImgRoi.m_bMirrorHoriz;
		m_bMirrorVert  = lImgRoi.m_bMirrorVert;
		m_nRotation	   = lImgRoi.m_nRotation;	
		m_dCrossX		= lImgRoi.m_dCrossX;
		m_dCrossY		= lImgRoi.m_dCrossY;
		m_bCrossVisible = lImgRoi.m_bCrossVisible;

		m_bGammaCorrectionEnable  = lImgRoi.m_bGammaCorrectionEnable;
		m_dGamma = lImgRoi.m_dGamma;

		return *this;
	}
	
}ImageRoi, *PImageRoi;


// 数据记录
typedef struct tagSysDataRecord
{
	BOOL				m_bAlignerSearchPltfmInfo;  // 记录搜索信息与平台信息	
	BOOL				m_bCommunication;			// 记录通信数据
	BOOL				m_bAlignToolLog;			// 对位工具日志

	BOOL				m_bImageName;				// 产品ID命名图像
	BOOL				m_bSaveNGImage;				// 记录错误图像数据
	int					m_nSaveFailedImageNum;

	BOOL                m_bSaveAllImage;			// 记录所有图像数据
	int					m_nSaveAllImageNum;
	BOOL				m_bSaveAllImageType;		// 存储所有图像的方式：0原始存图方式；1线程存图方式

	CImageType          m_eImageFormat;             // 存储图像的格式
	int                 m_nImageQuility;            // jpg图像的质量（即压缩比）
	int                 m_nVDBImageQuility;            // VDB的jpg图像的质量（即压缩比）

	
	BOOL				m_bVDBNameByID;				// 产品ID命名VDB
	BOOL                m_bSaveVDBFile;             // 存储VDB文件
	int					m_nSaveVDBMaxNums;			// 保存VDB文件最大个数
	int					m_nVBDImageType;			// 存储VDB格式时图片类型   0-eImgBmp\1-eImgPng
	int					m_GuitextWeight;			// 存储VDB格式时图片字体大小
	int					m_GuiLineWidth;			    // 存储VDB格式时图片线宽

	BOOL				m_bSaveObjectSnapVDBFile;				// 存对象拍照VDB
	BOOL				m_bSaveTargetSnapVDBFile;				// 存目标拍照VDB
	BOOL				m_bSaveProcessSnapVDBFile;				// 存流程拍照VDB
	BOOL				m_bSaveInsepectResultVDBFile;            //存储时按照检测结果进行判断
    
	BOOL                m_bCustomLogPath;           // 是否允许自定义存储路径
	BOOL                m_bDataRecordTime;			// 是否按时刻记录图像和文本并定期删除	
	int					m_nDataRecordTimeNum;		// 按时刻记录文件夹数量，大于等于1
	double				m_dDiskSpaceMaxUsePercent;	// 磁盘分区最大使用百分比，大于等于1，小于等于99
	CString             m_strDataRecordPath;        // 日志存储路径
	int					m_nStartHourRecordByDay;   //日志记录其实时间
	


//	BOOL				m_bThreadSaveImageMode;		  // 线程保存图片的方式 


	// 调试用，需要手动修改配置文件	
	BOOL                m_bDataMerged;              // 通信数据与对位相关数据是否合并记录在同一个文本文件中
	BOOL				m_bAlignerTime;				// 记录对位时间
	BOOL				m_bAlignerPrecision;		// 记录对位精度
//	BOOL				m_bSysDataRecord;			// 记录系统数据
//	CString             m_strDataRecordTimeAddr;    // 按时刻记录图像和文本的存储地址（这个地址只能是根目录下的一个文件夹）
//	BOOL                m_bSaveAllImage;			// 记录所有图像数据
//	CString             m_strSaveAllImgDataAddr;    // 所有图像数据的保存地址（这个地址只能是根目录下的一个文件夹）


	tagSysDataRecord()
	{
		m_bAlignerSearchPltfmInfo	= TRUE;
		m_bCommunication			= TRUE;
		m_bAlignToolLog				= FALSE;

		m_bImageName				= FALSE;
		m_bSaveNGImage				= FALSE;
		m_nSaveFailedImageNum		= 0;

		m_bSaveAllImage				= FALSE;
		m_nSaveAllImageNum			= 0;
		m_bSaveAllImageType			= FALSE;

		m_eImageFormat              = eBmp;
		m_nImageQuility             = 100;
		


		m_bSaveVDBFile              = FALSE;
		m_bVDBNameByID              =FALSE;
		m_nSaveVDBMaxNums			= 0;
		m_nVBDImageType				= 1;
		m_GuitextWeight			    = 1;// 存储VDB格式时图片字体大小
		m_GuiLineWidth  		    =1;// 存储VDB格式时图片线宽

		m_nVDBImageQuility             = 100;
		m_bSaveObjectSnapVDBFile     =TRUE;
		m_bSaveTargetSnapVDBFile     =TRUE;
		m_bSaveProcessSnapVDBFile     =TRUE;
		m_bSaveInsepectResultVDBFile = TRUE;

		m_bCustomLogPath            = FALSE;
		m_nDataRecordTimeNum		= 30;
		m_dDiskSpaceMaxUsePercent	= 90;
		m_strDataRecordPath         = _T("");
		m_nStartHourRecordByDay     =8;

//		m_bThreadSaveImageMode		= FALSE;


		
		m_bAlignerPrecision			= TRUE;		
		m_bAlignerTime				= TRUE;
		m_bDataMerged               = FALSE;
		m_bDataRecordTime			= FALSE;
//		m_bSysDataRecord			= TRUE;
//		m_strSaveAllImgDataAddr     = _T("C:\\ImgData");
//		m_strDataRecordTimeAddr     = _T("E:\\VSLog");


	}
	
	tagSysDataRecord& operator = (const tagSysDataRecord& lTPData)
	{
		if (this == &lTPData)
		{
			return *this;
		}

		m_bAlignerSearchPltfmInfo	= lTPData.m_bAlignerSearchPltfmInfo;
		m_bCommunication			= lTPData.m_bCommunication;
		m_bAlignToolLog				= lTPData.m_bAlignToolLog;

		m_bImageName				= lTPData.m_bImageName;
		m_bSaveNGImage				= lTPData.m_bSaveNGImage;
		m_nSaveFailedImageNum		= lTPData.m_nSaveFailedImageNum;

		m_bSaveAllImage				= lTPData.m_bSaveAllImage;
		m_nSaveAllImageNum			= lTPData.m_nSaveAllImageNum;
		m_bSaveAllImageType			= lTPData.m_bSaveAllImageType;

		m_eImageFormat              = lTPData.m_eImageFormat;
		m_nImageQuility             = lTPData.m_nImageQuility;

		m_bSaveVDBFile              = lTPData.m_bSaveVDBFile;
		m_bVDBNameByID				= lTPData.m_bVDBNameByID;
		m_nVDBImageQuility			=lTPData.m_nVDBImageQuility;
		m_nSaveVDBMaxNums			= lTPData.m_nSaveVDBMaxNums;
		m_nVBDImageType				= lTPData.m_nVBDImageType;
		m_bSaveObjectSnapVDBFile     =lTPData.m_bSaveObjectSnapVDBFile;
		m_bSaveTargetSnapVDBFile     =lTPData.m_bSaveTargetSnapVDBFile;
		m_bSaveProcessSnapVDBFile     =lTPData.m_bSaveProcessSnapVDBFile;
		m_bSaveInsepectResultVDBFile = lTPData.m_bSaveInsepectResultVDBFile;

		m_GuitextWeight             =lTPData.m_GuitextWeight;
		m_GuiLineWidth              =lTPData.m_GuiLineWidth;

		m_bCustomLogPath            = lTPData.m_bCustomLogPath;
		m_nDataRecordTimeNum		= lTPData.m_nDataRecordTimeNum;
		m_dDiskSpaceMaxUsePercent	= lTPData.m_dDiskSpaceMaxUsePercent;
		m_strDataRecordPath         = lTPData.m_strDataRecordPath;
		m_nStartHourRecordByDay		= lTPData.m_nStartHourRecordByDay;
//		m_bThreadSaveImageMode		= lTPData.m_bThreadSaveImageMode;
		
		m_bAlignerPrecision			= lTPData.m_bAlignerPrecision;		
		m_bAlignerTime				= lTPData.m_bAlignerTime;
		m_bDataMerged               = lTPData.m_bDataMerged;
		m_bDataRecordTime			= lTPData.m_bDataRecordTime;
//		m_bSysDataRecord			= lTPData.m_bSysDataRecord;
//		m_strDataRecordTimeAddr		= lTPData.m_strDataRecordTimeAddr;
//		m_strSaveAllImgDataAddr     = lTPData.m_strSaveAllImgDataAddr;
		
		return *this;
	}

} SysDataRecord,*PSysDataRecord;




// 对象补偿备注显示设置参数
typedef struct tagOffSetShowParam
{
	BOOL   m_bShowOffsetTips;		//是否显示对象补偿备注
	BOOL   m_bSetOffsetTip;			//是否允许修改对象补偿备注


	tagOffSetShowParam()
	{
		m_bShowOffsetTips = FALSE;
		m_bSetOffsetTip  = FALSE;
	}

	tagOffSetShowParam& operator = (const tagOffSetShowParam& lTPData)
	{
		m_bShowOffsetTips = lTPData.m_bShowOffsetTips;
		m_bSetOffsetTip  = lTPData.m_bSetOffsetTip;	
		return *this;
	}

}OffsetShowParam, *POffSetShowParam;

// 多段地址

//平台选项结构体
typedef struct tagPlatformOption
{
	BOOL m_bSearchWithDistanceInspect;//对象或目标拍照时是否进行线距检测（目前支持FOBC/FOBJ/FTGC/FTGT/FGTW/FGTP/FCHB）
	BOOL m_bSendInsDistanceInSearchResult;//启用线距检测后是否给PLC发送线距检测详细结果
	CString m_strOffsetPicturePath;//补偿方向图示的路径
	BOOL m_bEnableDmCodeCalib;//是否允许对象二维码标定
	int  m_nTarInfoCommModeInDiffPC;//对象和目标不在同一台控制器时，目标端和对象端目标信息的交互方式
	BOOL m_bEnableAlignAfterSearch;//是否允许拍照后对位（目前支持FOBC/FOBJ/FTGC/FTGT)
	BOOL m_bRecordCamFixMarkPos;//是否允许记录相机固定MARK位置功能（在相应位置候选模板4）
	BOOL m_bObjectSearchExProduct; //是否启用对象产品扩展，兼容老版本，老版本开启产品扩展功能时，对象拍照不用发扩展产品序号，该参数用于控制是否使用产品序号参数
	BOOL m_bTargetSearchExProduct;//是否启用目标产品扩展，兼容老版本，老版本开启产品扩展功能时，目标拍照不用发扩展产品序号，该参数用于控制是否使用产品序号参数
	BOOL m_bAlignExProduct;//是否启用对位产品扩展，该参数用于控制对位时是否使用产品序号参数
	BOOL    m_bSaveCalibData;			// 畸变校正，关联时保存图片和数据
	BOOL	m_bAutoCopyCalibData;		// 自动复制标定结果
	BOOL	m_bAutoCopyCalibDataExProduct;//自动拷贝标定结果到扩展 -需要m_bAutoCopyCalibData同时启用，且当前为产品扩展；只拷贝标定不拷贝关联信息
	BOOL m_bSendAreaInspectSubResult;//是否发送面积检测子结果到PLC备用寄存器(仅支持FCHB指令候补模板索引大于100时)
	BOOL	m_bReadWriteAxisPosWithCmd;	// TT 20200902	轴位置信息随指令参数一并读写
	double  m_dOffsetMaxX;				// 补偿最大值X 上限
	double  m_dOffsetMaxY;				// 补偿最大值Y 上限
	double  m_dOffsetMaxD;				// 补偿最大值D 上限
	double  m_dOffsetMaxX_LowerLimit;	// 补偿最大值X 下限
	double  m_dOffsetMaxY_LowerLimit;	// 补偿最大值Y 下限
	double  m_dOffsetMaxD_LowerLimit;	// 补偿最大值D 下限

	double  m_dAxisMoveMaxX;			// 防止撞击最大移动量+X 上限
	double  m_dAxisMoveMaxY;			// 防止撞击最大移动量+Y 上限
	double  m_dAxisMoveMaxD;			// 防止撞击最大移动量+D 上限

	double  m_dAxisMoveMaxX_LowerLimit;	// 防止撞击最大移动量-X 下限
	double  m_dAxisMoveMaxY_LowerLimit;	// 防止撞击最大移动量-Y 下限
	double  m_dAxisMoveMaxD_LowerLimit;	// 防止撞击最大移动量-D 下限
	// 像素判定
	BOOL	m_bOmitRePickObjectEnable;	// 是否启用忽略重复拾取（拾取不起来）的对象，拾取下一片。true：忽略；false：不忽略；
	double  m_dRePickOffsetMaxX;	// 认为是重复拾取时最大的偏差X，小于（X、Y同时满足）该偏差认为是重复拾取
	double  m_dRePickOffsetMaxY;	// 认为是重复拾取时最大的偏差Y，小于（X、Y同时满足）该偏差认为是重复拾取

	// 实际物理坐标判定
	BOOL	m_bOmitRePickObjectEnable_MM;	// 是否启用忽略重复拾取（拾取不起来）的对象，拾取下一片。true：忽略；false：不忽略；
	double  m_dRePickOffsetMaxX_MM;	// 认为是重复拾取时最大的偏差X，小于（X、Y同时满足）该偏差认为是重复拾取
	double  m_dRePickOffsetMaxY_MM;	// 认为是重复拾取时最大的偏差Y，小于（X、Y同时满足）该偏差认为是重复拾取

	OffsetShowParam			m_OffsetShowParam;					// 补偿备注显示设置参数记录

	TargetAutoCalibAlignMode m_eTarAutoCalibAlignMode;          //目标自动标定（目标也采用9宫格标定）时，对位模式
	BOOL m_bEnableCalibrateCheckByAxisMoveOption;
	double m_dCalibrateCheckAxisMoveRangeX;
	double m_dCalibrateCheckAxisMoveRangeY;
	double m_dCalibrateCheckAxisMoveRangeD;

	BOOL m_bSendMulAlignResult;					// 发送多个对位结果

	// 从选项中移植

	// 对位
	BOOL	m_bEnableMutiTarget;		// 多目标对位是否有效
	BOOL	m_bTargetUseFixedVirtualCoordinates;//目标对象分离，对象相机固定，对象平台移动拍照，目标使用对象关联的方式建立固定坐标系，不做目标修正。
	//BOOL    m_bEnableEightShapeAlign;

	BOOL	m_bCalibrateChangeXDirection;	//标定过程中反置轴的X方向
	BOOL	m_bCalibrateChangeYDirection;	//标定过程中反置轴的Y方向
	BOOL	m_bCalibrateChangeDDirection;	//标定过程中反置轴的D方向

	BOOL	m_bEnableCalibrateCheckBySize;	//启用标定验证--对象产品尺寸验证功能
	BOOL	m_bEnableRelevanceCheckBySize;	//启用关联验证--目标产品尺寸验证功能
	BOOL	m_bEnableCalibrateCheckByFixedTargetAligner;	//启用标定验证--通过轴移动产品验证尺寸

	//BOOL	m_bCalibSendAbsPos;             // 默认为ture----TRUE:标定时发送绝对轴坐标，FALSE:标定时发送相对值(相对基准轴位置)
	BOOL	m_bCalibSendRelativePos;		// 默认false----TRUE:标定时发送相对位置(相对当前位置)

	BOOL	m_bEnableMultiMarkMode;			// 手表贴合多标记点模式
	BOOL	m_bEnableAlignOffsetMarkByAera; // 是否根据区域进行补偿
	BOOL	m_bEnableTargetAndObjectPositionVirtualMode;	// 是否启用-目标和对象位置虚拟模式

	// 拾取相关
	BOOL	m_bLastPickSetting;			// 拾取判断是否为最后一片
	bool	m_bEnablePickPosUpdate;		// 拾取时旋转中心位置修正

	//拾取发送相对偏差量 相对偏差量是基于拾取标定轴，
	//大视野拾取时需要PLC在发送FALP指令时，发送轴为固定的轴位置，不能发送实时轴位置; 相对偏差量也是基于改固定的轴位置
	//而相机在在XY上模式，且不移动拍照时：PLC在发送FALP指令时，发送轴为实时对象轴位置，但是不能改变实时拍照位，此时相对偏差量是基于实时对象轴位置；
	// 而相机在在XY上模式，移动拍照时，PLC第二位置拍照位需要固定；此时相对偏差量是基于第二位置拍照位轴位置；
	BOOL	m_bEnablePickSendRelativateAxis; 

	//针对使用FAAL的做上料的时候，对固定点的时候，第一次拍照需要开启，后面关掉就行了，避免重复拍目标，减少对位时间
	BOOL	m_bFAALAlignOneTimeTargetSeachSnap; 
	
	// XY+D 相关
	BOOL	m_bObjMulSearchPosXYPD;
	BOOL    m_bTargetAngleMulAlignerXYPD;

	// 拍照相关

	BOOL    m_bAskStagePosEachMove;		// 每次平台移动前询问平台位置（不选择此项，对位速度加快）
	int		m_nSearchDelayTime;				// 定位失败后再次定位搜索延时
	BOOL	m_bAlignOneTime;			// 执行一次对位
	BOOL	m_bTargetPosSame;			// 拍摄目标位置重复
	BOOL	m_bObjectPosSame;			// 拍摄对象位置重复
	BOOL	m_bSaveAlignPos;			// 保存对位位置
	BOOL	m_bEnableSearchCindidate;	// 是否启用候补定位（候补指令可忽略）
	BOOL	m_bSearchTimes;				// 定位失败后再次定位
	int		m_nSearchTimes;				// 定位失败后再次定位次数
	BOOL	m_bEnableSearchShutter;		// 是否启用搜索定位时设置相机曝光时间
	BOOL    m_bManualAfTarSearchFail;   // 目标定位失败后手动选标
	BOOL    m_bManualAfObjSearchFail;   // 对象定位失败后手动选标
	BOOL	m_bAlignFinishClearSnapStatus;// 对位完成后清理拍照状态
	BOOL	m_bExeObjectCheckModeWhenAlign;	// 对位过程才执行检查模式，对象拍照时不执行检查模式，用于移动拍照情况，而难以在拍照过程中进行检查模式；
	BOOL	m_bExeTargetCheckModeWhenAlign;	// 对位过程才执行检查模式，对象拍照时不执行检查模式，用于移动拍照情况，而难以在拍照过程中进行检查模式；
	BOOL	m_bEnablePhotoLightCtrl;       //允许拍照时进行光源切换

	// 通信相关
	BOOL	m_bEnableAlignBenchCenter;	// 是否启用居中对位（如果启用，在寄存器通信的状态下，FAAL、FALG、FACO会单独将居中对位的轴位置写到指定寄存器）
	BOOL	m_bSetVisionManualSearchToPLC;		// 是否通知PLC视觉正在手动搜索
	BOOL	m_bVisionOnLineStatus;		// 视觉与PLC链接状态（寄存器通信）
	BOOL	m_bEnableCommMultiCalibExtension;	// 启用通信单产品多标定扩展

	BOOL m_bSetVisionManualSearchToPLC2;
	int m_nVisionManualSearchToPlcAddre;

	// 主界面相关
	BOOL    m_bManualAfTarSearchFailShowButton;   // 目标定位失败后手动选标显示按钮
	BOOL    m_bManualAfObjSearchFailShowButton;   // 对象定位失败后手动选标显示按钮
	BOOL	m_bShowFWEL;				// 显示焊接参数
//	BOOL	m_bShowPCB;					// 显示PCB参数
	BOOL	m_bShowEightShapeAlign;			// 显示八字型对位
	BOOL	m_bShowBend;

	BOOL	m_bTargetSeachSnapEverytime;
	BOOL	m_nBendFIsAlignFinished;	// 折弯对位完成判定方式  0- 对位完成     1-对位且折弯检测OK    3-仅折弯检测OK即

	std::vector<CMulRegTriggerSet> m_vnMulRegTriggerSet;		// 多段地址触发时参数设置;

	BOOL m_bEnableAngleCorrect;	// 启用旋转角偏差量修正;  用于拍照位置与贴合轴、下料位位置存在大角度时；视觉段进行偏移量角度处理
	BOOL m_dAngleCorrect;		// 旋转角度;  拍照位贴合位轴角度差；
	int m_nNumberOfInspectResults;//检测结果数量，存CSV或者发送检测结果时候的数量，例如华星的折弯检测发送4个结果，RX，RY，LX，LY，那么就写4
	CString m_sFCHBSaveCSVHead;
	CString m_StepID;
	CString	m_FieldCount;
	CString m_Production;
	CString m_FoamSize;
	CString m_BendingR;

	tagPlatformOption()
	{
		m_bSearchWithDistanceInspect = FALSE;
		m_bSendInsDistanceInSearchResult = FALSE;
		m_strOffsetPicturePath = _T("");
		m_bEnableDmCodeCalib = FALSE;
		m_nTarInfoCommModeInDiffPC = 0;
		m_bEnableAlignAfterSearch = FALSE;
		m_bRecordCamFixMarkPos = FALSE;
		m_bObjectSearchExProduct = FALSE;
		m_bTargetSearchExProduct = FALSE;
		m_bAlignExProduct = TRUE;
		m_bSaveCalibData = FALSE;
		m_bAutoCopyCalibData = FALSE;
		m_bAutoCopyCalibDataExProduct = FALSE;
		m_bSendAreaInspectSubResult = FALSE;

		m_dOffsetMaxX = 1;					// 补偿最大值X 上限
		m_dOffsetMaxY = 1;					// 补偿最大值Y 上限
		m_dOffsetMaxD = 0.5;				// 补偿最大值D 上限
		m_dAxisMoveMaxX = 5;				// 防止撞击最大移动量X 上限
		m_dAxisMoveMaxY = 5;				// 防止撞击最大移动量Y 上限
		m_dAxisMoveMaxD = 5;				// 防止撞击最大移动量D 上限

		m_dOffsetMaxX_LowerLimit = -1;		// 补偿最大值X 下限
		m_dOffsetMaxY_LowerLimit = -1;		// 补偿最大值Y 下限
		m_dOffsetMaxD_LowerLimit = -0.5;	// 补偿最大值D 下限
		m_dAxisMoveMaxX_LowerLimit = -5;	// 防止撞击最大移动量X 下限
		m_dAxisMoveMaxY_LowerLimit = -5;	// 防止撞击最大移动量Y 下限
		m_dAxisMoveMaxD_LowerLimit = -5;	// 防止撞击最大移动量D 下限

		// 像素判定
		m_bOmitRePickObjectEnable = FALSE;	// 是否启用忽略重复拾取（拾取不起来）的对象，拾取下一片。true：忽略；false：不忽略；
		m_dRePickOffsetMaxX = 10;			// 认为是重复拾取时最大的偏差X，小于（X、Y同时满足）该偏差认为是重复拾取
		m_dRePickOffsetMaxY = 10;			// 认为是重复拾取时最大的偏差Y，小于（X、Y同时满足）该偏差认为是重复拾取

		// 实际物理坐标判定
		m_bOmitRePickObjectEnable_MM = FALSE;	// 是否启用忽略重复拾取（拾取不起来）的对象，拾取下一片。true：忽略；false：不忽略；
		m_dRePickOffsetMaxX_MM = 0.1;			// 认为是重复拾取时最大的偏差X，小于（X、Y同时满足）该偏差认为是重复拾取
		m_dRePickOffsetMaxY_MM = 0.1;			// 认为是重复拾取时最大的偏差Y，小于（X、Y同时满足）该偏差认为是重复拾取
		m_eTarAutoCalibAlignMode = eAlignOnObject;//默认产品在对象端对位，然后移动固定距离到目标端
		m_bEnableCalibrateCheckByAxisMoveOption = FALSE;
		m_dCalibrateCheckAxisMoveRangeX = 2.0;
		m_dCalibrateCheckAxisMoveRangeY = 2.0;
		m_dCalibrateCheckAxisMoveRangeD = 2.0;
		m_bReadWriteAxisPosWithCmd = FALSE;
		m_bSendMulAlignResult = FALSE;

		// 对位
		m_bEnableMutiTarget = FALSE;
		m_bTargetUseFixedVirtualCoordinates = FALSE;
		m_bCalibrateChangeXDirection = FALSE;
		m_bCalibrateChangeYDirection = FALSE;
		m_bCalibrateChangeDDirection = FALSE;

		m_bEnableCalibrateCheckBySize = FALSE;	//启用标定验证--对象产品尺寸验证功能
		m_bEnableRelevanceCheckBySize= FALSE;	//启用关联验证--目标产品尺寸验证功能
		m_bEnableCalibrateCheckByFixedTargetAligner= FALSE;	//启用标定验证--通过轴移动产品验证尺寸

		//m_bCalibSendAbsPos = TRUE;
		m_bCalibSendRelativePos = FALSE;

		m_bEnableMultiMarkMode = FALSE;
		m_bEnableAlignOffsetMarkByAera = FALSE;
		m_bEnableTargetAndObjectPositionVirtualMode = FALSE;
		// 拾取相关
		m_bLastPickSetting = FALSE;
		m_bEnablePickPosUpdate = false;
		m_bEnablePickSendRelativateAxis = FALSE;
		// XY+D 相关
		m_bObjMulSearchPosXYPD = FALSE;
		m_bTargetAngleMulAlignerXYPD = FALSE;

		// 拍照相关
		m_bAskStagePosEachMove = TRUE;
		m_bAlignOneTime = FALSE;
		m_bTargetPosSame = FALSE;
		m_bObjectPosSame = FALSE;
		m_bSaveAlignPos = TRUE;
		m_bEnableSearchCindidate = FALSE;
		m_bSearchTimes = FALSE;
		m_nSearchTimes = 1;
		m_nSearchDelayTime = 0;
		m_bEnableSearchShutter = FALSE;
		m_bManualAfTarSearchFail =FALSE;
		m_bManualAfObjSearchFail = FALSE;
		m_bAlignFinishClearSnapStatus = FALSE;

		m_bEnablePhotoLightCtrl = FALSE;

		m_bExeObjectCheckModeWhenAlign = FALSE;
		m_bExeTargetCheckModeWhenAlign = FALSE;
		m_bFAALAlignOneTimeTargetSeachSnap=TRUE;

		// 通信相关
		m_bEnableAlignBenchCenter = FALSE;
		m_bSetVisionManualSearchToPLC = FALSE;
		m_bVisionOnLineStatus = FALSE;
		m_bEnableCommMultiCalibExtension = FALSE;

		m_bSetVisionManualSearchToPLC2 = FALSE;
		m_nVisionManualSearchToPlcAddre = 10000;

		//主界面相关
		m_bManualAfTarSearchFailShowButton = FALSE;
		m_bManualAfObjSearchFailShowButton = FALSE;
		m_bShowFWEL = FALSE;
//		m_bShowPCB = FALSE;
		m_bShowEightShapeAlign = FALSE;
		m_bShowBend = FALSE;
		m_bTargetSeachSnapEverytime = FALSE;
		m_nBendFIsAlignFinished = 0;
		m_vnMulRegTriggerSet.clear();
		m_vnMulRegTriggerSet.resize(2);
		m_vnMulRegTriggerSet[0].m_nMulIndex=1;
		m_vnMulRegTriggerSet[1].m_nMulIndex=2;
		m_vnMulRegTriggerSet[1].m_nMulRegOffset = 100;
		m_bEnableAngleCorrect = FALSE;
		m_nNumberOfInspectResults=4;
		m_dAngleCorrect = 0;
		//m_bInspectEnable = FALSE;
		m_sFCHBSaveCSVHead=_T("时间");
		m_StepID=_T("M6700N,CMRFA06,CAMTP01-INDX-RT01");
		m_FieldCount = _T("24");
		m_FoamSize = _T(",0,0,0,0");
		m_BendingR = _T(",0,0,0");
		m_Production = _T("YES,Production,Production");
	}
	tagPlatformOption& operator = (const tagPlatformOption& lTPData)
	{
		if (this == &lTPData)
		{
			return *this;
		}
		m_bSearchWithDistanceInspect = lTPData.m_bSearchWithDistanceInspect;
		m_bSendInsDistanceInSearchResult = lTPData.m_bSendInsDistanceInSearchResult;
		m_strOffsetPicturePath = lTPData.m_strOffsetPicturePath;
		m_bEnableDmCodeCalib = lTPData.m_bEnableDmCodeCalib;
		m_nTarInfoCommModeInDiffPC = lTPData.m_nTarInfoCommModeInDiffPC;
		m_bEnableAlignAfterSearch = lTPData.m_bEnableAlignAfterSearch;
		m_bRecordCamFixMarkPos = lTPData.m_bRecordCamFixMarkPos;
		m_bObjectSearchExProduct = lTPData.m_bObjectSearchExProduct;
		m_bTargetSearchExProduct = lTPData.m_bTargetSearchExProduct;
		m_bAlignExProduct = lTPData.m_bAlignExProduct;
		m_bSaveCalibData = lTPData.m_bSaveCalibData;
		m_bAutoCopyCalibData = lTPData.m_bAutoCopyCalibData;
		m_bAutoCopyCalibDataExProduct = lTPData.m_bAutoCopyCalibDataExProduct;

		m_bSendAreaInspectSubResult = lTPData.m_bSendAreaInspectSubResult;

		m_dOffsetMaxX = lTPData.m_dOffsetMaxX;					// 补偿最大值X
		m_dOffsetMaxY = lTPData.m_dOffsetMaxY;					// 补偿最大值Y
		m_dOffsetMaxD = lTPData.m_dOffsetMaxD;					// 补偿最大值D
		m_dAxisMoveMaxX = lTPData.m_dAxisMoveMaxX;				// 防止撞击最大移动量X
		m_dAxisMoveMaxY = lTPData.m_dAxisMoveMaxY;				// 防止撞击最大移动量Y
		m_dAxisMoveMaxD = lTPData.m_dAxisMoveMaxD;				// 防止撞击最大移动量D

		m_dOffsetMaxX_LowerLimit = lTPData.m_dOffsetMaxX_LowerLimit;
		m_dOffsetMaxY_LowerLimit = lTPData.m_dOffsetMaxY_LowerLimit;
		m_dOffsetMaxD_LowerLimit = lTPData.m_dOffsetMaxD_LowerLimit;					
		m_dAxisMoveMaxX_LowerLimit = lTPData.m_dAxisMoveMaxX_LowerLimit;				
		m_dAxisMoveMaxY_LowerLimit = lTPData.m_dAxisMoveMaxY_LowerLimit;				
		m_dAxisMoveMaxD_LowerLimit = lTPData.m_dAxisMoveMaxD_LowerLimit;				

		// 像素判定
		m_bOmitRePickObjectEnable = lTPData.m_bOmitRePickObjectEnable;	// 是否启用忽略重复拾取（拾取不起来）的对象，拾取下一片。true：忽略；false：不忽略；
		m_dRePickOffsetMaxX = lTPData.m_dRePickOffsetMaxX;			// 认为是重复拾取时最大的偏差X，小于（X、Y同时满足）该偏差认为是重复拾取
		m_dRePickOffsetMaxY = lTPData.m_dRePickOffsetMaxY;			// 认为是重复拾取时最大的偏差Y，小于（X、Y同时满足）该偏差认为是重复拾取

		// 实际物理坐标
		m_bOmitRePickObjectEnable_MM = lTPData.m_bOmitRePickObjectEnable_MM;	// 是否启用忽略重复拾取（拾取不起来）的对象，拾取下一片。true：忽略；false：不忽略；
		m_dRePickOffsetMaxX_MM = lTPData.m_dRePickOffsetMaxX_MM;			// 认为是重复拾取时最大的偏差X，小于（X、Y同时满足）该偏差认为是重复拾取
		m_dRePickOffsetMaxY_MM = lTPData.m_dRePickOffsetMaxY_MM;			// 认为是重复拾取时最大的偏差Y，小于（X、Y同时满足）该偏差认为是重复拾取
		m_OffsetShowParam = lTPData.m_OffsetShowParam;
		m_eTarAutoCalibAlignMode = lTPData.m_eTarAutoCalibAlignMode;
		m_bEnableCalibrateCheckByAxisMoveOption = lTPData.m_bEnableCalibrateCheckByAxisMoveOption;
		m_dCalibrateCheckAxisMoveRangeX =lTPData.m_dCalibrateCheckAxisMoveRangeX;
		m_dCalibrateCheckAxisMoveRangeY = lTPData.m_dCalibrateCheckAxisMoveRangeY;
		m_dCalibrateCheckAxisMoveRangeD = lTPData.m_dCalibrateCheckAxisMoveRangeD;
		m_bSendMulAlignResult = lTPData.m_bSendMulAlignResult;
		m_bReadWriteAxisPosWithCmd = lTPData.m_bReadWriteAxisPosWithCmd;
		m_bEnableMutiTarget = lTPData.m_bEnableMutiTarget;

		m_bTargetUseFixedVirtualCoordinates = lTPData.m_bTargetUseFixedVirtualCoordinates;
		m_bCalibrateChangeXDirection = lTPData.m_bCalibrateChangeXDirection;
		m_bCalibrateChangeYDirection = lTPData.m_bCalibrateChangeYDirection;
		m_bCalibrateChangeDDirection = lTPData.m_bCalibrateChangeDDirection;
		m_bEnableCalibrateCheckBySize = lTPData.m_bEnableCalibrateCheckBySize;
		m_bEnableRelevanceCheckBySize = lTPData.m_bEnableRelevanceCheckBySize;
		m_bEnableCalibrateCheckByFixedTargetAligner = lTPData.m_bEnableCalibrateCheckByFixedTargetAligner;
		//m_bCalibSendAbsPos = lTPData.m_bCalibSendAbsPos;
		m_bCalibSendRelativePos = lTPData.m_bCalibSendRelativePos;
		m_bEnableMultiMarkMode = lTPData.m_bEnableMultiMarkMode;
		m_bEnableAlignOffsetMarkByAera = lTPData.m_bEnableAlignOffsetMarkByAera;

		//m_bInspectEnable = lTPData.m_bInspectEnable;
		m_bEnableTargetAndObjectPositionVirtualMode = lTPData.m_bEnableTargetAndObjectPositionVirtualMode;

		m_bLastPickSetting = lTPData.m_bLastPickSetting;
		m_bEnablePickPosUpdate = lTPData.m_bEnablePickPosUpdate;
		m_bEnablePickSendRelativateAxis = lTPData.m_bEnablePickSendRelativateAxis;
		m_bObjMulSearchPosXYPD = lTPData.m_bObjMulSearchPosXYPD;
		m_bTargetAngleMulAlignerXYPD = lTPData.m_bTargetAngleMulAlignerXYPD;
		m_bAskStagePosEachMove = lTPData.m_bAskStagePosEachMove;
		m_bAlignOneTime = lTPData.m_bAlignOneTime;
		m_bTargetPosSame = lTPData.m_bTargetPosSame;
		m_bObjectPosSame = lTPData.m_bObjectPosSame;
		m_bSaveAlignPos = lTPData.m_bSaveAlignPos;
		m_bEnableSearchCindidate = lTPData.m_bEnableSearchCindidate;
		m_bSearchTimes = lTPData.m_bSearchTimes;
		m_nSearchTimes = lTPData.m_nSearchTimes;
        m_nSearchDelayTime = lTPData.m_nSearchDelayTime;
		m_bEnableSearchShutter = lTPData.m_bEnableSearchShutter;
		m_bManualAfTarSearchFail = lTPData.m_bManualAfTarSearchFail;
		m_bManualAfObjSearchFail = lTPData.m_bManualAfObjSearchFail;

		m_bEnablePhotoLightCtrl = lTPData.m_bEnablePhotoLightCtrl;

		m_bAlignFinishClearSnapStatus = lTPData.m_bAlignFinishClearSnapStatus;

		m_bExeObjectCheckModeWhenAlign = lTPData.m_bExeObjectCheckModeWhenAlign;
		m_bExeTargetCheckModeWhenAlign = lTPData.m_bExeTargetCheckModeWhenAlign;

		m_bEnableAlignBenchCenter = lTPData.m_bEnableAlignBenchCenter;
		m_bSetVisionManualSearchToPLC = lTPData.m_bSetVisionManualSearchToPLC;

		m_bSetVisionManualSearchToPLC2 = lTPData.m_bSetVisionManualSearchToPLC2;
		m_nVisionManualSearchToPlcAddre = lTPData.m_nVisionManualSearchToPlcAddre;

		m_bVisionOnLineStatus = lTPData.m_bVisionOnLineStatus;
		m_bEnableCommMultiCalibExtension = lTPData.m_bEnableCommMultiCalibExtension;
		m_bManualAfTarSearchFailShowButton = lTPData.m_bManualAfTarSearchFailShowButton;
		m_bManualAfObjSearchFailShowButton = lTPData.m_bManualAfObjSearchFailShowButton;
		m_bShowFWEL = lTPData.m_bShowFWEL;
//		m_bShowPCB = lTPData.m_bShowPCB;
		m_bShowEightShapeAlign = lTPData.m_bShowEightShapeAlign;
		m_bShowBend = lTPData.m_bShowBend;

		m_bTargetSeachSnapEverytime = lTPData.m_bTargetSeachSnapEverytime;
		m_nBendFIsAlignFinished = lTPData.m_nBendFIsAlignFinished;
		m_vnMulRegTriggerSet = lTPData.m_vnMulRegTriggerSet;
		m_bEnableAngleCorrect = lTPData.m_bEnableAngleCorrect;
		m_nNumberOfInspectResults=lTPData.m_nNumberOfInspectResults;
		m_dAngleCorrect = lTPData.m_dAngleCorrect;
		m_sFCHBSaveCSVHead=lTPData.m_sFCHBSaveCSVHead;
		m_StepID=lTPData.m_StepID;
		m_FoamSize=lTPData.m_FoamSize;
		m_BendingR=lTPData.m_BendingR;
		m_FieldCount=lTPData.m_FieldCount;
		m_Production=lTPData.m_Production;
		m_bFAALAlignOneTimeTargetSeachSnap=lTPData.m_bFAALAlignOneTimeTargetSeachSnap;
		return *this;
	}
}CPlatformOptionInfo,*PPlatformOptionInfo;


typedef struct tagPlatformPosLayoutInfo
{
	int  m_nViewLayoutType;						// 视图位置布局类型
	int	 m_nViewVisionRunWidth;					// 运行窗口宽度
	int  m_nViewMainLayoutType;					// 视图布局类型
	tagPlatformPosLayoutInfo()
	{
		 m_nViewLayoutType = 0;
		 m_nViewVisionRunWidth = 440;
		 m_nViewMainLayoutType = 0;
	}

	tagPlatformPosLayoutInfo& operator = (const tagPlatformPosLayoutInfo& lTPData)
	{
		 m_nViewLayoutType = lTPData.m_nViewLayoutType;
		 m_nViewVisionRunWidth = lTPData.m_nViewVisionRunWidth;
		 m_nViewMainLayoutType = lTPData.m_nViewMainLayoutType;
		 return *this;
	}

	
}CPlatformLayoutInfo,*PPlatformLayoutInfo;

//通信参数信息
typedef struct tagCommOptionInfo
{
	BOOL    m_bUsePlatformComm;
	int		m_nCommDelay;//发送延时
	int		m_nCommTimeOut;				// 串口超时
	int     m_nCommReSendTimes;			// 串口重复次数
	int		m_nCommXUnitExp;//X单位幂
	int		m_nCommYUnitExp;//Y单位幂
	int		m_nCommDUnitExp;//D单位幂
	int		m_nCommDataLenType;			// 串口数据长度：0：实际长度；1：固定长度；
	tagCommOptionInfo()
	{
		m_bUsePlatformComm = FALSE;
		m_nCommDelay = 0;					// 发送延时
		m_nCommTimeOut = 30;				// 串口超时
		m_nCommReSendTimes = 3;				// 串口重复次数
		m_nCommXUnitExp = 4;				// 串口X单位幂4
		m_nCommYUnitExp = 4;				// 串口Y单位幂4
		m_nCommDUnitExp = 5;				// 串口D单位幂5
		m_nCommDataLenType = 0;				// 串口数据长度：0：实际长度；1：固定长度；
	}
	tagCommOptionInfo& operator = (const tagCommOptionInfo& lTPData)
	{
		if (this == &lTPData)
		{
			return *this;
		}
		m_bUsePlatformComm = lTPData.m_bUsePlatformComm;        // 是否启用平台通信设置
		m_nCommDelay = lTPData.m_nCommDelay;					// 发送延时
		m_nCommTimeOut = lTPData.m_nCommTimeOut;				// 串口超时
		m_nCommReSendTimes = lTPData.m_nCommReSendTimes;				// 串口重复次数
		m_nCommXUnitExp = lTPData.m_nCommXUnitExp;				// 串口X单位幂4
		m_nCommYUnitExp = lTPData.m_nCommYUnitExp;				// 串口Y单位幂4
		m_nCommDUnitExp = lTPData.m_nCommDUnitExp;				// 串口D单位幂5
		m_nCommDataLenType = lTPData.m_nCommDataLenType;				// 串口数据长度：0：实际长度；1：固定长度；
		return *this;
	}
}CCommOptionInfo,*PCCommOptionInfo;

//通信共享类型
typedef enum tagShareCommType
{
	eShareCommNo=0,//不共享
	eShareCommSrc=1,//主通信接口	
	eShareCommTar=2,//副通信接口：共享主通信接口
}ShareCommType;

//平台的共享通信
typedef struct tagPlatShareComm
{
	tagPlatShareComm()
	{
		m_nPlatformIndex = -1;
		m_nShareCommIndex = -1;
		m_eCommShareType = eShareCommNo;
	}
	int m_nPlatformIndex;			// 平台序号
	int m_nShareCommIndex;			// 共享同一通信的接口序号
	ShareCommType m_eCommShareType; // 共享通信类型：主or副
}CPlatShareComm;

//单个通信接口的共享通信，与单个通信接口对应的所有平台信息（接口序号、平台数量、当前正在通信的平台、共享同一通信的平台列表）
typedef struct tagShareCommInfo
{
	tagShareCommInfo()
	{
		m_nShareCommIndex = -1;
		m_nShareCommPlatformNum = 0;		
		m_nCurCommPlatformIndex = -1;
		m_bCurCommChangeEnable = TRUE;
		InitializeCriticalSection(&m_csCurCommPlatformIndex);
	}
	~tagShareCommInfo()
	{
		m_vShareComms.clear();
		DeleteCriticalSection(&m_csCurCommPlatformIndex);
	}

	void Lock(){EnterCriticalSection(&m_csCurCommPlatformIndex);};
	void Unlock(){LeaveCriticalSection(&m_csCurCommPlatformIndex);};

	int m_nShareCommIndex;			// 共享同一通信的接口序号
	int m_nShareCommPlatformNum;	// 共享同一通信接口的平台数量
	int m_nCurCommPlatformIndex;	// 当前通信平台序号
	BOOL m_bCurCommChangeEnable;
	CRITICAL_SECTION    m_csCurCommPlatformIndex;
	std::vector<CPlatShareComm> m_vShareComms;
}CShareCommInfo;


//所有的共享通信信息，有N个用于共享的通信接口，则有N组共享通信信息，每组共享通信信息包括与其对应的平台列表
class CShareCommInfoArray
{
public:
	CShareCommInfoArray();
	~CShareCommInfoArray();

	// 清空平台共享通信信息
	void ClearPlatShareCommInfo();
	
	// 添加平台共享通信信息，即设置所有的共享通信信息
	// 说明：
	//		1.调用该函数前，需要调用ClearPlatShareCommInfo()清空平台共享通信信息，再将所有平台的共享信息加入
	//		2.如果bShareCommEnable == FALSE，那个该nPlatformIndex平台通信共享信息不会加入array中，那么查询该平台共享通信类型则返回eShareCommNo
	void AddPlatShareComm(int nPlatformIndex, int nShareCommIndex, BOOL bShareCommEnable);

	// 通过平台序号，查询其是否源通信接口
	ShareCommType GetShareCommType(int nPlatformIndex);

	// 通过平台序号，查询其对应的源共享通信接口序号
	int GetShareCommIndex(int nPlatformIndex);

	// 通过平台序号，查询其对应的共享通信接口序号
	int GetShareCommSrcPlatformIndex(int nPlatformIndex);

	// 通过平台序号，设置当前允许通信的平台序号
	BOOL SetCurCommPlatformIndex(int nPlatformIndex, int nCurCommPlatformIndex, BOOL bChangeEnable = TRUE);

	// 通过平台序号，查询当前允许通信的平台序号
	int GetCurCommPlatformIndex(int nPlatformIndex);

	// 通过平台序号，查询下一个允许通信的平台序号
	int GetNextCommPlatformIndex(int nPlatformIndex);


	// 加锁当前平台通信 
	BOOL LockCurPlatformIndex(int nCurCommPlatformIndex);

	// 解锁当前平台通信
	BOOL UnlockCurPlatformIndex(int nCurCommPlatformIndex);

protected:
	std::vector<CShareCommInfo*> m_vpShareCommInfos;
};

typedef struct tagCommParamInfo
{
	CommType			m_eCommType;				// 通信类型
	CmdParamOrderType	m_eCmdParamOrderType;		// 通信指令参数轴发送顺序类
	ProtocolType		m_eProtocolType;			// 通信协议类型
	ExProtocolType		m_eExProtocolType;			// 扩展通信协议类型；
	bool				m_bRegiterType;				// 读写PLC寄存器是单字节/双字节（默认是双字节=TRUE）
	int					m_nRegDataTypeIndex;		// 寄存器地址数据类型序号 DM EM
	MotionControlType	m_eMotionControlType;		// 运动控制类型
	BOOL                m_bUseSpecialComm;          // 与PCL是否使用特殊握手命令，诚亿定制

	tagCommParamInfo()
	{
		m_eCommType				= eSerial;
		m_eCmdParamOrderType	= eCPOrderXYD;
		m_eProtocolType			= eProCmdFast;
		m_eExProtocolType		= eExProDefault;
		m_eMotionControlType	= eMCNo;
		m_bRegiterType			= TRUE;		// 读写PLC寄存器是单字节/双字节（默认是双字节=TURE）
		m_nRegDataTypeIndex		= 0;		// 寄存器地址数据类型序号
		m_bUseSpecialComm		= FALSE;
	}

	tagCommParamInfo& operator = (const tagCommParamInfo& lTPData)
	{
		if (this == &lTPData)
		{
			return *this;
		}

		m_eCommType				= lTPData.m_eCommType;
		m_eCmdParamOrderType	= lTPData.m_eCmdParamOrderType;
		m_eProtocolType			= lTPData.m_eProtocolType;
		m_eExProtocolType		= lTPData.m_eExProtocolType;
		m_eMotionControlType	= lTPData.m_eMotionControlType;
		m_bRegiterType			= lTPData.m_bRegiterType;		// 读写PLC寄存器是单字节/双字节（默认是双字节=TURE）
		m_nRegDataTypeIndex		= lTPData.m_nRegDataTypeIndex;		// 寄存器地址数据类型序号
		m_bUseSpecialComm		= lTPData.m_bUseSpecialComm;

		return *this;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		// CommType
		if(FALSE == cXMLConfigurator.FindElem(_T("CommType")))
		{
			cXMLConfigurator.AddElem(_T("CommType"));
		}
		cXMLConfigurator.SetElemData((int)m_eCommType);

		// CmdParamOrderType
		if(FALSE == cXMLConfigurator.FindElem(_T("CmdParamOrderType")))
		{
			cXMLConfigurator.AddElem(_T("CmdParamOrderType"));
		}
		cXMLConfigurator.SetElemData((int)m_eCmdParamOrderType);

		// ProtocolType
		if(FALSE == cXMLConfigurator.FindElem(_T("ProtocolType")))
		{
			cXMLConfigurator.AddElem(_T("ProtocolType"));
		}
		cXMLConfigurator.SetElemData((int)m_eProtocolType);
	
		// ExProtocolType
		if(FALSE == cXMLConfigurator.FindElem(_T("ExProtocolType")))
		{
			cXMLConfigurator.AddElem(_T("ExProtocolType"));
		}
		cXMLConfigurator.SetElemData((int)m_eExProtocolType);

		// RegDataTypeIndex
		if(FALSE == cXMLConfigurator.FindElem(_T("RegDataTypeIndex")))
		{
			cXMLConfigurator.AddElem(_T("RegDataTypeIndex"));
		}
		cXMLConfigurator.SetElemData(m_nRegDataTypeIndex);

		// MotionControlType
		if(FALSE == cXMLConfigurator.FindElem(_T("MotionControlType")))
		{
			cXMLConfigurator.AddElem(_T("MotionControlType"));
		}
		cXMLConfigurator.SetElemData((int)m_eMotionControlType);

		// RegiterType
		if(FALSE == cXMLConfigurator.FindElem(_T("RegiterType")))
		{
			cXMLConfigurator.AddElem(_T("RegiterType"));
		}
		cXMLConfigurator.SetElemData(m_bRegiterType);

		// UseSpecialComm
		if(FALSE == cXMLConfigurator.FindElem(_T("UseSpecialComm")))
		{
			cXMLConfigurator.AddElem(_T("UseSpecialComm"));
		}
		cXMLConfigurator.SetElemData(m_bUseSpecialComm);


	}
	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		int nData = 0;
		// CommType
		if (cXMLConfigurator.FindElem(_T("CommType")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_eCommType = (CommType)nData;
		}

		// CmdParamOrderType
		if (cXMLConfigurator.FindElem(_T("CmdParamOrderType")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_eCmdParamOrderType = (CmdParamOrderType)nData;
		}

		// ProtocolType
		if (cXMLConfigurator.FindElem(_T("ProtocolType")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_eProtocolType = (ProtocolType)nData;
		}

		// ExProtocolType
		if (cXMLConfigurator.FindElem(_T("ExProtocolType")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_eExProtocolType = (ExProtocolType)nData;
		}

		// RegDataTypeIndex
		if (cXMLConfigurator.FindElem(_T("RegDataTypeIndex")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_nRegDataTypeIndex = nData;
		}

		// MotionControlType
		if (cXMLConfigurator.FindElem(_T("MotionControlType")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_eMotionControlType = (MotionControlType)nData;
		}

		// RegiterType
		if (cXMLConfigurator.FindElem(_T("RegiterType")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_bRegiterType = (BOOL)nData;
		}

		// UseSpecialComm
		if (cXMLConfigurator.FindElem(_T("UseSpecialComm")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_bUseSpecialComm = (BOOL)nData;
		}
	}

}CCommParamInfo;

// 权限项
typedef struct tagPermissionEntries
{
	bool m_bShow;	// 属性项 TRUE 显示,FALSE不显示;
	bool m_bEnable; // 属性项 TRUE可编辑,FALSE不可编辑

	bool m_bShowAllowSet;	//是否开放到界面设置
	bool m_bEnableAllowSet; //是否开放到界面设置

	bool m_bGroup;	// TRUE 分组项,   FALSE：默认代表存在具体属性项；

	CString m_StrName;	// 名称
	int m_nIndex;		// 索引值

	tagPermissionEntries()
	{
		m_bShow = true;
		m_bEnable = true;

		m_bShowAllowSet = true;
		m_bEnableAllowSet = true;

		m_bGroup = false;
		m_StrName = _T("");
		m_nIndex = -1;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		// m_bShow
		if(FALSE == cXMLConfigurator.FindElem(_T("bShowControl")))
		{
			cXMLConfigurator.AddElem(_T("bShowControl"));
		}
		cXMLConfigurator.SetElemData(m_bShow);

		// m_bEnable
		if(FALSE == cXMLConfigurator.FindElem(_T("bEnableControl")))
		{
			cXMLConfigurator.AddElem(_T("bEnableControl"));
		}
		cXMLConfigurator.SetElemData(m_bEnable);

		// m_bShowAllowSet
		if(FALSE == cXMLConfigurator.FindElem(_T("bShowAllowSet")))
		{
			cXMLConfigurator.AddElem(_T("bShowAllowSet"));
		}
		cXMLConfigurator.SetElemData(m_bShowAllowSet);

		// m_bEnableAllowSet
		if(FALSE == cXMLConfigurator.FindElem(_T("bEnableAllowSet")))
		{
			cXMLConfigurator.AddElem(_T("bEnableAllowSet"));
		}
		cXMLConfigurator.SetElemData(m_bEnableAllowSet);

		// m_bGroup
		if(FALSE == cXMLConfigurator.FindElem(_T("bGroup")))
		{
			cXMLConfigurator.AddElem(_T("bGroup"));
		}
		cXMLConfigurator.SetElemData(m_bGroup);

		// m_StrName
		if(FALSE == cXMLConfigurator.FindElem(_T("StrName")))
		{
			cXMLConfigurator.AddElem(_T("StrName"));
		}
		cXMLConfigurator.SetElemData(m_StrName);

		// m_nIndex
		if(FALSE == cXMLConfigurator.FindElem(_T("nIndex")))
		{
			cXMLConfigurator.AddElem(_T("nIndex"));
		}
		cXMLConfigurator.SetElemData(m_nIndex);

	}
	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		int nData = 0;
		// m_nShow
		if (cXMLConfigurator.FindElem(_T("bShowControl")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_bShow = nData;
		}

		// m_bEnable
		if (cXMLConfigurator.FindElem(_T("bEnableControl")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_bEnable = nData;
		}

		// m_bShowAllowSet
		if (cXMLConfigurator.FindElem(_T("bShowAllowSet")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_bShowAllowSet = nData;
		}

		// m_bEnableAllowSet
		if (cXMLConfigurator.FindElem(_T("bEnableAllowSet")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_bEnableAllowSet = nData;
		}

		// m_bGroup
		if (cXMLConfigurator.FindElem(_T("bGroup")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_bGroup = nData;
		}

		// m_StrName
		if (cXMLConfigurator.FindElem(_T("StrName")))// Level 4
		{
			m_StrName= (CString)cXMLConfigurator.GetElemData();
		}

		// m_nIndex
		if (cXMLConfigurator.FindElem(_T("nIndex")))// Level 4
		{
			cXMLConfigurator.GetElemData(nData);	
			m_nIndex = nData;
		}

	}

}CPermissionEntries;
#define Group_Max_Num	100
// 权限项分类
typedef enum tagAuthorizeGroup
{
	eGroupUnknown	= -1,										// 未知组
	eAuthorizeGroup_Camera			= 1 * Group_Max_Num,		// 相机设置
	eAuthorizeGroup_Comm			= 2 * Group_Max_Num,		// 通信设置
	eAuthorizeGroup_Platform		= 3 * Group_Max_Num,		// 平台设置
	eAuthorizeGroup_Log				= 4 * Group_Max_Num,		// 日志设置
	eAuthorizeGroup_PlatformOption	= 5 * Group_Max_Num,		// 平台选项
	eAuthorizeGroup_SoftwareOption	= 6 * Group_Max_Num,		// 软件选项
	eAuthorizeGroup_Calibration		= 7 * Group_Max_Num,		// 标定设置
	eAuthorizeGroup_Product			= 8 * Group_Max_Num,		// 产品设置
	eAuthorizeGroup_Other			= 10 * Group_Max_Num,		// 终止项
}CAuthorizeGroup;
// 权限设置项
typedef enum tagAuthorizeItem
{
	eAuthorizeItemUnknown = -1,					// 未知类型

	// 相机设置
	eCamremaAuthorize_Node				= eAuthorizeGroup_Camera + 0,
	eCamremaAuthorize_ParamSet			= eAuthorizeGroup_Camera + 1,		// 参数设置
	eCamremaAuthorize_ShowSet			= eAuthorizeGroup_Camera + 2,		// 显示设置
	eCamremaAuthorize_ColorSet			= eAuthorizeGroup_Camera + 3,		// 彩色相机设置
	eCamremaAuthorize_OtherItem			= eAuthorizeGroup_Camera + 4,		// 其他项

	// 通信设置
	eCommAuthorize_Node					= eAuthorizeGroup_Comm + 0,
	eCommAuthorize_ParamSet				= eAuthorizeGroup_Comm + 1,			// 通信参数
	eCommAuthorize_CommTypeSet			= eAuthorizeGroup_Comm + 2,			// 通信连接类型
	eCommAuthorize_TestSet				= eAuthorizeGroup_Comm + 3,			// 连接读写测试
	eCommAuthorize_OtherSet				= eAuthorizeGroup_Comm + 4,			// 其他通信设置
	eCommAuthorize_CommDelaySet			= eAuthorizeGroup_Comm + 5,			// 通信延时超时设置

	// 平台设置
	ePlatformAuthorize_Node				= eAuthorizeGroup_Platform + 0,
	ePlatformAuthorize_ParamSet			= eAuthorizeGroup_Platform + 1,		// 对位平台参数
	ePlatformAuthorize_Camplatform		= eAuthorizeGroup_Platform + 2,		// 相机平台参数


	// 日志设置
	eLogAuthorize_Node					= eAuthorizeGroup_Log + 0,
	eLogAuthorize_Content				= eAuthorizeGroup_Log + 1,			// 日志保存内容
	eLogAuthorize_SaveImg				= eAuthorizeGroup_Log + 2,			// 存图
	eLogAuthorize_PathSet				= eAuthorizeGroup_Log + 3,			// 路径设置
	eLogAuthorize_SaveByHour			= eAuthorizeGroup_Log + 4,			// 按小时存图
	eLogAuthorize_SaveVDB				= eAuthorizeGroup_Log + 5,			// 存VDB

	// 平台选项
	ePlatformOptionAuthorize_Node					= eAuthorizeGroup_PlatformOption + 0,
	ePlatformOptionAuthorize_SearchTool				= eAuthorizeGroup_PlatformOption + 1,		// 定位工具
	ePlatformOptionAuthorize_OffSet					= eAuthorizeGroup_PlatformOption + 2,		// 补偿设置
	ePlatformOptionAuthorize_AlignType				= eAuthorizeGroup_PlatformOption + 3,		// 对位方式设置
	ePlatformOptionAuthorize_AlignOption			= eAuthorizeGroup_PlatformOption + 4,		// 对位参数设置
	ePlatformOptionAuthorize_PlatformComm			= eAuthorizeGroup_PlatformOption + 5,		// 平台通信设置
	ePlatformOptionAuthorize_SnapSet				= eAuthorizeGroup_PlatformOption + 6,		// 拍照设置
	ePlatformOptionAuthorize_PickSet				= eAuthorizeGroup_PlatformOption + 7,		// 拾取设置
	ePlatformOptionAuthorize_CalibrationSet			= eAuthorizeGroup_PlatformOption + 8,		// 标定关联设置
	ePlatformOptionAuthorize_Inspect				= eAuthorizeGroup_PlatformOption + 9,		// 检测
	ePlatformOptionAuthorize_More					= eAuthorizeGroup_PlatformOption + 10,		// 更多设置

	// 软件选项
	eOptionAuthorize_Node				= eAuthorizeGroup_SoftwareOption + 0,
	eOptionAuthorize_Language			= eAuthorizeGroup_SoftwareOption + 1,		// 语言设置
	eOptionAuthorize_StartSet			= eAuthorizeGroup_SoftwareOption + 2,		// 启动设置
	eOptionAuthorize_KeyBoard			= eAuthorizeGroup_SoftwareOption + 3,		// 键盘设置
	eOptionAuthorize_LogShowSet			= eAuthorizeGroup_SoftwareOption + 4,		// 信息显示设置
	eOptionAuthorize_SystemSwitch		= eAuthorizeGroup_SoftwareOption + 5,		// 系统切换、注册设置
	eOptionAuthorize_CheckModeSet		= eAuthorizeGroup_SoftwareOption + 6,		// 检查模式
	eOptionAuthorize_Interface			= eAuthorizeGroup_SoftwareOption + 7,		// 界面显示设置
	eOptionAuthorize_WSlayout			= eAuthorizeGroup_SoftwareOption + 8,		// 工位布局设置
	eOptionAuthorize_OtherSet			= eAuthorizeGroup_SoftwareOption + 9,		// 其他设置

	//标定设置
	eCalibrateAuthorize_Node					= eAuthorizeGroup_Calibration + 0,
	eCalibrateAuthorize_XYDMove					= eAuthorizeGroup_Calibration + 1,				// 标定输入参数
	eCalibrateAuthorize_DistortionCorrection	= eAuthorizeGroup_Calibration + 2,				// 畸变矫正
	eCalibrateAuthorize_TemplateSet				= eAuthorizeGroup_Calibration + 3,				// 模板设置
	eCalibrateAuthorize_CalibrationAlgorithm	= eAuthorizeGroup_Calibration + 4,				// 标定算法
	eCalibrateAuthorize_DmCodeSet				= eAuthorizeGroup_Calibration + 5,				// 二维码标定
	eCalibrateAuthorize_CopyCalib				= eAuthorizeGroup_Calibration + 6,				// 拷贝标定
	eCalibrateAuthorize_CalibList				= eAuthorizeGroup_Calibration + 7,				// 标定产品列表

	// 产品设置
	eProductAuthorize_Node							= eAuthorizeGroup_Product + 0,
	eProductAuthorize_CheckMode						= eAuthorizeGroup_Product + 1,					// 检查模式
	eProductAuthorize_Offset						= eAuthorizeGroup_Product + 2,					// 对位补偿
	eProductAuthorize_AlignToolParamSet				= eAuthorizeGroup_Product + 3,					// 对位延时、精度
	eProductAuthorize_AlignTypeSet					= eAuthorizeGroup_Product + 4,					// 对位模式切换设置
	eProductAuthorize_AlignOther					= eAuthorizeGroup_Product + 5,					// 其他对位设置

	eCalibrateAuthorize_ProudctList					= eAuthorizeGroup_Product + 6,					// 产品列表
	eProductAuthorize_TargetTemplateSet				= eAuthorizeGroup_Product + 7,					// 
	eProductAuthorize_ObjectTemplateSet				= eAuthorizeGroup_Product + 8,					// 对象模板界面设置
	eProductAuthorize_VirtualObjectTemplateSet		= eAuthorizeGroup_Product + 9,					// 虚拟模板界面设置
	eProductAuthorize_CalibTargetTemplateSet		= eAuthorizeGroup_Product + 10,					// 关联模板界面设置
	eProductAuthorize_InspectTemplateSet			= eAuthorizeGroup_Product + 11,					// 检测模板界面设置
	
	eProductAuthorize_SpecialApplicationSet			= eAuthorizeGroup_Product + 12,					// 特殊类应用设置
	// 其他设置
	eOtherAuthorize_Node					= eAuthorizeGroup_Other + 0,							// 其他设置


}CAuthorizeItem;
// 自定义树结构
template<typename Type>
class CTree
{
public:
	Type node;
	Type father;
	std::vector<CTree> children;

	CTree(const Type & root)
	{
		this->node=root;
		this->father=NULL;
	}

	CTree()
	{
		this->father=NULL;
		this->node=NULL;
	}

	void AddChild(CTree child)
	{
		child.father=(this->node);
		children.push_back(child);
	}

	void AddChild(Type node)
	{
		CTree addTree=CTree(node);
		addTree.father=this->node;
		children.push_back(addTree);
	}

};


#define MaxCodeNums 14 //最大指令码个数

// 单个平台的运行参数
class CVisionASMConfig
{
public:
	CVisionASMConfig();

	// 检查m_TotalProductData与m_TotalCalibData的一致性
	BOOL CheckProductDataCalibDataConsistency();
public:

	CString					m_strConfigPath;					// 配置文件XML全路径

	CPlatformInfo*			m_pPlatformInfo;					// 对位平台信息
	CCamPlatformInfo*		m_pCamPlatformInfo;					// 相机平台信息
	CPlatformInfo*			m_pMidPlatformInfo;					// 中间对位平台信息
	
	vcCommInfo*				m_pCommParam;						// 串口通信信息：串口或网口
	CCommParamInfo			m_CommParamInfo;					// 通信参数信息 包括通信类型、通信协议等

	std::vector<CPosInfo>    m_vPosInfo;						// 位置信息：关联的相机索引				
	std::vector<CameraParam> m_vCameraParam;					// 相机信息：以序列号采集
	std::vector<ImageRoi>	m_vImageRoi;						// 图像显示参数
	
//	CCalibrateParam			m_CalibrateParam;					// 标定参数								与平台相关
//	std::vector<CCalibratedInfo*> m_vpCalibratedInfo;			// 标定后参数						    与平台相关
	CTotalProductData	    m_TotalProductData;				    // 产品所对应的标定信息和对位信息		与平台相关
	CTotalCalibData			m_TotalCalibData;					//

	SysDataRecord			m_DataRecord;						// 中间数据记录
	CPlatformOptionInfo     m_PlatformOptionInfo;               // 平台选项信息
	CPlatformLayoutInfo		m_PlatformLayoutInfo;				// 工位位置布局配置
	CCommOptionInfo          m_commOptionInfo;                    // 实际使用的通信选项信息
	CCommOptionInfo          m_commOptionInfoXML;               //XML文件中存储的通信选项信息


	CTree<CPermissionEntries*> m_cTreeAuthorizeManagement;		// 权限管理
	std::map<int,std::pair<CString,std::pair<int,CString>>> m_MapAuthorizeItem; // 权限项的名称、保存字符名称
	CTree<CPermissionEntries*> CreateAuthorizeTree(CAuthorizeItem eItem);		// 根据权限项创建树形结构
	void DeleteTree();															// 释放m_cTreeAuthorizeManagement内存
	void DeleteTreeChild(std::vector<CTree<CPermissionEntries*>> &children);	// 递归释放树的子节点内存
	bool FindItem(const CAuthorizeItem eItem,std::vector<CTree<CPermissionEntries*>> &children,CString &str,bool &bShow,bool &bEenable);
	bool GetCheckedByItem(const CAuthorizeItem eItem,CTree<CPermissionEntries*> m_cTreepair,CString &str,bool &bShow,bool &bEenable);

	void SaveToXML(vcXMLConfigurator& cXMLConfigurator,std::vector<CTree<CPermissionEntries*>> &children); // m_cTreeAuthorizeManagement保存XML
	void LoadFromXML(vcXMLConfigurator& cXMLConfigurator,std::vector<CTree<CPermissionEntries*>> &children);


	std::vector<CKeyenceCodeParam> m_vKeyenceCodeParam; //指令码
};

#define DMCODECORNERSNUM 200

class CGuiGraphics
{
public:
	CGuiGraphics();
	virtual ~CGuiGraphics();

	std::vector<scGuiCross*>				m_vGuiObjectCross;
	std::vector<BOOL>                       m_vbShowObjectCross;

	std::vector<scGuiCoordCross*>			m_vGuiObjectCoordCross;
	std::vector<BOOL>                       m_vbShowObjectCoordCross;

	std::vector<scGuiPolyline*>             m_vGuiObjectPolyline;
	std::vector<BOOL>                       m_vbShowObjectPolyline;

	std::vector<scGuiCross*>				m_vGuiTargetCross;
	std::vector<BOOL>                       m_vbShowTargetCross;

	std::vector<scGuiCoordCross*>			m_vGuiTargetCoordCross;
	std::vector<BOOL>                       m_vbShowTargetCoordCross;

	std::vector<scGuiPolyline*>             m_vGuiTargetPolyline;
	std::vector<BOOL>                       m_vbShowTargetPolyline;

};

/************************************************************************************
/* Function name	: CreateDirectoryPro
/* Description	    : 创建目录，父目录不要求一定存在。如果父目录不存在，则依次创建之。
/* Return type		: BOOL 
/* Argument
/* [in] --  LPCTSTR lpszDir:	目录名
/************************************************************************************/
BOOL CreateDirectoryPro(LPCTSTR lpszDir);
BOOL ClearDirectoryPro(LPCTSTR lpszDir);	// 清空目录
BOOL RemoveDirectoryPro(LPCTSTR lpszDir);	// 删除非空目录
BOOL RenameDirecotryPro(LPCTSTR lpszOldDir, LPCTSTR lpszNewDir);
void AlignToolLogRecord(CString strlog,int nPlatformIndex);

////20160510增加对角补偿
///************************************************************************************
///* Function name	: AngleOffsetCalculate
///* Description	    : 根据产品尺寸计算对角线角度偏移。
///* Return type		: bool 
///* Argument
///* [in] --  double dObjectL1:	对象水平长度L1
///* [in] --  double dObjectW1:	对象垂直长度W1
///* [in] --  double dTargetL2:	目标水平长度L2
///* [in] --  double dTargetW2:	目标水平长度W2
///* [in] --  double nPosMode:	对角拍照模式 0:左上加右下; 1:右上加左下
///* [in\out] --  scDegree& angleOffset:	返回对角线角度偏移
///************************************************************************************/
//bool AngleOffsetCalculate(double dObjectL1, double dObjectW1, double dTargetL2, double dTargetW2, int nPosMode, scDegree& angleOffset);
//
////20160510增加对角补偿
///************************************************************************************
///* Function name	: XYOffsetCalculate
///* Description	    : 根据产品尺寸计算对角线角度偏移。
///* Return type		: bool 
///* Argument
///* [in] --  double dObjectL1:	对象水平长度L1
///* [in] --  double dObjectW1:	对象垂直长度W1
///* [in] --  double dTargetL2:	目标水平长度L2
///* [in] --  double dTargetW2:	目标水平长度W2
///* [in] --  double dF1:			对象目标水平偏移F1
///* [in] --  double dF2:			对象目标水平偏移F2
///* [in] --  double nPosMode:	对角拍照模式 0:左上加右下; 1:右上加左下
///* [in\out] --  double& dOffsetX:	返回X偏移
///* [in\out] --  double& dOffsetY:	返回Y偏移
///************************************************************************************/
//bool XYOffsetCalculate(double dObjectL1, double dObjectW1, double dTargetL2, double dTargetW2, double dF1, double dF2, int nPosMode, double& dOffsetX, double& dOffsetY);
//
//
////20160510增加对角补偿
///************************************************************************************
///* Function name	: AngleDiagonalCalculate
///* Description	    : 根据产品尺寸计算对角线角度。
///* Return type		: bool 
///* Argument
///* [in] --  double dObjectL1:	对象水平长度L1
///* [in] --  double dObjectW1:	对象垂直长度W1
///* [in] --  double nPosMode:	对角拍照模式 0:左上加右下; 1:右上加左下
///* [in\out] --  scDegree& angleDiagonal:	返回对角线角度
///************************************************************************************/
//bool AngleDiagonalCalculate(double dObjectL1, double dObjectW1, int nPosMode, scDegree& angleDiagonal);

//20161020从字符串数组获取字符串
/************************************************************************************
/* Function name	: GetSysInfoString
/* Description	    : 从字符串数组获取字符串
/* Return type		: CString 
/* Argument
/* [in] --  CStringArray psaSysInfoStrings: 字符串数组
/* [in] --  int nIndex:	序号
/************************************************************************************/
CString GetSysInfoString(CStringArray	*psaSysInfoStrings, int nIndex);

CString GetLogDirecotryPath();