#pragma once

#include "svDef.h"
#include "svRect.h"
#include "svAffineRect.h"
#include "svImage.h"
#include "svPose.h"
#include "svFeature.h"
#include "svXMLConfigurator.h"

#pragma pack(push,8)

class SVGEOMSEARCH_API scGeomSearchResult : public scPose
{
public:
	scGeomSearchResult();

	double	       GetSizeX()		  const { return m_dSizeX; };
	double	       GetSizeY()		  const { return m_dSizeY; };
	double		   GetScore()		  const { return m_dScore; };
	double	       GetContrast()	  const { return m_dContrast; };
	scRect		   MatchRect()		  const { return m_MatchRect; };
	scAffineRect   MatchRegion()	  const { return m_MatchRegion; };
	double         GetWeightedScore() const { return m_dWeightedScore;};
	double		   GetEval()          const { return m_dEval; };
	double		   GetClutter()       const { return m_dClutter; };
	double		   GetCoverage()      const { return m_dCoverage; };
	double         GetFitError()      const { return m_dFitError;};
	bool           Accepted()         const { return m_bAccepted; };
	bool           isFineStage()      const { return m_bIsFineStage; }

	int            GetPatternIndex() const { return m_nPatternIndex; };
	int            GetDofIndex()     const { return m_nDofIndex;     };
	sc2XformLinear Get2XformLinear() const { return m_2XformLinear;  };

	sc2Vector	   GetAuxiliaryModelOrigin1() const { return m_AuxiliaryModelOrigin_1; }; 
	sc2Vector	   GetAuxiliaryModelOrigin2() const { return m_AuxiliaryModelOrigin_2; }; 

private:
	double	       m_dScore;           // 评价分数 
	double	       m_dSizeX;           // X方向缩放
	double	       m_dSizeY;           // X方向缩放
	double	       m_dContrast;        // 对比度
	scRect	       m_MatchRect;        // 匹配外接矩形
	scAffineRect   m_MatchRegion;      // 匹配区域

	int            m_nPatternIndex;    // 模板序号
	int            m_nDofIndex;        // 自由度序号
	sc2XformLinear m_2XformLinear;     // 训练坐标系到实时坐标系的2维线性变换

	double	       m_dWeightedScore;   // 预留加权评价分数，现与评价分数相同
	double         m_dEval;            // 评估值0~1
	double         m_dClutter;         // 混叠值0~正无穷
	double         m_dCoverage;        // 覆盖值0~1
	double         m_dFitError;        // 拟合误差0~正无穷

	bool           m_bAccepted;        // 该结果是否得到接收条件
	bool           m_bIsFineStage;     // 该结果是否由FineSearch阶段得到

	sc2Vector      m_AuxiliaryModelOrigin_1; //辅助标记点1
	sc2Vector      m_AuxiliaryModelOrigin_2; //辅助标记点2

private:

	friend class scGeomSearchResultSet;
	friend class scGeomSearchRuntime;
};

class SVGEOMSEARCH_API scGeomSearchResultSet
{
public:
	scGeomSearchResultSet();
	scGeomSearchResultSet(const std::vector<scGeomSearchResult>& vResults);
	scGeomSearchResultSet(const scGeomSearchResultSet& rhs);
	scGeomSearchResultSet& operator = (const scGeomSearchResultSet& rhs);

	~scGeomSearchResultSet();

public:
	//重置当前ResultSet为空
	void Reset();

	//返回result的个数
	long GetResultsNum() const;

	//返回所有的result
	const std::vector<scGeomSearchResult>& GetResults() const ;

	//返回序号为index的result
	const scGeomSearchResult& GetResult(long index) const;

	//对定位结果重排序
	void ReOrderByScore();
	void ReOrderByClutter();
	void ReOrderByCoverage();
	void ReOrderByFitError();
	void ReOrderByWeightedScore();
	void ReOrderByContrast();
	void ReOrderByPositonX();
	void ReOrderByPositonY();

private:

	std::vector<scGeomSearchResult> m_vResults;

	void copy(const scGeomSearchResultSet& rhs);

	friend class scGeomSearchRuntime;
};

