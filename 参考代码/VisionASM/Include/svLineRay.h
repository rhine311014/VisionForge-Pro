#pragma once

#include "svShape.h"
#include "svAngle.h"
#include <vector>

#pragma pack(push,8)

class scLineSeg;
class scLine;
class scLineRay;
smDerivedShapePtrhDecl(scLineRay);

class SVCORE_API scLineRay : public scShape
{
public:
	// 构造函数
	scLineRay(): m_vPos(0,0),m_vDir(1,0){};

	// 拷贝构造函数
	scLineRay(const scLineRay& ray);

	// 根据射线起始点的位置坐标pos和射线方向向量dir构造射线
	// 若方向向量dir为零向量,则调试状态下会失败
	scLineRay(const sc2Vector& pos,const sc2Vector& dir);

	// 根据射线起始点的位置坐标pos和射线旋转角度rot构造射线
	scLineRay(const sc2Vector& pos,const scRadian rot);

	// 设置射线起始点的位置坐标pos和射线方向向量dir构造射线
	// 若方向向量dir为零向量，则返回false，否则返回true
	bool SetPosDirection(const sc2Vector& pos, const sc2Vector& dir);

	// 根据射线起始点的位置坐标pos和射线旋转角度rot设置射线
	void SetPosRotation(const sc2Vector& pos, const scRadian& rot);

	// 用起始点point1和终止点point2来构造射线,
	// 若两点为同一点则返回false，否则返回true
	bool SetFromStartEnd(const sc2Vector& point1, const sc2Vector& point2);

	// 用线段lineSeg来构造射线，
	// 若线段起始点和终止点相同则返回false，否则返回true
	bool SetFromLineSeg(const scLineSeg& lineSeg);

	void SetPos(const sc2Vector& pos){ m_vPos = pos;};	 // 设置射线起始点位置坐标pos

	void SetRotation(const scRadian& rot);				 // 设置射线的旋转角度

	const sc2Vector& GetPos() const{ return m_vPos;};    // 获取射线起始点

	const sc2Vector& GetDir() const{ return m_vDir;};	 // 获取射线方向向量

	// 获取射线的旋转角度,范围是(-π,π]
	// 如果方向向量为(0,0),则返回的角度为0
	scRadian GetRotation() const;

	scRadian GetAngle(const scLine& other)const; // 计算从当前射线到给定直线other的旋转角度（-PI < a <= PI）

	scRadian GetAngle(const scLineRay& other) const; // 计算从当前射线到给定射线other的旋转角度（-PI < a <= PI）

	scLine GetLine() const;						// 返回当前射线所在的直线

	scLineRay Map(const sc2XformLinear& x) const;   // 对当前射线对象进行二维几何变换x，得到新的射线对象

	bool operator==(const scLineRay& other) const;										// 重载==操作符
	bool operator!=(const scLineRay& other) const										// 重载!=操作符
	{ return !(*this == other);}

	virtual ShapeType GetType() const{ return eLineRay; };								// 返回类型的枚举值
	scShapePtrh Clone() const;														// 复制得到新的图形对象指针句柄
	bool IsOpenContour() const;														// 是否为开轮廓，对于有向射线总是返回false	
	bool IsRegion() const;															// 是否为区域，对于有向射线总是返回false
	bool IsFinite() const;															// 是否为有限的，对于有向射线总是返回false
	bool IsEmpty() const;															// 是否为空，对于有向射线总是返回false

	scRect BoundingBox() const;														// 获取包围盒，对于有向射线没有包围盒，返回值无效
	sc2Vector NearestPoint(const sc2Vector& p) const;								// 计算得到射线上离点p最近的点位置坐标
	double Perimeter() const;														// 获取边长，对于有向射线返回值无效

private:
	bool IsEqual(const scShape& rhs) const;						// 判断给定图形rhs是否与当前图形相等，仅供内部使用

private:
	sc2Vector m_vDir, m_vPos;
};

#pragma pack(pop)