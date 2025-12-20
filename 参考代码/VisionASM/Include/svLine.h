// svLine.h


#pragma once

#include "svShape.h"
#include "svAngle.h"
#include <vector>

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            直线
// ---------------------------------------------------------------------

class scLineSeg;
class scLine;
smDerivedShapePtrhDecl(scLine);


class SVCORE_API scLine : public scShape  
{
public:
	// 构造函数
	scLine(): m_vPos(0,0),m_vDir(1,0){};											

	// 根据直线所经过点的位置坐标pos和直线方向向量dir构造直线
	// 若方向向量dir为零向量,则调试状态下会失败
	scLine(const sc2Vector& pos, const sc2Vector& dir);				

	// 根据直线所经过点的位置坐标pos和直线旋转角度rot构造直线
	scLine(const sc2Vector& pos, const scRadian& rot);	
	scLine(double posX,double posY,double rot);

	// 根据二维向量v构造直线，直线角度为向量v的角度，直线经过向量v所表示的点
	// 若v为零向量,则调试状态下会失败
	scLine(const sc2Vector& v);									

	// 用距离dist和旋转角度rot来构造直线
	scLine(double dist, const scRadian& rot);					

	// 设置直线所经过点的位置坐标pos和直线方向向量dir构造直线
	// 若方向向量dir为零向量，则返回false，否则返回true
	bool SetPosDirection(const sc2Vector& pos, const sc2Vector& dir);
	bool SetPosDirection(double posX,double posY,double dirX,double dirY);

	// 根据直线所经过点的位置坐标pos和直线旋转角度rot设置直线
	void SetPosRotation(const sc2Vector& pos, const scRadian& rot);
	void SetPosRotation(double posX,double posY,double rot);

	// 用距离dist和旋转角度rot来设置直线
	void SetDistRotation(double dist, const scRadian& rot);
	void SetDistRotation(double dist, double rot);

	// 用起始点point1和终止点point2来构造直线,
	// 若两点为同一点则返回false，否则返回true
	bool SetFromStartEnd(const sc2Vector& point1, const sc2Vector& point2);
	bool SetFromStartEnd(double point1X,double point1Y,double point2X,double point2Y);

	// 用线段lineSeg来构造直线，
	// 若线段起始点和终止点相同则返回false，否则返回true
	bool SetFromLineSeg(const scLineSeg& lineSeg);

	

	void SetPos(const sc2Vector& pos){ m_vPos = pos;};								// 设置直线经过的点位置坐标pos
	void SetPos(double posX,double posY){ m_vPos = sc2Vector(posX,posY);};

	void SetRotation(const scRadian& rot);											// 设置直线的旋转角度

	const sc2Vector& GetPos() const{ return m_vPos;};								// 获取直线所经过的点

	const sc2Vector& GetDir() const{ return m_vDir;};								// 获取直线的方向向量
		
	// 获取直线的旋转角度,范围是(-π,π]
	// 如果方向向量为(0,0),则返回的角度为0
	scRadian GetRotation() const;													

	scRadian GetAngle(const scLine& other) const;									// 计算从当前直线到给定直线other的旋转角度（-PI < a <= PI）

	svStd vector<sc2Vector> Intersect(const scLine& other) const;					// 计算当前直线与给定直线line的交点

	scLine Parallel(const sc2Vector& p) const;										// 获得经过给定点p且平行于当前直线的直线对象

	bool IsParallel(const scLine& line) const;										// 判断当前直线与给定直线line是否平行

	scLine Normal(const sc2Vector& p) const;										// 计算得到新的直线，该直线经过点p，且旋转角度为当前旋转角度+90

	sc2Vector Project(const sc2Vector& p) const;									// 计算给定点p在当前直线上的投影点

	double Offset(const sc2Vector& p) const;										// 计算距离，该距离是当前直线参考点到给定点在直线上的投影点之间的距离 

	scLine Map(const sc2XformLinear& x) const;										// 对当前直线对象进行二维几何变换x，得到新的直线对象
	// returns    This line mapped by c.

	bool operator==(const scLine& other) const;										// 重载==操作符
	bool operator!=(const scLine& other) const										// 重载!=操作符
	{ return !(*this == other);}


	virtual ShapeType GetType() const{ return eLine; };								// 返回类型的枚举值
	scShapePtrh Clone() const;														// 复制得到新的图形对象指针句柄
	bool IsOpenContour() const;														// 是否为开轮廓，对于有向直线总是返回false	
	bool IsRegion() const;															// 是否为区域，对于有向直线总是返回false
	bool IsFinite() const;															// 是否为有限的，对于有向直线总是返回false
	bool IsEmpty() const;															// 是否为空，对于有向直线总是返回false

	scRect BoundingBox() const;														// 获取包围盒，对于有向直线没有包围盒，返回值无效
	sc2Vector NearestPoint(const sc2Vector& p) const;								// 计算得到直线上离点p最近的点位置坐标
	double Perimeter() const;														// 获取边长，对于有向直线返回值无效

private:
	bool IsEqual(const scShape& rhs) const;											// 判断给定图形rhs是否与当前图形相等，仅供内部使用


private:
	sc2Vector m_vDir, m_vPos;

};

#pragma pack(pop)