class SVGEOMSEARCH_API scGeomSearchDof
{
public:
	scGeomSearchDof();
	scGeomSearchDof(const scGeomSearchDof& rhs);
	scGeomSearchDof& operator = (const scGeomSearchDof& rhs);
	~scGeomSearchDof();
public:
	// 设置rotate广义自由度
	bool	SetNominalRotate(scDegree rotate);
	bool	SetNominalRotate(double dRotate);
	bool	SetZoneRotate(scDegree low, scDegree high, scDegree dupRange);
	bool	SetZoneRotate(double dLow, double dHigh, double dDupRange);

	// 设置size广义自由度
	bool	SetNominalSize(double dSize);
	bool	SetZoneSize(double dLow, double dHigh, double dDupRange);

	// 设置sizex广义自由度
	bool	SetNominalSizeX(double dSizeX);
	bool	SetZoneSizeX(double dLow, double dHigh, double dDupRange);

	// 设置sizey广义自由度
	bool	SetNominalSizeY(double dSizeY);
	bool	SetZoneSizeY(double dLow, double dHigh, double dDupRange);

	// 获取rotate广义自由度
	bool    GetRotate(scDegree& low, scDegree& high, scDegree& dupRange) const;
	bool    GetRotate(double& dLow, double& dHigh, double& dDupRange) const;

	// 获取size广义自由度
	bool	GetSize(double& dLow, double& dHigh, double& dDupRange) const;

	// 获取sizex广义自由度
	bool	GetSizeX(double& dLow, double& dHigh, double& dDupRange) const;

	// 获取sizey广义自由度
	bool    GetSizeY(double& dLow, double& dHigh, double& dDupRange) const;

	// 获取是否为固定旋转
	bool    GetIsFixedRotate() const;

	// 获取是否为固定缩放
	bool    GetIsFixedSize() const;

	friend class scGeomSearchRuntime;

private:	
	void*	m_hDofList;

	void copy(const scGeomSearchDof& rhs);
	void clean();
};

class SVGEOMSEARCH_API scGeomSearchDofSet
{
public:
	scGeomSearchDofSet();
	scGeomSearchDofSet(std::vector<scGeomSearchDof>& vDofs);
	scGeomSearchDofSet(const scGeomSearchDofSet& rhs);
	scGeomSearchDofSet& operator = (const scGeomSearchDofSet& rhs);
	~scGeomSearchDofSet();

public:
	//重置当前DofSet为空
	void Reset();

	//插入序号为index的dof
	bool InsertDof(long index, const scGeomSearchDof& dof);

	//增加一个dof
	void AdddDof(const scGeomSearchDof& dof);

	//删除序号为index的dof
	bool DeleteDof(long index);

	//返回dof的个数
	long GetDofsNum() const;

	//返回所有的dof
	const std::vector<scGeomSearchDof>& GetDofs() const;

	//返回序号为index的dof
	const scGeomSearchDof& GetDof(long index) const;

private:
	std::vector<scGeomSearchDof> m_vDofs;

	void copy(const scGeomSearchDofSet& rhs);

	friend class scGeomSearchRuntime;
};

class SVGEOMSEARCH_API scGeomSearchPattern
{
public:
	scGeomSearchPattern();
	scGeomSearchPattern(const scGeomSearchPattern& rhs);
	scGeomSearchPattern& operator = (const scGeomSearchPattern& rhs);
	~scGeomSearchPattern();

public:

	// 设置/获取颗粒度
	bool	SetGranularity(double dGranularity);
	double	GetGranularity() const;
	bool	SetGranularityFine(double dGranularityFine);
	double	GetGranularityFine() const;

	// 设置/获取噪声阈值
	bool	SetNoiseThre(unsigned char ucNoiseThre = 10);
	int		GetNoiseThre() const;

	// 设置/获取梯度幅值和阈值 
	bool	SetTotalGradMagThre(long lTotalGradMagThre);
	long	GetTotalGradMagThre() const;

	// 设置/获取2个辅助的模板标记点 
	void	SetAuxiliaryModelOrigin(sc2Vector  origin1, sc2Vector  origin2);
	void	GetAuxiliaryModelOrigin(sc2Vector &origin1, sc2Vector &origin2) const;

	// 训练
	bool	Train(const scImageGray& image, scAffineRect affineRect, double dModelOriginX, double dModelOriginY);
	bool	Train(const scImageGray& image, const scShape* roi, double dModelOriginX, double dModelOriginY);	
	bool	Train(const scImageGray& image, const scImageGray& maskimage, const scShape* roi, double dModelOriginX, double dModelOriginY);
	bool	Train(const scImageGray& image, const scImageGray& maskimage, scAffineRect affineRect, double dModelOriginX, double dModelOriginY);

