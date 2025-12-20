// svMaskAffineRect.h

#pragma once

#include "svDef.h"
#include "svShape.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            仿射矩形
// ---------------------------------------------------------------------
// 

class scAffineRect;
class scMaskAffineRect;
smDerivedShapePtrhDecl(scMaskAffineRect);

class SVCORE_API scMaskAffineRect : public scShape  
{
public:
	scMaskAffineRect();														// 构造函数

	scMaskAffineRect (const sc2Vector& center, double xLen, double yLen,	// 用中心center，x边长xLen，y边长yLen，旋转角度xRotation和切变角skew构造仿射矩形
		const sc2Vector& center_mask, double xLen_mask, double yLen_mask,
		const scRadian& xRotation = scRadian(0),
		const scRadian& skew      = scRadian(0));

	scMaskAffineRect(double centerX,double centerY, 
		double xLen, double yLen,
		double center_maskX,double center_maskY, 
		double xLen_mask, double  yLen_mask,
		double xRotation ,double skew);

	void SetCenterLengthsRotAndSkew(const sc2Vector& center,			// 设置中心center，x边长xLen，y边长yLen，x边旋转角度xRotation和切变角skew
		double xLen, double yLen,
		const sc2Vector& center_mask,
		double xLen_mask, double yLen_mask,
		const scRadian& xRotation = scRadian(0),
		const scRadian& skew      = scRadian(0));

	void SetCenterLengthsRotAndSkew(double centerX,double centerY, 
		double xLen, double yLen,
		double center_maskX,double center_maskY, 
		double xLen_mask, double yLen_mask,
		double xRotation ,double skew);

	void SetCornerPoLengthsRotAndSkew(const sc2Vector& cornerPo,		// 设置原点cornerPo，x边长xLen，y边长yLen，x边旋转角度xRotation和切变角skew
		double xLen, double yLen,
		const sc2Vector& cornerPo_mask,
		double xLen_mask, double yLen_mask,
		const scRadian& xRotation = scRadian(0),
		const scRadian& skew      = scRadian(0));

	void SetCornerPoLengthsRotAndSkew(double cornerPoX,double cornerPoY, 
		double xLen, double yLen,
		double cornerPo_maskX,double cornerPo_maskY, 
		double xLen_mask, double yLen_mask,
		double xRotation ,double skew);

	void SetCenter(const sc2Vector& center);							// 设置中心center
	void SetCenter(double centerX,double centerY);
	void SetCornerPo(const sc2Vector& po);								// 设置原点po
	void SetCornerPo(double posX,double posY);

	void SetXLength(double xLen);										// 设置x边长xLen
	void SetYLength(double yLen);										// 设置y边长yLen

	void SetCenterMask(const sc2Vector& center);						// 设置中心center
	void SetCenterMask(double centerX,double centerY);
	void SetCornerPoMask(const sc2Vector& po);							// 设置原点po
	void SetCornerPoMask(double posX,double posY);

	void SetXLengthMask(double xLen);									// 设置x边长xLen
	void SetYLengthMask(double yLen);									// 设置y边长yLen

	void SetXRotation(const scRadian& xRot);							// 设置x边旋转角度

	void SetSkew(const scRadian& skew);									// 设置切变角度skew


	const sc2XformLinear& GetMaskAffineRectFromUnitSq () const;				// 获取仿射变换
	
	scAffineRect GetAffineRect() const;
	scAffineRect GetAffineRectMask() const;

	sc2Vector GetCornerPo() const;										// 获取原点
	sc2Vector GetCornerPx() const;										// 获取x角点
	sc2Vector GetCornerPy() const;										// 获取y角点
	sc2Vector GetCornerPopp() const;									// 获取对角度点

	sc2Vector GetCenter() const;										// 获取中心
	sc2Vector GetSize() const;											// 获取尺寸

	double GetXLength() const;											// 获取x边长
	double GetYLength() const;											// 获取y边长

	sc2Vector GetCornerPoMask() const;										// 获取原点
	sc2Vector GetCornerPxMask() const;										// 获取x角点
	sc2Vector GetCornerPyMask() const;										// 获取y角点
	sc2Vector GetCornerPoppMask() const;									// 获取对角度点

	sc2Vector GetCenterMask() const;										// 获取中心
	sc2Vector GetSizeMask() const;											// 获取大小

	double GetXLengthMask() const;											// 获取x边长
	double GetYLengthMask() const;											// 获取y边长


	scRadian GetXRotation() const;										// 获取x边的旋转角度
	scRadian GetYRotation() const;										// 获取y边的旋转角度
	scRadian GetSkew() const;											// 获取切变角度

	scMaskAffineRect Map(const sc2XformLinear& c) const;				// 对当前图形进行sc2Xform变换c，得到新的仿射矩形

	bool Degenerate() const;											// 判断仿射矩形的参数是否有效

	static bool IsBadSkew(const scRadian& skew);						// 判断切变角是否有效

	bool operator==(const scMaskAffineRect& rhs) const;						// 重载==操作符
	bool operator!=(const scMaskAffineRect& rhs) const						// 重载!=操作符
	{ return !(*this == rhs);};				

	virtual ShapeType GetType() const{ return eMaskAffineRect; };				// 返回类型的枚举值
	scShapePtrh Clone() const;											// 复制当前仿射矩形
	bool IsOpenContour() const;											// 是否为开轮廓，总是返回false
	bool IsRegion() const;												// 是否为区域，总是返回true
	bool IsFinite() const;												// 是否为有限的，总是返回true
	bool IsEmpty() const;												// 是否为空，总是返回false

	scRect BoundingBox() const;											// 获取当前仿射矩形的最小外接矩形
	
	sc2Vector NearestPoint(const sc2Vector& p) const;					// 计算当前仿射矩形上距离给定点p最近的点
	
	bool Within(const sc2Vector& p) const;								// 判断给定点是否在当前仿射矩形区域内

	double Perimeter();

	void Normalize();

	bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;					// 获得掩模图像

private:
	bool IsEqual(const scShape& rhs) const;								// 判断给定图形rhs是否与当前图形相等，仅供内部使用

private:
	void	SetUnitSq();
	void	ClipMask();

	sc2Vector	m_vPc;													// 中心点
	double		m_dXLen, m_dYLen;										// x边长，y边长
	sc2Vector	m_vPc_Mask;												// 中心点
	double		m_dXLen_Mask, m_dYLen_Mask;								// x边长，y边长
	scRadian	m_rXRot, m_rYRot;										// x边旋转角度（相对于X轴），y边旋转角度（相对于Y轴）
	sc2Vector	m_vPo, m_vPx, m_vPy, m_vPopp;							// Po点，Px点，Py点，Popp点
	sc2Vector	m_vPo_Mask, m_vPx_Mask, m_vPy_Mask, m_vPopp_Mask;		// Po点，Px点，Py点，Popp点

	sc2XformLinear m_xAffineRectFromUS;								// 从单位正方形到仿射矩形的变换
};

#pragma pack(pop)
