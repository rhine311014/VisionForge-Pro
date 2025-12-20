// svAnnulusSection.h


#pragma once

#include "svShape.h"
#include "svAnnulus.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            圆环段
// ---------------------------------------------------------------------
// 

class scAnnulusSection;
smDerivedShapePtrhDecl(scAnnulusSection);


class SVCORE_API scAnnulusSection : public scShape  
{
public:
	// 构造函数
	scAnnulusSection();	

	// 用圆心center，基准半径radius，起始角度startPhi,张角spanPhi和径向缩放比例dRadialScale
	// 来构造圆环段
	scAnnulusSection(const sc2Vector& center, double radius, 
		const scRadian& startPhi, const scRadian& spanPhi, double dRadialScale);

	// 用圆环ann，起始角度startPhi,张角spanPhi来构造圆环段
	scAnnulusSection(const scAnnulus& ann, 
		const scRadian& startPhi, const scRadian& spanPhi);

	// 返回圆环段所在的圆环
	const scAnnulus& GetAnnulus() const { return m_Annulus;}				
	
	// 返回圆环段的所在圆环的中心
	const sc2Vector& GetCenter() const { return m_Annulus.GetCenter();}		

	// 返回起始角	
	const scRadian& GetStartPhi() const { return m_rStartPhi;}

	// 返回张角
	const scRadian& GetSpanPhi() const { return m_rSpanPhi;}

	// 返回终止角
	const scRadian GetEndPhi() const { return m_rStartPhi + m_rSpanPhi;}

	// 返回基准圆半径
	double GetRadius() const { return m_Annulus.GetRadius();};

	// 返回径向缩放比例
	double	GetRadialScale() const { return m_Annulus.GetRadialScale();}	

	// 返回张角是否为增长的
	bool GetIsIncreasingAngle() const { return m_rSpanPhi.ToDouble() >= 0;};

	// 返回是否为离心的
	bool GetIsCentrifugal() const { return m_Annulus.GetRadialScale() > 1;}


	// 设置圆环段参数:圆心center，基准半径radius，起始角度startPhi,张角spanPhi和径向缩放比例dRadialScale
	void SetCenterRadiusStartPhiSpanPhiRadialScale(const sc2Vector& center, double radius, 
		const scRadian& startPhi, const scRadian& spanPhi, double dRadialScale);

	
	// 设置圆环中心
	void SetCenter(const sc2Vector& c){ m_Annulus.SetCenter(c);};

	// 设置圆环的基准半径r
	void SetRadius(double r){ m_Annulus.SetRadius(r);};

	// 设置径向缩放比例dRadialScale
	void SetRadialScale(double dRadialScale){ m_Annulus.SetRadialScale(dRadialScale);};

	// 设置起始角startPhi
	void SetStartPhi(const scRadian& startPhi) { m_rStartPhi = startPhi.SignedNorm();};

	// 设置张角phi2
	void SetSpanPhi(const scRadian& spanPhi) { m_rSpanPhi = spanPhi;};


	// 设置圆环段所在的圆环
	void SetAnnulus(const scAnnulus& annulus) { m_Annulus = annulus;};

	
	// 返回圆环段所在的圆环的内圆
	scCircle GetInnerCircle() const { return m_Annulus.GetInnerCircle();}					
	
	// 返回圆环段所在的圆环的外圆
	scCircle GetOuterCircle() const { return m_Annulus.GetOuterCircle();}					

	// 对当前圆环段进行二维变换(XY一致缩放平移但没有旋转和切变),得到新的圆环段
	scAnnulusSection Map(const sc2XformLinear& c) const;				
	
	// 判断圆环段是否退化
	bool Degenerate() const { return m_Annulus.Degenerate(); }						

	sc2Vector GetCornerPo   () const;										// 返回原点
	sc2Vector GetCornerPx   () const;										// 返回x角点
	sc2Vector GetCornerPy   () const;										// 返回y角点
	sc2Vector GetCornerPopp () const;										// 返回对角点

	// 重载==操作符
	bool operator==(const scAnnulusSection&) const;						
	
	// 重载!=操作符
	bool operator!=(const scAnnulusSection&) const;						


	// 返回类型的枚举值
	virtual ShapeType GetType() const{ return eAnnulusSection; };

	// 复制得到新的圆环段
	virtual scShapePtrh Clone() const;	

	// 是否为开的轮廓,总是返回false
	virtual bool IsOpenContour() const;										
	
	// 是否为区域,总是返回true
	virtual bool IsRegion() const;											
	
	// 是否为有限的,总是返回true
	virtual bool IsFinite() const;											
	
	// 是否为空,总是返回false
	virtual bool IsEmpty() const;										
	
	// 获取圆环段的最小外接矩阵	
	virtual scRect BoundingBox() const;								


	// 计算圆环段上离给定点p最近的点
	virtual sc2Vector NearestPoint(const sc2Vector& p) const;		
	
	// 判断给定点是否在当前圆环段内
	virtual bool Within(const sc2Vector& p) const;								

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;		// 获得掩模图像

private:
	virtual bool IsEqual(const scShape& rhs) const;

	scAnnulus m_Annulus;													// 圆环
	scRadian m_rStartPhi;													// 起始角
	scRadian m_rSpanPhi;													// 张角

};


// 重载==操作符
inline bool scAnnulusSection::operator==(const scAnnulusSection& rhs) const
{
	return m_Annulus == rhs.m_Annulus
		&& m_rStartPhi == rhs.m_rStartPhi
		&& m_rSpanPhi == rhs.m_rSpanPhi;
};

// 重载!=操作符
inline bool scAnnulusSection::operator!=(const scAnnulusSection& rhs) const
{
	return !(*this == rhs);
};

#pragma pack(pop)
