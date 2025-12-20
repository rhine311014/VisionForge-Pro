
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
#define INSPCET_TOOL_LINE_NUMS 4
#define INI_SEARCH_ANGLE 2
#define INI_CONTRASTTHRE 5
#define INI_ACCEPTTHRE	0.4
#define INI_CONFUSIONTHRE 0.6
//训练Data参数类
typedef struct tagCornerCheckSearchTrainDataParam : public CBaseTrainDataParam
{
	BOOL		    m_bEnableCorner;					//启用角定位
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


	bool m_bEnableCheck;								//启用检测
	CSearchToolType m_SearchToolType;					//定位工具类型
	CSearchToolType m_InspectToolType;					//检测工具类型

	bool			m_bEnableInspect[((INSPCET_TOOL_LINE_NUMS)/2)+2];		//启用线距检测工具
	int				m_nOutLineNum1[INSPCET_TOOL_LINE_NUMS+1];			// 找线局外点
	double			m_dContrastThre1[INSPCET_TOOL_LINE_NUMS+1] ;		// 找线对比度阈值
	EdgePolarity	m_edgeProperty1[INSPCET_TOOL_LINE_NUMS+1];		// 找线边缘属性
	bool			m_bSortByScore1[INSPCET_TOOL_LINE_NUMS+1];		// 找线按分数排序
	long			m_lFilterHalfWidth1[INSPCET_TOOL_LINE_NUMS+1];	// 找线滤波器半宽
	BOOL			m_bEnableDualEdge1[INSPCET_TOOL_LINE_NUMS+1];		// 启用/禁用找线工具双边缘

								//找圆工具//
	//////////////////////////////////////////////////////////////////////////
	int						m_nCircleOutLineNum;					//忽略数
	double					m_dCircleContrastThre;				//对比度
	long						m_lCircleFilterHalfWidth;				//滤波半款
	EdgePolarity				m_CircleedgeProperty;					//极性
	bool						m_bEnableCircleDualEdge;				//启用双极性
	bool						m_bSortByScore2;						//找圆按分数排序


	tagCornerCheckSearchTrainDataParam()
	{
		m_bEnableCorner=true;
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

		m_dCircleContrastThre		= 10.0;
		m_lCircleFilterHalfWidth	= 1;
		m_nCircleOutLineNum		    = 10;
		m_CircleedgeProperty		= eDarkToLight;
		m_bEnableCircleDualEdge		= false;
		m_bSortByScore2				 = true;

		for (int i=0; i<LINE_NUM; i++)
		{
			m_dContrastThre[i]		= 1.0;
			m_lFilterHalfWidth[i]	= 1;
			m_nOutLineNum[i]		= 0;
			m_edgeProperty[i]		= eDontCare;
			m_bSortByScore[i]		= true;
			m_bEnableDualEdge[i]	= false;
		}

		m_bEnableCheck = false;
		m_SearchToolType = ePatternCornerSearchTool;
		m_InspectToolType = ePatternSearchTool;

        for(int i=0;i<(INSPCET_TOOL_LINE_NUMS/2)+1;i++)
        {
            m_bEnableInspect[i]= false;
        }

		for(int i=0;i<INSPCET_TOOL_LINE_NUMS+1;i++)
		{
			m_dContrastThre1[i]		= 5;
			m_lFilterHalfWidth1[i]	= 1;
			m_nOutLineNum1[i]		= 0;
			m_edgeProperty1[i]		= eDontCare;
			m_bSortByScore1[i]		= true;
			m_bEnableDualEdge1[i]	= false;

		}

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCornerSearchCheckInspectTool;
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

}CCornerCheckSearchTrainDataParam;

// CornerCheckSearch定位工具训练Gui参数类
typedef struct tagCornerCheckSearchTrainGuiParam : public CBaseTrainGuiParam
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

	// 线距检测工具 找线工具
	int				m_nCaliperNum1[INSPCET_TOOL_LINE_NUMS+1];			// 卡尺个数
	double			m_dStartX1[INSPCET_TOOL_LINE_NUMS+1];				// 起始点X
	double			m_dStartY1[INSPCET_TOOL_LINE_NUMS+1];				// 起始点Y
	double			m_dEndX1[INSPCET_TOOL_LINE_NUMS+1];					// 终止点X
	double			m_dEndY1[INSPCET_TOOL_LINE_NUMS+1];					// 终止点Y
	double			m_dCaliperProjectLen1[INSPCET_TOOL_LINE_NUMS+1];		// 投影长度
	double			m_dCaliperSearchLen1[INSPCET_TOOL_LINE_NUMS+1];		// 搜索长度
	scRadian		m_CaliperSearchRotation1[INSPCET_TOOL_LINE_NUMS+1];	// 搜索角度
	svStd vector<double> m_vdMaskData_FindLine1[INSPCET_TOOL_LINE_NUMS+1];// 掩膜数据

	bool			m_bEnableLineMask[INSPCET_TOOL_LINE_NUMS+1];
	svStd vector<bool> m_vbCaliperMask_FindLine1[INSPCET_TOOL_LINE_NUMS+1];// 卡尺是否被掩膜
	bool			m_InspectLineVisible[INSPCET_TOOL_LINE_NUMS+1];			// 线是否可见
	bool			m_bInspectCaliperVisible[INSPCET_TOOL_LINE_NUMS+1];			//找线工具是否可见


								//找圆工具//
	//////////////////////////////////////////////////////////////////////////
	sc2Vector		m_CircleCenter;								// 圆心X
	double			m_dCircleRadius;							// 半径
	scRadian		m_CircleArcStart;							// 起始弧度
	scRadian        m_CircleArcSpan;							// 弧度跨度
	double			m_dCircleCaliperProjectLen;					// 投影长度
	double			m_dCircleCaliperSearchLen;					// 搜索长度
	svStd vector<double> m_vdCircleCaliperMaskData;				// 掩膜数据
	svStd vector<bool>   m_vbCircleCaliperMask;					// 卡尺是否被掩膜

	int						m_nCircleCaliperNum;					//卡尺数
	BOOL		 				m_bCircleCaliperVisible;				//显示卡尺	
	BOOL						m_bEnableCircleMask;					//启用掩摸
	CaliperSearchDirection	m_CircleCaliperSearchDirection;		//向心

	tagCornerCheckSearchTrainGuiParam()
	{
	
		
		//圆参数
		m_CircleCenter				= sc2Vector(200,200);
		m_dCircleRadius					= 50.0;
		m_CircleArcStart					= scRadian(0);
		m_CircleArcSpan					= scRadian(sk2PI);
		m_dCircleCaliperProjectLen		= 30.0;
		m_dCircleCaliperSearchLen			= 250.0;

		m_nCircleCaliperNum				= 20;
		m_CircleCaliperSearchDirection	= eInward;
		m_bCircleCaliperVisible			= false;
		m_bEnableCircleMask				= false;
	
	
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


		for(int i=0 ;i<INSPCET_TOOL_LINE_NUMS+1;i++)
		{
			m_nCaliperNum1[i]				= 20;
			m_dStartX1[i]					= 100.0;
			m_dStartY1[i]					= 200.0+(i%2)*100;
			m_dEndX1[i]						= 500.0 ;
			m_dEndY1[i]						= 200.0+(i%2)*100;
			m_dCaliperProjectLen1[i]		= 20.0;
			m_dCaliperSearchLen1[i]			= 100.0;
			m_CaliperSearchRotation1[i]		= scRadian(skPI/2.0);
			m_InspectLineVisible[i]			= false;
			m_bEnableLineMask[i]			= false;
			m_bInspectCaliperVisible[i]		= false;
		}

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCornerSearchCheckInspectTool;
	}

}CCornerCheckSearchTrainGuiParam;

