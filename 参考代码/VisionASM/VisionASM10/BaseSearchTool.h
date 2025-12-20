// BaseSearchTool.h: interface for the CBaseSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASESEARCHTOOL_H__CE9FD7C2_B8EE_48EE_99D1_907E23E841E0__INCLUDED_)
#define AFX_BASESEARCHTOOL_H__CE9FD7C2_B8EE_48EE_99D1_907E23E841E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "svGuiDisplay.h"
#include "svImage.h"
#include "svAffineRect.h"
#include "svObround.h"
#include "svGenRect.h"
#include "svXMLConfigurator.h"
#include "svCodeDM.h"
#include "svCodeQR.h"
#include "svCode1D.h"
#pragma pack(push,8)

#define MAXRESNUM		40
#define VISIONPLUS_IMGPROC
#define MAXPTSNUM       4000
// #ifdef SV_SEARCHTOOL_EXPORTS 
// #define SV_SEARCHTOOL_API _declspec(dllexport)
// #else
// #define SV_SEARCHTOOL_API _declspec(dllimport)
// #endif

// 定位工具类型
typedef enum tagSearchToolType
{
	eSearchToolUnknown					= -1,	// 未知类型
	eQuickSearchTool					= 0,	// QuickSearch定位工具
	ePatternSearchTool					= 1,	// 区域定位工具
	eLineSearchTool						= 2,	// 线定位工具
	eCornerSearchTool					= 3,	// 角定位工具
	eCircleSearchTool					= 4,	// 圆定位工具
	eFixedPosSearchTool					= 5,	// 固定位置定位工具
	eLineSearch2Tool					= 6,	// 找线工具2
	eCrossSearchTool					= 7,	// 十字mark定位工具
	ePatCornerSearchTool				= 8,    // 组合区域角定位
	eProCircleSearchTool				= 9,    // 专用圆定位工具
	ePatternCornerSearchTool			= 10,	// 区域角定位工具	
	eCombinedQuickSearchTool			= 11,	// 组合区域角定位工具
	eCombinedPatternSearchTool			= 12,	// 组合区域角定位工具
	eBaseCornerSearchTool				= 13,	// 基础角定位工具
	ePatternSearchCheckTool				= 14,	// 区域定位检查工具
	eQuickSearchCheckTool				= 15,	// 几何定位检查工具
	eQuickSearchObroundTool				= 16,	// 几何长圆定位工具
	ePatternSearchObroundTool			= 17,	// 区域长圆定位工具
	eCircleSearchTool2					= 18,	// 圆定位工具2(TPK点胶针头定位)
	eGCCPatternCornerSearchTool			= 19,   // GCC专用角定位工具
	eMPatternSearchTool					= 20,	// 多标记点区域定位工具
	eMQuickSearchTool					= 21,	// 多标记点几何定位工具
	eMQuickCornerSearchTool				= 22,	// 多标记点几何角定位工具
	eQuickSearchGenRectTool				= 23,	// 几何圆角矩形定位工具
	ePatternSearchGenRectTool			= 24,	// 区域圆角矩形定位工具
	ePatternLineSearchTool				= 25,	// 区域线定位工具
	e2PatternSearchTool					= 26,	// 2区域定位工具，分别修真各自的找线工具，两线的交点作为定位结果，两线段中点分别作为辅助参考点返回
	eQuickLineCircleSearchTool			= 27,	// 几何定位+线定位+圆定位
	ePatternLineCircleSearchTool		= 28,	// 区域定位+线定位+圆定位
	ePatternCircleSearchTool			= 29,	// 区域圆定位工具
	eQuickSearchCheckLineTool			= 30,	// 几何检查线定位工具
	ePatternSearchCheckLineTool			= 31,	// 区域检查线定位工具
	ePatternBlobSearchTool				= 32,   // 区域Blob定位工具
	eCornerSearchCheckTool				= 33,	// 角定位检测工具
	eRectSearchTool						= 34,	// 矩形定位工具
	ePatternRectSearchTool				= 35,	// 区域矩形定位工具	
	eAreaInspectTool					= 36,   // Blob检测工具
	eAreaInspectToolEx					= 37,   // 亮度、均匀度检查工具
	eChordSearchTool					= 38,	// 弦定位工具
	eTwoPatModelSearchTool				= 39,	// 
	eMPatternCornerSearchTool			= 40,	// 多标记点区域角定位工具
	ePatternCornerCircleSearchTool		= 42,	// 区域角+圆定位工具
	eQuickCornerCircleSearchTool		= 43,	// 几何角+圆定位工具
	ePatternPosLineDistanceTool			= 44,	// 区域点线距离工具
    eCornerSearchCheckInspectTool		= 45,	// 角定位检查加测量工具
	ePatternDouleSearchTool				= 46,   // 区域二次定位工具
	ePatternChordSearchTool				= 47,	// 区域弦定位工具
    eQuickMulCircleSearchTool			= 48,	// 多圆定位工具
	eDmCodeCalibrateTool				= 49,	// 二维码标定工具
	ePatternCircle4LineSearchTool		= 50,	// 区域四弦定位工具
	eQuickCircle4LineSearchTool			= 51,	// 几何四弦定位工具
	eSpecialPickSearchTool				= 52,	// 拾取专用定位工具
	ePatternCornerLineSearchTool		= 53,	// 区域角线工具
	ePatternMulCircleSearchTool			= 54,	// 区域多圆定位工具
	ePatternCornerMulCircleSearchTool	= 55,	// 区域角+多圆定位工具
	ePatternCornerMulObroundTool		= 56,	// 区域角双长圆定位工具
	eQuickCornerMulObroundTool			= 57,	// 几何角双长圆定位工具
	eCircleCornerSearchCheckInspectTool = 58,	// 圆角定位检查测量工具
	eACFInspectTool						= 59,	// ACF检查工具(灰度+Blob尺寸)
	ePatternVirtualRectSearchTool		= 60,	// 区域虚拟矩形定位工具
	eVirtualRectSearchTool				= 61,	// 虚拟矩形定位工具
	eTwoQuickModelSearchTool			= 62,	// 双几何定位工具
	eIdReaderDmCodeTool					= 63,	// DM\QR读码工具
	eIdReaderBarCodeTool				= 64,	// Bar条码读码工具 需要VSB/VSC加密狗支持
	eQuickPosLineDistanceTool           =65,//几何点线距离检测工具
	eCombinationCornerSearchTool        =66,//组合角定位工具
	eMQuickPickSearchTool               =67,//多标记点几何拾取双吸头定位工具
	eMPatternPickSearchTool				=68,//多标记点区域拾取双吸头定位工具
	eAreaInspectPickTool			= 69,   //忙放拾取工具--腾盛这边用的
	eMQuickPickCornerSearchTool               =70,//多标记点几何角拾取双吸头定位工具
	eMPatternPickCornerSearchTool				=71,//多标记点区域角拾取双吸头定位工具
}CSearchToolType;