	// 清除训练内容, 恢复为初始状态
	void    UnTrain();

	// 是否已训练
	bool    IsTrained() const;

	// 模板链集是否可编辑	
	bool    TrainFeaturesEditable() const;

	// 获取训练模板特征对应的链集
	bool    GetTrainFeaturesLow(scFeatureletChainSet& featureChainSet, bool bPatternCoordinate = true) const;
	bool    GetTrainFeaturesHigh(scFeatureletChainSet& featureChainSet, bool bPatternCoordinate = true) const;
	bool    GetTrainFeaturesQuick(scFeatureletChainSet& featureChainSet, bool bPatternCoordinate = true) const;

	bool    SetEditedFeaturesQuick(const scFeatureletChainSet& featureChainSet, bool bPatternCoordinate = true);
	bool    SetEditedFeaturesLow(const scFeatureletChainSet& featureChainSet, bool bPatternCoordinate = true);
	bool    SetEditedFeaturesHigh(const scFeatureletChainSet& featureChainSet, bool bPatternCoordinate = true);

	// 获取训练参考点
	bool	GetModelOrigin(sc2Vector& origin) const;

	// 获取训练源图像
	bool    GetTrainSrcImage(scImageGray& image) const;

	// 获取训练结果图像
	bool	GetTrainResultImage(scImageGray& image) const;

	// 获取训练模板图像
	bool	GetTrainModelImage(scImageGray& image) const;

	// 获取训练区域
	scRect		 GetTrainRectRegion() const;	
	scAffineRect GetTrainAffineRectRegion() const;

	// 保存/载入模式
	bool	SavePatternToBinaryFile(const TCHAR* strFilePath, bool bSaveTrainSrcImg = true);
	bool	LoadPatternFromBinaryFile(const TCHAR* strFilePath);
	bool    SavePatternToXML(scXMLConfigurator* pXML, bool bSaveTrainSrcImg = true);
	bool    LoadPatternFromXML(scXMLConfigurator* pXML);

	// 获取当前模式的镜像模式
	bool    GetMirrorPattern(scGeomSearchPattern& pattern, MirrorType mirrorType) const;

private:
	void*	    m_hPatMaxPattern;

	bool        m_bIsTrained;
	scImageGray m_TrainSrcImage;

	sc2Vector   m_AuxiliaryOrigin_1;
	sc2Vector   m_AuxiliaryOrigin_2;

	void copy(const scGeomSearchPattern& rhs);
	void clean();

	friend class scGeomSearchRuntime;
	friend class scGeomSearchPatternSet;
};

class SVGEOMSEARCH_API scGeomSearchPatternSet
{
public:
	scGeomSearchPatternSet();
    scGeomSearchPatternSet(const std::vector<scGeomSearchPattern>& vPatterns);
	scGeomSearchPatternSet(const scGeomSearchPatternSet& rhs);
	scGeomSearchPatternSet& operator = (const scGeomSearchPatternSet& rhs);
	~scGeomSearchPatternSet();

public:
	//重置当前PatternSet为空
	void Reset();

	//插入序号为index的pattern
	bool InsertPattern(long index, const scGeomSearchPattern& pattern);

	//增加一个pattern
	void AddPattern(const scGeomSearchPattern& pattern);

	//删除序号为index的pattern
	bool DeletePattern(long index);

	//返回pattern的个数
	long GetPatternsNum() const;

	//返回所有的pattern
	const std::vector<scGeomSearchPattern>& GetPatterns() const;

	//返回序号为index的pattern
	const scGeomSearchPattern& GetPattern(long index) const;

	// 保存/载入到文件
	bool SaveToBinaryFile(const TCHAR* strFilePath, bool bSaveTrainSrcImg = true);
	bool LoadFromBinaryFile(const TCHAR* strFilePath);
	bool SaveToXML(scXMLConfigurator* pXML, bool bSaveTrainSrcImg = true);
	bool LoadFromXML(scXMLConfigurator* pXML);

private:
	std::vector<scGeomSearchPattern> m_vPatterns;

	void copy(const scGeomSearchPatternSet& rhs);
	friend class scGeomSearchRuntime;
};