// PatternnCornerSearch定位工具搜索Data参数类
typedef struct tagCornerCheckSearchDataParam : public CBaseSearchDataParam
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

	bool	m_bCheckAfterSearch;						// true 检查在搜索之后

	double 	m_dPixelRatio[INSPCET_TOOL_LINE_NUMS/2+1];							// 像素当量 （mm/piexl）
	int		m_nDistanceMode[INSPCET_TOOL_LINE_NUMS/2+1];							// 测量距离模式（0:L1到L2, 1:L2到L2）
	int		m_nDistanceType[INSPCET_TOOL_LINE_NUMS/2+1];							// 距离类型 （0:平均值,1:最大值,2:最小值）
	bool	m_bEnableMax1[INSPCET_TOOL_LINE_NUMS/2+1];							// 启用最大值管控
	double	m_dDistanceMax1[INSPCET_TOOL_LINE_NUMS/2+1];							// 最大值管控值 (mm)
	bool	m_bEnableMin1[INSPCET_TOOL_LINE_NUMS/2+1];							// 启用最小值管控
	double 	m_dDistanceMin1[INSPCET_TOOL_LINE_NUMS/2+1];							// 最小值管控值 (mm)
	bool	m_bEnableInspectOKNG[INSPCET_TOOL_LINE_NUMS/2+1];						// 是否启用检测NGOK状态，启用时 检测的NG 状态会影响定位结果最后的OKNG状态

	double  m_dInspectDistanceValue[INSPCET_TOOL_LINE_NUMS/2+1];					//测量值
	int		m_nInspectOKNG[INSPCET_TOOL_LINE_NUMS/2+1];							//测量结果OK  //  0：找线NG，没有距离 1：找线OK，结果OK     -1：找线OK，结果NG
	CString	m_strInspectResultInfo[INSPCET_TOOL_LINE_NUMS/2+1];						//测量结果OK  //  检测结果 详细信息；

	CString m_strInspectDistanceValueLabel[INSPCET_TOOL_LINE_NUMS/2+1];			//测量值显示 标签名
	bool m_bLine2ForceVerticalLine1;					//线2强制垂直线1

	tagCornerCheckSearchDataParam()
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
		m_bCheckAfterSearch			=true;

		for(int i=0;i<INSPCET_TOOL_LINE_NUMS/2+1;i++)
		{
			m_dPixelRatio[i]			= 0.01;
			m_nDistanceMode[i]			= 0;
			m_nDistanceType[i]			= 0;
			m_bEnableMax1[i]			= false;
			m_dDistanceMax1[i]			= 0.1;
			m_bEnableMin1[i]			= false;
			m_dDistanceMin1[i]			= 0.0;

			m_dInspectDistanceValue[i]	= 0.0;
			m_bEnableInspectOKNG[i]     =false;
			m_nInspectOKNG[i]			=-2;
			m_strInspectResultInfo[i]	= _T("");
			//m_strInspectDistanceValueLabel[i] = _T("");

		}
		m_strInspectDistanceValueLabel[0] = _T("X:");
		m_strInspectDistanceValueLabel[1] = _T("Y:");
		m_bLine2ForceVerticalLine1=FALSE;

	}

	virtual void InspectOKNG_AndResultInfo()
	{
		for (int i =0 ;i<(INSPCET_TOOL_LINE_NUMS/2)+1;i++)
		{
			switch (m_nInspectOKNG[i])
			{
			case -1:
				{
					m_strInspectResultInfo[i].Format(_T("找线OK，结果NG"));
				}
				break;
			case 0:
				{
					m_strInspectResultInfo[i].Format(_T("找线NG"));
				}
				break;
			case 1:
				{
					m_strInspectResultInfo[i].Format(_T("找线OK，结果OK"));
				}
				break;
			default:
				{
					m_strInspectResultInfo[i].Format(_T(""));
				}
			}
		}
	}


	virtual CSearchToolType GetSearchToolType()
	{
		return eCornerSearchCheckInspectTool;
	}

}CCornerCheckSearchSearchDataParam;