// 训练Data参数基类
typedef struct tagBaseTrainDataParam
{
	long m_nImageChannel;
	tagBaseTrainDataParam()										
	{
		m_nImageChannel	= 0;// 0-(R+G+B)/3;1-R;2-G;3-B;
	}
	virtual CSearchToolType GetSearchToolType() = 0;
	virtual ~tagBaseTrainDataParam(){}
}CBaseTrainDataParam;

// 训练Gui参数基类
typedef struct tagBaseTrainGuiParam
{
	virtual CSearchToolType GetSearchToolType() = 0;
	virtual ~tagBaseTrainGuiParam(){}
}CBaseTrainGuiParam;

// 搜索Data参数基类
typedef struct tagBaseSearchDataParam
{
	virtual CSearchToolType GetSearchToolType() = 0;
	virtual ~tagBaseSearchDataParam(){}
}CBaseSearchDataParam;

// 搜索Gui参数基类
typedef struct tagBaseSearchGuiParam
{
	virtual CSearchToolType GetSearchToolType() = 0;
	virtual ~tagBaseSearchGuiParam(){}
}CBaseSearchGuiParam;

#define INSPCET_TOOL_LINE_NUMS2 4

typedef struct tagResultL2LInspectDistance
{

	double m_dStartPosX[INSPCET_TOOL_LINE_NUMS2+1];		// 找线结果线段P1点X
	double m_dStartPosY[INSPCET_TOOL_LINE_NUMS2+1];		// 找线结果线段P1点Y
	double m_dEndPosX[INSPCET_TOOL_LINE_NUMS2+1];		// 找线结果线段P2点X
	double m_dEndPosY[INSPCET_TOOL_LINE_NUMS2+1];		// 找线结果线段P2点Y

	double m_dCircleCenterX;		//找圆结果圆心X
	double m_dCircleCenterY;		//找圆结果圆心Y
	double m_dCircleRadius;			//找圆结果半径

	double m_dMeasurePosX[(INSPCET_TOOL_LINE_NUMS2/2)+1];		// 测量起点X
	double m_dMeasurePosY[(INSPCET_TOOL_LINE_NUMS2/2)+1];		// 测量起点Y
	double m_dProjectPosX[(INSPCET_TOOL_LINE_NUMS2/2)+1];		// 投影点X
	double m_dProjectPosY[(INSPCET_TOOL_LINE_NUMS2/2)+1];		// 投影点Y

	double m_dDistanceMM[(INSPCET_TOOL_LINE_NUMS2/2)+1];		// 物理距离
	double m_dDistancePixel[(INSPCET_TOOL_LINE_NUMS2/2)+1];	// 像素距离
	int m_nResult[(INSPCET_TOOL_LINE_NUMS2/2)+1];				// 检测结果 //  0：找线NG，没有距离 1：找线OK，结果OK     -1：找线OK，结果NG

	tagResultL2LInspectDistance()
	{
		m_dCircleCenterX = 0.0;
		m_dCircleCenterY = 0.0;
		m_dCircleRadius = 0.0 ;
		for(int i=0;i<(INSPCET_TOOL_LINE_NUMS2/2)+1;i++)
		{
			m_dStartPosX[2*i] = 0.0;
			m_dStartPosY[2*i] = 0.0;
			m_dEndPosX[2*i] = 0.0;
			m_dEndPosY[2*i] = 0.0;
			m_dStartPosX[2*i+1] = 0.0;
			m_dStartPosY[2*i+1] = 0.0;
			m_dEndPosX[2*i+1] = 0.0;
			m_dEndPosY[2*i+1] = 0.0;

			m_dMeasurePosX[i] = 0.0;
			m_dMeasurePosY[i] = 0.0;
			m_dDistanceMM[i] = 0.0;
			m_dDistancePixel[i] = 0.0;
			m_nResult[i] = 0;
		}
	}

}CResultL2LInspectDistance;

