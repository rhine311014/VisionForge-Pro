// svObround.h

#pragma once

#include "svDef.h"
#include "svShape.h"
#include "svCircularArc.h"
#include "svLineSeg.h"
#include "svAffineRect.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            长圆形
// ---------------------------------------------------------------------
// 


class scObround;
smDerivedShapePtrhDecl(scObround);

class SVCORE_API scObround : public scShape  
{
public:
	scObround();														// 构造函数

	scObround (const sc2Vector& center, double xLen, double yLen,		// 用中心center，x边长xLen，y边长yLen，旋转角度xRotation和切变角skew构造仿射矩形
		const scRadian& xRotation = scRadian(0));

	scObround(double centerX,double centerY, 
		double xLen, double yLen, double xRotation);

	scObround (const scAffineRect& affrect);							// 用仿射矩形定义长圆形，默认skew=0

	void SetCenterLengthsRotation(const sc2Vector& center,			// 设置中心center，x边长xLen，y边长yLen，x边旋转角度xRotation和切变角skew
		double xLen, double yLen,
		const scRadian& xRotation = scRadian(0));

	void SetCenterLengthsRotation(double centerX,double centerY, 
		double xLen, double yLen, double xRotation);

	void GetCenterLengthsRotation(sc2Vector& center,				// 获取中心center，x边长xLen，y边长yLen，x边旋转角度xRotation和切变角skew
		double& xLen, double& yLen,	scRadian& xRotation);

	void SetCornerPoLengthsRotation(const sc2Vector& cornerPo,		// 设置原点cornerPo，x边长xLen，y边长yLen，x边旋转角度xRotation和切变角skew
		double xLen, double yLen,
		const scRadian& xRotation = scRadian(0));

	void SetCornerPoLengthsRotation(double cornerPoX,double cornerPoY, 
		double xLen, double yLen, double xRotation);

	void GetCornerPoLengthsRotation(sc2Vector& cornerPo,			// 获取原点cornerPo，x边长xLen，y边长yLen，x边旋转角度xRotation和切变角skew
		double xLen, double yLen, scRadian& xRotation);

	void SetAffineRect(const scAffineRect& affrect);				// 设置仿射矩形，默认认为skew=0
	scAffineRect GetAffineRect() const;									// 获取仿射矩形

	void SetCenter(const sc2Vector& center);							// 设置中心center
	void SetCenter(double centerX,double centerY);
	void SetCornerPo(const sc2Vector& po);								// 设置原点po
	void SetCornerPo(double poX,double poY);

	void SetXLength(double xLen);										// 设置x边长xLen
	void SetYLength(double yLen);										// 设置y边长yLen

	void SetRotation(const scRadian& rot);							// 设置x边旋转角度

	scCircularArc GetCircularArcXO() const;
	scCircularArc GetCircularArcYOPP() const;
	scLineSeg GetLineSegOY() const;
	scLineSeg GetLineSegOPPX() const;

	sc2Vector GetCornerPo() const;										// 获取原点
	sc2Vector GetCornerPx() const;										// 获取x角点
	sc2Vector GetCornerPy() const;										// 获取y角点
	sc2Vector GetCornerPopp() const;									// 获取对角度点

	sc2Vector GetCenter() const;										// 获取中心

	double GetXLength() const;											// 获取x边长
	double GetYLength() const;											// 获取y边长

	scRadian GetRotation() const;										// 获取x边的旋转角度

	bool Degenerate() const;											// 判断仿射矩形的参数是否有效

	bool operator==(const scObround& rhs) const;						// 重载==操作符
	bool operator!=(const scObround& rhs) const							// 重载!=操作符
	{ return !(*this == rhs);};				

	virtual ShapeType GetType() const{ return eObround; };				// 返回类型的枚举值
	scShapePtrh Clone() const;											// 复制当前仿射矩形
	bool IsOpenContour() const;											// 是否为开轮廓，总是返回false
	bool IsRegion() const;												// 是否为区域，总是返回true
	bool IsFinite() const;												// 是否为有限的，总是返回true
	bool IsEmpty() const;												// 是否为空，总是返回false

	scRect BoundingBox() const;											// 获取当前仿射矩形的最小外接矩形
	
	sc2Vector NearestPoint(const sc2Vector& p) const;					// 计算当前仿射矩形上距离给定点p最近的点
	
	bool Within(const sc2Vector& p) const;								// 判断给定点是否在当前仿射矩形区域内

	double Perimeter();

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;		// 获得掩模图像

private:
	bool IsEqual(const scShape& rhs) const;								// 判断给定图形rhs是否与当前图形相等，仅供内部使用

private:

	void	SetUnitSq();

	sc2Vector	m_vPc;												// 中心点
	double		m_dXLen, m_dYLen;										// x边长，y边长
	scRadian	m_rXRot, m_rYRot;										// x边旋转角度（相对于X轴），y边旋转角度（相对于Y轴）
	sc2Vector	m_vPo, m_vPx, m_vPy, m_vPopp;							// Po点，Px点，Py点，Popp点

	sc2XformLinear m_xAffineRectFromUS;									// 从单位正方形到仿射矩形的变换
};

#pragma pack(pop)
