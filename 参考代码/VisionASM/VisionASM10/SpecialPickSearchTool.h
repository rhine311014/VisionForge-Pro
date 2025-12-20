
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
#include "svGuiFindCircle.h"
#include "svGuiCircle.h"

#include "PatternSearchTool.h"
#include "QuickSearchTool.h"
#include "FixedPosSearchTool.h"
#include "svBlob.h"
#include "svPose.h"
#include "svGuiContour.h"
#include "svXMLConfigurator.h"
#include "svGuiFeaturelet.h"

#define LINE_NUMS 4  // 精定位用到的线条总数目（包含线1、角定位2、矩形4等)
#define RECT_NUMS 4 // 精定位用到二次区域/几何定位数目
#define CIRCLE_NUMS 	2 // 精定位用到圆个数
#define POINT_NUMS	4	// 固定点个数
#define INSPCET_TOOL_LINE_NUMS 4

#define INI_SEARCH_ANGLE 2
#define INI_CONTRASTTHRE 5
#define INI_ACCEPTTHRE	0.4
#define INI_CONFUSIONTHRE 0.6



//训练Data参数类

// 卡尺训练参数
typedef struct tagCaliperTrainDataParam
{
	double			m_dContrastThre;			// 对比度阈值
	long			m_lFilterHalfWidth;		// 滤波器半宽
	int				m_nOutLineNum;  		// 局外点
	EdgePolarity	m_edgeProperty;			// 边缘属性
	bool			m_bSortByScore;			// 按分数排序
	BOOL			m_bEnableDualEdge;		// 启用/禁用双边缘

	tagCaliperTrainDataParam()
	{
		m_dContrastThre		= 10.0;
		m_lFilterHalfWidth	= 1;
		m_nOutLineNum		= 0;
		m_edgeProperty		= eDontCare;
		m_bSortByScore		= true;
		m_bEnableDualEdge	= false;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		//ContrastThre
		if(FALSE == cXMLConfigurator.FindElem(_T("ContrastThre")))
		{
			cXMLConfigurator.AddElem(_T("ContrastThre"));
		}
		cXMLConfigurator.SetElemData(m_dContrastThre);

		// fileterhalfwidth1
		if(FALSE == cXMLConfigurator.FindElem(_T("FilterHalfWidth")))
		{
			cXMLConfigurator.AddElem(_T("FilterHalfWidth"));
		}
		cXMLConfigurator.SetElemData(m_lFilterHalfWidth);

		// outlinenum1
		if(FALSE == cXMLConfigurator.FindElem(_T("OutLineNum")))
		{
			cXMLConfigurator.AddElem(_T("OutLineNum"));
		}
		cXMLConfigurator.SetElemData(m_nOutLineNum);

		// edgeproperty1
		if(FALSE == cXMLConfigurator.FindElem(_T("EdgeProperty")))
		{
			cXMLConfigurator.AddElem(_T("EdgeProperty"));
		}
		cXMLConfigurator.SetElemData(m_edgeProperty);

		// SortByScore1
		if(FALSE == cXMLConfigurator.FindElem(_T("SortByScore")))
		{
			cXMLConfigurator.AddElem(_T("SortByScore"));
		}
		cXMLConfigurator.SetElemData(m_bSortByScore);

		// DualEdge1
		if(FALSE == cXMLConfigurator.FindElem(_T("DualEdge")))
		{
			cXMLConfigurator.AddElem(_T("DualEdge"));
		}
		cXMLConfigurator.SetElemData(m_bEnableDualEdge);

	}

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		bool bRet = false;
		bRet = cXMLConfigurator.FindElem(_T("ContrastThre"));
		if (bRet)
		{
			m_dContrastThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		
		// FilterHalfWidth1
		bRet = cXMLConfigurator.FindElem(_T("FilterHalfWidth"));
		if (bRet)
		{
			m_lFilterHalfWidth = _ttol(cXMLConfigurator.GetElemData());
		}
		
		// outlinenum1
		bRet = cXMLConfigurator.FindElem(_T("OutLineNum"));
		if (bRet)
		{
			m_nOutLineNum = _ttol(cXMLConfigurator.GetElemData());
		}
		
		// edgeproperty1
		bRet = cXMLConfigurator.FindElem(_T("EdgeProperty"));
		if (bRet)
		{
			m_edgeProperty = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());
		}
		
		// SortByScore1
		bRet = cXMLConfigurator.FindElem(_T("SortByScore"));
		if (bRet)
		{
			m_bSortByScore = _ttoi(cXMLConfigurator.GetElemData());
		}
		
		// DualEdge1
		bRet = cXMLConfigurator.FindElem(_T("DualEdge"));
		if (bRet)
		{
			m_bEnableDualEdge = _ttoi(cXMLConfigurator.GetElemData());
		}
		

	}

}CCaliperTrainDataParam;

