// svEllipse.h

#pragma once

#include "svShape.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            椭圆形
// ---------------------------------------------------------------------
// 


class scEllipse;
smDerivedShapePtrhDecl(scEllipse);


class SVCORE_API scEllipse : public scShape  
{
public:
	// 构造函数
	scEllipse();										

	// 用中心center、X方向和Y方向的半轴长度构成的向量radii、旋转角度angle来构造椭圆；
	// 向量radii的x、y分量分别是X方向和Y方向的半轴长度，它们应该为正值；
	// angle将被规整化到[-π，π)；
	scEllipse(const sc2Vector& center,
			  const sc2Vector& radii,
			  const scRadian&  rotation = scRadian(0.));

	scEllipse(double centerX,double centerY, double radiiX,double radiiY,double  rotation);

	// 用二维仿射变换ellFromUnitedCirc来构造椭圆
	scEllipse(const sc2XformLinear &ellFromUnitCirc);			
	
	
	// 设置椭圆参数:中心center，半轴长度radii，旋转角度rotation
	bool SetCenterRadiiRotation(const sc2Vector& center, const sc2Vector& radii, const scRadian& rotation);
	bool SetCenterRadiiRotation(double centerX,double centerY, double radiiX,double radiiY,double  rotation);

	// 设置X方向和Y方向的半轴长度
	// radii的x分量和y分量应为正值，否则会设置失败；
	// 如果设置成功则返回true，否则返回false
	bool SetRadii(const sc2Vector& radii);		
	bool SetRadii(double radiiX,double radiiY);
	const sc2Vector &GetRadii() const { return m_vRadii;};				// 获取X方向和Y方向的半轴长度	

	void SetCenter(const sc2Vector& center);							// 设置中心	
	void SetCenter(double centerX,double centerY);
	const sc2Vector &GetCenter() const { return m_vCenter;};			// 获取中心			
	
	void SetRotation(const scRadian& rotation);							// 设置旋转角度						
	const scRadian& GetRotation() const { return m_rRotation;};			// 获取旋转角度


	const sc2XformLinear &GetEllFromUnitCirc() const;					// 获取仿射变换
	bool SetEllFromUnitCirc(const sc2XformLinear& X);					// 设置仿射变换X


	scEllipse Map(const sc2XformLinear& c) const;						// 对当前椭圆形进行二维变换(XY一致缩放平移但没有旋转和切变),得到新的椭圆形

	scEllipse Translate(const sc2Vector& offset) const;					// 平移当前椭圆得到新的椭圆,平移量为offset

	scEllipse Rotate(const scRadian& theta) const;						// 以椭圆中心为中心旋转当前椭圆得到新的椭圆，旋转角度theta

	scEllipse Scale(const sc2Vector& mag) const;						// 对当前椭圆的半轴进行缩放变换得到新的椭圆，缩放值为mag


	sc2Vector GetPoint(const scRadian& phi) const;						// 返回角度参数phi在当前椭圆上对应的点

	scRadian GetTangent(const scRadian& phi) const;						// 返回角度参数phi在当前椭圆上对应点的切线角度

	scRadian GetPhi(const sc2Vector& p) const;							// 椭圆上给定点p所对应的角度参数phi

	bool Degenerate() const;											// 判断当前椭圆是否退化

	bool operator==(const scEllipse &rhs) const;						// 重载==操作符

	bool operator!=(const scEllipse &rhs) const							// 重载!=操作符
	{ return !(*this == rhs);}


	virtual ShapeType GetType() const{ return eEllipse; };				// 返回类型的枚举值
	scShapePtrh Clone() const;											// 复制当前的椭圆得到新的椭圆
	bool IsOpenContour() const;											// 是否为开轮廓,总是返回false
	bool IsRegion() const;												// 是否为区域,总是返回true
	bool IsFinite() const;												// 是否为有限的,总是返回true
	bool IsEmpty() const;												// 是否为空,总是返回false

	scRect BoundingBox() const;											// 获取椭圆形的最小外接矩形
	sc2Vector NearestPoint(const sc2Vector& p) const;					// 计算椭圆上离给定点p最近的点
	bool Within(const sc2Vector& p) const;								// 判断给定点p是否在椭圆内

	//double Perimeter() const;											// 获取椭圆的边缘长度

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;		// 获得掩模图像

private:
	bool IsEqual(const scShape& rhs) const;								// 判断给定图形rhs是否与当前图形相等，仅供内部使用

private:

	sc2Vector m_vCenter;												// 中心
	sc2Vector m_vRadii;													// 长短轴半径
	scRadian m_rRotation;												// 旋转角度

	sc2XformLinear m_xEllFromUnitCirc;									// 由单位圆映射到椭圆的仿射变换
};

#pragma pack(pop)
