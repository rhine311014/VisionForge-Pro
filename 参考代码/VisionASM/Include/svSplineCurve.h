#pragma once
#include "svShape.h"
#include "svPolyline.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            样条曲线
// ---------------------------------------------------------------------
class scSplineCurve;
smDerivedShapePtrhDecl(scSplineCurve);

//节点参数化方法
enum NodeMethod {
	SubtenseAdd = 0,		// 弦长累加法
	SubtenseSqrt = 1,		// 弦长平方根法
	Unified = 2,			// 统一法，除去首尾重复节点，其余节点的间隔相等
};

// 假设样条曲线为k阶，控制点数量为n，节点的数量为n+k+1，权重的数量为n；
// 假设样条曲线为k阶，拟合点数量为n，转换得到的控制点数量为n+2，节点数量为n+k+3，权重的数量为n+2；

class SVCORE_API scSplineCurve : public scShape 
{
public:
	// 构造函数
	scSplineCurve();
	scSplineCurve(const std::vector<double>& vNode,const std::vector<sc2Vector>& vControlPoints,const std::vector<double>& vWeight,int nOrder,bool bFlagOpenClose = false);
	scSplineCurve(const std::vector<sc2Vector>& vControlPoints,const std::vector<double>& vWeight,int nOrder,bool bFlagOpenClose = false);
	scSplineCurve(const std::vector<sc2Vector>& vFitPoints,int nOrder,const std::vector<double>& vWeight,NodeMethod method = SubtenseAdd,bool bFlagOpenClose = false);
	scSplineCurve(const scSplineCurve& spline);

	//设置/获取节点向量，节点向量是非递减数的集合
	bool SetNode(const std::vector<double>& vNode);
	std::vector<double> GetNode()const ;

	// 将节点向量归一化，范围在[0,1]
	std::vector<double> NodeNormalize(const std::vector<double>& vNode)const;

	// 设置/获取阶次
	void SetOrder(int nOrder){m_nOrder = nOrder;};
	int GetOrder()const {return m_nOrder;};

	// 设置/获取样条曲线是否闭合
	void IsClosed(bool bFlagOpenClose) {m_bFlagOpenClose = bFlagOpenClose;};
	bool IsClosed()const {return m_bFlagOpenClose;};

	// 设置/获取权重
	void SetWeight(const std::vector<double>& vdWeight);
	std::vector<double> GetWeight()const{return m_vdWeight;};
	double GetWeight(int nIndex)const;																				// 获取第nIdex个控制点的权重

	void SetNodeMethod(NodeMethod method){m_NodeMethod = method;};													// 设置节点参数化方法，针对拟合点计算节点的方法
	NodeMethod GetNodeMethod()const{return m_NodeMethod;};															// 获取节点参数化方法，针对拟合点计算节点的方法
	
	bool SetControlPoints(const std::vector<sc2Vector>& vControlPoints,const std::vector<double>& vWeight);			// 重新设置控制点和权重,以统一法计算节点
	std::vector<sc2Vector> GetControlPoints()const {return m_vControlPoints;};										// 获取控制点

	bool SetControlPoint(int nIndex,const sc2Vector& controlPoint,double dWeight =1.0);								// 改变第nIdex个控制点和权重
	sc2Vector GetControlPoint(int nIndex)const;																		// 获取第nIdex个控制点
	
	bool InsertControlPoint(int idx, const sc2Vector& point,double dWeight =1.0);									// 在idx位置插入新的控制点和权重
	bool InsertControlPoints(int idx, const std::vector<sc2Vector>& points,const std::vector<double>& vWeight);		// 在第idx位置插入一组控制点和权重

	bool AddControlPoint(const sc2Vector& point,double dWeight =1.0);												// 在样条曲线控制点序列的尾部添加新的控制点和权重
	bool AddControlPoints(const std::vector<sc2Vector>& points,const std::vector<double>& vWeight);					// 在样条曲线控制点序列的尾部添加一组控制点和权重

	bool RemoveControlPoint(int idx);																				// 删除第idx个控制点和权重
	void RemoveControlPoints();																						// 删除所有控制点和权重
	int GetControlPointsNum()const;																					// 获取控制点数量


	bool SetFitPoints(const std::vector<sc2Vector>& vFitPoints,const std::vector<double>& vWeight);					// 重新设置拟合点和权重
	std::vector<sc2Vector> GetFitPoints()const{return m_vFitPoints;};												// 获取拟合点
	
	bool SetFitPoint(int nIndex, const sc2Vector& fitPoint,double dWeight = 1.0);									// 设置第nIdex个拟合点和权重
	sc2Vector GetFitPoint(int nIndex)const;																			// 获取第nIdex个拟合点

