#pragma once

#include "svFeature.h"
#include "svRect.h"
#include "svAffineRect.h"
#include "svCircle.h"
#include "svMeasure.h"

#pragma pack(push,8)

// 按照什么排序
typedef enum tagConcaveSortType
{
	eSortByArea = 0,				// 按照面积进行排序
	eSortByDeepth,					// 按照深度进行排序
	eSortByWidth,					// 按照宽度进行排序
	eSortByLength,				    // 按照周长进行排序
}CConcaveSortType;

typedef struct tagConcaveDefect
{
	scFeatureletVector			m_vecConcaveContour;     //凹缺陷轮廓点序列
	double						m_dConcaveArea;          //凹缺陷面积
	double						m_dConvaveDeepth;        //凹缺陷深度
	double                      m_dConcaveWidth;		 //凹缺陷宽度
	tagConcaveDefect(){
		m_vecConcaveContour.clear();
		m_dConcaveArea   = 0;
		m_dConvaveDeepth = 0;
		m_dConcaveWidth  = 0;
	}
	void SetConcaveContour(const scFeatureletVector& contour){
		m_vecConcaveContour.clear();
		m_vecConcaveContour = contour;
	}
}CConcaveDefect;

class SVCORE_API scContourAnalysis
{
public:
	scContourAnalysis();
	scContourAnalysis(const scFeatureletVector& contour);
	scContourAnalysis(const std::vector<POINT>& contour);
	scContourAnalysis(const std::vector<sc2Vector>& contour);
	~scContourAnalysis();

public:
	void SetContour(const scFeatureletVector& contour);
	void SetContour(const std::vector<POINT>& contour);
	void SetContour(const std::vector<sc2Vector>& contour);
	void GetContour(scFeatureletVector& contour);

public:
	bool	IsConvex();									//是否为凸包

	bool	BoundingRect(scRect& rect);					//外接矩形
	bool	MinEncloseRect(scAffineRect& affrect);		//最小外接矩形
	bool	MinEncloseCircle(scCircle& circle);			//最小外接圆
	bool    MaxInscribedRect(scRect& rect);				//最大内接矩形
	bool	MaxInscribedCircle(scCircle& circle);		//最大内接圆（线性规划法求解）

	//缺陷过滤设置
	void	EnableFilterWidth(bool bEnable);
	bool	IsEnabledFilterWidth();
	void	SetFilterWidthThre(double dThre);
	double	GetFilterWidthThre(); 

	void	EnableFilterDeepth(bool bEnable);
	bool	IsEnabledFilterDeepth();
	void	SetFilterDeepthThre(double dThre);
	double  GetFilterDeepthThre();

	void	EnableFilterArea(bool bEnable);
	bool	IsEnabledFilterArea();
	void	SetFilterAreaThre(double dThre);
	double  GetFilterAreaThre();

	void	EnableFilterLength(bool bEnable);
	bool	IsEnabledFilterLength();
	void	SetFilterLengthThre(int nThre);
	int     GetFilterLengthThre();

	//缺陷排序设置
	void EnableSort(bool bEnable, CConcaveSortType eSortType ,SortOrder eOrder);

	//执行轮廓凹缺陷检测
	bool Execute(std::vector<CConcaveDefect> & concaveDef);

public:
	bool ConvexHull();								//获取轮廓凸包
	scFeatureletVector GetConvexHullFeacturelet();	//轮廓凸包序列
	std::vector<long>  GetConvexHullIndex();		//轮廓凸包序列对应的索引

private:
	bool GetConcaveDefect(std::vector<CConcaveDefect> & concaveDef);																							//获取缺陷轮廓数组                                                      
	bool CalConcaveAreaAndDeepth(const scFeatureletVector vecConcaveContour ,double & m_dConvaveDeepth, double & m_dConcaveArea);                               //计算轮廓深度和面积
	bool CalRotateParam(const scFeatureletVector vecConcaveContour,double & dXBias, double & dYBias , double & dRotateTheta );                                  //计算坐标变换相关参数
	bool CalConcaveArea(const scFeatureletVector vecConcaveContour,const double dXBias ,const double dYBias, const double dRotateTheta ,double & dConcaveArea); //计算轮廓面积
	std::vector<scFeatureletVector> SplitConcaveContour(const scFeatureletVector srcFeaturelet);                                                                //分割凸包点中的轮廓

private:
	scFeatureletVector                 m_listFeaturelet;																	//原始轮廓
	scFeatureletVector                 m_listConvexHullFeaturelet;															//凸包轮廓
	std::vector<long>                  m_listConvexHullIndex;																//凸包索引
					                   
	//过滤参数	  	               
	bool                               m_bEnableFilterWidth;
	double                             m_dFilteWidthThre;
	bool                               m_bEnableFilterDeepth;
	double                             m_dFilteDeepthThre;
	bool                               m_bEnableFilterArea;
	double                             m_dFilteAreaThre;
	bool                               m_bEnableFilterLength;
	int				                   m_nFilteLengthThre;
	
	//排序参数
	bool                               m_bEnableSort;
	CConcaveSortType                   m_eSortType;
	SortOrder                          m_eSortOrder;
};

#pragma pack(pop)