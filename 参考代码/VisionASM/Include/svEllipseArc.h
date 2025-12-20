// svEllipseArc.h


#pragma once

#include "svShape.h"
#include "svEllipse.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            椭圆弧
// ---------------------------------------------------------------------
// 


class scEllipseArc;
smDerivedShapePtrhDecl(scEllipseArc);

class SVCORE_API scEllipseArc : public scShape  
{
public:

	// 用椭圆ell构造当前椭圆弧对象
	scEllipseArc(const scEllipse& ell = scEllipse());			

	// 用椭圆ell,起始角startPhi和转角spanPhi来构造椭圆弧对象
	scEllipseArc(const scEllipse& ell,							
		const scRadian &startPhi, const scRadian &spanPhi);


	// 获取当前椭圆弧所在的椭圆
	const scEllipse& GetEllipse() const { return m_Ellipse;};	
	
	// 获取起始角
	scRadian GetStartPhi() const{ return m_rStartPhi; };		

	// 获取转角
	scRadian GetSpanPhi() const { return m_rSpanPhi;};					
	
	// 设置当前椭圆弧所在的椭圆
	void SetEllipse(const scEllipse& ell){ m_Ellipse = ell;};							
	
	// 设置起始角
	void SetStartPhi(const scRadian& startPhi){ m_rStartPhi = startPhi.SignedNorm();};							
	
	// 设置转角
	void SetSpanPhi(const scRadian& spanPhi){ m_rSpanPhi = spanPhi;};							

	// 获取终止角
	scRadian GetEndPhi() const { return m_rStartPhi + m_rSpanPhi;};	

	bool SetRadii(const sc2Vector& radii) { return m_Ellipse.SetRadii(radii);};				// 设置X方向和Y方向的半轴长度
	bool SetRadii(double radiiX,double radiiY){return m_Ellipse.SetRadii(radiiX,radiiY);};
	const sc2Vector &GetRadii() const { return m_Ellipse.GetRadii();};						// 获取X方向和Y方向的半轴长度	

	void SetCenter(const sc2Vector& center) { m_Ellipse.SetCenter(center);};				// 设置中心	
	void SetCenter(double centerX,double centerY){m_Ellipse.SetCenter(centerX,centerY);};
	const sc2Vector &GetCenter() const { return m_Ellipse.GetCenter();};					// 获取中心			

	void SetRotation(const scRadian& rotation) { m_Ellipse.SetRotation(rotation);};	// 设置旋转角度						
	const scRadian& GetRotation() const { return m_Ellipse.GetRotation();};					// 获取旋转角度


	// 对当前椭圆弧进行sc2XformLinear变换c,得到新的椭圆弧
//	scEllipseArc Map(const sc2XformLinear& c) const;			
	
	// 平移当前椭圆弧得到新的椭圆弧
	scEllipseArc Translate(const sc2Vector& offset) const;	
	
	// 旋转椭圆弧得到新的椭圆弧	
	scEllipseArc Rotate(const scRadian& theta) const;			
	
	// 对椭圆弧所在椭圆的X方向和Y方向进行缩放得到新的椭圆弧
	// mag的x分量和y分量应该大于0，否则调试模式下会报断言错误
	scEllipseArc Scale(const sc2Vector& mag) const;				


	// 重载==操作符
	bool operator==(const scEllipseArc &rhs) const;				
	

	// 重载!=操作符
	bool operator!=(const scEllipseArc &rhs) const;				

	
	// 判断角度phi是否在椭圆弧转角范围内
	// 如果SetPhiSpan()>=0，则判断phi是否在[phiStart(), phiEnd())
	// 如果SetPhiSpan()<0，则判断phi是否在(phiEnd(), phiStart()]
	bool IsInSpan(const scRadian& phi) const;					


	// 重载基类scShape的方法
	//////////////////////////////////////////

	// 返回类型的枚举值
	virtual ShapeType GetType() const{ return eEllipseArc; };

	// 复制当前椭圆弧得到新的椭圆弧
	virtual scShapePtrh Clone() const;							
	
	// 是否为开的轮廓,总是返回true
	virtual bool IsOpenContour() const;							
	
	// 是否为区域,总是返回false
	virtual bool IsRegion() const;								
	
	// 是否为有限的,总是返回true
	virtual bool IsFinite() const;					
	
	// 是否为空,总是返回false			
	virtual bool IsEmpty() const;							
	
	// 获取椭圆弧的最小外接矩形	
	virtual scRect BoundingBox() const;						
	
	// 计算椭圆弧上离给定点p最近的点
//	virtual sc2Vector NearestPoint(const sc2Vector& p) const;	
	
	// 获取椭圆弧的边长
//	virtual double Perimeter() const;						

private:
	virtual bool IsEqual(const scShape& rhs) const;

	scEllipse m_Ellipse;
	scRadian m_rStartPhi;
	scRadian m_rSpanPhi;

};



// 重载==操作符
inline bool scEllipseArc::operator==(const scEllipseArc &rhs) const
{
	return m_Ellipse == rhs.m_Ellipse
		&& m_rStartPhi == rhs.m_rStartPhi
		&& m_rSpanPhi == rhs.m_rSpanPhi;
}



// 重载!=操作符
inline bool scEllipseArc::operator!=(const scEllipseArc &rhs) const
{
	return !(*this == rhs);
}

#pragma pack(pop)