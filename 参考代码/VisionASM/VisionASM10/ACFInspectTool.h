#pragma once

#include "BaseSearchTool.h"
#include "svPatternSearchRot.h"
#include "svQuickSearch.h"
#include "svGuiAffineRect.h"
#include "svGuiRect.h"
#include "svGuiCross.h"
#include "svGuiContour.h"
#include "svGuiFeaturelet.h"

#include "svFind.h"
#include "svGuiFindLine.h" 
#include "svGuiLine.h"
#include "svGuiCoordCross.h"

#pragma pack(push,8)

#define  MAXROWNUM 5		// 等分行数目
#define  MAXCOLNUM 5		// 等分列数目
#define  MAXTOOLNUM 4		// 默认检测区个数
#define  MAXALLOWTOOLNUM 20 // 最大允许检测区个数
#define LINE_NUM 2
// ACFInspect定位工具训练Data参数类
typedef struct tagACFInspectTrainDataParam : public CBaseTrainDataParam
{

	CSearchToolType m_Rough_SearchToolType;
	CoarseScale		m_CoarseScale;						// 最粗定位层次
	Accuracy		m_Accuracy;							// 定位精度
	double			m_dAngleLow;						// 低角度
	double			m_dAngleHigh;						// 高角度
	bool			m_bMaskEnable;						// 是否设置掩模图像

	double			m_dGrau;							// 颗粒度
	long			m_lNoiseThre;						// 噪声阈值
	long			m_lTotalMag;						// 幅值和

	// 找线工具
	double			m_dContrastThre[LINE_NUM] ;			// 对比度阈值
	long			m_lFilterHalfWidth[LINE_NUM];		// 滤波器半宽
	int				m_nOutLineNum[LINE_NUM];			// 局外点
	EdgePolarity	m_edgeProperty[LINE_NUM];			// 边缘属性
	bool			m_bSortByScore[LINE_NUM];			// 按分数排序
	BOOL			m_bEnableDualEdge[LINE_NUM];		// 启用/禁用找线工具双边缘
	//int				m_nFitPointIndex[LINE_NUM];			// 拟合点序号


	bool			m_bEnableBaseSearch;
	std::vector<BOOL>			m_bEnableHistogram;					//启用灰度检测
	std::vector<BOOL>			m_bEnableBlob;						//启用Blob检测

	int            m_nInsAreaNum;   //检测区域个数

	// Blob检测
	std::vector<int> m_nDefectType;
	std::vector<int> m_nBinaryThre;
	std::vector<int> m_nMinAcceptArea;
	std::vector<int> m_nMaxAcceptArea;
	std::vector<double> m_dMaxDefectArea;

	std::vector<double>     m_dMinMean; //最小亮度
	std::vector<double>     m_dMaxMean; //最大亮度

	std::vector<double>     m_dMinStdDev; //最小方差
	std::vector<double>     m_dMaxStdDev; //最大方差

	std::vector<BOOL>       m_bMeanInvalid;  //是否使用亮度检测
	std::vector<BOOL>       m_bStdDevInvalid;  //是否使用方差检测

	std::vector<BOOL>       m_bMeanInvert;     //亮度检测结果取反
	std::vector<BOOL>       m_bStdDevInvert;   //方差检测结果取反

	// 单个检测区域的纵向等分数目
	std::vector<int>		m_nColNum;
	// 单个检测区域的横向等分数目
	std::vector<int>		m_nRowNum;
	std::vector<int>		m_nNGAllowed;		// 单个检测区域等分内允许NG等分区域的数目  等分区域NG的数目大于该值时才整体失败；


	tagACFInspectTrainDataParam()
	{
		m_nInsAreaNum = MAXTOOLNUM;
		m_Rough_SearchToolType = ePatternSearchTool;
		m_bEnableBaseSearch = false; // 默认不开启定位
		m_CoarseScale	=	CoarseScale32;
		m_Accuracy		=	AccuracyCoarse;
		m_dAngleLow		=	-2;
		m_dAngleHigh	=	2;
		m_bMaskEnable	=	false;

		m_dGrau			= 2.0;
		m_lNoiseThre	= 10;
		m_lTotalMag		= 200;

		// 找线
		for (int i=0; i<LINE_NUM; i++)
		{
			m_dContrastThre[i]		= 1.0;
			m_lFilterHalfWidth[i]	= 1;
			m_nOutLineNum[i]		= 0;
			m_edgeProperty[i]		= eDontCare;
			m_bSortByScore[i]		= true;
			m_bEnableDualEdge[i]	= false;
		}

		for (int i=0;i<m_nInsAreaNum;i++)
		{
// 			m_dCurMean.push_back(-1.0);
// 			m_dCurStdDev.push_back(-1.0);

			m_nDefectType.push_back(0);
			m_nBinaryThre.push_back(80);
			m_nMinAcceptArea.push_back(3);
			m_nMaxAcceptArea.push_back(100000);
			m_dMaxDefectArea.push_back(50.0);

			m_dMinMean.push_back(0.0);
			m_dMaxMean.push_back(255.0);
			m_dMinStdDev.push_back(0.0);
			m_dMaxStdDev.push_back(255.0);

			m_bMeanInvalid.push_back(TRUE);
			m_bStdDevInvalid.push_back(TRUE);

			m_bMeanInvert.push_back(FALSE);
			m_bStdDevInvert.push_back(FALSE);

			m_nRowNum.push_back(1);
			m_nColNum.push_back(1);
			m_nNGAllowed.push_back(0);  

			m_bEnableHistogram.push_back(TRUE);
			m_bEnableBlob.push_back(TRUE);

		}


	}
	virtual CSearchToolType GetSearchToolType()
	{
		return eACFInspectTool;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator);

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator);

	virtual CSearchToolType GetRoughSearchToolType()
	{
		return m_Rough_SearchToolType;
	}

	virtual bool SetRoughSearchToolType(CSearchToolType mSearchToolType)
	{
		m_Rough_SearchToolType = mSearchToolType;
		return true;
	}

}CACFInspectTrainDataParam;