// CornerCheckSearch定位工具搜索Gui参数类
typedef struct tagCornerCheckSearchGuiParam : public CBaseSearchGuiParam
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

	bool			m_bInspectVisibleFitPoint[INSPCET_TOOL_LINE_NUMS+1];// 线距检测工具是否显示拟合点

	bool			m_bEnableShowFitCirclePoint;			//找圆工具是否显示拟合点

	tagCornerCheckSearchGuiParam()
	{
		m_LineColor					= RGB(0, 255, 0);
		m_nLineWidth				= 1;
		m_bRectVisible				= true;
		m_bPointVisible				= true;
		m_bLineVisible				= true;
		m_bLineCrossVisible			= true;
		m_bEnableShowFitCirclePoint =  false;

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

		for (int i=0;i <INSPCET_TOOL_LINE_NUMS+1;i++)
		{
			m_bInspectVisibleFitPoint[i] = false;
		}

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eCornerSearchCheckInspectTool;
	}

}CCornerCheckSearchSearchGuiParam;

// 训练结果
typedef struct tagCornerCheckSearchTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y
	double m_dModelX1; // 参考点X1					
	double m_dModelY1; // 参考点Y1

	double m_dStartX[LINE_NUM]; // 线段起点X
	double m_dStartY[LINE_NUM]; // 线段起点Y
	double m_dEndX[LINE_NUM];	  // 线段终点X
	double m_dEndY[LINE_NUM];	  // 线段终点Y

	double m_dStartX1[INSPCET_TOOL_LINE_NUMS]; // 线段起点X
	double m_dStartY1[INSPCET_TOOL_LINE_NUMS]; // 线段起点Y
	double m_dEndX1[INSPCET_TOOL_LINE_NUMS];	  // 线段终点X
	double m_dEndY1[INSPCET_TOOL_LINE_NUMS];	  // 线段终点Y

	// Circle
	double m_dCircleCenterX;	//圆心x
	double m_dCircleCenterY;	//圆心y
	double m_dCircleRadius;	//半径

	tagCornerCheckSearchTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
		m_dModelX1 = 0;
		m_dModelY1 = 0;

		// Circle
		m_dCircleCenterX = 0;
		m_dCircleCenterY = 0;
		m_dCircleRadius = 0;

		for (int i=0; i<LINE_NUM; i++)
		{
			m_dStartX[i] = 0;
			m_dStartY[i] = 0;
			m_dEndX[i]   = 0;
			m_dEndY[i]   = 0;
		}

		for(int i=0;i<INSPCET_TOOL_LINE_NUMS;i++)
		{
			m_dStartX1[i] = 0;
			m_dStartY1[i] = 0;
			m_dEndX1[i]   = 0;
			m_dEndY1[i]   = 0;
		}

	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eCornerSearchCheckInspectTool;
	}
}CCornerCheckSearchTrainResult;


