// svRect.h

#pragma once

#include "svMath.h"
#include "svShape.h"

#pragma pack(push,8)


// ---------------------------------------------------------------------
//                            矩形
// ---------------------------------------------------------------------
// 矩形类scRect用来表示矩形,派生自图形基类scShape

class scLineSeg;
smDerivedShapePtrhDecl(scRect);

class SVCORE_API scRect : public scShape
{

public:
	// 构造函数
	scRect();

	// 用矩形左上角点ul和尺寸sz构造矩形对象
	scRect(const sc2Vector& ul, const sc2Vector& sz);

	scRect(double w, double h);									// 用矩形的尺寸(宽高)来构造矩形
	scRect(double x, double y, double w, double h);				// 用矩形的左上角点和尺寸(宽高)构造矩形

	void   Set(const sc2Vector& ul, const sc2Vector& sz);		// 用矩形的左上角点和尺寸(宽高)设置矩形
	void   Set(double x, double y, double w, double h);			// 用矩形的左上角点和尺寸(宽高)设置矩形

	void   SetOrigion(const sc2Vector& ul);						// 设置矩形的左上角点
	const sc2Vector& GetOrigin() const{ return m_vectUL;};		// 获取矩形的左上角点

	void   SetSize(const sc2Vector& sz);						// 设置矩形的尺寸
	const sc2Vector& GetSize () const{ return m_vectSZ;};		// 获取矩形的尺寸

	void   SetWidth(double w);									// 设置矩形的宽
	double GetWidth()const{ return m_vectSZ.GetX();};			// 获取矩形的宽

	void   SetHeight(double h);									// 设置矩形的高
	double GetHeight()const{ return m_vectSZ.GetY();};			// 获取矩形的高


	const sc2Vector& GetUL() const{ return m_vectUL;};			// 获取矩形左上角点
	sc2Vector GetUR() const;									// 获取矩形右上角点
	sc2Vector GetLR() const;									// 获取矩形右下角点
	sc2Vector GetLL() const;									// 获取矩形左下角点
	
	double	GetLeft()	const { return m_vectUL.GetX();};		// 获取矩形的最小x坐标
	double	GetTop()	const { return m_vectUL.GetY();};		// 获取矩形的最小y坐标
	double	GetRight()	const									// 获取矩形的最大x坐标
	{ return m_vectUL.GetX() + m_vectSZ.GetX();};
	double	GetBottom() const									// 获取矩形的最大y坐标
	{ return m_vectUL.GetY() + m_vectSZ.GetY();};

	
	long GetPixelLeft() const									// 获取矩形以长整型表示的最小x坐标
	{ return long(floor(m_vectUL.GetX() + 0.5));};		

	long GetPixelTop() const									// 获取矩形以长整型表示的最小y坐标
	{ return long(floor(m_vectUL.GetY() + 0.5));};	

	long GetPixelRight() const									// 获取矩形以长整型表示的最大x坐标
	{ return long(floor(m_vectUL.GetX() + m_vectSZ.GetX() + 0.5));};

	long GetPixelBottom() const									// 获取矩形以长整型表示的最大y坐标
	{ return long(floor(m_vectUL.GetY() + m_vectSZ.GetY() + 0.5));};

	long GetPixelWidth() const									// 获取矩形以长整型表示的宽度
	{ return ( GetPixelRight() - GetPixelLeft() );};

	long GetPixelHeight() const									// 获取矩形以长整型表示的高度
	{ return ( GetPixelBottom() - GetPixelTop() );};

	bool   IsNull() const										// 判断矩形是否为Null
	{ return sfRealEqual(GetWidth(),0.)||sfRealEqual(GetHeight(),0.);};

	scLineSeg GetLSeg() const;									// 获取矩形的左边线段
	scLineSeg GetRSeg() const;									// 获取矩形的右边线段
	scLineSeg GetTSeg() const;									// 获取矩形的上边线段
	scLineSeg GetBSeg() const;									// 获取矩形的下边线段

	scRect Map(const sc2XformLinear& c) const;					// 对当前矩形进行二维变换(XY一致缩放平移但没有旋转和切变),得到新的矩形对象

	scRect Intersect(const scRect& r) const;					// 计算给定矩形r与当前矩形的交
	scRect operator&(const scRect& r) const						// 计算给定矩形r与当前矩形的交
	{return Intersect(r);};
	scRect& operator&=(const scRect& r);						// 计算给定矩形r与当前矩形的交

	scRect Enclose(const scRect& r) const;						// 计算同时包围给定图形r和当前矩形的最小外接矩形
	scRect operator|(const scRect& r) const 					// 计算同时包围给定图形r和当前矩形的最小外接矩形
	{return Enclose(r);};
	scRect& operator|=(const scRect& r);						// 计算同时包围给定图形r和当前矩形的最小外接矩形

	// 用给定的left、right、top、bottom值对当前矩形进行裁剪得到新的矩形
	scRect Trim(double left, double right, double top, double bottom) const;

	scRect Transpose () const;									// 对当前矩形进行转置变换得到新的矩形

	// 判断矩形是否退化，当矩形的宽高都为非正数时返回true
	bool Degenerate() const { return m_vectSZ.GetX() <= 0. && m_vectSZ.GetY() <= 0.;} 

	bool operator==(const scRect& rhs) const;					// 重载==操作符
	bool operator!=(const scRect& rhs) const					// 重载!=操作符
	{ return !(*this == rhs);};
					

	
	ShapeType GetType() const{ return eRect; };						// 返回类型的枚举值
	scShapePtrh Clone() const;									// 复制当前矩形得到新的图形
	bool IsOpenContour() const;									// 是否为开的轮廓,总是返回false
	bool IsRegion() const;										// 是否为区域,总是true
	bool IsFinite() const;										// 是否为有限的,总是返回true
	bool IsEmpty() const;										// 是否为空,总是返回false
		
	scRect BoundingBox() const;									// 获取当前矩形的最小外接矩形
	double Perimeter() const;									// 获取图形的边长
	
	// 计算矩形上离给定点距离最短的点
	sc2Vector NearestPoint(const sc2Vector& p) const; 

	// 对当前矩形进行sc2Xform变换X，得到新的图形对象
//	scShapePtrh MapShape(const sc2XformLinear& X) const;	

	// 判断给定点是否在当前矩形内，如果在矩形内则返回true否则返回false
	bool Within(const sc2Vector& p) const;

	void Normalize();

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;		// 获得掩模图像

private:
	bool IsEqual(const scShape& rhs) const;								// 判断给定图形rhs是否与当前图形相等，仅供内部使用

private:
	sc2Vector	m_vectUL;			// 左上角点
	sc2Vector	m_vectSZ;			// 尺寸

};

inline sc2Vector scRect::GetUR() const
{ return sc2Vector(m_vectUL.GetX() + m_vectSZ.GetX(), m_vectUL.GetY());}

inline sc2Vector scRect::GetLR() const
{ return sc2Vector(m_vectUL.GetX() + m_vectSZ.GetX(), m_vectUL.GetY() + m_vectSZ.GetY());}

inline sc2Vector scRect::GetLL() const
{ return sc2Vector(m_vectUL.GetX(), m_vectUL.GetY() + m_vectSZ.GetY());}

#pragma pack(pop)