typedef struct tagResultAreaInspectTool
{
	double m_dLeftUpX;		// 左上角点X
	double m_dLeftUpY;		// 左上角点Y
	double m_dWidth;		// 矩形宽度
	double m_dHeight;		// 矩形高度
	int m_nResult;			// 检测结果

	tagResultAreaInspectTool()
	{
		m_dLeftUpX = 0;
		m_dLeftUpY = 0;
		m_dWidth = 0;
		m_dHeight = 0;
		m_nResult = 0;
	}

}CResultAreaInspectTool;


#define MaxAreaInspectToolExResultNum	100	// 面积检测EX工具最大结果数量
typedef struct tagResultAreaInspectToolEx
{
	std::vector<double> m_vdLeftUpX;	// 左上角点X
	std::vector<double> m_vdLeftUpY;	// 左上角点Y
	std::vector<double> m_vdWidth;		// 矩形宽度
	std::vector<double> m_vdHeight;		// 矩形高度
	std::vector<int> m_vnResult;		// 检测结果
	int m_nValidResultNum;				// 有效结果数量

	tagResultAreaInspectToolEx()
	{
		m_vdLeftUpX.resize(MaxAreaInspectToolExResultNum, 0);
		m_vdLeftUpY.resize(MaxAreaInspectToolExResultNum, 0);
		m_vdWidth.resize(MaxAreaInspectToolExResultNum, 0);
		m_vdHeight.resize(MaxAreaInspectToolExResultNum, 0);
		m_vnResult.resize(MaxAreaInspectToolExResultNum, 0);
		m_nValidResultNum = 0;
	}

}CResultAreaInspectToolEx;


