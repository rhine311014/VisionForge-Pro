#pragma once

#include "svPolyline.h"
#include "svFeature.h"

#pragma pack(push,8)

// 轮廓过滤工具：先轮廓平滑，再轮廓过滤
class SVCORE_API scContourFilterTool
{
public:
	scContourFilterTool();
	~scContourFilterTool();

	// 设置/获取平滑参数(小于1时，不做平滑处理)；平滑参数越大，轮廓越平滑
	void SetSmoothPara(int nSmoothPara);
	int GetSmoothPara()const;

	// 设置/获取过滤距离阈值；距离阈值越大，过滤去除掉的点数越多，结果点数量越少
	void SetDistanceThre(double dDisThre);
	double GetDistanceThre()const;

	// 执行单个轮廓过滤
	bool Execute(const scPolyline& inputContour);
	bool Execute(const scFeatureletVector& inputContour,bool bClosed);
	bool Execute(const std::vector<sc2Vector>& vInputPoints,bool bClosed);

	// 执行多个轮廓过滤
	bool Execute(const scFeatureletChainSet& inputContour);

	// 获取执行结果(平滑+过滤的结果)：nIndex为轮廓索引号，单个轮廓索引号为0，多个轮廓索引号为0~n-1
	std::vector<sc2Vector> GetResultVertices(int nIndex = 0)const;
	scPolyline GetResultPolyline(int nIndex = 0)const;
	scFeatureletVector GetResultFeatureletVector(int nIndex = 0)const;

	// 获取所有执行结果：单个轮廓对应一个结果轮廓，多个轮廓对应多个结果轮廓
	scFeatureletChainSet GetResultFeatureletChainSet()const;

	// 获取执行结果(平滑+过滤的结果)在原轮廓上的顶点索引
	std::vector<int> GetResultVerticesIndex(int nIndex = 0);

	// 获取所有执行结果(平滑+过滤的结果)在原轮廓上的顶点索引
	std::vector<std::vector<int>> GetResultVerticesIndexAll();

	// 获取平滑结果(平滑后过滤前的中间结果)：nIndex为轮廓索引号，单个轮廓索引号为0，多个轮廓索引号为0~n-1
	scPolyline GetSmoothResultPolyline(int nIndex = 0)const;
	scFeatureletVector GetSmoothResultFeatureletVector(int nIndex = 0)const;
	std::vector<sc2Vector> GetSmoothResultVertices(int nIndex = 0)const;

	// 获取所有平滑结果：单个轮廓对应一个结果轮廓，多个轮廓对应多个结果轮廓
	scFeatureletChainSet GetSmoothResultFeatureletChainSet()const;

private:
	int m_nSmoothPara; // 平滑点数
	double m_dDistanceThre; // 距离阈值

	scFeatureletChainSet m_SmoothFeatureletChainSet; // 平滑结果
	scFeatureletChainSet m_FilterFeatureletChainSet; // 过滤结果
	std::vector<std::vector<int>> m_vnVerticesIndex; // 顶点索引
};

// 轮廓平滑
bool SVCORE_API sfContourSmooth(const scPolyline& inputContour,int nSmoothPara,scPolyline& outputContour);
bool SVCORE_API sfContourSmooth(const scFeatureletVector& inputContour,bool bClosed,int nSmoothPara,scFeatureletVector& outputContour);
bool SVCORE_API sfContourSmooth(const std::vector<sc2Vector>& inputContour,bool bClosed,int nSmoothPara,std::vector<sc2Vector>& outputContour);

// 轮廓过滤
bool SVCORE_API sfContourFilter(const scPolyline& inputContour,double dDisThre,scPolyline& outputContour);
bool SVCORE_API sfContourFilter(const scFeatureletVector& inputContour,bool bClosed,double dDisThre,scFeatureletVector& outputContour);
bool SVCORE_API sfContourFilter(const std::vector<sc2Vector>& inputContour,bool bClosed,double dDisThre,std::vector<sc2Vector>& outputContour);
bool SVCORE_API sfContourFilter(const scPolyline& inputContour,double dDisThre,std::vector<int>& vnVerticesIndex);
bool SVCORE_API sfContourFilter(const scFeatureletVector& inputContour,bool bClosed,double dDisThre,std::vector<int>& vnVerticesIndex);
bool SVCORE_API sfContourFilter(const std::vector<sc2Vector>& inputContour,bool bClosed,double dDisThre,std::vector<int>& vnVerticesIndex);

#pragma pack(pop)