// Blob 粗定位的训练参数 搜索参数
typedef struct tagBlobTrainSearchDataParam
{
	bool			m_bBlobMode;				//亮Blob或暗Blob:true,暗blob;false,亮blob
	long			m_lBlobGrayThre;			//灰度阈值
	long			m_lConnectivityMinPels;		//修剪、填充的最小面积
	long			m_lBlobAreaMinThre;			//最小面积阈值
	long			m_lBlobAreaMaxThre;			//最大面积阈值
	int				m_nBlobOutMode;				//设置输出模式

	int				m_nBlobSearchNums;

	tagBlobTrainSearchDataParam()
	{
		m_bBlobMode				= false;				
		m_lBlobGrayThre			= 128;
		m_lConnectivityMinPels	= 100;
		m_lBlobAreaMinThre		= 100;	    
		m_lBlobAreaMaxThre		= 100000;	
		m_nBlobOutMode			= 0;

		m_nBlobSearchNums		= 1;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		
		//m_bBlobMode
		if(FALSE == cXMLConfigurator.FindElem(_T("BlobMode")))
		{
			cXMLConfigurator.AddElem(_T("BlobMode"));
		}
		cXMLConfigurator.SetElemData(m_bBlobMode);
		
		//m_lBlobGrayThre
		if(FALSE == cXMLConfigurator.FindElem(_T("BlobGrayThre")))
		{
			cXMLConfigurator.AddElem(_T("BlobGrayThre"));
		}
		cXMLConfigurator.SetElemData(m_lBlobGrayThre);

		//m_lConnectivityMinPels
		if(FALSE == cXMLConfigurator.FindElem(_T("ConnectivityMinPels")))
		{
			cXMLConfigurator.AddElem(_T("ConnectivityMinPels"));
		}
		cXMLConfigurator.SetElemData(m_lConnectivityMinPels);

		//m_lBlobAreaMinThre
		if(FALSE == cXMLConfigurator.FindElem(_T("BlobAreaMinThre")))
		{
			cXMLConfigurator.AddElem(_T("BlobAreaMinThre"));
		}
		cXMLConfigurator.SetElemData(m_lBlobAreaMinThre);

		//m_lBlobAreaMaxThre
		if(FALSE == cXMLConfigurator.FindElem(_T("BlobAreaMaxThre")))
		{
			cXMLConfigurator.AddElem(_T("BlobAreaMaxThre"));
		}
		cXMLConfigurator.SetElemData(m_lBlobAreaMaxThre);

		//m_nBlobOutMode
		if(FALSE == cXMLConfigurator.FindElem(_T("BlobOutMode")))
		{
			cXMLConfigurator.AddElem(_T("BlobOutMode"));
		}
		cXMLConfigurator.SetElemData(m_nBlobOutMode);

		//m_nBlobOutMode
		if(FALSE == cXMLConfigurator.FindElem(_T("BlobSearchNums")))
		{
			cXMLConfigurator.AddElem(_T("BlobSearchNums"));
		}
		cXMLConfigurator.SetElemData(m_nBlobSearchNums);
	}

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		bool bRet = false;
		bRet = cXMLConfigurator.FindElem(_T("BlobMode"));
		if (bRet)
		{
			m_bBlobMode = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("BlobGrayThre"));
		if (bRet)
		{
			m_lBlobGrayThre = _ttol(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("ConnectivityMinPels"));
		if (bRet)
		{
			m_lConnectivityMinPels = _ttol(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("BlobAreaMinThre"));
		if (bRet)
		{
			m_lBlobAreaMinThre = _ttol(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("BlobAreaMaxThre"));
		if (bRet)
		{
			m_lBlobAreaMaxThre = _ttol(cXMLConfigurator.GetElemData());
		}

		
		bRet = cXMLConfigurator.FindElem(_T("BlobOutMode"));
		if (bRet)
		{
			m_nBlobOutMode = _ttoi(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("BlobSearchNums"));
		if (bRet)
		{
			m_nBlobSearchNums = _ttoi(cXMLConfigurator.GetElemData());
		}
	}


}CBlobTrainSearchDataParam;

typedef struct tagHistogramSearchDataParam
{
	double			m_dHeadXoffset;				// 分析区基于精定位结果的X方向偏移 头
	double			m_dHeadYoffset;				// 分析区基于精定位结果的Y方向偏移 头
	double			m_dTailXoffset;				// 分析区基于精定位结果的X方向偏移 尾
	double			m_dTailYoffset;				// 分析区基于精定位结果的Y方向偏移 尾

	int			m_nHeadLength;				// 分析区长度 像素 头
	int			m_nHeadWidth;				// 分析区宽度 像素 头
	int			m_nTailLength;				// 分析区长度 像素 尾
	int			m_nTailWidth;				// 分析区长度 像素 尾

	int			m_MeanHighHead;    //模式
	int			m_nHeadGreyValue;
	int			m_nTailGreyValue;

	tagHistogramSearchDataParam()
	{
		m_dHeadXoffset				= 0;				
		m_dHeadYoffset				= 0;
		m_nHeadLength				= 50;
		m_nHeadWidth				= 30;

		m_dTailXoffset				= 0;				
		m_dTailYoffset				= 0;
		m_nTailLength				= 50;
		m_nTailWidth				= 30;


	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		// m_dHeadXoffset
		if(FALSE == cXMLConfigurator.FindElem(_T("HeadXoffset")))
		{
			cXMLConfigurator.AddElem(_T("HeadXoffset"));
		}
		cXMLConfigurator.SetElemData(m_dHeadXoffset);

		// m_dHeadXoffset
		if(FALSE == cXMLConfigurator.FindElem(_T("HeadYoffset")))
		{
			cXMLConfigurator.AddElem(_T("HeadYoffset"));
		}
		cXMLConfigurator.SetElemData(m_dHeadYoffset);


		// HeadLength
		if(FALSE == cXMLConfigurator.FindElem(_T("HeadLength")))
		{
			cXMLConfigurator.AddElem(_T("HeadLength"));
		}
		cXMLConfigurator.SetElemData(m_nHeadLength);

		// m_nHeadWidth
		if(FALSE == cXMLConfigurator.FindElem(_T("HeadWidth")))
		{
			cXMLConfigurator.AddElem(_T("HeadWidth"));
		}
		cXMLConfigurator.SetElemData(m_nHeadWidth);

		// m_dTailXoffset
		if(FALSE == cXMLConfigurator.FindElem(_T("TailXoffset")))
		{
			cXMLConfigurator.AddElem(_T("TailXoffset"));
		}
		cXMLConfigurator.SetElemData(m_dTailXoffset);

		// m_dTailYoffset
		if(FALSE == cXMLConfigurator.FindElem(_T("TailYoffset")))
		{
			cXMLConfigurator.AddElem(_T("TailYoffset"));
		}
		cXMLConfigurator.SetElemData(m_dTailYoffset);

		// m_nTailLength
		if(FALSE == cXMLConfigurator.FindElem(_T("TailLength")))
		{
			cXMLConfigurator.AddElem(_T("TailLength"));
		}
		cXMLConfigurator.SetElemData(m_nTailLength);

		// m_nTailWidth
		if(FALSE == cXMLConfigurator.FindElem(_T("TailWidth")))
		{
			cXMLConfigurator.AddElem(_T("TailWidth"));
		}
		cXMLConfigurator.SetElemData(m_nTailWidth);

		// m_MeanHighHead
		if(FALSE == cXMLConfigurator.FindElem(_T("MeanHighHead")))
		{
			cXMLConfigurator.AddElem(_T("MeanHighHead"));
		}
		cXMLConfigurator.SetElemData(m_MeanHighHead);

		// m_nHeadGreyValue
		if(FALSE == cXMLConfigurator.FindElem(_T("HeadGreyValue")))
		{
			cXMLConfigurator.AddElem(_T("HeadGreyValue"));
		}
		cXMLConfigurator.SetElemData(m_nHeadGreyValue);

		// m_nTailGreyValue
		if(FALSE == cXMLConfigurator.FindElem(_T("TailGreyValue")))
		{
			cXMLConfigurator.AddElem(_T("TailGreyValue"));
		}
		cXMLConfigurator.SetElemData(m_nTailGreyValue);

	}

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		bool bRet = false;
		bRet = cXMLConfigurator.FindElem(_T("HeadXoffset"));
		if (bRet)
		{
			m_dHeadXoffset = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("HeadYoffset"));
		if (bRet)
		{
			m_dHeadYoffset = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("HeadLength"));
		if (bRet)
		{
			m_nHeadLength = _ttol(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("HeadWidth"));
		if (bRet)
		{
			m_nHeadWidth = _ttol(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("TailXoffset"));
		if (bRet)
		{
			m_dTailXoffset = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("TailYoffset"));
		if (bRet)
		{
			m_dTailYoffset = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("TailLength"));
		if (bRet)
		{
			m_nTailLength = _ttol(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("TailWidth"));
		if (bRet)
		{
			m_nTailWidth = _ttol(cXMLConfigurator.GetElemData());
		}


		bRet = cXMLConfigurator.FindElem(_T("MeanHighHead"));
		if (bRet)
		{
			m_MeanHighHead = _ttol(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("HeadGreyValue"));
		if (bRet)
		{
			m_nHeadGreyValue = _ttol(cXMLConfigurator.GetElemData());
		}

		bRet = cXMLConfigurator.FindElem(_T("TailGreyValue"));
		if (bRet)
		{
			m_nTailGreyValue = _ttol(cXMLConfigurator.GetElemData());
		}
	}


}CHistogramSearchDataParam;


// 拾取专用定位工具训练参数
typedef struct tagSpecialPickSearchTrainDataParam : public CBaseTrainDataParam
{
	// 粗定位
	CSearchToolType m_Rough_SearchToolType;
	CPatternSearchTrainDataParam m_Struct_Rough_PatternSearchTrainData;			// 粗定位-区域定位工具训练参数
	CQuickSearchTrainDataParam m_Struct_Rough_QuickSearchTrainData;				// 粗定位-几何定位工具训练参数
	
	BOOL m_bEnableAccurateLocation;						// 是否启用精定位

	BOOL m_bEnableFourPoint;							//是否启用四标记点
	CSearchToolType m_Accurate_SearchToolType;
	// 涉及精定位二次区域/几何
	CPatternSearchTrainDataParam m_Struct_Accurate_PatternSearchTrainData[RECT_NUMS]; // 精定位-区域定位工具训练参数
	CQuickSearchTrainDataParam m_Struct_Accurate_QuickSearchTrainData[RECT_NUMS];	  // 精定位-几何定位工具训练参数

	// 涉及精定位找线用到的训练参数
	CCaliperTrainDataParam m_FindLineCaliperTrainDataParam[LINE_NUMS];

	// 涉及精定位找圆用的训练参数;
	CCaliperTrainDataParam m_FindCircleCaliperTrainDataParam[CIRCLE_NUMS];

	// 涉及精定位找Blob训练参数
	//bool			m_bBlobMode;							//亮Blob或暗Blob:true,暗blob;    false,亮blob
	//long			m_lBlobGrayThre;						//灰度硬分割阈值
	//long			m_lBlobAreaMinThre;						//最小面积阈值
	//long			m_lBlobAreaMaxThre;						//最大面积阈值
	CBlobTrainSearchDataParam m_Struct_Accurate_BlobTrainDataParam;

	// 头尾检查
	BOOL m_bEnableHeadTail;								//启用头尾检查
	CSearchToolType m_HeadTail_SearchToolType;
	CPatternSearchTrainDataParam m_Struct_HeadTail_PatternSearchTrainData;// 头尾检查-区域定位工具训练参数
	CQuickSearchTrainDataParam m_Struct_HeadTail_QuickSearchTrainData;//  头尾检查-几何定位工具训练参数
	CHistogramSearchDataParam m_Struct_HeadTail_HistogramSearchTrainData;


	// 正反检查
	BOOL m_bEnablePositiveAndNegative;					// 启用正反检查
	CSearchToolType m_PN_SearchToolType;
	CPatternSearchTrainDataParam m_Struct_PositiveAndNegative_PatternSearchTrainData;// 头尾检查-区域定位工具训练参数
	CQuickSearchTrainDataParam m_Struct_PositiveAndNegative_QuickSearchTrainData;//  头尾检查-几何定位工具训练参数


	// 混料防呆
	BOOL m_bEnableMixing;
	CSearchToolType m_Mixing_SearchToolType;
	CPatternSearchTrainDataParam m_Struct_Mixing_PatternSearchTrainData;// 头尾检查-区域定位工具训练参数
	CQuickSearchTrainDataParam m_Struct_Mixing_QuickSearchTrainData;//  头尾检查-几何定位工具训练参数



	tagSpecialPickSearchTrainDataParam()
	{
		// 粗定位
		m_Rough_SearchToolType = ePatternSearchTool;
		// 精定位
		m_bEnableAccurateLocation = TRUE;
		m_bEnableFourPoint = FALSE;
		m_Accurate_SearchToolType = ePatternSearchTool;


		//m_bBlobMode = false;				
		//m_lBlobGrayThre = 128;			
		//m_lBlobAreaMinThre= 100;	    
		//m_lBlobAreaMaxThre = 100000;	

		// 头尾检查
		m_bEnableHeadTail = FALSE;
		m_HeadTail_SearchToolType = ePatternSearchTool;

		// 正反检查
		m_bEnablePositiveAndNegative = FALSE;
		m_PN_SearchToolType = ePatternSearchTool;

		// 混料防呆
		m_bEnableMixing = FALSE;
		m_Mixing_SearchToolType = ePatternSearchTool;
	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eSpecialPickSearchTool;
	}

	virtual CSearchToolType GetRoughSearchToolType()
	{
		return m_Rough_SearchToolType;
	}

	virtual bool SetRoughSearchToolType(CSearchToolType mSearchToolType)
	{
		m_Rough_SearchToolType = mSearchToolType;
		return true;
	}

	virtual CSearchToolType GetAccurateSearchToolType()
	{
		return m_Accurate_SearchToolType;
	}
	virtual bool SetAccurateSearchToolType(CSearchToolType mInspectToolType)
	{
		m_Accurate_SearchToolType = mInspectToolType;
		return true;
	}
	
	virtual CSearchToolType GetHeadTailCheckToolType()
	{
		return m_HeadTail_SearchToolType;
	}
	virtual bool SetHeadTailCheckToolType(CSearchToolType mCheckToolType)
	{
		m_HeadTail_SearchToolType = mCheckToolType;
		return true;
	}

	virtual CSearchToolType GetPositiveAndNegativeCheckToolType()
	{
		return m_PN_SearchToolType;
	}
	virtual bool SetPositiveAndNegativeCheckToolType(CSearchToolType mCheckToolType)
	{
		m_PN_SearchToolType = mCheckToolType;
		return true;
	}

	virtual CSearchToolType GetMixingCheckToolType()
	{
		return m_Mixing_SearchToolType;
	}
	virtual bool SetMixingCheckToolType(CSearchToolType mCheckToolType)
	{
		m_Mixing_SearchToolType = mCheckToolType;
		return true;
	}

}CSpecialPickSearchTrainDataParam;

// 粗定位GUI
typedef struct tagRoughSearchTrainGuiParam
{
	scRect			m_TrainRect;						// 训练区域
	scAffineRect	m_TrainAffineRect;					// 几何训练区域 //或者Blob训练区

	sc2Vector		m_ModelPoint;						// 参考点
	sc2Vector		m_ModelPoint1;						// 参考点
	sc2Vector		m_ModelPoint2;						// 参考点
	scRect			m_SearchRect;						// 搜索区域
	bool			m_bTrainRectVisible;				// 训练区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见
	bool			m_bSearchRectVisible;				// 搜索区域是否可见
	int				m_nLineWidth;						// 线宽
	COLORREF		m_LineColor;						// 颜色

	tagRoughSearchTrainGuiParam()
	{
		m_bModelPointVisible			= true;
		m_bTrainRectVisible				= true;
		m_bSearchRectVisible			= true;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		// TrainRect
		//if(FALSE == cXMLConfigurator.FindElem(_T("TrainRect")))
		//{
		cXMLConfigurator.AddElem(_T("TrainRect"));
		//}

		cXMLConfigurator.EnterElem();

		// OriginX
		if(FALSE == cXMLConfigurator.FindElem(_T("OriginX")))
		{
			cXMLConfigurator.AddElem(_T("OriginX"));
		}
		cXMLConfigurator.SetElemData(m_TrainRect.GetOrigin().GetX());

		// OriginY
		if(FALSE == cXMLConfigurator.FindElem(_T("OriginY")))
		{
			cXMLConfigurator.AddElem(_T("OriginY"));
		}
		cXMLConfigurator.SetElemData(m_TrainRect.GetOrigin().GetY());

		// SizeX
		if(FALSE == cXMLConfigurator.FindElem(_T("SizeX")))
		{
			cXMLConfigurator.AddElem(_T("SizeX"));
		}
		cXMLConfigurator.SetElemData(m_TrainRect.GetSize().GetX());

		// SizeY
		if(FALSE == cXMLConfigurator.FindElem(_T("SizeY")))
		{
			cXMLConfigurator.AddElem(_T("SizeY"));
		}
		cXMLConfigurator.SetElemData(m_TrainRect.GetSize().GetY());

		cXMLConfigurator.LeaveElem();


		// TrainAffineRect
		//if(FALSE == cXMLConfigurator.FindElem(_T("TrainAffineRect")))
		//{
		cXMLConfigurator.AddElem(_T("TrainAffineRect"));
		//}
		cXMLConfigurator.EnterElem();

		// CenterX
		if(FALSE == cXMLConfigurator.FindElem(_T("CenterX")))
		{
			cXMLConfigurator.AddElem(_T("CenterX"));
		}
		cXMLConfigurator.SetElemData(m_TrainAffineRect.GetCenter().GetX());

		// CenterY
		if(FALSE == cXMLConfigurator.FindElem(_T("CenterY")))
		{
			cXMLConfigurator.AddElem(_T("CenterY"));
		}
		cXMLConfigurator.SetElemData(m_TrainAffineRect.GetCenter().GetY());

		// XLength
		if(FALSE == cXMLConfigurator.FindElem(_T("XLength")))
		{
			cXMLConfigurator.AddElem(_T("XLength"));
		}
		cXMLConfigurator.SetElemData(m_TrainAffineRect.GetXLength());

		// YLength
		if(FALSE == cXMLConfigurator.FindElem(_T("YLength")))
		{
			cXMLConfigurator.AddElem(_T("YLength"));
		}
		cXMLConfigurator.SetElemData(m_TrainAffineRect.GetYLength());

		// XRotation
		if(FALSE == cXMLConfigurator.FindElem(_T("XRotation")))
		{
			cXMLConfigurator.AddElem(_T("XRotation"));
		}
		cXMLConfigurator.SetElemData(m_TrainAffineRect.GetXRotation().ToDouble());

		// Skew
		if(FALSE == cXMLConfigurator.FindElem(_T("Skew")))
		{
			cXMLConfigurator.AddElem(_T("Skew"));
		}
		cXMLConfigurator.SetElemData(m_TrainAffineRect.GetSkew().ToDouble());

		cXMLConfigurator.LeaveElem();


		// search rect
		cXMLConfigurator.AddElem(_T("SearchRect"));
		cXMLConfigurator.EnterElem();

		// OriginX
		if(FALSE == cXMLConfigurator.FindElem(_T("OriginX")))
		{
			cXMLConfigurator.AddElem(_T("OriginX"));
		}
		cXMLConfigurator.SetElemData(m_SearchRect.GetOrigin().GetX());

		// OriginY
		if(FALSE == cXMLConfigurator.FindElem(_T("OriginY")))
		{
			cXMLConfigurator.AddElem(_T("OriginY"));
		}
		cXMLConfigurator.SetElemData(m_SearchRect.GetOrigin().GetY());

		// SizeX
		if(FALSE == cXMLConfigurator.FindElem(_T("SizeX")))
		{
			cXMLConfigurator.AddElem(_T("SizeX"));
		}
		cXMLConfigurator.SetElemData(m_SearchRect.GetSize().GetX());

		// SizeY
		if(FALSE == cXMLConfigurator.FindElem(_T("SizeY")))
		{
			cXMLConfigurator.AddElem(_T("SizeY"));
		}
		cXMLConfigurator.SetElemData(m_SearchRect.GetSize().GetY());

		cXMLConfigurator.LeaveElem();



		cXMLConfigurator.AddElem(_T("ModelPoint"));
		cXMLConfigurator.EnterElem();

		// CenterX
		if(FALSE == cXMLConfigurator.FindElem(_T("CenterX")))
		{
			cXMLConfigurator.AddElem(_T("CenterX"));
		}
		cXMLConfigurator.SetElemData(m_ModelPoint.GetX());

		// CenterY
		if(FALSE == cXMLConfigurator.FindElem(_T("CenterY")))
		{
			cXMLConfigurator.AddElem(_T("CenterY"));
		}
		cXMLConfigurator.SetElemData(m_ModelPoint.GetY());

		cXMLConfigurator.LeaveElem();


		cXMLConfigurator.AddElem(_T("ModelPoint1"));
		cXMLConfigurator.EnterElem();

		// CenterX
		if(FALSE == cXMLConfigurator.FindElem(_T("CenterX")))
		{
			cXMLConfigurator.AddElem(_T("CenterX"));
		}
		cXMLConfigurator.SetElemData(m_ModelPoint1.GetX());

		// CenterY
		if(FALSE == cXMLConfigurator.FindElem(_T("CenterY")))
		{
			cXMLConfigurator.AddElem(_T("CenterY"));
		}
		cXMLConfigurator.SetElemData(m_ModelPoint1.GetY());

		cXMLConfigurator.LeaveElem();


		cXMLConfigurator.AddElem(_T("ModelPoint2"));
		cXMLConfigurator.EnterElem();

		// CenterX
		if(FALSE == cXMLConfigurator.FindElem(_T("CenterX")))
		{
			cXMLConfigurator.AddElem(_T("CenterX"));
		}
		cXMLConfigurator.SetElemData(m_ModelPoint2.GetX());

		// CenterY
		if(FALSE == cXMLConfigurator.FindElem(_T("CenterY")))
		{
			cXMLConfigurator.AddElem(_T("CenterY"));
		}
		cXMLConfigurator.SetElemData(m_ModelPoint2.GetY());

		cXMLConfigurator.LeaveElem();
	}
	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		sc2Vector vPos;
		bool bRet = false;
		bRet = cXMLConfigurator.FindElem(_T("TrainRect"));
		if (bRet)
		{
			//return;
			cXMLConfigurator.EnterElem();
			bRet = cXMLConfigurator.FindElem(_T("OriginX"));
			if (bRet)
			{
				vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			bRet = cXMLConfigurator.FindElem(_T("OriginY"));
			if (bRet)
			{
				vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}
			m_TrainRect.SetOrigion(vPos);

			bRet = cXMLConfigurator.FindElem(_T("SizeX"));
			if (bRet)
			{
				vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			bRet = cXMLConfigurator.FindElem(_T("SizeY"));
			if (bRet)
			{
				vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			m_TrainRect.SetSize(vPos);
			cXMLConfigurator.LeaveElem();
		}

		

		////TrainAffineRect
		bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect"));
		if (bRet)
		{
			cXMLConfigurator.EnterElem();
			bRet = cXMLConfigurator.FindElem(_T("CenterX"));
			if (bRet)
			{
				vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}	

			bRet = cXMLConfigurator.FindElem(_T("CenterY"));
			if (bRet)
			{
				vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}	

			m_TrainAffineRect.SetCenter(vPos);

			bRet = cXMLConfigurator.FindElem(_T("XLength"));
			if (bRet)
			{
				m_TrainAffineRect.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			bRet = cXMLConfigurator.FindElem(_T("YLength"));
			if (bRet)
			{
				m_TrainAffineRect.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			bRet = cXMLConfigurator.FindElem(_T("XRotation"));
			if (bRet)
			{
				m_TrainAffineRect.SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
			}

			bRet = cXMLConfigurator.FindElem(_T("Skew"));
			if (bRet)
			{
				m_TrainAffineRect.SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));	
			}

			cXMLConfigurator.LeaveElem();
		}
		


		// search rect
		bRet = cXMLConfigurator.FindElem(_T("SearchRect"));
		if (bRet)
		{
			cXMLConfigurator.EnterElem();

			bRet = cXMLConfigurator.FindElem(_T("OriginX"));
			if (bRet)
			{
				vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			bRet = cXMLConfigurator.FindElem(_T("OriginY"));
			if (bRet)
			{
				vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			m_SearchRect.SetOrigion(vPos);

			bRet = cXMLConfigurator.FindElem(_T("SizeX"));
			if (bRet)
			{
				vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			bRet = cXMLConfigurator.FindElem(_T("SizeY"));
			if (bRet)
			{
				vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			m_SearchRect.SetSize(vPos);
			cXMLConfigurator.LeaveElem();
		}
		


		// model point
		bRet = cXMLConfigurator.FindElem(_T("ModelPoint"));
		if (!bRet)
		{
			//return;
		}
		else
		{
			cXMLConfigurator.EnterElem();

			bRet = cXMLConfigurator.FindElem(_T("CenterX"));
			if (bRet)
			{
				vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			bRet = cXMLConfigurator.FindElem(_T("CenterY"));
			if (bRet)
			{
				vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			m_ModelPoint = vPos;
			cXMLConfigurator.LeaveElem();
		}

		bRet = cXMLConfigurator.FindElem(_T("ModelPoint1"));
		if (!bRet)
		{

		}
		else
		{
			cXMLConfigurator.EnterElem();

			bRet = cXMLConfigurator.FindElem(_T("CenterX"));
			if (bRet)
			{
				vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			bRet = cXMLConfigurator.FindElem(_T("CenterY"));
			if (bRet)
			{
				vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			m_ModelPoint1 = vPos;
			cXMLConfigurator.LeaveElem();
		}

		bRet = cXMLConfigurator.FindElem(_T("ModelPoint2"));
		if (!bRet)
		{
			 
		}
		else
		{
			cXMLConfigurator.EnterElem();

			bRet = cXMLConfigurator.FindElem(_T("CenterX"));
			if (bRet)
			{
				vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			bRet = cXMLConfigurator.FindElem(_T("CenterY"));
			if (bRet)
			{
				vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}

			m_ModelPoint2 = vPos;
			cXMLConfigurator.LeaveElem();
		}
	


	}

}CRoughSearchTrainGuiParam;

// 找线GUI
typedef struct tagFindLineTrainGuiParam
{
	int				m_nCaliperNum;			// 卡尺个数
	double			m_dStartX;				// 起始点X
	double			m_dStartY;				// 起始点Y
	double			m_dEndX;				// 终止点X
	double			m_dEndY;				// 终止点Y
	double			m_dCaliperProjectLen;	// 投影长度
	double			m_dCaliperSearchLen;	// 搜索长度
	scRadian		m_CaliperSearchRotation;// 搜索角度
	COLORREF        m_LineColor;			// 图形颜色
	int				m_nLineWidth;			// 图形线宽
	bool			m_bLineVisible;			// 线是否可见
	bool			m_bEnableCornerLineMask;  // 启用掩膜
	bool			m_bCaliperVisible;			// 找线工具是否可见
	svStd vector<double> m_vdMaskData_FindLine;// 掩膜数据
	svStd vector<bool> m_vbCaliperMask_FindLine;// 卡尺是否被掩膜

	tagFindLineTrainGuiParam()
	{
		m_nCaliperNum				= 50;
		m_dStartX					= 500.0;
		m_dStartY					= 500.0;
		m_dEndX						= 1000.0;
		m_dEndY						= 500.0;
		m_dCaliperProjectLen		= 30.0;
		m_dCaliperSearchLen			= 250.0;
		m_CaliperSearchRotation		= scRadian(skPI/2.0);
		m_bEnableCornerLineMask		=false;

		m_LineColor						= RGB(0, 0, 255);
		m_nLineWidth					= 1;
		m_bLineVisible					= true;
		m_bEnableCornerLineMask       =false;
		m_bCaliperVisible				=false;

	}

	void SetInitPostion(double StartX,double StartY,double EndX,double EndY)
	{
		m_dStartX					= StartX;
		m_dStartY					= StartY;
		m_dEndX						= EndX;
		m_dEndY						= EndY;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		CString strInfo =  _T("");
		//CaliperNum1
		if(FALSE == cXMLConfigurator.FindElem(_T("CaliperNum1")))
		{
			cXMLConfigurator.AddElem(_T("CaliperNum1"));
		}
		cXMLConfigurator.SetElemData(m_nCaliperNum);

		//startX1
		if(FALSE == cXMLConfigurator.FindElem(_T("StartX1")))
		{
			cXMLConfigurator.AddElem(_T("StartX1"));
		}
		cXMLConfigurator.SetElemData(m_dStartX);


		// startY1
		if(FALSE == cXMLConfigurator.FindElem(_T("StartY1")))
		{
			cXMLConfigurator.AddElem(_T("StartY1"));
		}
		cXMLConfigurator.SetElemData(m_dStartY);

		// EndX1
		if(FALSE == cXMLConfigurator.FindElem(_T("EndX1")))
		{
			cXMLConfigurator.AddElem(_T("EndX1"));
		}
		cXMLConfigurator.SetElemData(m_dEndX);


		// EndY1
		if(FALSE == cXMLConfigurator.FindElem(_T("EndY1")))
		{
			cXMLConfigurator.AddElem(_T("EndY1"));
		}
		cXMLConfigurator.SetElemData(m_dEndY);


		// Project Len1
		if(FALSE == cXMLConfigurator.FindElem(_T("ProjectLen1")))
		{
			cXMLConfigurator.AddElem(_T("ProjectLen1"));
		}
		cXMLConfigurator.SetElemData(m_dCaliperProjectLen);


		// Search Len1
		if(FALSE == cXMLConfigurator.FindElem(_T("SearchLen1")))
		{
			cXMLConfigurator.AddElem(_T("SearchLen1"));
		}
		cXMLConfigurator.SetElemData(m_dCaliperSearchLen);

		// Search Rotation1
		if(FALSE == cXMLConfigurator.FindElem(_T("SearchRotation1")))
		{
			cXMLConfigurator.AddElem(_T("SearchRotation1"));
		}
		cXMLConfigurator.SetElemData(m_CaliperSearchRotation.ToDouble());

		//Caliper Visible1
		if(FALSE == cXMLConfigurator.FindElem(_T("CaliperVisible1")))
		{
			cXMLConfigurator.AddElem(_T("CaliperVisible1"));
		}
		cXMLConfigurator.SetElemData(m_bCaliperVisible);

		// CornerLineMaskEnable
		if(FALSE == cXMLConfigurator.FindElem(_T("CornerLineMaskEnable")))
		{
			cXMLConfigurator.AddElem(_T("CornerLineMaskEnable"));
		}
		cXMLConfigurator.SetElemData(m_bEnableCornerLineMask);


		CString strTmp = _T("MaskParam");
		cXMLConfigurator.DeleteElem(strTmp);
		cXMLConfigurator.AddElem(strTmp);

		cXMLConfigurator.EnterElem();
		strInfo.Format(_T("%d"),m_vdMaskData_FindLine.size());
		cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);

		for (int j=0; j<m_vdMaskData_FindLine.size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			strInfo.Format(_T("%f"), m_vdMaskData_FindLine.at(j));
			cXMLConfigurator.AddElem(strTmp, strInfo);
		}
		strInfo.Format(_T("%d"),m_vbCaliperMask_FindLine.size());
		cXMLConfigurator.AddElem(_T("CaliperMaskDataNum"), strInfo);
		for (int j=0; j<m_vbCaliperMask_FindLine.size(); j++)
		{
			strTmp.Format(_T("CaliperMaskData%d"), j);
			strInfo.Format(_T("%d"), (int)m_vbCaliperMask_FindLine.at(j));
			cXMLConfigurator.AddElem(strTmp, strInfo);
		}
		cXMLConfigurator.LeaveElem();


	}

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		bool bRet = false;

		// CaliperNum1
		bRet = cXMLConfigurator.FindElem(_T("CaliperNum1"));
		if (bRet)
		{
			m_nCaliperNum = _ttoi(cXMLConfigurator.GetElemData());
		}
		
		// startX
		bRet = cXMLConfigurator.FindElem(_T("StartX1"));
		if (bRet)
		{
			m_dStartX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		
		// startY
		bRet = cXMLConfigurator.FindElem(_T("StartY1"));
		if (bRet)
		{
			m_dStartY= _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		
		// EndX
		bRet = cXMLConfigurator.FindElem(_T("EndX1"));
		if (bRet)
		{
			m_dEndX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		
		// EndY
		bRet = cXMLConfigurator.FindElem(_T("EndY1"));
		if (bRet)
		{
			m_dEndY = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		
		// Project Len
		bRet = cXMLConfigurator.FindElem(_T("ProjectLen1"));
		if (bRet)
		{
			m_dCaliperProjectLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		
		// Search Len
		bRet = cXMLConfigurator.FindElem(_T("SearchLen1"));
		if (bRet)
		{
			m_dCaliperSearchLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		
		// Search Rotation
		bRet = cXMLConfigurator.FindElem(_T("SearchRotation1"));
		if (!bRet)
		{
			m_CaliperSearchRotation = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}

		// Caliper Visible
		bRet = cXMLConfigurator.FindElem(_T("CaliperVisible1"));
		if (!bRet)
		{
			m_bCaliperVisible = true;
		}
		else
		{
			m_bCaliperVisible = _ttoi(cXMLConfigurator.GetElemData());
		}
		
		bRet = cXMLConfigurator.FindElem(_T("CornerLineMaskEnable"));
		if (bRet)
		{
			m_bEnableCornerLineMask = _ttoi(cXMLConfigurator.GetElemData());
		}



		// 掩膜数据
		CString strTmp = _T("MaskParam");
		bRet = cXMLConfigurator.FindElem(strTmp);
		if (bRet)
		{
			cXMLConfigurator.EnterElem();
			bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
			if (bRet)
			{
				m_vdMaskData_FindLine.resize(_ttoi(cXMLConfigurator.GetElemData()));
				for (int j=0; j<m_vdMaskData_FindLine.size(); j++)
				{
					strTmp.Format(_T("MaskData%d"), j);
					bRet = cXMLConfigurator.FindElem(strTmp);
					if (!bRet) return;
					m_vdMaskData_FindLine.at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
				}
				bRet = cXMLConfigurator.FindElem(_T("CaliperMaskDataNum"));
				if (bRet)
				{
					m_vbCaliperMask_FindLine.resize(_ttoi(cXMLConfigurator.GetElemData()));
					for (int j=0; j<m_vbCaliperMask_FindLine.size(); j++)
					{
						strTmp.Format(_T("CaliperMaskData%d"), j);
						bRet = cXMLConfigurator.FindElem(strTmp);
						if (bRet) 
						{
							m_vbCaliperMask_FindLine.at(j) = _ttoi(cXMLConfigurator.GetElemData());
						}
					}
				}
			}


			cXMLConfigurator.LeaveElem();
		}
	}

}CFindLineTrainGuiParam;

// 找圆GUI
typedef struct tagFindCircleTrainGuiParam
{
	sc2Vector		m_CircleCenter;						// 圆心X
	double			m_dRadius;							// 半径
	scRadian		m_ArcStart;							// 起始弧度
	scRadian        m_ArcSpan;							// 弧度跨度
	int				m_nCaliperNum;						// 卡尺个数
	double			m_dCaliperProjectLen;				// 投影长度
	double			m_dCaliperSearchLen;				// 搜索长度
	CaliperSearchDirection m_CaliperSearchDirection;	// 搜索方向
	bool			m_bCircleVisible;					// 找圆工具是否可见
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	bool			m_bCircleCaliperVisible;			//找圆卡尺是否可见
	tagFindCircleTrainGuiParam()
	{
		m_CircleCenter				= sc2Vector(200,200);
		m_dRadius					= 100.0;
		m_ArcStart					= scRadian(0);
		m_ArcSpan					= scRadian(sk2PI);
		m_nCaliperNum				= 20;
		m_dCaliperProjectLen		= 10.0;
		m_dCaliperSearchLen			= 50.0;
		m_CaliperSearchDirection	= eInward;

		m_LineColor					= RGB(0, 0, 255);
		m_nLineWidth				= 1;
		m_bCircleVisible			= true;
		m_bCircleCaliperVisible		= false;
	}

	void SetInitPostion(double CenterX,double CenterY)
	{
		m_CircleCenter				= sc2Vector(CenterX,CenterY);
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{

	}

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{

	}


}CFindCircleTrainGuiParam;

// SpecialPickSearch定位工具训练Gui参数类
typedef struct tagSpecialPickSearchTrainGuiParam : public CBaseTrainGuiParam
{

	//粗定位训练GUI
	CRoughSearchTrainGuiParam m_RoughSearchTrainGuiParam;

	// 涉及精定位二次区域/几何
	CRoughSearchTrainGuiParam m_AccurateSearchTrainGuiParam[RECT_NUMS];
	
	// 涉及精定位找线工具
	CFindLineTrainGuiParam m_FindLineTrainGuiParam[LINE_NUMS];

	// 涉及精定位找圆工具GUI参数
	CFindCircleTrainGuiParam m_FindCircleTrainGuiParam[CIRCLE_NUMS];

	//涉及精定位Blob 训练GUI
	scAffineRect	m_BlobAffineRect;			       // Blob区域
	bool			m_bBlobAffineRectVisible;			            // Blob工具是否可见

	//涉及精定位固定点GUI
	CFixedPosSearchTrainGuiParam m_FixedPosSearchTrainGuiParam[POINT_NUMS]; //可能存在多个固定点


	//头尾检查
	// 区域或几何头尾检查时训练GUI
	CRoughSearchTrainGuiParam m_HeadTail_Check_TrainGuiParam;
	//涉及头尾的Blob训练GUI
	scAffineRect	m_BlobAffineRect_HeadTail;			       // Blob区域
	bool			m_bBlobVisible_HeadTail;			            // Blob工具是否可见
	// 涉及头尾亮暗分析训练GUI

	// 涉及头尾的亮度训练GUI
	//scAffineRect	m_BrightnessAffineRect_HeadTail１;
	//scAffineRect	m_BrightnessAffineRect_HeadTail２;
	//bool	m_bBrightnessVisible_HeadTail;

	// 正反检查
	CRoughSearchTrainGuiParam m_PN_Check_TrainGuiParam;
	//涉及正反的Blob训练GUI
	scAffineRect	m_BlobAffineRect_PN;			       // Blob区域
	bool			m_bBlobVisible_PN;			            // Blob工具是否可见
	// 涉及正反亮暗分析训练GUI
	scAffineRect	m_BrightnessAffineRect_PN;
	bool	m_bBrightnessVisible_PN;
	
	// 混料检测
	CRoughSearchTrainGuiParam m_Mixing_Check_TrainGuiParam;


	tagSpecialPickSearchTrainGuiParam()
	{
		if(LINE_NUMS >=4)
		{
			m_FindLineTrainGuiParam[0].SetInitPostion(270, 130, 420, 130);
			m_FindLineTrainGuiParam[1].SetInitPostion(270, 130, 270, 420);
			m_FindLineTrainGuiParam[2].SetInitPostion(270, 420, 420, 420);
			m_FindLineTrainGuiParam[3].SetInitPostion(420, 420, 420, 130);
		}

		// 初始化位置
		if(CIRCLE_NUMS >=4)
		{
			m_FindCircleTrainGuiParam[0].SetInitPostion(500,500);
			m_FindCircleTrainGuiParam[1].SetInitPostion(1000,500);
			m_FindCircleTrainGuiParam[2].SetInitPostion(1000,1000);
			m_FindCircleTrainGuiParam[3].SetInitPostion(500,1000);
		}

		m_bBlobAffineRectVisible = true;

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eSpecialPickSearchTool;
	}

}CSpecialPickSearchTrainGuiParam;

// 拾取专用定位工具搜索Data参数类
typedef struct tagSpecialPickSearchDataParam : public CBaseSearchDataParam
{
	// 粗定位搜索参数
	CPatternSearchSearchDataParam m_Struct_Rough_PatternSearchSearchData;
	CQuickSearchSearchDataParam m_Struct_Rough_QuickSearchSearchData;
	CBlobTrainSearchDataParam m_Rough_BlobSearchTrainData;						// 粗定位-Blob粗定位

	// 精定位Blob 时搜索参数
	bool			m_bBlobMode;				//亮Blob或暗Blob:true,暗blob;false,亮blob
	long			m_lBlobGrayThre;			//灰度阈值
	long			m_lConnectivityMinPels;		//修剪、填充的最小面积
	long			m_lBlobAreaMinThre;			//最小面积阈值
	long			m_lBlobAreaMaxThre;			//最大面积阈值
	int			m_nBlobOutMode1;				//参考点1设置输出模式
	int			m_nBlobOutMode2;				//参考点2设置输出模式

	bool	m_bCoordSortEnable;											//粗定位-启用搜索结果排序
	CoordSortMethod m_nCoordSortMethod;									//粗定位-排序方式
	SortOrder m_nCoordSortOrderX;										//粗定位-X排序
	SortOrder m_nCoordSortOrderY;										//粗定位-Y排序
	double		m_dCoordSortGap;										//粗定位-X排序间隔

	// 涉及精定位二次区域/几何搜索参数 （是否还需要设置搜索个数？？）
	CPatternSearchSearchDataParam m_Struct_Accurate_PatternSearchSearchData[RECT_NUMS]; // 精定位-区域定位工具训练参数
	CQuickSearchSearchDataParam m_Struct_Accurate_QuickSearchSearchData[RECT_NUMS];	//  精定位-几何定位工具训练参数

	bool m_bVisibleFitPoint_Line[LINE_NUMS];
	bool m_bVisibleFitPoint_Circle[CIRCLE_NUMS];


	CPatternSearchSearchDataParam m_HeadTail_PatternSearchSearchData;
	CQuickSearchSearchDataParam m_HeadTail_QuickSearchSearchData;

	CPatternSearchSearchDataParam m_PN_PatternSearchSearchData;
	CQuickSearchSearchDataParam m_PN_QuickSearchSearchData;

	CPatternSearchSearchDataParam m_Mixing_PatternSearchSearchData;
	CQuickSearchSearchDataParam m_Mixing_QuickSearchSearchData;

	// 头尾检查时
	int m_nHeadTail_FailType;	// 首尾检查失败时， 输出类型 0：定位失败    1：定位成功-PLC拾取后旋转180度     2：PLC正常拾取（先旋转180度再拾取）
	
	int	 m_nResultOutMode1;					// 矩形定位工具选择矩形输出点1的位置
	int	 m_nResultOutMode2;					// 矩形定位工具选择矩形输出点1的位置

	tagSpecialPickSearchDataParam()
	{
		m_bBlobMode				= false;				
		m_lBlobGrayThre			= 128;
		m_lConnectivityMinPels	= 100;
		m_lBlobAreaMinThre		= 100;	    
		m_lBlobAreaMaxThre		= 100000;	
		m_nBlobOutMode1			= 0;
		m_nBlobOutMode2			= 0;

		m_bCoordSortEnable = false;
		m_nCoordSortMethod = eCoordXOnly;
		m_nCoordSortOrderX = eAscending;
		m_nCoordSortOrderY = eAscending;
		m_dCoordSortGap = 64;

		for(int it =0;it<LINE_NUMS;it++)
		{
			m_bVisibleFitPoint_Line[it] = false;
		}

		for(int it =0;it<CIRCLE_NUMS;it++)
		{
			m_bVisibleFitPoint_Circle[it] = false;
		}

		m_nHeadTail_FailType = 2;
		m_nResultOutMode1 = 0;
		m_nResultOutMode2 = 1;
	}


	virtual CSearchToolType GetSearchToolType()
	{
		return eSpecialPickSearchTool;
	}

}CSpecialPickSearchSearchDataParam;

// SpecialPickSearch定位工具搜索Gui参数类
typedef struct tagSpecialPickSearchGuiParam : public CBaseSearchGuiParam
{

	COLORREF        m_LineColor;					// 图形颜色
	int				m_nLineWidth;					// 图形线宽
	bool			m_bRectVisible;					// 搜索结果矩形可见
	bool			m_bPointVisible;				// 参考点是否可见

	bool			m_bLineVisible;					// 找线工具是否可见
	bool			m_bLineCrossVisible;			// 找线工具是否可见

	bool			m_bVisibleFitPoint;				// 拟合数据点是否可见
	COLORREF        m_cLineColorOKFitPoint;			// 拟合数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint;			// 拟合数据点NG颜色

	bool			m_bFindLineVisibleFitPoint[LINE_NUMS];// 找线工具是否显示拟合点
	bool			m_bFindCircleVisibleFitPoint[CIRCLE_NUMS];// 找圆工具是否显示拟合点
	bool			m_bBlobVisible;						// Blob是否可见
	bool			m_bAffineRectVisible;

	bool			m_bBlobVisible_Contour;
	bool			m_bBlobVisible_Contour_Rough;

	tagSpecialPickSearchGuiParam()
	{
		m_LineColor					= RGB(0, 255, 0);
		m_nLineWidth				= 1;
		m_bRectVisible				= true;
		m_bPointVisible				= true;
		m_bLineVisible				= true;
		m_bLineCrossVisible			= true;
		m_bBlobVisible				= true;
		m_bVisibleFitPoint			= false;
		m_cLineColorOKFitPoint		= RGB(255,255,0);
		m_cLineColorNGFitPoint		= RGB(255,0,0);
		m_bAffineRectVisible		= true;
		m_bBlobVisible_Contour      = false;
		m_bBlobVisible_Contour_Rough = false;
		for (int i=0;i <LINE_NUMS;i++)
		{
			m_bFindLineVisibleFitPoint[i] = false;
		}

		for (int i=0;i <CIRCLE_NUMS;i++)
		{
			m_bFindCircleVisibleFitPoint[i] = false;
		}

	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eSpecialPickSearchTool;
	}

}CSpecialPickSearchSearchGuiParam;

//
typedef struct tagRectTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y

	double m_dModelX1; // 参考点1X					
	double m_dModelY1; // 参考点1Y

	double m_dModelX2; // 参考点2X					
	double m_dModelY2; // 参考点2Y
	tagRectTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
		m_dModelX2 = 0;
		m_dModelY2 = 0;
		m_dModelX1 = 0;
		m_dModelY1 = 0;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		//ModelX
		if(FALSE == cXMLConfigurator.FindElem(_T("ModelX")))
		{
			cXMLConfigurator.AddElem(_T("ModelX"));
		}
		cXMLConfigurator.SetElemData(m_dModelX);

		//m_dModelY
		if(FALSE == cXMLConfigurator.FindElem(_T("ModelY")))
		{
			cXMLConfigurator.AddElem(_T("ModelY"));
		}
		cXMLConfigurator.SetElemData(m_dModelY);


		//ModelX
		if(FALSE == cXMLConfigurator.FindElem(_T("ModelX1")))
		{
			cXMLConfigurator.AddElem(_T("ModelX1"));
		}
		cXMLConfigurator.SetElemData(m_dModelX1);

		//m_dModelY
		if(FALSE == cXMLConfigurator.FindElem(_T("ModelY1")))
		{
			cXMLConfigurator.AddElem(_T("ModelY1"));
		}
		cXMLConfigurator.SetElemData(m_dModelY1);

		//ModelX2
		if(FALSE == cXMLConfigurator.FindElem(_T("ModelX2")))
		{
			cXMLConfigurator.AddElem(_T("ModelX2"));
		}
		cXMLConfigurator.SetElemData(m_dModelX2);


		//ModelY2
		if(FALSE == cXMLConfigurator.FindElem(_T("ModelY2")))
		{
			cXMLConfigurator.AddElem(_T("ModelY2"));
		}
		cXMLConfigurator.SetElemData(m_dModelY2);
	}

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		bool bRet = false;
		bRet = cXMLConfigurator.FindElem(_T("ModelX"));
		if (bRet)
		{
			m_dModelX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		
		bRet = cXMLConfigurator.FindElem(_T("ModelY"));
		if (bRet)
		{
			m_dModelY =  _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("ModelX1"));
		if (bRet)
		{
			m_dModelX1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("ModelY1"));
		if (bRet)
		{
			m_dModelY1 =  _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("ModelX2"));
		if (bRet)
		{
			m_dModelX2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("ModelY2"));
		if (bRet)
		{
			m_dModelY2 =  _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

	}

}CRectTrainResult;

typedef struct tagFindLineTrainResult
{
	double m_dStartX; // 线段起点X					
	double m_dStartY; // 线段起点Y
	double m_dEndX;	  // 线段终点X
	double m_dEndY;	  // 线段终点Y
	tagFindLineTrainResult()
	{
		m_dStartX = 0;
		m_dStartY = 0;
		m_dEndX = 0;
		m_dEndY = 0;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator)
	{
		//StartX
		if(FALSE == cXMLConfigurator.FindElem(_T("StartX")))
		{
			cXMLConfigurator.AddElem(_T("StartX"));
		}
		cXMLConfigurator.SetElemData(m_dStartX);

		//StartY
		if(FALSE == cXMLConfigurator.FindElem(_T("StartY")))
		{
			cXMLConfigurator.AddElem(_T("StartY"));
		}
		cXMLConfigurator.SetElemData(m_dStartY);

		//EndX
		if(FALSE == cXMLConfigurator.FindElem(_T("EndX")))
		{
			cXMLConfigurator.AddElem(_T("EndX"));
		}
		cXMLConfigurator.SetElemData(m_dEndX);

		//EndY
		if(FALSE == cXMLConfigurator.FindElem(_T("EndY")))
		{
			cXMLConfigurator.AddElem(_T("EndY"));
		}
		cXMLConfigurator.SetElemData(m_dEndY);
	}

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator)
	{
		bool bRet = false;
		bRet = cXMLConfigurator.FindElem(_T("StartX"));
		if (bRet)
		{
			m_dStartX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("StartY"));
		if (bRet)
		{
			m_dStartY =  _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("EndX"));
		if (bRet)
		{
			m_dEndX =  _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("EndY"));
		if (bRet)
		{
			m_dEndY =  _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

	}



}CFindLineTrainResult;

typedef struct tagFindCircleTrainResult
{
	double m_dCenterX;
	double m_dCenterY;
	double m_dRadius;
	tagFindCircleTrainResult()
	{
		m_dCenterX = 0;
		m_dCenterY = 0;
		m_dRadius = 0;
	}
}CFindCircleTrainResult;



// 训练结果
typedef struct tagSpecialPickSearchTrainResult
{
	// 粗定位
	CRectTrainResult m_Rough_RectTrainResult;

	// 精定位
	// 涉及精定位二次区域/几何
	CRectTrainResult m_Accurate_RectTrainResult[RECT_NUMS];
	
	CFindLineTrainResult m_FindLineTrainResult[LINE_NUMS];

	CFindCircleTrainResult m_FindCircleTrainResult[CIRCLE_NUMS];



	tagSpecialPickSearchTrainResult()
	{
		

	}

	virtual CSearchToolType GetSearchToolType()
	{
		return eSpecialPickSearchTool;
	}
}CSpecialPickSearchTrainResult;


typedef struct tagFindLineTrainData2
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

	tagFindLineTrainData2()
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

}StructFindLineTrainData2;

class CSpecialPickSearchTool :public CBaseSearchTool
{
public:
	CSpecialPickSearchTool(void);
	CSpecialPickSearchTool(CSpecialPickSearchTool &cSpecialPickSearchTool);
	CSpecialPickSearchTool& operator=(CSpecialPickSearchTool &cSpecialPickSearchTool);
	~CSpecialPickSearchTool(void);
private:
	// 初始化成员
	void Initialize_Member_Variables();

	// 执行区域粗定位搜索
	bool ExecutePatternSerach(cpImage &ImageSearch,scPatternSearchRot* m_PatternSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dConfusionThre,double dTransX,double dTransY,double dRotation);

	// 执行几何粗定位搜索
	bool ExecuteQuickSearch(cpImage &ImageSearch,scQuickSearchTool* m_QuickSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dAngleLow,double m_dAngleHigh,double m_dSizeLow1,double m_dSizeHigh1,double dTransX,double dTransY,double dRotation);

	// 执行找线
	bool ExecuteFindLine(cpImage &ImageSearch,scFindLine* m_FindLineTool,StructFindLineTrainData2  m_sFindLineTrainData,double dTransX,double dTransY,double dRotation,scLineSeg &ResultLine);

	// GUI 显示矩形
	void ShowResultRect(std::vector<scGuiGraphic*>& m_pGuiSearchRectArray,scGuiAffineRect m_GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* m_AffineRect,const CString strRect,COLORREF color,int LineWidth);
	void ShowResultRect(std::vector<scGuiGraphic*>& m_pGuiSearchRectArray,scGuiAffineRect m_GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* m_AffineRect,const CString strRect,const CString strRectLabel,COLORREF color,int LineWidth);

	// GUI 显示交叉点
	void ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM*4],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,COLORREF color,int LineWidth);
	void ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM*4],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,const CString strCrossLabel,COLORREF color,int LineWidth);
	// GUI 显示线
	void ShowResultLine(std::vector<scGuiGraphic*>& m_pGuiSearchLineArray,scGuiLine m_GuiSearchLine[MAXRESNUM],const int nOffset,scLine *m_scLine,const CString strLine,COLORREF color,int LineWidth);



	// 训练区传递GUI参数
	void SetTrainGuiParam_For_Rect(scGuiCross *p_GuiModelPoint,scGuiRect *p_GuiTrainRect,scGuiAffineRect *p_GuiTrainAffineRect,scGuiRect *p_GuiSearchRect,CRoughSearchTrainGuiParam mp_RoughSearchTrainGuiParam);
	//void GetTrainGuiParam_For_Rect(scGuiCross *p_GuiModelPoint,scGuiRect *p_GuiTrainRect,scGuiAffineRect *p_GuiTrainAffineRect,scGuiRect *p_GuiSearchRect,CRoughSearchTrainGuiParam *mp_RoughSearchTrainGuiParam);

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
	
	virtual bool GetMaskImage(cpImage &ImageMask);											// 获取训练原始图像
	

	virtual bool SetMaskImage1(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool GetMaskImage1(cpImage &ImageMask);										// 获取训练原始图像
	virtual bool SetMaskImage2(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool GetMaskImage2(cpImage &ImageMask);										// 获取训练原始图像
	virtual bool SetMaskImage3(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool GetMaskImage3(cpImage &ImageMask);										// 获取训练原始图像
	virtual bool SetMaskImage4(const cpImage &ImageMask);									// 设置训练原始图像
	virtual bool GetMaskImage4(cpImage &ImageMask);										// 获取训练原始图像

	virtual bool SetMaskImage_HeadTail(const cpImage &ImageMask);								// 设置训练原始图像
	virtual bool GetMaskImage_HeadTail(cpImage &ImageMask);									// 获取训练原始图像
	virtual bool SetMaskImage_PN(const cpImage &ImageMask);								// 设置训练原始图像
	virtual bool GetMaskImage_PN(cpImage &ImageMask);									// 获取训练原始图像
	virtual bool SetMaskImage_Mixing(const cpImage &ImageMask);								// 设置训练原始图像
	virtual bool GetMaskImage_Mixing(cpImage &ImageMask);									// 获取训练原始图像

	virtual bool Train();																	// 执行训练
	virtual bool IsTrained();																// 是否训练成功
	virtual bool GetTrainResultImage(cpImage &ImageTrainResult);							// 获取训练结果图像
	virtual bool GetTrainResultImage2(cpImage &ImageTrainResult,const int index);							// 获取训练结果图像

	virtual bool GetTrainResultImage_HeadTail(cpImage &ImageTrainResult);
	virtual bool GetTrainResultImage_PN(cpImage &ImageTrainResult);
	virtual bool GetTrainResultImage_Mixing(cpImage &ImageTrainResult);

	virtual bool GetTrainResult(CBaseTrainResult *pTrainResult);							// 获取训练结果
	virtual bool GetPatternImage(cpImage &ImagePattern);	// 获取训练模板原始图像
	virtual bool GetPatternImage2(cpImage &ImagePattern);
	virtual bool GetPatternImage2(cpImage &ImagePattern,const int Index);
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
	
	bool ExecuteInspectSearch(const double dTransX,const double dTransY,const double dRotation,const cpImage &ImageSearch, CResultL2LInspectDistance &ResultL2LInspectDistance);

	bool GetFindLineCenterPoint(scFindLine* m_FindLineTool,sc2Vector &pointCenter2);

	bool SetPatternSearchToolParam(scPatternSearchRot *m_PatternSearchTool_Rough,CPatternSearchTrainDataParam Rough_PatternSearchTrainData);
	void SetPatternSearchToolImageMask(scPatternSearchRot *m_PatternSearchTool_Rough,CPatternSearchTrainDataParam Rough_PatternSearchTrainData,CRoughSearchTrainGuiParam Rough_PatternSearchTrainGui, cpImage &m_ImageTrain, cpImage &m_ImageMask);
	bool SetQuickSearchToolParam(scQuickSearchTool *m_QuickSearchTool_Rough,CQuickSearchTrainDataParam Rough_QuickSearchTrainData);
	void SetQuickSearchToolImageMask(scQuickSearchTool *m_QuickSearchTool_Rough,CQuickSearchTrainDataParam Rough_QuickSearchTrainData,CRoughSearchTrainGuiParam Rough_QuickSearchTrainGui, cpImage &m_ImageTrain, cpImage &m_ImageMask);

	void SetFindLineSearchToolParam(scFindLine *mFindLineTool,CCaliperTrainDataParam mFindLineTrainDataParam,CFindLineTrainGuiParam mFindLineTrainGuiParam);

	// 根据点构建2矩形区域判定头尾，执行灰度头尾判定
	BOOL ExeHistogramHeadTail(const double X1,const double Y1,const double D,const double X2,const double Y2,int i,bool &IsHead, cpImage &ImageSearch);


private:
	scPatternSearchRot					m_PatternSearchTool_Rough;		// 粗定位-区域定位工具		
	scQuickSearchTool                   m_QuickSearchTool_Rough;		// 粗定位-几何定位工具
	scBlobTool							m_BlobTool_Rough;			     // 粗定位 Blob工具

	scPatternSearchRot					m_PatternSearchTool_Accurate[RECT_NUMS];	// 精定位-区域定位工具
	scQuickSearchTool					m_QuickSearchTool_Accurate[RECT_NUMS];	// 精定位-几何定位工具

	// 找线工具
	scFindLine							m_FindLineTool_Accurate[LINE_NUMS];		// 涉及精定位找线工具
	// 找圆工具
	scFindCircle						m_FindCicleTool_Accurate[CIRCLE_NUMS]; // 涉及精定位找圆工具
	// Blob工具
	scBlobTool							m_BlobTool;			            // Blob工具


	// 头尾检查
	scPatternSearchRot					m_PatternSearchTool_HeadTail;		// 头尾检查-区域定位工具		
	scQuickSearchTool                   m_QuickSearchTool_HeadTail;			// 头尾检查-几何定位工具


	// 正反检查
	scPatternSearchRot					m_PatternSearchTool_PN;		// 正反检查-区域定位工具
	scQuickSearchTool                   m_QuickSearchTool_PN;		// 正反检查-几何定位工具

	// 混料防呆
	scPatternSearchRot					m_PatternSearchTool_Mixing;		// 混料检查-区域定位工具		
	scQuickSearchTool                   m_QuickSearchTool_Mixing;		// 混料检查-几何定位工具



	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd;			// 显示训练结果对象句柄
	scGuiDisplay					   *m_pGuiTrainResultDisplay2;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd2;			// 显示训练结果对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CSpecialPickSearchTrainDataParam		m_tmpTrainDataParam;			// 当前训练Data参数
	CSpecialPickSearchTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CSpecialPickSearchSearchDataParam		m_tmpSearchDataParam;			// 当前搜索Data参数	
	CSpecialPickSearchSearchGuiParam		m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CSpecialPickSearchTrainResult			m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 粗定位-当前训练图像
	cpImage							m_tmpImageMask;					// 粗定位-当前掩模图像

	//scImage8							m_tmpImageTrain_Accurate1;		// 精定位-当前训练图像
	cpImage							m_tmpImageMask_Accurate1;		// 精定位-当前掩模图像
	//scImage8							m_tmpImageTrain_Accurate2;		// 精定位-当前训练图像
	cpImage							m_tmpImageMask_Accurate2;		// 精定位-当前掩模图像
	//scImage8							m_tmpImageTrain_Accurate3;		// 精定位-当前训练图像
	cpImage							m_tmpImageMask_Accurate3;		// 精定位-当前掩模图像
	//scImage8							m_tmpImageTrain_Accurate4;		// 精定位-当前训练图像
	cpImage							m_tmpImageMask_Accurate4;		// 精定位-当前掩模图像

	cpImage							m_tmpImageMask_HeadTail;		// 头尾-当前掩模图像
	cpImage							m_tmpImageMask_PN;				// 正反-当前训练图像
	cpImage							m_tmpImageMask_Mixing;			// 混料-当前掩模图像

	CSpecialPickSearchTrainDataParam		m_TrainDataParam;				// 训练Data参数
	CSpecialPickSearchTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CSpecialPickSearchSearchDataParam		m_SearchDataParam;				// 搜索Data参数	
	CSpecialPickSearchSearchGuiParam		m_SearchGuiParam;				// 搜索Gui参数	
	CSpecialPickSearchTrainResult			m_TrainResult;					// 训练结果

	cpImage							m_ImageTrain;					// 粗定位-训练图像
	cpImage							m_ImageMask;					// 粗定位-掩模图像

	//scImage8							m_ImageTrain_Accurate1;		// 精定位-训练图像
	cpImage							m_ImageMask_Accurate1;		// 精定位-掩模图像
	//scImage8							m_ImageTrain_Accurate2;		// 精定位-训练图像
	cpImage							m_ImageMask_Accurate2;		// 精定位-掩模图像
	//scImage8							m_ImageTrain_Accurate3;		// 精定位-训练图像
	cpImage							m_ImageMask_Accurate3;		// 精定位-掩模图像
	//scImage8							m_ImageTrain_Accurate4;		// 精定位-训练图像
	cpImage							m_ImageMask_Accurate4;		// 精定位-掩模图像



	//scImage8							m_ImageMask_HeadTail;		// 头尾-当前掩模图像
	//scImage8							m_ImageMask_PN;				// 正反-当前训练图像
	//scImage8							m_ImageMask_Mixing;			// 混料-当前掩模图像

	cpImage							m_ImageTrainResult;			// 粗定位-训练结果图像

	cpImage							m_ImageTrainResult_Accurate1;// 精定位-训练结果图像
	cpImage							m_ImageTrainResult_Accurate2;// 精定位-训练结果图像
	cpImage							m_ImageTrainResult_Accurate3;// 精定位-训练结果图像
	cpImage							m_ImageTrainResult_Accurate4;// 精定位-训练结果图像


	//scImage8							m_ImageTrain_HeadTail;		// 头尾检查-训练图像
	cpImage							m_ImageMask_HeadTail;		// 头尾检查-掩模图像
	cpImage							m_ImageTrainResult_HeadTail;// 头尾检查-训练结果图像

	//scImage8							m_ImageTrain_PN;		// 正反检查-训练图像
	cpImage							m_ImageMask_PN;		// 正反检查-掩模图像
	cpImage							m_ImageTrainResult_PN;// 正反检查-训练结果图像

	//scImage8							m_ImageTrain_Mixing;		// 混料检查-训练图像
	cpImage							m_ImageMask_Mixing;		// 混料检查-掩模图像
	cpImage							m_ImageTrainResult_Mixing;// 混料检查-训练结果图像



	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;			// 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;							// 是否训练成功

	// 训练GUI
	scGuiRect							m_GuiTrainRect;							// 粗定位-区域训练矩形
	scGuiRect							m_GuiTrainRect_Accurate[RECT_NUMS];	// 精定位-区域训练矩形

	scGuiAffineRect						m_GuiTrainAffineRect;					  // 粗定位-几何训练区
	scGuiAffineRect						m_GuiTrainAffineRect_Accurate[RECT_NUMS]; // 精定位-几何训练区

	scGuiRect							m_GuiSearchRect;						//  粗定位-搜索矩形
	scGuiRect							m_GuiSearchRect_Accurate[RECT_NUMS];	// 精定位- 搜索矩形

	scGuiCross                          m_GuiModelPoint;						// 参考点1
	scGuiCross                          m_GuiModelPoint2;						// 参考点2
	scGuiCross                          m_GuiModelPoint_Accurate[RECT_NUMS];	// 精定位-参考点


	scGuiFindLine                       m_GuiFindLine[LINE_NUMS];		// 找线工具GUI
	scGuiLine                           m_GuiTrainResultLine[LINE_NUMS];	// 训练结果线段
	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽

	scGuiFindCircle						m_GuiFindCircle[CIRCLE_NUMS];	// 找圆工具GUI

	std::vector<scGuiGraphic*>          m_pGuiSearchBlobContour;			// 搜索结果
	scGuiContour						m_GuiSearchBlobContour[MAXRESNUM];

	std::vector<scGuiGraphic*>          m_pGuiSearchBlobContour_Rough;			// 搜索结果
	scGuiContour						m_GuiSearchBlobContour_Rough[MAXRESNUM];

	scGuiContour						m_GuiContour;                // Blob交互控件
	scGuiAffineRect						m_GuiTrainBlobAffineRect;		//  精定位-Blob训练区
	scGuiFeaturelet						m_GuiTrainFeaturelet;			// 训练特征链集
	scGuiFeaturelet						m_GuiTrainFeaturelet2;			// 训练特征链集

	// 头尾检查GUI
	scGuiRect							m_GuiTrainRect_HeadTail;		// 头尾检查-区域训练矩形
	scGuiAffineRect						m_GuiTrainAffineRect_HeadTail;	// 头尾检查-几何训练区
	scGuiRect							m_GuiSearchRect_HeadTail;		// 头尾检查-搜索矩形
	scGuiCross                          m_GuiModelPoint_HeadTail;		// 头尾检查-参考点

	// 正反检查GUI
	scGuiRect							m_GuiTrainRect_PN;				// 正反检查-区域训练矩形
	scGuiAffineRect						m_GuiTrainAffineRect_PN;	// 正反检查-几何训练区
	scGuiRect							m_GuiSearchRect_PN;		// 正反检查-搜索矩形
	scGuiCross                          m_GuiModelPoint_PN;		// 正反检查-参考点

	// 混料检查GUI
	scGuiRect							m_GuiTrainRect_Mixing;		// 混料检查-区域训练矩形
	scGuiAffineRect						m_GuiTrainAffineRect_Mixing;// 混料检查-几何训练区
	scGuiRect							m_GuiSearchRect_Mixing;		// 混料检查-搜索矩形
	scGuiCross                          m_GuiModelPoint_Mixing;		// 混料检查-参考点

	std::vector<scGuiGraphic*>          m_pGuiSearchRectArray;			// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;			// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点
	std::vector<scGuiGraphic*>          m_pGuiSearchLineTextArray;		// 搜索结果文本

	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray2;		// 搜索结果直线交点
	scGuiCross							m_GuiSearchLineCross2[MAXRESNUM];

	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray3;		// 搜索结果直线交点
	scGuiCross							m_GuiSearchLineCross3[MAXRESNUM];

	std::vector<scGuiGraphic*>          m_pGuiSearchRectArray_Accurate; //精定位结果矩形


	std::vector<scGuiGraphic*>          m_pGuiSearchRectArray_HeadTail; // 头尾结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray_HeadTail;// 搜索结果参考点

	scGuiAffineRect						m_GuiSearchAffineRect[MAXRESNUM]; // 搜索矩形
	scGuiCross							m_GuiSearchCross[MAXRESNUM];	  // 搜索线交点
	scGuiCross							m_GuiSearchCross_Line[MAXRESNUM];	// 找线的中点


	scGuiAffineRect						m_GuiSearchAffineRect_HeadTail[MAXRESNUM]; // 搜索矩形
	scGuiCross							m_GuiSearchCross_HeadTail[MAXRESNUM];	  // 搜索交点

	scGuiLine							m_GuiSearchLine[MAXRESNUM*2];
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];
	scGuiLineSeg						m_GuiSearchLineSeg[MAXRESNUM*2];

	scGuiAffineRect						m_GuiSearchAffineRect_A[MAXRESNUM];
	scGuiText							m_GuiSearchLineText[MAXRESNUM];



	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽


	std::vector<scGuiGraphic*>          m_pGuiSearchResult_Line_FitPointArray1;
	scGuiCoordCross						m_GuiSearchResult_Line_FitPoint1[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiSearchResult_Line_FitPointArray2;
	scGuiCoordCross						m_GuiSearchResult_Line_FitPoint2[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiSearchResult_Line_FitPointArray3;
	scGuiCoordCross						m_GuiSearchResult_Line_FitPoint3[MAXPTSNUM];
	std::vector<scGuiGraphic*>          m_pGuiSearchResult_Line_FitPointArray4;
	scGuiCoordCross						m_GuiSearchResult_Line_FitPoint4[MAXPTSNUM];

	std::vector<scGuiGraphic*>          m_pGuiSearchResult_Circle_FitPointArray1;
	scGuiCoordCross						m_GuiSearchResult_Circle_FitPoint1[MAXPTSNUM];

	std::vector<scGuiGraphic*>          m_pGuiSearchResult_Circle_FitPointArray2;
	scGuiCoordCross						m_GuiSearchResult_Circle_FitPoint2[MAXPTSNUM];

	std::vector<scGuiGraphic*>          m_pGuiSearchResult_Circle_FitPointArray3;
	scGuiCoordCross						m_GuiSearchResult_Circle_FitPoint3[MAXPTSNUM];

	std::vector<scGuiGraphic*>          m_pGuiSearchResult_Circle_FitPointArray4;
	scGuiCoordCross						m_GuiSearchResult_Circle_FitPoint4[MAXPTSNUM];

	// 
	std::vector<scGuiGraphic*>          m_pGuiSearchAffineRectArray;	// 搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchBlobArray;			// 搜索结果Blob
	std::vector<scGuiGraphic*>          m_pGuiBlobBoundCenterArray;	
	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray_Blob;			// 搜索结果参考点
	scGuiAffineRect						m_GuiSearchAffineRectBlob[MAXRESNUM];
	scGuiCross							m_GuiSearchCross_Blob[MAXRESNUM];
	scGuiCross							m_GuiBlobBoundCenterCross[MAXRESNUM];

	};

