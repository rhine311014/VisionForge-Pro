#pragma once

#include "svDef.h"
#include "svError.h"
#include "svMath.h"
#include "svAngle.h"
#include "svVector.h"
#include "svXform.h"
#include "svRect.h"
#include <cassert>

#pragma pack(push,8)

class scFeaturelet;
class scFeatureletChainSet;

#define scFeatureletVector svStd vector<scFeaturelet>

class SVCORE_API scFeaturelet
{
	friend class scFeatureletChainSet;

public:
	scFeaturelet(const sc2Vector &position = sc2Vector(), const scRadian &angle = scRadian(0),
		double magnitude = 0, double weight = 1, bool isMod180 = false) :
		m_VectorPos(position), m_Angle(angle), m_dWeight(weight), m_bIsMod180(isMod180)
	{ SetMagnitude(magnitude); }

	//设置/获取特征位置
	const sc2Vector& GetPosition() const { return m_VectorPos;}
	void SetPosition(const sc2Vector &position) { m_VectorPos = position; }
	void SetPosition(double positionX,double positionY){m_VectorPos = sc2Vector(positionX,positionY);};

	//设置/获取特征角度
	scRadian GetAngle() const { return m_Angle; }
	void SetAngle(scRadian angle) { m_Angle = angle; }

	//如果isMod180 == true，返回取模180度的角度，范围[0，PI）；否则，返回取模360度的角度，范围[0，2PI）
	scRadian GetAngleMod() const;

	//设置/获取特征幅值
	double GetMagnitude() const { return m_dMagnitude; }
	void SetMagnitude(double magnitude) { m_dMagnitude = max(magnitude,0); }

	//设置/获取特征权重
	double GetWeight() const { return m_dWeight; }
	void SetWeight(double weight) { m_dWeight = weight; }

	//设置/获取是否取模180度
	bool IsMod180() const { return m_bIsMod180; }
	void SetMod180(bool isMod180) { m_bIsMod180 = isMod180; }

	//返回当前特征经过xform映射以后得到的新特征。新特征的位置和角度由当前特征经过xform
	//映射得到，新特征的幅值、权重、是否取模180度等信息与当前特征一致。
	//假定特征的角度对应垂直于切线方向的梯度方向，映射一个特征的角度由以下几个步骤组成：
	//1）计算切线方向，即tangent = angle - 90度；2）映射切线方向tangent；3）计算梯度方向
	//即angle = tangent + 90度。
	scFeaturelet Map(const sc2XformBase &xform) const;

	//当且仅当两个特征的所有信息相等时，两个特征相等
	bool operator==(const scFeaturelet& rhs) const;
	bool operator!=(const scFeaturelet& rhs) const { return !(*this == rhs); }

private:
	sc2Vector	m_VectorPos;	//位置
	scRadian	m_Angle;		//角度（方向）
	double		m_dMagnitude;	//幅值
	double		m_dWeight;		//权重
	bool		m_bIsMod180;	//是否取模180度
};

inline scRadian scFeaturelet::GetAngleMod() const
{
	if (m_bIsMod180)
	{
		return m_Angle.NormMod180();
	}

	return m_Angle.Norm();	
}

class SVCORE_API scFeatureletChainSet
{
	struct sviFeatureletChainSet
	{
		long		lStartIndex;	//起始特征索引
		long		lLength;		//长度
		bool		bIsClosed;	//是否闭合

		sviFeatureletChainSet() { lStartIndex = 0; lLength = 0; bIsClosed = true; }

		sviFeatureletChainSet(long startindex, long length, bool isclosed)
			: lStartIndex(startindex), lLength(length), bIsClosed(isclosed) { }
	};

public:
	//默认构造函数，构造一个包含0个chain的空chainset
	scFeatureletChainSet();

	//根据给定的Featurelet构造chainset
	scFeatureletChainSet(const svStd vector<scFeatureletVector> &features, const svStd vector<bool> &isClosed);

	//拷贝构造函数（深拷贝）
	scFeatureletChainSet(const scFeatureletChainSet &chainSet);

	//重载操作符=（深拷贝）
	scFeatureletChainSet& operator=(const scFeatureletChainSet &chainSet);

	//析构函数
	virtual ~scFeatureletChainSet();

	//重置当前chainset为空
	void Reset() { *this = scFeatureletChainSet(); }

	//替换序号为chainIndex的chain
	void ReplaceChain(long chainIndex, const svStd vector<scFeaturelet> &features, bool isClosed);

	//插入序号为chainIndex的chain
	void InsertChain(long chainIndex, const svStd vector<scFeaturelet> &features, bool isClosed);

