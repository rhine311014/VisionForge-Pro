// svMaskRect.h

#pragma once

#include "svMath.h"
#include "svShape.h"
#include "svRect.h"

#pragma pack(push,8)


// ---------------------------------------------------------------------
//                         带掩膜的矩形
// ---------------------------------------------------------------------
// 矩形类scMaskRect用来表示带掩膜的矩形,派生自图形基类scShape

class scLineSeg;
class scMaskRect;
smDerivedShapePtrhDecl(scMaskRect);

class SVCORE_API scMaskRect : public scShape
{
public:
	// 构造函数
	scMaskRect();

	// 用矩形构造
	scMaskRect(const scRect& rect);
	scMaskRect(const scRect& rect, const scRect& rect_mask);

	// 用矩形左上角点ul和尺寸sz构造矩形对象
	scMaskRect(const sc2Vector& ul, const sc2Vector& sz);
	scMaskRect(const sc2Vector& ul, const sc2Vector& sz, const sc2Vector& ul_mask, const sc2Vector& sz_mask);

	// 用矩形的左上角点和尺寸(宽高)构造矩形
	scMaskRect(double x, double y, double w, double h);
	scMaskRect(double x, double y, double w, double h, double x_mask, double y_mask, double w_mask, double h_mask);				

	// 用矩形构造设置
	void   Set(const scRect& rect);
	void   Set(const scRect& rect, const scRect& rect_mask);

	// 用矩形的左上角点和尺寸(宽高)设置矩形
	void   Set(const sc2Vector& ul, const sc2Vector& sz);
	void   Set(const sc2Vector& ul, const sc2Vector& sz, const sc2Vector& ul_mask, const sc2Vector& sz_mask);

	// 用矩形的左上角点和尺寸(宽高)设置矩形
	void   Set(double x, double y, double w, double h);
	void   Set(double x, double y, double w, double h, double x_mask, double y_mask, double w_mask, double h_mask);


	void   SetRect(const scRect& rect);							// 设置矩形
	const scRect& GetRect() const{ return m_rect;};				// 获取矩形

	void   SetRect_Mask(const scRect& rect_mask);				// 设置矩形
	const scRect& GetRect_Mask() const{ return m_rectMask;};	// 获取矩形


	void   SetOrigion(const sc2Vector& ul);									// 设置矩形的左上角点
	void   SetOrigion(double ulX,double ulY);
	const sc2Vector& GetOrigin() const{ return m_rect.GetOrigin();};		// 获取矩形的左上角点

	void   SetSize(const sc2Vector& sz);									// 设置矩形的尺寸
	void   SetSize(double szX,double szY);
	const sc2Vector& GetSize () const{ return m_rect.GetSize();};			// 获取矩形的尺寸

	void   SetWidth(double w);												// 设置矩形的宽
	double GetWidth()const{ return m_rect.GetWidth();};						// 获取矩形的宽

	void   SetHeight(double h);												// 设置矩形的高
	double GetHeight()const{ return m_rect.GetHeight();};					// 获取矩形的高

	void   SetOrigion_Mask(const sc2Vector& ul);									// 设置矩形的左上角点
	void   SetOrigion_Mask(double ulX,double ulY);
	const sc2Vector& GetOrigin_Mask() const{ return m_rectMask.GetOrigin();};		// 获取矩形的左上角点

	void   SetSize_Mask(const sc2Vector& sz);										// 设置矩形的尺寸
	void   SetSize_Mask(double szX,double szY);
	const sc2Vector& GetSize_Mask () const{ return m_rectMask.GetSize();};			// 获取矩形的尺寸

	void   SetWidth_Mask(double w);													// 设置矩形的宽
	double GetWidth_Mask()const{ return m_rectMask.GetWidth();};					// 获取矩形的宽

	void   SetHeight_Mask(double h);												// 设置矩形的高
	double GetHeight_Mask()const{ return m_rectMask.GetHeight();};					// 获取矩形的高


	const sc2Vector& GetUL() const{ return m_rect.GetUL();};						// 获取矩形左上角点
	sc2Vector GetUR() const{ return m_rect.GetUR();};								// 获取矩形右上角点
	sc2Vector GetLR() const{ return m_rect.GetLR();};								// 获取矩形右下角点
	sc2Vector GetLL() const{ return m_rect.GetLL();};								// 获取矩形左下角点

	const sc2Vector& GetUL_Mask() const{ return m_rectMask.GetUL();};				// 获取矩形左上角点
	sc2Vector GetUR_Mask() const{ return m_rectMask.GetUR();};						// 获取矩形右上角点
	sc2Vector GetLR_Mask() const{ return m_rectMask.GetLR();};						// 获取矩形右下角点
	sc2Vector GetLL_Mask() const{ return m_rectMask.GetLL();};						// 获取矩形左下角点

	
	double	GetLeft()	const { return m_rect.GetLeft();};							// 获取矩形的最小x坐标
	double	GetTop()	const { return m_rect.GetTop();};							// 获取矩形的最小y坐标
	double	GetRight()	const { return m_rect.GetRight();};							// 获取矩形的最大x坐标
	double	GetBottom() const { return m_rect.GetBottom();};						// 获取矩形的最大y坐标