// ACFInspect定位工具训练Gui参数类
typedef struct tagACFInspectTrainGuiParam : public CBaseTrainGuiParam
{
	scRect			m_TrainRect;						// 训练区域
	scAffineRect	m_TrainAffineRect;					// 几何训练区域
	sc2Vector		m_ModelPoint;						// 参考点
	scRect			m_SearchRect;						// 搜索区域
	bool			m_bTrainRectVisible;				// 搜索区域是否可见
	bool			m_bModelPointVisible;				// 模板参考点是否可见

	
	COLORREF          m_LineColor;						// 图形颜色
	int				m_nLineWidth;						// 图形线宽
	BOOL			m_bSearchRectVisible;				// 检测区域是否可见
    std::vector<scAffineRect*>		m_InspectAffineRect;						// 检测区域

	// 找线工具
	int				m_nCaliperNum[LINE_NUM];			// 卡尺个数
	double			m_dStartX[LINE_NUM];				// 起始点X
	double			m_dStartY[LINE_NUM];				// 起始点Y
	double			m_dEndX[LINE_NUM];					// 终止点X
	double			m_dEndY[LINE_NUM];					// 终止点Y
	double			m_dCaliperProjectLen[LINE_NUM];		// 投影长度
	double			m_dCaliperSearchLen[LINE_NUM];		// 搜索长度
	scRadian		m_CaliperSearchRotation[LINE_NUM];	// 搜索角度
	bool			m_bCaliperVisible[LINE_NUM];		// 卡尺是否可见

	// 找线工具掩膜
	svStd vector<double> m_vdCaliperMaskData[LINE_NUM];			// 掩膜数据
	svStd vector<bool>   m_vbCaliperMask[LINE_NUM];				// 卡尺是否被掩膜
	bool			     m_bEnableLineMask[LINE_NUM];			// 是否启用掩膜
	bool			m_bLineVisible;						// 找线工具是否可见




	int            m_nInsAreaNum;						//检测区域个数

	tagACFInspectTrainGuiParam()
	{
		m_bSearchRectVisible = TRUE;
		m_LineColor = RGB(0,0,255);
		m_nLineWidth = 1;
		m_nInsAreaNum = MAXTOOLNUM;
		for (int i=0;i<m_nInsAreaNum;i++)
		{ 
			scAffineRect* tempAffineRect = new scAffineRect;
			m_InspectAffineRect.push_back(tempAffineRect);

		}

		m_nCaliperNum[0]				= 2;
		m_dStartX[0]					= 100.0;
		m_dStartY[0]					= 100.0;
		m_dEndX[0]						= 100.0;
		m_dEndY[0]						= 100.0;
		m_dCaliperProjectLen[0]			= 20.0;
		m_dCaliperSearchLen[0]			= 100.0;
		m_CaliperSearchRotation[0]		= scRadian(skPI/2.0);
		m_bEnableLineMask[0]			= false;
		m_bCaliperVisible[0]			= false;

		m_nCaliperNum[1]				= 2;
		m_dStartX[1]					= 100.0;
		m_dStartY[1]					= 100.0;
		m_dEndX[1]						= 100.0;
		m_dEndY[1]						= 100.0;
		m_dCaliperProjectLen[1]			= 20.0;
		m_dCaliperSearchLen[1]			= 100.0;
		m_CaliperSearchRotation[1]		= scRadian(skPI/2.0);
		m_bEnableLineMask[1]			= false;
		m_bCaliperVisible[1]			= false;

		m_bModelPointVisible = true;
		m_bTrainRectVisible	 = true;
		m_bLineVisible		 = true;
		
	}

	~tagACFInspectTrainGuiParam()
	{
		for (int i=0;i<m_InspectAffineRect.size();i++)
		{
			if (m_InspectAffineRect.at(i)!=NULL)
			{
				delete m_InspectAffineRect.at(i);
				m_InspectAffineRect.at(i)= NULL;
			}
		}
		m_InspectAffineRect.clear();
	}

	tagACFInspectTrainGuiParam& operator=(tagACFInspectTrainGuiParam& cACFInspectToolExTrainGuiParam)
	{

		// 找线工具
		m_nCaliperNum[0]				= cACFInspectToolExTrainGuiParam.m_nCaliperNum[0];		
		m_dStartX[0]					= cACFInspectToolExTrainGuiParam.m_dStartX[0];				
		m_dStartY[0]					= cACFInspectToolExTrainGuiParam.m_dStartY[0];				
		m_dEndX[0]						= cACFInspectToolExTrainGuiParam.m_dEndX[0];				
		m_dEndY[0]						= cACFInspectToolExTrainGuiParam.m_dEndY[0];					
		m_dCaliperProjectLen[0]			= cACFInspectToolExTrainGuiParam.m_dCaliperProjectLen[0];		
		m_dCaliperSearchLen[0]			= cACFInspectToolExTrainGuiParam.m_dCaliperSearchLen[0];		
		m_CaliperSearchRotation[0]		= cACFInspectToolExTrainGuiParam.m_CaliperSearchRotation[0];
		m_bEnableLineMask[0]			= cACFInspectToolExTrainGuiParam.m_bEnableLineMask[0];		
		m_bCaliperVisible[0]			= cACFInspectToolExTrainGuiParam.m_bCaliperVisible[0];		

		m_nCaliperNum[1]				= cACFInspectToolExTrainGuiParam.m_nCaliperNum[1];			
		m_dStartX[1]					= cACFInspectToolExTrainGuiParam.m_dStartX[1];				
		m_dStartY[1]					= cACFInspectToolExTrainGuiParam.m_dStartY[1];				
		m_dEndX[1]						= cACFInspectToolExTrainGuiParam.m_dEndX[1];					
		m_dEndY[1]						= cACFInspectToolExTrainGuiParam.m_dEndY[1];					
		m_dCaliperProjectLen[1]			= cACFInspectToolExTrainGuiParam.m_dCaliperProjectLen[1];		
		m_dCaliperSearchLen[1]			= cACFInspectToolExTrainGuiParam.m_dCaliperSearchLen[1];		
		m_CaliperSearchRotation[1]		= cACFInspectToolExTrainGuiParam.m_CaliperSearchRotation[1];	
		m_bEnableLineMask[1]			= cACFInspectToolExTrainGuiParam.m_bEnableLineMask[1];	
		m_bCaliperVisible[1]			= cACFInspectToolExTrainGuiParam.m_bCaliperVisible[1];		

		m_bModelPointVisible			= cACFInspectToolExTrainGuiParam.m_bModelPointVisible;
		m_bTrainRectVisible				= cACFInspectToolExTrainGuiParam.m_bTrainRectVisible;
		m_bLineVisible					= cACFInspectToolExTrainGuiParam.m_bLineVisible;

		m_nInsAreaNum = cACFInspectToolExTrainGuiParam.m_nInsAreaNum;
		for (int i=0;i<m_InspectAffineRect.size();i++)
		{
			if (m_InspectAffineRect.at(i)!=NULL)
			{
				delete m_InspectAffineRect.at(i);
				m_InspectAffineRect.at(i)= NULL;
			}
		}
		m_InspectAffineRect.clear();

		for(int i=0;i<cACFInspectToolExTrainGuiParam.m_InspectAffineRect.size();i++)
		{
			scAffineRect* tempAffineRect = new scAffineRect;
			if(cACFInspectToolExTrainGuiParam.m_InspectAffineRect[i])
			{
				*tempAffineRect = *cACFInspectToolExTrainGuiParam.m_InspectAffineRect[i];
			}else
			{
				tempAffineRect->SetCenterLengthsRotAndSkew(sc2Vector(250+i*100, 200), 50, 50,scRadian(0),scRadian(0));
			}
			m_InspectAffineRect.push_back(tempAffineRect);
		}
		m_LineColor = cACFInspectToolExTrainGuiParam.m_LineColor;					// 图形颜色
		m_nLineWidth = cACFInspectToolExTrainGuiParam.m_nLineWidth;					// 图形线宽
		m_bSearchRectVisible = cACFInspectToolExTrainGuiParam.m_bSearchRectVisible;	// 检测区域是否可见
		m_TrainRect= cACFInspectToolExTrainGuiParam.m_TrainRect;
		m_TrainAffineRect= cACFInspectToolExTrainGuiParam.m_TrainAffineRect;					
		m_ModelPoint= cACFInspectToolExTrainGuiParam.m_ModelPoint;						
		m_SearchRect= cACFInspectToolExTrainGuiParam.m_SearchRect;						
		return *this;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator);


	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator);

	virtual CSearchToolType GetSearchToolType()
	{
		return eACFInspectTool;
	}
}CACFInspectTrainGuiParam;