class SVGEOMSEARCH_API scGeomSearchRuntime
{
public:
	scGeomSearchRuntime();
	~scGeomSearchRuntime();

public:
	// 设置/获取评价分数阈值
	bool	SetAcceptThreshold(double accept);
	double	GetAcceptThreshold() const;

	// 设置/获取对比度阈值
	bool	SetContrastThreshold(long contrast =10);
	long	GetContrastThreshold() const;

	// 设置/获取重叠阈值
	bool	SetOverlapThreshold(double overlap);
	double	GetOverlapThreshold() const;

	// 设置/获取目标个数
	bool    SetNumToFind(long num);
	long	GetNumToFind() const;

	// 设置/获取是否忽略极性
	void	SetIgnorePolarity(bool bIgnore);
	bool	GetIgnorePolarity() const;

	// 设置/获取是否全图搜索
	void	EnableFullImage(bool bEnable = true);
	bool	GetIsFullImage() const;

	// 设置/获取是否采用高精度算法
	void	EnableAlgoAccu(bool bEnable = true);
	bool	GetIsAlgoAccu() const;

	// 设置/ 获取低/高分辨率attract次数， 设置不同的次数对整体速度影响不明显
	bool SetAttractTimes(int low = 2, int high = 2); 
	int  GetAttractTimesLow() const;
	int  GetAttractTimesHigh() const;

	// 设置/获取 clutter 分数阈值
	bool   SetClutterThld(double thld = -1.0); 
	double GetClutterThld() const;

	// 设置/获取 fiterror 阈值
	bool   SetFitErrorThld(double thld = -1.0); 
	double GetFitErrorThld() const;

	// 设置/获取 允许删除粗定位重叠结果，用于删除粗定位重叠结果，在搜索不到最佳模式时尝试使用，重叠值越大，处理速度会慢一些
	// 在不设置此参数的情况下，内部默认为true
	void	EnableDeleteCoarseOverlap(bool bEnable = true);
	bool	GetIsDeleteCoarseOverlap() const;

	// 设置训练模板/训练模板集合
	bool    SetPattern(const scGeomSearchPattern& pattern);
	bool    SetPatternSet(const scGeomSearchPatternSet&  patternSet);
    
	// 获取训练模板集合
	const scGeomSearchPatternSet&  GetPatternSet() const;

	// 清空训练模板集合
	void    ResetPatternSet();

	// 获取训练模板数量
	long    GetPatternNum() const;

	// 设置/获取广义自由度
	bool    SetDof(const scGeomSearchDof& dof = scGeomSearchDof());
	bool    SetDofSet(const scGeomSearchDofSet&  dofSet);

	// 获取广义自由度集合
	const scGeomSearchDofSet&  GetDofSet() const;

	//清空广义自由度
	void    ResetDofSet();

	//获取广义自由度数量
	long    GetDofNum() const;

	// 执行定位
	bool	Execute(const scImageGray& image, scRect roi);

	// 获取定位结果个数
	long	GetResultNum() const;

	// 获取定位结果
	scGeomSearchResultSet GetResultSet() const;                        // 按score由高到低
	scGeomSearchResultSet GetResultSetOrderByClutter() const;          // 按clutter由低到高
	scGeomSearchResultSet GetResultSetOrderByCoverage() const;         // 按Coverage由高到低
	scGeomSearchResultSet GetResultSetOrderByFitError() const;         // 按FitError由低到高
	scGeomSearchResultSet GetResultSetOrderByWeightedScore() const;    // 按加权分数由高到低
	scGeomSearchResultSet GetResultSetCoarseStage() const;             // 返回粗定位阶段结果

	// 保存/载入搜索参数
	bool    SaveSearchParamToFile(const TCHAR* strFilePath);
	bool    LoadSearchParamFromFile(const TCHAR* strFilePath);

	// 获取搜索ROI rect参数
	scRect  GetSearchRect() const;

private:	
	void*	m_hPatMaxRuntime;

	long	m_lAcceptNum;
	bool	m_bEnableAccu;
	bool    m_bFullImage;  // 是否全图搜索
	scRect  m_SearchRect;

	scGeomSearchPatternSet m_PatternSet; 
	scGeomSearchDofSet     m_DofSet;
	scGeomSearchResultSet  m_ResultSet;
};

#pragma pack(pop)