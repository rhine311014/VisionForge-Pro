// svEllipseAnnulus.h


#pragma once

#include "svShape.h"
#include "svCircle.h"
#include "svEllipse.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            椭圆环
// ---------------------------------------------------------------------
// 


class scEllipseAnnulus;
smDerivedShapePtrhDecl(scEllipseAnnulus);

class SVCORE_API scEllipseAnnulus : public scShape  
{
public:
	
	// 构造函数
	scEllipseAnnulus();	

	
	// 用基准椭圆的中心center，半轴radii，旋转角度rotation，以及另一个椭圆的半轴相对于radii的比例系数构造椭圆环
	// 半轴radii的x和y分量分别是X方向和Y方向的半轴长度
	scEllipseAnnulus(const sc2Vector& center, const sc2Vector& radii, const scRadian& rotation, double dRadialScale);
	scEllipseAnnulus(double centerX,double centerY, double radiiX,double radiiY, double rotationRadian, double dRadialScale);

	// 用基准椭圆和径向缩放比例构造椭圆环
	scEllipseAnnulus(const scEllipse& ell, double dRadialScale);

	// 设置椭圆环参数：基准椭圆的中心center，半轴radii，旋转角度rotation，以及另一个椭圆的半轴相对于radii的比例系数
	bool SetCenterRadiiAngleRotationRadialScale(
		const sc2Vector& center, const sc2Vector& radii, const scRadian& rotation, double dRadialScale);
	bool SetCenterRadiiAngleRotationRadialScale(
		double centerX,double centerY, double radiiX,double radiiY, double rotationRadian, double dRadialScale);

	// 设置椭圆环参数：基准椭圆和径向缩放比例
	bool SetEllipseRadialScale(const scEllipse& ell, double dRadialScale);

	// 设置基准椭圆
	void SetEllipse(const scEllipse& ell);

	// 设置椭圆环中心
	void SetCenter(const sc2Vector& center);
	void SetCenter(double centerX,double centerY);

	// 设置椭圆环基准半轴
	bool SetRadii(const sc2Vector& radii);
	bool SetRadii(double radiiX,double radiiY);

	// 设置椭圆环旋转角度
	void SetRotation(const scRadian& rotation);

	// 设置椭圆环的径向缩放比例
	bool SetRadialScale(double dRadialScale);


	// 获取中心							
	const sc2Vector& GetCenter() const{ return m_ellipse.GetCenter();};	

	// 获取半轴
	const sc2Vector& GetRadii() const { return m_ellipse.GetRadii();}

	// 获取旋转角度
	const scRadian& GetRotation() const {return m_ellipse.GetRotation();};

	// 获取径向缩放比例
	double	GetRadialScale() const { return m_dRadialScale;}
	
	// 获取内椭圆的X方向和Y方向的半轴长度			
	sc2Vector GetInnerRadii() const
	{ return m_dRadialScale > 1 ? m_ellipse.GetRadii() : m_ellipse.GetRadii() * m_dRadialScale;};								
	
	// 获取外椭圆的X方向和Y方向的半轴长度
	sc2Vector GetOuterRadii() const
	{ return m_dRadialScale > 1 ? m_ellipse.GetRadii() * m_dRadialScale : m_ellipse.GetRadii();};	

	// 获取基准椭圆
	const scEllipse& GetEllipse() const { return m_ellipse;};

	// 获取内椭圆
	scEllipse GetInnerEllipse() const 
	{ return scEllipse(GetCenter(), GetInnerRadii(), GetRotation());};		

	// 获取外椭圆
	scEllipse GetOuterEllipse() const 
	{ return scEllipse(GetCenter(), GetOuterRadii(), GetRotation());};	


	// 对当前椭圆环进行二维变换(XY一致缩放平移但没有旋转和切变),得到新的椭圆环
	scEllipseAnnulus Map(const sc2XformLinear& c) const;
								

	// 判断椭圆环参数是否有效
	bool Degenerate() const { return m_ellipse.Degenerate()|| m_dRadialScale <= 0 || m_dRadialScale == 1.;}			

	// 重载==操作符
	bool operator==(const scEllipseAnnulus& rhs) const;			
	
	// 重载!=操作符
	bool operator!=(const scEllipseAnnulus& rhs) const {return !(*this == rhs);};			

	
	//////////////////////////////////////////////////////////////////////////
	// 重载scShape的方法

	// 返回类型的枚举值
	virtual ShapeType GetType() const{ return eEllipseAnnulus; };

	// 复制当前椭圆环得到新的椭圆环
	virtual scShapePtrh Clone() const;		
	
	// 是否为开轮廓,总是返回false				
	virtual bool IsOpenContour() const;						
	
	// 是否为区域,总是返回true
	virtual bool IsRegion() const;							
	
	// 是否为有限的,总是返回true
	virtual bool IsFinite() const;							
	
	// 是否为空的,总是返回false
	virtual bool IsEmpty() const;							
	
	// 获取椭圆环的外接矩形
	virtual scRect BoundingBox() const;					
	
	// 计算椭圆环上离给定点p最近的点
	virtual sc2Vector NearestPoint(const sc2Vector& p) const;	

	// 判断给定点是否在当前椭圆环内
	virtual bool Within(const sc2Vector& p) const;				

	virtual bool IsMultipleContours() const;

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;		// 获得掩模图像

private:
	virtual bool IsEqual(const scShape &rhs) const;

private:
	scEllipse m_ellipse;
	double	  m_dRadialScale;

};

// 重载==操作符
inline bool scEllipseAnnulus::operator==(const scEllipseAnnulus& rhs) const
{
	return m_ellipse == rhs.m_ellipse 
		&& m_dRadialScale == rhs.m_dRadialScale;
}

#pragma pack(pop)