// ACFInspect定位工具搜索Data参数类
typedef struct tagACFInspectSearchDataParam : public CBaseSearchDataParam
{
	int		m_nSearchNum;								// 搜索个数
	double	m_dAcceptThre;								// 接收阈值
	double  m_dConfusionThre;							// 混淆阈值。混淆阈值不小于接收阈值

	double  m_dAngleLow;								// 低角度
	double	m_dAngleHigh;								// 高角度
	double  m_dSizeLow;
	double  m_dSizeHigh;
	bool	m_bEnableSharpScore;						
	double	m_dSharpThre;

	tagACFInspectSearchDataParam()
	{
		m_nSearchNum	= 1;
		m_dAcceptThre	= 0.7;
		m_dConfusionThre = 0.8;
		m_dAngleLow		= -5.0;
		m_dAngleHigh	= 5.0;
		m_dSizeLow = 1.0;
		m_dSizeHigh = 1.0;

		m_bEnableSharpScore = false;
		m_dSharpThre = 0.5;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator);
	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator);

	virtual CSearchToolType GetSearchToolType()
	{
		return eACFInspectTool;
	}

}CACFInspectSearchDataParam;

// ACFInspect定位工具搜索Gui参数类
typedef struct tagACFInspectSearchGuiParam : public CBaseSearchGuiParam
{
	COLORREF        m_LineColor;						// 图形颜色
	int				m_nLineWidth;					// 图形线宽
	BOOL			m_bAffineRectVisible;				// 搜索结果矩形可见
	BOOL			m_bModelPointVisible;				// 参考点是够可见

	bool			m_bRectVisible;						// 区域搜索结果矩形可见


	bool			m_bLineVisible;						// 找线工具是否可见
	bool			m_bLineCrossVisible;				// 找线工具是否可见

	bool			m_bEnableShowLinePoint;				// 拟合数据点是否可见
	bool			m_bEnableShowLinePoint1;			// 拟合数据点是否可见
	COLORREF        m_cLineColorOKFitPoint;				// 拟合数据点OK颜色
	COLORREF        m_cLineColorNGFitPoint;				// 拟合数据点NG颜色

	tagACFInspectSearchGuiParam()
	{
		m_bModelPointVisible = TRUE;
		m_LineColor = RGB(0, 0, 255);
		m_nLineWidth = 1;
		m_bAffineRectVisible = TRUE;

		m_bRectVisible	= true;
		m_bLineVisible			= true;
		m_bLineCrossVisible		= true;

		m_bEnableShowLinePoint = false;
		m_bEnableShowLinePoint1 = false;
		m_cLineColorOKFitPoint = RGB(255,255,0);
		m_cLineColorNGFitPoint = RGB(255,0,0);

	}



	virtual CSearchToolType GetSearchToolType()
	{
		return eACFInspectTool;
	}

}CACFInspectSearchGuiParam;