typedef struct tagResultPosLineDistanceTool
{
	double m_dProjectPosX0;		// 投影点X
	double m_dProjectPosY0;		// 投影点Y
	double m_dDistanceMM0;		// 物理距离
	double m_dDistancePixel0;	// 像素距离
	int m_nResult0;				// 检测结果

	//  0：找线NG，没有距离
	//  1：找线OK，结果OK
	// -1：找线OK，结果NG
	// -2：没启用线距

	double m_dProjectPosX1;		// 投影点X
	double m_dProjectPosY1;		// 投影点Y
	double m_dDistanceMM1;		// 物理距离
	double m_dDistancePixel1;	// 像素距离
	int m_nResult1;				// 检测结果

	tagResultPosLineDistanceTool()
	{
		m_dProjectPosX0 = 0;
		m_dProjectPosY0 = 0;
		m_dDistanceMM0 = 0;
		m_dDistancePixel0 = 0;
		m_nResult0 = 0;

		m_dProjectPosX1 = 0;
		m_dProjectPosY1 = 0;
		m_dDistanceMM1 = 0;
		m_dDistancePixel1 = 0;
		m_nResult1 = 0;
	}

}CResultPosLineDistanceTool;

typedef struct tagResultCircleCornerDistance
{
	//尺寸管控的结果
	int					m_nSizeControlResult;
	std::vector<double> m_vSizeMM;		// 物理距离

	double m_dConcentricityX;  // 两圆时同心度分解
	double m_dConcentricityY;  // 两圆时同心度分解

	//距离管控的结果
	int					m_nDistControlResult;
	std::vector<double> m_vDistMM;		// 物理距离
	//脏污检测的结果
	int					m_nBlobResult;
	std::vector<double>	m_vBlobArea;

	//找线结果
	std::vector<int>			m_vLineSegResult;			// 每个检测的结果
	std::vector<scLineSeg>		m_vLineSeg;
	std::vector<CString>		m_vLineSegName;
	//找圆结果
	std::vector<sc2Vector>		m_vCircle;
	std::vector<double>			m_vRadius;
	//找长圆结果
	std::vector<sc2Vector>		m_vLongObroundCenter;
	std::vector<double>			m_vObroundLenX;
	std::vector<double>			m_vObroundLenY;
	std::vector<double>			m_vObroundRadian;


	tagResultCircleCornerDistance()
	{
		m_nSizeControlResult = 0;
		m_nDistControlResult = 0;
		m_nBlobResult = 0;
		m_dConcentricityX =0;
		m_dConcentricityY =0;
	}

}CResultCircleCornerDistance;


typedef struct tagResultCircleSearchTool
{
	sc2Vector m_scCircleCircle;
	double m_dRadius;

	tagResultCircleSearchTool()
	{
		m_scCircleCircle = sc2Vector(0, 0);
		m_dRadius = 0;
	}

}CResultCircleSearchTool;

typedef struct tagResultSearchRect
{
	scAffineRect m_searchRect;
	BOOL m_bFindRect;
	tagResultSearchRect()
	{
		m_searchRect=scAffineRect(sc2Vector(-10000,-10000),0,0);
		m_bFindRect = FALSE;
	}
}CResultSearchRect;

#define MaxResultLineNum	8//最大直线数量
typedef struct tagResultLine  //找线结果（现用于区域和几何四弦定位工具）
{
	int m_nLineNum;
	BOOL m_bFindResult;
	std::vector<double> m_vLineX;
	std::vector<double> m_vLineY;
	tagResultLine()
	{
		m_bFindResult = TRUE;
		m_nLineNum = 0;
		m_vLineX.resize(MaxResultLineNum);
		m_vLineY.resize(MaxResultLineNum);
	}
}CResultLine;

typedef struct tagResultObround
{
	scObround m_scObround;
}CResultObround;

typedef struct tagResultGenRect
{
	scGenRect m_scGenRect;
}CResultGenRect;

// 拾取工具头尾检查
typedef struct tagResultSpecialPickHeadTail
{
	bool bEnable; // 已经启用头尾
	int nResultCode; //头尾检查处理类型码   -1失败   0无处理,  1 拾取后旋转180   2 旋转180后拾取 ；

	tagResultSpecialPickHeadTail()
	{
		bEnable = false;
		nResultCode = 0;
	}
}CResultSpecialPickHeadTail;