typedef struct tagFindLineTrainData
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

	tagFindLineTrainData()
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

}StructFindLineTrainData;

class CCornerCheckSearchTool :public CBaseSearchTool
{
public:
	CCornerCheckSearchTool(void);
	CCornerCheckSearchTool(CCornerCheckSearchTool &cCornerCheckSearchTool);
	CCornerCheckSearchTool& operator=(CCornerCheckSearchTool &cCornerCheckSearchTool);
	~CCornerCheckSearchTool(void);
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

	bool ExecuteInspectTrain();											// 执行线距检测训练	
	void ShowFitPoint(std::vector<scGuiGraphic*>& m_pGuiInspectFitPointArray,scGuiCoordCross m_GuiInspectFitPoint[MAXPTSNUM],scFindLine* m_FindLineTool,const CString strFitPoint);
	void ShowFitPoint(std::vector<scGuiGraphic*>& m_pGuiInspectFitPointArray,scGuiCoordCross m_GuiInspectFitPoint[MAXPTSNUM],scFindCircle* m_FindLineTool,const CString strFitPoint);
	bool ExecuteInspectSearch(const double dTransX,const double dTransY,const double dRotation, cpImage &ImageSearch, CResultL2LInspectDistance &ResultL2LInspectDistance);

	bool GetFindLineCenterPoint(scFindLine* m_FindLineTool,sc2Vector &pointCenter2);

