// svCircularArc.h


#pragma once

#include "svShape.h"
#include "svCircle.h"

#pragma pack(push,8)


// ---------------------------------------------------------------------
//                            圆弧
// ---------------------------------------------------------------------
// 


class scCircularArc;
smDerivedShapePtrhDecl(scCircularArc);


class SVCORE_API scCircularArc : public scShape  
{
public:

	// 用圆形circle来构造圆弧对象
	scCircularArc(const scCircle& circle = scCircle(sc2Vector(0,0),1.));	

	// 用圆形circle,起始角startPhi和张角spanPhi来构造圆弧
	scCircularArc(const scCircle& circle,								
		const scRadian& startPhi, const scRadian& spanPhi);

	// 用圆弧所在圆的圆心center和半径radius，以及起始角startPhi和张角spanPhi来构造圆弧
	scCircularArc(const sc2Vector& center, double radius, 
		const scRadian& startPhi, const scRadian& spanPhi);

	// 用圆弧所在圆的圆心center和半径radius，以及起始角startPhi和张角spanPhi来构造圆弧
	scCircularArc(double centerX,double centerY, double radius,
		double startPhi, double spanPhi);
		
	// 设置圆弧所在圆的圆心center和半径radius，以及起始角startPhi和张角spanPhi
	void SetCenterRadiusStartPhiSpanPhi(const sc2Vector& center, double radius, 
		const scRadian& startPhi, const scRadian& spanPhi);

	// 设置圆弧所在圆的圆心center和半径radius，以及起始角startPhi和张角spanPhi
	void SetCenterRadiusStartPhiSpanPhi(double centerX,double centerY, double radius,
		double startPhi, double spanPhi);

	bool Set3Points(sc2Vector startpoint, sc2Vector endpoint, sc2Vector thirdpoint);

	// 设置圆弧所在的圆circle
	void SetCircle(const scCircle& circle){ m_Circle = circle;};							

	// 设置起始角
	void SetStartPhi(const scRadian& startPhi){ m_rStartPhi = startPhi.SignedNorm();};							

	// 设置张角
	void SetSpanPhi(const scRadian& spanPhi){ m_rSpanPhi = spanPhi;};		

	// 返回圆弧所在的圆
	const scCircle &GetCircle() const { return m_Circle;};	

	// 获取圆弧所在圆的圆心
	sc2Vector GetCircleCenter() const {return m_Circle.GetCenter();};

	// 获取圆弧所在圆的圆心
	double GetCircleRadius() const { return m_Circle.GetRadius();};

	// 获取起始角
	scRadian GetStartPhi() const{ return m_rStartPhi; };		

	// 获取张角
	scRadian GetSpanPhi() const { return m_rSpanPhi;};		
					

	// 获取终止角
	scRadian GetEndPhi() const { return m_rStartPhi + m_rSpanPhi;};		

	// 获取圆弧的起始点
	sc2Vector GetStartPoint() const {return m_Circle.GetPoint(m_rStartPhi);};
	
	// 获取圆环的中点
	sc2Vector GetMidPoint() const {return m_Circle.GetPoint(m_rStartPhi + m_rSpanPhi/2);};

	// 获取圆弧的终点
	sc2Vector GetEndPoint() const {return m_Circle.GetPoint(m_rStartPhi + m_rSpanPhi);};
	
	//  对当前圆弧进行二维几何变换（一致缩放及平移但没有旋转和切变）,得到新的圆弧
	scCircularArc Map(const sc2XformLinear& c) const;					


	// 重载==操作符
	bool operator==(const scCircularArc& rhs) const;					

	// 重载!=操作符
	bool operator!=(const scCircularArc& rhs) const;					

	// 判断弧度phi是否在圆弧的转角内
	// 如果SetPhiSpan()>=0，则判断phi是否在[phiStart(), phiEnd())
	// 如果SetPhiSpan()<0，则判断phi是否在(phiEnd(), phiStart()]
	bool IsInSpan(const scRadian &phi) const;							

	// 重载基类scShape的方法
	//////////////////////////////////////////////////////////////////////////
	
	// 返回类型的枚举值
	virtual ShapeType GetType() const{ return eCircleArc; };							

	// 复制当前圆弧得到新的圆弧
	virtual scShapePtrh Clone() const;	

	// 是否为开的轮廓,总是返回true
	virtual bool IsOpenContour() const;							
	
	// 是否为区域,总是返回false
	virtual bool IsRegion() const;								

	// 是否为有限的,总是返回true
	virtual bool IsFinite() const;					

	// 是否为空,总是返回false			
	virtual bool IsEmpty() const;
	
	// 获取圆弧的最小外接矩形
	virtual scRect BoundingBox() const;						
	
	// 计算圆弧上离给定点p最近的点
	virtual sc2Vector NearestPoint(const sc2Vector& p) const;	
	
	// 获取圆弧的边长
	virtual double Perimeter() const;			

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;		// 获得掩模图像

private:
	virtual bool IsEqual(const scShape &rhs) const;

	scCircle m_Circle;
	scRadian m_rStartPhi;
	scRadian m_rSpanPhi;
};

inline void scCircularArc::SetCenterRadiusStartPhiSpanPhi(const sc2Vector& center, double radius, 
								   const scRadian& startPhi, const scRadian& spanPhi)
{
	m_Circle.SetCenterRadius(center, radius);
	m_rStartPhi = startPhi.SignedNorm();
	m_rSpanPhi = spanPhi;
}

inline void scCircularArc::SetCenterRadiusStartPhiSpanPhi(double centerX,double centerY, double radius,
	double startPhi, double spanPhi)
{
	SetCenterRadiusStartPhiSpanPhi(sc2Vector(centerX,centerY),radius,scRadian(startPhi),scRadian(spanPhi));
}

// 重载==操作符
inline bool scCircularArc::operator==(const scCircularArc &rhs) const
{
	return m_Circle == rhs.m_Circle
		&& m_rStartPhi == rhs.m_rStartPhi
		&& m_rSpanPhi == rhs.m_rSpanPhi;
}



// 重载!=操作符
inline bool scCircularArc::operator!=(const scCircularArc &rhs) const
{
	return !(*this == rhs);
}

#pragma pack(pop)