	double	GetLeft_Mask()	const { return m_rectMask.GetLeft();};					// 获取矩形的最小x坐标
	double	GetTop_Mask()	const { return m_rectMask.GetTop();};					// 获取矩形的最小y坐标
	double	GetRight_Mask()	const { return m_rectMask.GetRight();};					// 获取矩形的最大x坐标
	double	GetBottom_Mask() const { return m_rectMask.GetBottom();};				// 获取矩形的最大y坐标

	
	long GetPixelLeft() const { return m_rect.GetPixelLeft();};						// 获取矩形以长整型表示的最小x坐标
	long GetPixelTop() const { return m_rect.GetPixelTop();};						// 获取矩形以长整型表示的最小y坐标
	long GetPixelRight() const { return m_rect.GetPixelRight();};					// 获取矩形以长整型表示的最大x坐标
	long GetPixelBottom() const { return m_rect.GetPixelBottom();};					// 获取矩形以长整型表示的最大y坐标
	long GetPixelWidth() const { return m_rect.GetPixelWidth();};					// 获取矩形以长整型表示的宽度
	long GetPixelHeight() const { return m_rect.GetPixelHeight();};					// 获取矩形以长整型表示的高度


	long GetPixelLeft_Mask() const { return m_rectMask.GetPixelLeft();};			// 获取矩形以长整型表示的最小x坐标
	long GetPixelTop_Mask() const { return m_rectMask.GetPixelTop();};				// 获取矩形以长整型表示的最小y坐标
	long GetPixelRight_Mask() const { return m_rectMask.GetPixelRight();};			// 获取矩形以长整型表示的最大x坐标
	long GetPixelBottom_Mask() const { return m_rectMask.GetPixelBottom();};		// 获取矩形以长整型表示的最大y坐标
	long GetPixelWidth_Mask() const { return m_rectMask.GetPixelWidth();};			// 获取矩形以长整型表示的宽度
	long GetPixelHeight_Mask() const { return m_rectMask.GetPixelHeight();};		// 获取矩形以长整型表示的高度


	bool   IsNull() const															// 判断矩形是否为Null
	{ return sfRealEqual(GetWidth(),0.)||sfRealEqual(GetHeight(),0.);};


	scLineSeg GetLSeg() const;							// 获取矩形的左边线段
	scLineSeg GetRSeg() const;							// 获取矩形的右边线段
	scLineSeg GetTSeg() const;							// 获取矩形的上边线段
	scLineSeg GetBSeg() const;							// 获取矩形的下边线段

	scLineSeg GetLSeg_Mask() const;						// 获取矩形的左边线段
	scLineSeg GetRSeg_Mask() const;						// 获取矩形的右边线段
	scLineSeg GetTSeg_Mask() const;						// 获取矩形的上边线段
	scLineSeg GetBSeg_Mask() const;						// 获取矩形的下边线段


	scMaskRect Map(const sc2XformLinear& c) const;				// 对当前矩形进行二维变换(XY一致缩放平移但没有旋转和切变),得到新的矩形对象

	scMaskRect Intersect(const scRect& r) const;				// 计算给定矩形r与当前矩形的交
	scMaskRect operator&(const scRect& r) const					// 计算给定矩形r与当前矩形的交
	{return Intersect(r);};
	scMaskRect& operator&=(const scRect& r);					// 计算给定矩形r与当前矩形的交

	scMaskRect Enclose(const scRect& r) const;					// 计算同时包围给定图形r和当前矩形的最小外接矩形
	scMaskRect operator|(const scRect& r) const 				// 计算同时包围给定图形r和当前矩形的最小外接矩形
	{return Enclose(r);};
	scMaskRect& operator|=(const scRect& r);					// 计算同时包围给定图形r和当前矩形的最小外接矩形

	// 用给定的left、right、top、bottom值对当前矩形进行裁剪得到新的矩形
	scMaskRect Trim(double left, double right, double top, double bottom) const;

	scMaskRect Transpose () const;								// 对当前矩形进行转置变换得到新的矩形

	// 判断矩形是否退化，当矩形的宽高都为非正数时返回true
	bool Degenerate() const { return m_rect.Degenerate();} 

	bool operator==(const scMaskRect& rhs) const;				// 重载==操作符
	bool operator!=(const scMaskRect& rhs) const				// 重载!=操作符
	{ return !(*this == rhs);};

	
	ShapeType GetType() const{ return eMaskRect; };				// 返回类型的枚举值
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
	scRect	m_rect;
	scRect	m_rectMask;
};

#pragma pack(pop)