// 搜索结果
#define MaxAuxiliarySearchResultNum	4	// 最大辅助位置数量
typedef struct tagSearchResult
{
	tagSearchResult()
	{
		m_dPosX = 0;
		m_dPosY = 0;
		m_dAngle = 0;
		m_dScore = 0;

		m_dLineAngle1 = 0;
		m_dLineAngle2 = 0;
		m_dLineAngle3 = 0;
		m_bFindLine1 = FALSE;
		m_bFindLine2 = FALSE;
		m_bFindLine3 = FALSE;

// 		m_dWidth = 0;
// 		m_dHeight = 0;

		m_vdAuxiliaryPosX.resize(MaxAuxiliarySearchResultNum, 0);		// 辅助位置X
		m_vdAuxiliaryPosY.resize(MaxAuxiliarySearchResultNum, 0);		// 辅助位置Y
		m_vdAuxiliaryAngle.resize(MaxAuxiliarySearchResultNum, 0);		// 辅助旋转角度，单位度

		m_bResultPickIndex=-1;
	}
	double m_dPosX;			// 位置X
	double m_dPosY;			// 位置Y
	double m_dAngle;		// 旋转角度，单位度
	double m_dScore;		// 相似度分数

	double m_dLineAngle1;
	double m_dLineAngle2;
	double m_dLineAngle3;
	BOOL   m_bFindLine1;
	BOOL   m_bFindLine2;
	BOOL   m_bFindLine3;
// 	double m_dWidth;		// 矩形宽度
// 	double m_dHeight;		// 矩形高度
	int m_bResultPickIndex;//结果给吸头1还是吸头2 用在多标记点拾取定位工具

	std::vector<double> m_vdAuxiliaryPosX;	// 辅助位置X
	std::vector<double> m_vdAuxiliaryPosY;	// 辅助位置Y
	std::vector<double> m_vdAuxiliaryAngle;	// 辅助旋转角度，单位度

	CResultAreaInspectTool			m_resultAreaInspectTool;
	CResultAreaInspectToolEx		m_resultAreaInspectToolEx;
	CResultPosLineDistanceTool		m_resultPosLineDistanceTool;

	CResultCircleSearchTool			m_resultCircleSearchTool;
	CResultCircleSearchTool			m_resultCircleSearchTool2;//iyo
	CResultL2LInspectDistance		m_ResultL2LInspectDistance;
	CResultSearchRect				m_resultSearchRect;
	CResultLine						m_ResultLine;   //找线结果
	CResultCircleCornerDistance		m_CircleCornerDistance;
	CResultSpecialPickHeadTail		m_resultSpecialPickHeadTail;



	///返回结果及控制GUI显示
	std::vector<CResultSearchRect>			m_vTrainSearchRect;				// 粗定位搜索矩形结果
	std::vector<CResultSearchRect>			m_vACFSearchRect;				// 区域矩形搜索结果
	std::vector<CResultCircleSearchTool>	m_vCircleSearchResult;			// 圆形搜索结果
	std::vector<CResultLine>				m_vLineSearchResult;			// 线搜索结果
	std::vector<CResultLine>				m_vLineSegSearchResult;			// 线段搜索结果
	std::vector<CResultObround>				m_vObroundSearchResult;			// 长圆搜索结果
	std::vector<CResultGenRect>				m_vGenRectSearchResult;			// 圆角搜索结果 


	std::vector<DecodeMatrixResult>					m_vcodeDMResult;	// DM读码结果，包含二维码位置和二维码信息
	std::vector<DecodeQRResult>						m_vcodeQRResult;	// QR读码结果，包含二维码位置和二维码信息

	std::vector<Code1DResult>				m_vcode1DResult;			// 1D条码读码结果；包含条码类型、角度和码具体信息

	std::vector<std::vector<int>>			m_vACFRowColStatus;			// ACF灰度Blob等分行列序列检测状态 用于空盘检测;

	std::vector<CString>			        m_vText;			       // 用于主界面显示的文字;
	std::vector<BOOL>			            m_vTextBool;			       // 用于主界面显示的文字颜色;

}CSearchResult;