// ACFInspect训练结果
typedef struct tagACFInspectTrainResult
{
	double m_dModelX; // 参考点X					
	double m_dModelY; // 参考点Y
	double m_dSharpness;	//模板清晰度值

	double m_dStartX[LINE_NUM]; // 线段起点X
	double m_dStartY[LINE_NUM]; // 线段起点Y
	double m_dEndX[LINE_NUM];	  // 线段终点X
	double m_dEndY[LINE_NUM];	  // 线段终点Y

	tagACFInspectTrainResult()
	{
		m_dModelX = 0;
		m_dModelY = 0;
		m_dSharpness = 1.0;

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
		return eACFInspectTool;
	}

	void SaveModelToFile(vcXMLConfigurator& cXMLConfigurator);

	void LoadModelFromFile(vcXMLConfigurator& cXMLConfigurator);

}CACFInspectTrainResult;

//
//	ACFInspect定位工具
//
class /*SV_SEARCHTOOL_API*/ CACFInspectTool : public CBaseSearchTool  
{
public:
	CACFInspectTool();
	CACFInspectTool(CACFInspectTool &cACFInspectToolEx);
	CACFInspectTool& operator=(CACFInspectTool &cACFInspectToolEx);
	virtual ~CACFInspectTool();
public:
	bool	m_bInspectFinish;
	bool	m_bInspectOK;

// 	double	m_dDefectArea;
// 	int 	m_nMaxGreyVal;
	std::vector<double>  	m_dCurMean;   //当前亮度（平均值）
	std::vector<double>     m_dCurStdDev; //当前均匀度（方差）
	std::vector<double>     m_dCurDefArea; //当前缺陷面积（方差）
	std::vector<double>     m_dmaxGrayValue;//当前最大灰度值
	//scGuiContour m_GuiContour;                 // Blob交互控件
public:

