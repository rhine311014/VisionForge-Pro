#pragma once

#pragma pack(push,8)

#include "svDef.h"
#include "svPose.h"
#include "svRect.h"
#include "svAffineRect.h"

// 映射点结构
typedef struct tagPointMapper{
	sc2Vector m_TrainPoint; // 训练点
	sc2Vector m_MatchPoint; // 匹配点
	int	m_nTrainIndex;
	int	m_nMatchIndex;
}PointMapper;

class SVCORE_API scPointMatcherResult : public scPose
{
public:
	scPointMatcherResult();

	double		GetScoreCoverage() { return m_dScoreCoverage; };
	double		GetScoreFit() { return m_dScoreFit; };

	scRect		 MatchRect()   { return m_MatchRect; };
	scAffineRect MatchRegion() { return m_MatchRegion; };

	svStd vector<PointMapper> GetPointMapper() { return m_vPointMapper; };

public:
	double	  m_dScoreCoverage;	// 覆盖分数
	double	  m_dScoreFit;		// 拟合分数

	scRect	  m_MatchRect;
	scAffineRect m_MatchRegion;

	svStd vector<PointMapper> m_vPointMapper;	// 点集映射列表
};

class SVCORE_API scPointMatcher 
{
public:
	scPointMatcher(void);
	virtual ~scPointMatcher();

public:
	//设置网格尺寸
	void	SetGridLengthAuto(bool  bEnabled);
	void	SetGridLength(int  nGridLength);
	bool	GetGridLengthAuto();
	int		GetGridLength();

	//设置捕获范围
	void	SetCaptureRangeAuto(bool  bEnabled);
	void	SetCaptureRange(double  dCaptureRange);	        
	bool	GetCaptureRangeAuto();
	double	GetCaptureRange();	 

	//设置尺度步长
	void    SetDofSizeStepSizeAuto(bool  bEnabled);
	void    SetDofSizeStepSize(double dDofSizeStepSize);
	bool    GetDofSizeStepSizeAuto();
	double  GetDofSizeStepSize();

	//设置角度步长
	void    SetDofRotateStepSizeAuto(bool  bEnabled);
	void    SetDofRotateStepSize(double dDofRotateStepSize);
	bool    GetDofRotateStepSizeAuto();
	double  GetDofRotateStepSize();

	//执行训练
	bool Train(  svStd vector<sc2Vector> vTrainPoints, double dModelOriginX, double dModelOriginY);
	
	//获取训练参考点
	bool	GetModelOrigin(sc2Vector& origin);

	//获取训练点集
	svStd vector<sc2Vector> GetTrainPoints();
    
	//获取训练区域
	scRect	GetTrainRectRegion();

	//保存载入训练结果
	bool	SaveTrainModelToFile  (const char* strFilePath);
	bool	LoadTrainModelFromFile(const char* strFilePath);
	
public:
	void	SetAcceptNum(long lAcceptNum);				    //设置可接受的目标个数
	void	SetAcceptThre(double dAcceptThre);				//设置可接受的覆盖分数阈值
	void	SetOverlapThre(double dOverlapThre);			//设置重叠阈值
		
	bool	SetDofRotate(double dRotate);					            //设置固定角度值
	bool	SetDofRotate(double dLow, double dHigh, double dDupRange);	//设置角度搜索范围
	bool	SetDofSize(double dSize);							        //设置固定一致缩放值
	bool	SetDofSize(double dLow, double dHigh, double dDupRange);	//设置一致缩放范围

	void	EnableSearchRect(bool enable);
	void	SetSearchRect(scRect roiSearchRect);
	
	//执行定位
	bool	Execute(svStd vector<sc2Vector> vMatchPoints);

	//获取定位结果
	long	GetResultNum();
	scPointMatcherResult 	GetPointMatcherResult(long nIndex);
	
private:
	void*				     m_hPointMatcher;
	svStd vector<scPointMatcherResult> 	 m_vResultArray;
};

#pragma pack(pop)