	//添加一个chain
	void AddChain(const svStd vector<scFeaturelet> &features, bool isClosed) { InsertChain(NumChains(), features, isClosed); }

	//删除序号为chainIndex的chain
	void DeleteChain(long chainIndex);

	//合并两个chain set
	void Append(const scFeatureletChainSet &featureletChainSet);

	//返回chain的个数
	long NumChains() const;

	//返回featurelet的个数
	long NumFeaturelets() const;

	//为当前chain set预留numChains个chain、numFeatures个featurelet的空间
	void Reserve(long numChains, long numFeatures) const;

	//返回序号为chainIndex的chain是否闭合
	bool IsClosed(long chainIndex) const;

	//返回所有chain是否闭合
	svStd vector<bool> IsClosed() const;

	//返回序号为chainIndex的chain的长度（即其包含的featurelet的个数）
	long ChainLength(long chainIndex) const;

	//返回序号为chainIndex的chain的起始特征索引
	long StartFeatureIndex(long chainIndex) const;

	//返回所有的featurelet
	const svStd vector<scFeaturelet>& Featurelets() const;

	//返回序号为chainIndex的chain的所有featurelet
	svStd vector<scFeaturelet> Featurelets(long chainIndex) const;

	//返回所有的chain
	svStd vector<scFeatureletVector> FeatureletChains() const;

	//返回第chainIndex个chain的第featureIndex个featurelet
	const scFeaturelet& Featurelet(long chainIndex, long featureIndex) const;

	//映射当前的chain set
	void Map(const sc2XformBase &xform, scFeatureletChainSet& chainSet) const;

	//返回序号为chainIndex的chain的外接矩形
	scRect BoundingBox(long chainIndex) const;

	//返回当前chain set的外接矩形
	scRect BoundingBox() const;

	//返回序号为chainIndex的chain（必须为闭合chain）的是否为右手系
	//如果某closed chain的signed area为非负，则其为右手系；feature num为
	//0、1、2的闭合曲线为右手系；否则，非右手系。
	bool IsRightHanded(long chainIndex) const;

	//将序号为chainIndex的chain中的featurelet进行逆向排序
	//该操作可能会改变闭合曲线的handedness（长度为0、1、2的闭合曲线除外）
	void ReverseFeatureletOrder(long chainIndex);

	//将所有chain中的featurelet进行逆向排序
	//该操作可能会改变闭合曲线的handedness（长度为0、1、2的闭合曲线除外）
	void ReverseFeatureletOrder();

	//将序号为chainIndex的chain中的featurelet转向，即其angle = angle + 180度
	//新的featurelet的角度将按360度取模
	void ReverseFeatureletOrientations(long chainIndex);

	//将所有chain中的featurelet转向，即其angle = angle + 180度
	//新的featurelet的角度将按360度取模
	void ReverseFeatureletOrientations();

	//如果序号为chainIndex的chain中的所有featurelet都取模180度，则返回true；否则，返回false
	bool AnyFeatureletIsMod180(long chainIndex) const;

	//如果chain set中的所有featurelet都取模180度，则返回true；否则，返回false
	bool AnyFeatureletIsMod180() const;

	//当且仅当两者所有的chain、featurelet都相等时，返回true；否则，返回false
	bool operator==(const scFeatureletChainSet& rhs) const;
	bool operator!=(const scFeatureletChainSet& rhs) const { return !(*this == rhs); }

	//返回满足如下条件的featurelet的比例：featurelet与其相邻featurelet组成的向量与featurelet的
	//angle对应向量之间的叉积为正。
	//假定a_i为第i个featurelet的angle的对应向量，f_i为第i个featurelet的位置，isMod180_i为第i个
	//featurelet是否取模180度，N为featurelet的个数，则计算过程如下：
	// ProportionPositiveCrossProduct() = 
	// 
	// 1 N-1
	// - Sum if (isMod180_{i}) then 0 
	// M i=0 else sign((f{i}-f{i-1}) cross a_i) + sign((f{i+1}-f{i}) cross a_i)
	//
	// where:
	//
	// sign(x) = {1 if x > 0, 0 if x == 0, -1 if x < 0}
	//
	//     N-1  
	// M = Sum  if (isMod180_{i}) then 0 else 2
	//     i=0
	//
	//以上计算过程假定序号为chainIndex的chain为闭合的，对于非闭合的chain其首尾featurelet只各计算1次
	double ProportionPositiveCrossProduct(long chainIndex) const;

private:
	bool IsValidChainIndex(long chainIndex) const;

	mutable svStd vector<scFeaturelet>  m_vectorFeatures;
	mutable svStd vector<sviFeatureletChainSet> m_vectorFeatureChainSet;
};

#pragma pack(pop)