	//////////////////////////工具属性相关////////////////////////////////
	virtual CSearchToolType GetSearchToolType();										    // 获取工具类型


	//////////////////////////显示更新相关////////////////////////////////
	virtual bool SetDisplay(scGuiDisplay *pGuiDisplay);										// 设置显示控件指针
	virtual bool SetTrainResultDisplay(scGuiDisplay *pGuiDisplay);							// 设置训练结果显示控件指针
	virtual scGuiDisplay* GetTrainResultDisplay();											// 获取训练结果显示控件指针
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



public:
	bool SaveTrainGuiData();		
	virtual void  ResizeToolsAllParam(int nNewToolNum);
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
	bool ClearTrainVectorGui();																// 清除vector中的Gui
	// 执行区域粗定位搜索
	bool ExecutePatternSerach(cpImage &ImageSearch,scPatternSearchRot* m_PatternSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dConfusionThre,double dTransX,double dTransY,double dRotation);

	// 执行几何粗定位搜索
	bool ExecuteQuickSearch(cpImage &ImageSearch,scQuickSearchTool* m_QuickSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dAngleLow,double m_dAngleHigh,double m_dSizeLow1,double m_dSizeHigh1,double dTransX,double dTransY,double dRotation);
	// GUI 显示矩形
	void ShowResultRect(std::vector<scGuiGraphic*>& m_pGuiSearchRectArray,scGuiAffineRect m_GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* m_AffineRect,const CString strRect,COLORREF color,int LineWidth);
	void ShowResultRect(std::vector<scGuiGraphic*>& m_pGuiSearchRectArray,scGuiAffineRect m_GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* m_AffineRect,const CString strRect,const CString strRectLabel,COLORREF color,int LineWidth);

