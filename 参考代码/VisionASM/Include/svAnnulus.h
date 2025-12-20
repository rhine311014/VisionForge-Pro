// svAnnulus.h


#pragma once

#include "svShape.h"
#include "svCircle.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            圆环
// ---------------------------------------------------------------------
// 

class scAnnulus;
smDerivedShapePtrhDecl(scAnnulus);

class SVCORE_API scAnnulus : public scShape  
{
public:
	
	// 构造函数
	scAnnulus();	

	// 用圆心c,基准半径r和径向缩放比例dRadialScale来构造圆环
	scAnnulus(const sc2Vector& c, double r, double dRadialScale = 0.5);

	// 用基准圆circle和径向缩放比例dRadialScale来构造圆环
	scAnnulus(const scCircle& circle, double dRadialScale = 0.5);

	// 用圆心c,基准半径r和径向缩放比例dRadialScale来构造圆环
	scAnnulus(double cX,double cY, double r, double dRadialScale = 0.5);

	// 设置圆环参数：圆环中心c，基准半径r和径向缩放比例dRadialScale
	void SetCenterRadiusRadialScale(const sc2Vector& c, double r, double dRadialScale);

	// 设置圆环参数：圆环中心c，基准半径r和径向缩放比例dRadialScale
	void SetCenterRadiusRadialScale(double cX,double cY, double r, double dRadialScale);
	
	// 设置圆环中心
	void SetCenter(const sc2Vector& c){ m_vCenter = c;};

	// 设置圆环中心
	void SetCenter(double cX,double cY){m_vCenter = sc2Vector(cX,cY);};

	// 设置圆环的基准半径r
	void SetRadius(double r){ if (r < 0) return; m_dRadius = r;};

	// 设置径向缩放比例dRadialScale
	void SetRadialScale(double dRadialScale)
	{ if (dRadialScale < 0) return; m_dRadialScale = dRadialScale;};

	// 获取圆环的基准半径
	double GetRadius() const { return m_dRadius;};

	// 获取径向的缩放比例
	double GetRadialScale() const {return m_dRadialScale;};

	// 获取圆环中心心
	const sc2Vector& GetCenter() const{ return m_vCenter;};				
	
	// 获取内圆半径	
	double GetInnerRadius() const
	{ return m_dRadialScale > 1. ? m_dRadius : m_dRadius * m_dRadialScale;};						
	
	// 获取外圆半径	
	double GetOuterRadius() const
	{ return m_dRadialScale > 1. ? m_dRadius * m_dRadialScale : m_dRadius;};

	// 获取内圆
	scCircle GetInnerCircle() const{ return scCircle(m_vCenter, GetInnerRadius());};		

	// 获取外圆
	scCircle GetOuterCircle() const{ return scCircle(m_vCenter, GetOuterRadius());};	

	// 对当前圆环进行二维变换(XY一致缩放平移但没有旋转和切变),得到新的圆环
	scAnnulus Map(const sc2XformLinear& c) const;

	// 判断圆环是否退化
	bool Degenerate() const { return m_dRadialScale == 1.
		|| m_dRadius <= 0.|| m_dRadialScale <= 0.; }
	// returns    true if the annulus is degenerate.

	// 重载==操作符
	bool operator==(const scAnnulus& rhs) const;	

	// 重载!=操作符
	bool operator!=(const scAnnulus& rhs) const { return !(*this == rhs);}
	


	//////////////////////////////////////////////////////////////////////////
	// 重载scShape的方法
	
	// 返回类型的枚举值
	virtual ShapeType GetType() const{ return eAnnulus; };

	// 复制当前的圆环对象得到新的圆环
	virtual scShapePtrh Clone() const;	
	
	// 是否为开轮廓,总是返回false				
	virtual bool IsOpenContour() const;					
	
	// 是否为区域,总是返回true
	virtual bool IsRegion() const;						
	
	// 是否有限,总是返回true
	virtual bool IsFinite() const;						
	
	// 是否为空,总是返回false
	virtual bool IsEmpty() const;							

	// 获取圆环的最小外接矩形
	virtual scRect BoundingBox() const;				
	
	// 计算圆环上离给定点p最近的点
	virtual sc2Vector NearestPoint(const sc2Vector& p) const;	

	// 判断给定点是否在圆环内
	virtual bool Within(const sc2Vector& p) const;			

	virtual bool IsMultipleContours() const;

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;		// 获得掩模图像

private:
	virtual bool IsEqual(const scShape& rhs) const;

private:
	sc2Vector m_vCenter;
	double	m_dRadius;
	double	m_dRadialScale;

};

#pragma pack(pop)