	bool InsertFitPoint(int idx, const sc2Vector& point,double dWeight = 1.0);										// 在idx位置插入新的拟合点和权重
	bool InsertFitPoints(int idx, const std::vector<sc2Vector>& points,const std::vector<double>& vWeight);			// 在第idx位置插入一组拟合点和权重

	bool AddFitPoint(const sc2Vector& point,double dWeight = 1.0);													// 在样条曲线拟合点序列的尾部添加新的拟合点和权重
	bool AddFitPoints(const std::vector<sc2Vector>& points,const std::vector<double>& vWeight);						// 在样条曲线拟合点序列的尾部添加一组拟合点和权重

	bool RemoveFitPoint(int idx);																					// 删除第idx个拟合点和权重
	void RemoveFitPoints();																							// 删除所有拟合点和权重
	int GetFitPointsNum()const;																						// 获取拟合点数量

	
	scPolyline GetPolyline(int nNum)const;																			// 设置固定点数获取多边形
	std::vector<sc2Vector> GetSplinePoints(int nNum)const;															// 设置固定点数获取样条曲线上的点

	scPolyline GetPolyline(double dDistacne)const;																	// 设置距离步长获取多边形
	std::vector<sc2Vector> GetSplinePoints(double dDistacne)const;													// 设置距离步长获取样条曲线上的点

	sc2Vector GetSplinePoint(double dStep)const;																	// 设置dStep的值，获取样条曲线上的一点，dStep范围在[0,1]

	//std::vector<sc2Vector> GetSplinePointX(double dDistacne,const scRect& rect)const;
	
	// 重载==操作符
	bool operator==(const scSplineCurve& rhs) const;					

	// 重载!=操作符
	bool operator!=(const scSplineCurve& rhs) const { return !(*this == rhs); };	

	// 重载=操作符
	scSplineCurve& operator=(const scSplineCurve &rhs);

	virtual ShapeType GetType() const{ return eSplineCurve; };					// 返回类型的枚举值
	scShapePtrh Clone() const;													// 复制当前样条曲线得到新的样条曲线
	bool IsOpenContour() const;													// 是否为开轮廓,如果IsClose()为假且样条曲线至少有一个顶点,则判定为开轮廓返回true
	bool IsRegion() const;														// 是否为区域,如果闭合且至少有一个点,则返回true
	bool IsFinite() const;														// 是否为有限的,总是返回true
	bool IsEmpty() const;														// 是否为空,如果样条曲线没有控制顶点,则返回true

	scRect BoundingBox() const;													// 获取样条曲线的最小外接矩形

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;							// 获得掩模图像

	sc2Vector NearestPoint(const sc2Vector& p) const;							// 计算样条曲线上离点p最近的点

	bool Within(const sc2Vector& p) const;										// 判断点p是否在样条曲线内部

	scSplineCurve Map(const sc2XformLinear& c) const;							// 对当前样条曲线进行二维几何变换（一致缩放及平移但没有切变和旋转）,得到新的样条曲线对象


private:
	//根据参数t值和次数k与节点矢量node，计算第i个k次的B样条基函数
	double GetBasisFunctionValue(double t, int i, int k,const std::vector<double>& vNode)const; 

	//根据参数t值的第nIndex区间和次数k与节点矢量node，计算第i个k次的B样条基函数的多项式
	std::vector<double> GetBasisFunction(int nIndex, int i, int k,const std::vector<double>& vNode)const; 

	// 多项式相乘，返回结果多项式的系数
	std::vector<double> PolynomialMultiply(const std::vector<double>& vCoeff1, const std::vector<double>& vCoeff2)const;

	// 获取样条曲线分段函数的多项式分子系数
	std::vector<std::vector<sc2Vector>> GetNumeratorCoeff()const; 

	// 获取样条曲线分段函数的多项式分母系数
	std::vector<std::vector<double>> GetDenominatorCoeff()const;

	sc2Vector GetPoints(double dStep,const std::vector<sc2Vector>& vNumerator,const std::vector<double>& vDenominator)const;

	// 计算节点，nFlag - 1 控制点生成， nFlag - 2 拟合点生成 
	std::vector<double> Node(int nFlag,NodeMethod method)const;

	bool FitPointsToControlPoints();					// 已知拟合点，反求控制点
	bool ControlPointsToFitPoints();					// 控制点转拟合点

	bool IsEqual(const scShape& rhs) const;				// 判断给定图形rhs是否与当前图形相等，仅供内部使用

private:
	std::vector<double> m_vdNode;						// 节点向量
	std::vector<sc2Vector> m_vControlPoints;			// 控制点
	std::vector<sc2Vector> m_vFitPoints;				// 拟合点
	NodeMethod m_NodeMethod;							// 节点参数化方法
	int m_nOrder;										// 阶次
	bool m_bFlagOpenClose;								// 样条曲线是否闭合，true - 闭合，false - 开
	std::vector<double> m_vdWeight;						// 权重,默认全为1
};