	// GUI 显示交叉点
	void ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM*4],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,COLORREF color,int LineWidth);
	void ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM*4],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,const CString strCrossLabel,COLORREF color,int LineWidth);
private:
	
	scPatternSearchRot					m_PatternSearchTool;		// 粗定位-区域定位工具		
	scQuickSearchTool                   m_QuickSearchTool;			// 粗定位-几何定位工具

	scFindLine							m_FindLineTool[LINE_NUM];		// 找线工具
	scGuiDisplay					   *m_pGuiTrainResultDisplay;		// 显示训练结果对象指针
	HWND								m_hTrainResultDispWnd;			// 显示训练结果对象句柄

	scGuiDisplay					   *m_pGuiDisplay;					// 显示对象指针
	HWND                                m_hDispWnd;						// 显示对象句柄
	scGuiInteractiveContainer          *m_pGuiInteractiveContainer;		// 动态容器
	scGuiStaticContainer			   *m_pGuiStaticContainer;			// 静态容器
	CString                             m_strKeyText;					// 关键字前缀
	CACFInspectTrainDataParam			m_tmpTrainDataParam;			// 当前训练Data参数
	CACFInspectTrainGuiParam			m_tmpTrainGuiParam;				// 当前训练Gui参数	
	CACFInspectSearchDataParam			m_tmpSearchDataParam;			// 当前搜索Data参数	
	CACFInspectSearchGuiParam			m_tmpSearchGuiParam;			// 当前搜索Gui参数
	CACFInspectTrainResult				m_tmpTrainResult;				// 当前训练结果
	cpImage							m_tmpImageTrain;				// 当前训练图像
	cpImage							m_tmpImageMask;					// 当前掩模图像
	CACFInspectTrainDataParam			m_TrainDataParam;				// 训练Data参数
	CACFInspectTrainGuiParam			m_TrainGuiParam;				// 训练Gui参数	
	CACFInspectSearchDataParam			m_SearchDataParam;				// 搜索Data参数	
	CACFInspectSearchGuiParam			m_SearchGuiParam;				// 搜索Gui参数
	CACFInspectTrainResult				m_TrainResult;					// 训练结果
	cpImage							m_ImageTrain;					// 训练图像
	cpImage							m_ImageMask;					// 掩模图像
 	cpImage							m_ImageTrainResult;				// 训练结果图像
	std::vector<CSearchResult>			m_SearchResultArray;			// 结果保存
	bool								m_bTrainGuiShow;				// 是否显示训练Gui
	bool                                m_bTrainResultGuiShow;			// 是否显示训练结果Gui
	bool								m_bSearchResultGuiShow;			// 是否显示搜索结果Gui
	bool								m_bTrainOK;						// 是否训练成功
	
	// 训练GUI
			
	

	scGuiCross                          m_GuiModelPoint;				// 参考点
	scGuiRect							m_GuiSearchRect;				// 搜索矩形

	scGuiRect							m_GuiTrainRectRough;			// 训练矩形
	scGuiAffineRect                     m_GuiTrainAffineRect;			// 训练矩形

	scGuiFindLine                       m_GuiFindLine[LINE_NUM];		// 两个找线工具
	scGuiLine                           m_GuiTrainResultLine[LINE_NUM];	// 训练结果线段
	COLORREF                            m_TrainGUIColor;				// 训练图形颜色
	int                                 m_nTrainGuiLineWidth;			// 训练图形线宽


	scGuiFeaturelet						m_GuiTrainFeaturelet;			// 训练特征链集	
	std::vector<scGuiAffineRect*>       m_GuiTrainRect;				// 检测训练矩形

	// 搜索结果GUI
	std::vector<scGuiGraphic*>          m_pGuiSearchRectArray;			// 区域搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchAffineRectArray;	// 几何搜索结果矩形
	std::vector<scGuiGraphic*>          m_pGuiSearchLineArray;			// 搜索结果直线
	std::vector<scGuiGraphic*>          m_pGuiSearchLineCrossArray;		// 搜索结果直线交点	

	scGuiCross							m_GuiSearchCross2[MAXRESNUM*2];	// 搜索结果参考点
	scGuiLine							m_GuiSearchLine[MAXRESNUM*2];	// 搜索结果直线
	scGuiCross							m_GuiSearchLineCross[MAXRESNUM];// 搜索结果直线的交点

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPointArray_1;	// 搜索结果拟合点1
	scGuiCoordCross						m_GuiSearchResultFitPoint_1[4096];

	std::vector<scGuiGraphic*>          m_pGuiSearchResultFitPointArray_2;	// 搜索结果拟合点2
	scGuiCoordCross						m_GuiSearchResultFitPoint_2[4096];

	COLORREF                            m_SearchResultColor;			// 搜索结果颜色
	int                                 m_nSearchResultGuiLineWidth;	// 搜索结果线宽

	// 搜索结果GUI
	scGuiAffineRect						m_GuiSearchAffineRect[MAXALLOWTOOLNUM];
	std::vector<scGuiGraphic*>          m_pGuiSearchResultRectArray;	// 搜索结果矩形

	scGuiAffineRect						m_GuiSearchAffineRect2[MAXRESNUM];
	std::vector<scGuiGraphic*>          m_pGuiSearchResultRectArray2;	// 初定位搜索结果矩形

	std::vector<scGuiGraphic*>          m_pGuiSearchCrossArray;			// 搜索结果参考点
	scGuiCross							m_GuiSearchCross[MAXALLOWTOOLNUM];			// 搜索线交点

	std::vector<scGuiGraphic*>          m_pGuiSearchBlobContour;		// 搜索结果
	scGuiContour						m_GuiSearchBlobContour[MAXALLOWTOOLNUM];
	
	scGuiAffineRect						m_GuiNGAffineRect[MAXALLOWTOOLNUM*MAXCOLNUM*MAXROWNUM];// 等分NG显示
	std::vector<scGuiGraphic*>          m_pGuiNGAffineRectArray;	// 搜索结果矩形

};

#pragma pack(pop)