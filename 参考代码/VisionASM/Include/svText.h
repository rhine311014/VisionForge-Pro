// svText.h

#pragma once

#include "svShape.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            点（二维）
// ---------------------------------------------------------------------
// 矩形类scText用来表示二维平面上的点,派生自图形基类scShape

class scText;
smDerivedShapePtrhDecl(scText);

class SVCORE_API scText : public scShape
{
public:
	
	// 构造函数
	scText();			
	
	// 析构函数						
	virtual ~scText();

	scText(const scText& other);

	scText(double dX, double dY, double dRotation, LPCTSTR lpszText);

	scText(const sc2Vector& pos, const scRadian& rot, LPCTSTR lpszText);

	scText(double dX, double dY, double dRotation, LPCTSTR lpszText, LOGFONT font);

	scText(const sc2Vector& pos, const scRadian& rot, LPCTSTR lpszText, LOGFONT font);
	
	void SetXYRotationText(double dX,					// 文本输出位置的X坐标
		double dY,					// 文本输出位置的Y坐标
		double dRotation,			// 文本输出的旋转角度(单位弧度)
		LPCTSTR lpszText);			// 文本

	void SetXYRotation(double dX,						// 文本输出位置的X坐标
		double dY,						// 文本输出位置的Y坐标
		double dRotation);				// 文本输出的旋转角度(单位弧度)

	// 设置文本输出位置pos，旋转角度rot和文本lpszText
	void SetXYRotationText(const sc2Vector& pos, const scRadian& rot, LPCTSTR lpszText);

	// 设置文本输出位置pos，旋转角度rot
	void SetXYRotation(const sc2Vector& pos, const scRadian& rot);

	void GetXYRotation(double& dX,						// 文本输出位置的X坐标
		double& dY,						// 文本输出位置的Y坐标
		double& dRotation) const;		// 文本输出的旋转角度

	void GetXYRotation(sc2Vector& pos, scRadian& rot) const;// 文本输出的位置和旋转角度

	sc2Vector GetPos() const;							// 获取位置
	scRadian  GetRotation() const;						// 获取旋转角度

	void	SetText(LPCTSTR lpszText);					// 设置文本
	LPCTSTR GetText() const;							// 获取文本

	void	 SetTextFont(LOGFONT font);				// 设置文本的字体
	LOGFONT  GetTextFont() const;								// 获取文本的字体

	// 对当前点进行二维变换得到新的点
	scText Map(const sc2XformLinear& c);		

	bool operator==(const scText& rhs) const;			// 重载==操作符
	bool operator!=(const scText& rhs) const			// 重载!=操作符
	{return !(*this == rhs);}
	
	virtual ShapeType GetType() const{ return eText; };	// 返回类型的枚举值
	scShapePtrh Clone() const;							// 复制得到新的图形对象
	bool IsOpenContour() const;							// 是否为开的轮廓，总是返回TRUE
	bool IsRegion() const;								// 是否为区域，总是返回FALSE
	bool IsFinite() const;								// 是否为有限的，总是返回TRUE
	bool IsEmpty() const;								// 是否为空，总是返回FALSE

	// 获取最小外接矩形，总是返回一个尺寸为(0,0)的矩形
	scRect BoundingBox() const;			

	// 计算离二维向量p最近的点，总是返回当前点
	sc2Vector NearestPoint(const sc2Vector &p) const;	

	// 获取边长，总是返回0.0
	double Perimeter() const;
	
	// 对当前点进行二维变换得到新的点
//	scShapePtrh MapShape(const sc2XformLinear& X) const; 

	scText& operator=(const scText& other);

private:
	bool IsEqual(const scShape& rhs) const;				// 判断给定图形rhs是否与当前图形相等，仅供内部使用

private:
	void*	 m_pLShape;
	LOGFONT	 m_fontText;
};

#pragma pack(pop)
