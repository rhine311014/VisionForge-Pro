// svEllipseAnnulusSection.h


#pragma once

#include "svShape.h"
#include "svAnnulus.h"
#include "svEllipseAnnulus.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            椭圆环段
// ---------------------------------------------------------------------
// 


class scEllipseAnnulusSection;
smDerivedShapePtrhDecl(scEllipseAnnulusSection);


class SVCORE_API scEllipseAnnulusSection : public scShape  
{
public:	
	// 构造函数
	scEllipseAnnulusSection();	
	
	// 用椭圆环段所在椭圆环的中心center,半轴长度radii，旋转角度rotation,
	// 起始角度startPhi，张角spanPhi和径向缩放比例dRadialScale构造椭圆环段
	scEllipseAnnulusSection(const sc2Vector& center, const sc2Vector& radii, const scRadian& rotation,
		const scRadian& startPhi, const scRadian& spanPhi, double dRadialScale);

	// 用椭圆环ann、起始角startPhi和张角spanPhi构造椭圆环段
	scEllipseAnnulusSection(const scEllipseAnnulus& ann, const scRadian& startPhi, const scRadian& spanPhi);


	// 设置椭圆环段参数：椭圆环的中心center,半轴长度radii，旋转角度rotation，
	// 起始角度startPhi，张角spanPhi和径向缩放比例dRadialScale
	bool SetCenterRaddiiRotationStartPhiSpanPhiRadialScale(const sc2Vector& center, 
		const sc2Vector& radii, const scRadian& rotation,
		const scRadian& startPhi, const scRadian& spanPhi, double dRadialScale);

	// 设置椭圆环段参数：椭圆环ellipseAnnulus，起始角度startPhi和张角spanPhi
	void SetEllipseAnnulusRadialScale(const scEllipseAnnulus& ellipseAnnulus, 
		const scRadian& startPhi, const scRadian& spanPhi);

	// 设置中心
	void SetCenter(const sc2Vector& center) { m_ellipseAnnulus.SetCenter(center);};

	// 设置基准椭圆的半轴
	bool SetRadii(const sc2Vector& radii) { return m_ellipseAnnulus.SetRadii(radii);};

	// 设置旋转角度
	void SetRotation(const scRadian& rotation){ m_ellipseAnnulus.SetRotation(rotation);};

	// 设置起始角度
	void SetStartPhi(const scRadian& startPhi){ m_rStartPhi = startPhi.SignedNorm();};

	// 设置张角
	void SetSpanPhi(const scRadian& spanPhi){ m_rSpanPhi = spanPhi;};

	// 设置径向缩放比例
	bool SetRadialScale(double dRadialScale){ return m_ellipseAnnulus.SetRadialScale(dRadialScale);};
	
	// 设置椭圆环
	void SetEllipseAnnulus(const scEllipseAnnulus& ellipseAnnulus) { m_ellipseAnnulus = ellipseAnnulus; };

	// 获取中心
	const sc2Vector& GetCenter() const { return m_ellipseAnnulus.GetCenter();}

	// 获取基准椭圆的半轴
	const sc2Vector& GetRadii() const { return m_ellipseAnnulus.GetRadii();}

	// 获取旋转角度
	const scRadian& GetRotation() const { return m_ellipseAnnulus.GetRotation();}

	// 获取起始角度
	const scRadian& GetStartPhi() const { return m_rStartPhi;}

	// 获取张角
	const scRadian& GetSpanPhi() const { return m_rSpanPhi;}

	// 获取终止角度
	scRadian GetEndPhi() const { return m_rStartPhi + m_rSpanPhi;}

	// 获取径向缩放比例
	double GetRadialScale() const { return m_ellipseAnnulus.GetRadialScale();}

	// 是否为离心的
	bool GetIsCentrifugal() const { return m_ellipseAnnulus.GetRadialScale() > 1;}

	// 角度是否为增加的
	bool GetIsIncreasingAngle() const { return m_rSpanPhi.ToDouble() >= 0;}
	
	// 获得椭圆环
	const scEllipseAnnulus& GetEllipseAnnulus() const { return m_ellipseAnnulus;}	

	// 获取内椭圆
	scEllipse GetInnerEllipse() const { return m_ellipseAnnulus.GetInnerEllipse();}				
	
	// 获取外椭圆
	scEllipse GetOuterEllipse() const { return m_ellipseAnnulus.GetOuterEllipse();}		


	// 判断椭圆环段是否有退化
	bool Degenerate() const { return m_ellipseAnnulus.Degenerate(); }					

	sc2Vector GetCornerPo   () const;											// 获取起点Po
	sc2Vector GetCornerPx   () const;											// 获取角点Px
	sc2Vector GetCornerPy   () const;											// 获取角点Py
	sc2Vector GetCornerPopp () const;											// 获取对角点Popp

	bool operator==(const scEllipseAnnulusSection& rhs) const;					// 重载==操作符
	bool operator!=(const scEllipseAnnulusSection& rhs) const;					// 重载!=操作符

	// 重载scShape的方法
	//////////////////////////////////////////////////////////////////////////

	virtual ShapeType GetType() const{ return eEllipseAnnulusSection; };				// 返回类型的枚举值
	virtual scShapePtrh Clone() const;											// 复制得到新的椭圆环段
	virtual bool IsOpenContour() const;											// 是否为开的轮廓,总是返回false
	virtual bool IsRegion() const;												// 是否为区域,总是返回true
	virtual bool IsFinite() const;												// 是否为有限的,总是返回true
	virtual bool IsEmpty() const;												// 是否为空,总是返回false

	virtual scRect BoundingBox() const;											// 获取椭圆环段的最小外接矩形
//	virtual sc2Vector NearestPoint(const sc2Vect &p) const;						// 计算椭圆环段上离给定点p最近的点

//	virtual bool Within(const sc2Vector& p) const;								// 判断给定点是否在当前椭圆环段内

private:
	virtual bool IsEqual(const scShape& rhs) const;

	void NormalizePhi2(bool bNormalzie = true);

private:

	scEllipseAnnulus m_ellipseAnnulus;					// 椭圆环
	scRadian m_rStartPhi;								// 起始角
	scRadian m_rSpanPhi;								// 张角
};


// 重载==操作符
inline bool scEllipseAnnulusSection::operator==(const scEllipseAnnulusSection& rhs) const
{
	return m_ellipseAnnulus == rhs.m_ellipseAnnulus
		&& m_rStartPhi == rhs.m_rStartPhi
		&& m_rSpanPhi == rhs.m_rSpanPhi;
};

// 重载!=操作符
inline bool scEllipseAnnulusSection::operator!=(const scEllipseAnnulusSection& rhs) const
{
	return !(*this == rhs);
};

#pragma pack(pop)