// 训练结果
typedef struct tagBaseTrainResult
{
	virtual CSearchToolType GetSearchToolType() = 0;
	virtual ~tagBaseTrainResult(){}
}CBaseTrainResult;

//
// 定位基类
//
class /*SV_SEARCHTOOL_API*/ CBaseSearchTool  
{
public:
	CBaseSearchTool();
	CBaseSearchTool(CBaseSearchTool &cBaseSearchTool);
	virtual ~CBaseSearchTool();
public:
	int m_CamWidth;
	int m_CamHeight;

	BOOL GetProcessImage(CBaseTrainDataParam* pTrainData,cpImage &src,cpImage &dst);
public:

	//////////////////////////工具属性相关////////////////////////////////
	virtual CSearchToolType GetSearchToolType() = 0;										    // 获取工具类型
	
	//////////////////////////显示更新相关////////////////////////////////
	virtual bool SetDisplay(scGuiDisplay *pGuiDisplay) = 0;										// 设置显示控件指针				
	virtual scGuiDisplay* GetDisplay() = 0;														// 获取显示空间指针
	virtual bool SetTrainResultDisplay(scGuiDisplay *pGuiDisplay){return false;};				// 设置训练结果显示控件指针
	virtual scGuiDisplay* GetTrainResultDisplay(){return nullptr;};								// 获取训练结果显示控件指针
	virtual bool SetTrainResultDisplay2(scGuiDisplay *pGuiDisplay){return false;};				// 设置训练结果显示控件指针
	virtual scGuiDisplay* GetTrainResultDisplay2(){return nullptr;};							// 获取训练结果显示控件指针
	virtual bool SetTrainResultDisplay3(scGuiDisplay *pGuiDisplay){return false;};				// 设置训练结果显示控件指针
	virtual scGuiDisplay* GetTrainResultDisplay3(){return nullptr;};							// 获取训练结果显示控件指针
	virtual bool SetGuiKeyText(CString strGuiKeyText) = 0;										// 设置Gui图形的关键字
	virtual bool GetGuiKeyText(CString &strGuiKeyText) = 0;										// 获取Gui图形的关键字
	virtual bool ShowTrainGui(bool bShow) = 0;													// 更新训练界面显示
	virtual bool ShowTrainResultGui(bool bShow) = 0 ;											// 是否显示训练结果界面
	virtual bool ShowSearchResultGui(bool bShow) = 0;											// 更新搜索界面显示
	virtual bool UpdateGuiDisplay() = 0;														// 更新搜索界面显示
	virtual bool ClearAllGui() = 0;															    // 清除定位工具在Display上的控件
	virtual bool ClearResult() = 0;																// 清除结果
	virtual bool SetGuiByCam(double m_Width,double m_Height) = 0;								// 根据相机宽高设置GUI大小