	void SetSearchResult(const CResultL2LInspectDistance &tmpL2LReslut, CSearchResult& searchResult);

private:
	scPatternSearchRot					m_PatternSearchTool;			// 区域角定位的粗定位搜索工具
	scPatternSearchRot					m_PatternSearchTool1;			// 检查工具的区域搜索工具
	scQuickSearchTool                   m_QuickSearchTool;				// 几何角定位QuickSearch搜索工具
	scQuickSearchTool                   m_QuickSearchTool1;				// 检查工具QuickSearch搜索工具

	scFindLine							m_FindLineTool[LINE_NUM];		// 找线工具

	scFindLine							m_InspectFindLineTool[INSPCET_TOOL_LINE_NUMS+1];// 线距检测找线工具

	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd;			// 显示训练结果对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay2;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd2;			// 显示训练结果对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CCornerCheckSearchTrainDataParam		m_tmpTrainDataParam;			// 当前训练Data参数
	CCornerCheckSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CCornerCheckSearchSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CCornerCheckSearchSearchGuiParam		m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CCornerCheckSearchTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像

	cpImage							m_tmpImageTrain1;				// 当前训练图像
	cpImage							m_tmpImageMask1;					// 当前掩模图像

	CCornerCheckSearchTrainDataParam		m_TrainDataParam;				// 训练Data参数
	CCornerCheckSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CCornerCheckSearchSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CCornerCheckSearchSearchGuiParam		m_SearchGuiParam;				// 搜索Gui参数	
	CCornerCheckSearchTrainResult			m_TrainResult;					// 训练结果
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

	scGuiFindLine						m_GuiInspectFindLine[INSPCET_TOOL_LINE_NUMS+1];// 线距检测找线工具 
	scGuiLine                           m_GuiInspectTrainResultLine[INSPCET_TOOL_LINE_NUMS+1];	// 训练结果线段
	scGuiFeaturelet						m_GuiTrainFeaturelet;			// 训练特征链集
	scGuiFeaturelet						m_GuiTrainFeaturelet2;			// 训练特征链集

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchRectArray;			// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;			// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineTextArray;		// 搜索结果文本


	scGuiCircle							m_GuiSearchCircle[MAXRESNUM];
	scGuiCross							m_GuiSearchCircleCenter[MAXRESNUM];

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

	std::vector<scGuiGraphic*>          m_pGuiInspectFitPoint1Array;
	scGuiCoordCross						m_GuiInspectFitPoint1[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiInspectFitPoint2Array;
	scGuiCoordCross						m_GuiInspectFitPoint2[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiInspectFitPoint3Array;
	scGuiCoordCross						m_GuiInspectFitPoint3[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiInspectFitPoint4Array;
	scGuiCoordCross						m_GuiInspectFitPoint4[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiInspectFitPoint5Array;
	scGuiCoordCross						m_GuiInspectFitPoint5[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiInspectFitPoint6Array;
	scGuiCoordCross						m_GuiInspectFitPoint6[MAXPTSNUM];

	std::vector<scGuiGraphic*>          m_pGuiSearchCircleArray;		// 搜索结果圆
	std::vector<scGuiGraphic*>          m_pGuiSearchCircleCenterArray;	// 搜索结果圆心
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArrayAuxilary1;	// 搜索结果参考点辅助点1
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArrayAuxilary2;	// 搜索结果参考点辅助点2

	
};