	///////////////////////////训练相关/////////////////////////////////////
	virtual bool SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam) = 0;					// 设置训练Data参数				
	virtual bool GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam) = 0;					// 获取训练Data参数
	virtual bool SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam) = 0;						// 获取搜索Gui参数
	virtual bool GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam) = 0;						// 获取搜索Gui参数
	virtual bool SetTrainImage(const cpImage &ImageTrain) = 0;									// 设置训练原始图像
	virtual bool GetTrainImage(cpImage &ImageTrain) = 0;										// 获取训练原始图像
	virtual bool SetMaskImage(const cpImage &ImageMask) = 0;									// 设置训练原始图像
	virtual bool GetMaskImage(cpImage &ImageMask) = 0;											// 获取训练原始图像
	virtual bool SetMaskImage2(const cpImage &ImageMask){return false;}						// 设置训练原始图像
	virtual bool GetMaskImage2(cpImage &ImageMask){return false;}								// 获取训练原始图像

	virtual bool SetMaskImage_HeadTail(const cpImage &ImageMask){return false;};									// 设置训练原始图像
	virtual bool GetMaskImage_HeadTail(cpImage &ImageMask){return false;};											// 获取训练原始图像

	virtual bool SetMaskImage_PN(const cpImage &ImageMask){return false;};									// 设置训练原始图像
	virtual bool GetMaskImage_PN(cpImage &ImageMask){return false;};											// 获取训练原始图像

	virtual bool SetMaskImage_Mixing(const cpImage &ImageMask) {return false;};									// 设置训练原始图像
	virtual bool GetMaskImage_Mixing(cpImage &ImageMask){return false;};											// 获取训练原始图像

	virtual bool Train() = 0;																	// 执行训练
	virtual bool IsTrained() = 0;																// 是否训练成功
	virtual bool GetTrainResultImage(cpImage &ImageTrainResult) = 0;							// 获取训练结果图像
	virtual bool GetTrainResultImage2(cpImage &ImageTrainResult){return false;}							// 获取训练结果图像
	virtual bool GetTrainResultImage2(cpImage &ImageTrainResult,const int index){return false;}			// 获取训练结果图像
	virtual bool GetTrainResultImage3(cpImage &ImageTrainResult){return false;}							// 获取训练结果图像

	virtual bool GetTrainResultImage_HeadTail(cpImage &ImageTrainResult){return false;};							// 获取训练结果图像
	virtual bool GetTrainResultImage_PN(cpImage &ImageTrainResult){return false;};							// 获取训练结果图像
	virtual bool GetTrainResultImage_Mixing(cpImage &ImageTrainResult){return false;};							// 获取训练结果图像


	virtual bool GetTrainResult(CBaseTrainResult *pTrainResult) = 0;							// 获取训练结果
	virtual bool GetPatternImage(cpImage &ImagePattern) = 0;									// 获取训练模板原始图像
	virtual bool GetPatternImage2(cpImage &ImagePattern){return false;}									// 获取训练模板原始图像
	virtual bool GetPatternImage2(cpImage &ImagePattern,const int Index){return false;}									// 获取训练模板原始图像
	virtual bool GetPatternImage3(cpImage &ImagePattern){return false;}									// 获取训练模板原始图像

	virtual bool RestoreTrainParam() = 0;														// 恢复训练参数	

	///////////////////////////搜索相关/////////////////////////////////////	
	virtual bool SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam) = 0;				// 设置搜索Data参数
	virtual bool GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam) = 0;				// 获取搜索Data参数
	virtual bool SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam) = 0;					// 设置搜索Gui参数
	virtual bool GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam) = 0;					// 获取搜索Gui参数
	virtual bool Search(cpImage &ImageSearch) = 0;										// 执行搜索 
	virtual int  GetResultNum() = 0;															// 获取搜索结果个数
	virtual bool GetResult(int nIndex, CSearchResult &searchResult) = 0;						// 获取搜索结果
	virtual bool RestoreSearchParam() = 0;														// 恢复搜索参数	

	/////////////////////////参数加载与保存////////////////////////////////	
	virtual bool LoadModelFromFile(CString strDir) = 0;											// 加载模板文件, strDir为文件夹路径
	virtual bool SaveModelToFile(CString strDir) = 0;											// 保存模板文件, strDir为文件夹路径
	virtual bool LoadModelTrainPatternImage(CString strDir , cpImage &ImagePattern);			// 加载模板训练区域图片， strDir为文件夹路径,ImagePattern为获取得到的训练区域图片
	/////////////////////////公共函数////////////////////////////////	
	static  CSearchToolType GetSearchToolTypeFromFile(CString strDir);							// 获取工具类型, strDir为为文件夹路径
	static  BOOL SaveSearchToolTypeToFile(CString strDir);										// 获取工具类型, strDir为为文件夹路径
	static  void CreateGUID(CString &strGUID);													// 创建Guid
	//////////////////////语言类型//////////////////////////////////////
	virtual void	SetLanguage(int nLanguage);									// 设置中英文菜单 0：eng；1：chs
	int             m_iLanguage;
	int				m_nResultErrNumber;	//错误号
	virtual int		GetResultErrNumber();
	virtual CString	GetResultErrInfo() {return _T("");}

	CString m_strShowInspectInfo;

};
#pragma pack(pop)

#endif // !defined(AFX_BASESEARCHTOOL_H__CE9FD7C2_B8EE_48EE_99D1_907E23E841E0__INCLUDED_)
