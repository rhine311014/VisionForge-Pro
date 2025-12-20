// svGuiEllipseAnnulus.h

// ---------------------------------------------------------------------
//                           椭圆环交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svEllipseAnnulus.h"

#pragma pack(push,8)


class SVGUI_API scGuiEllipseAnnulus  : public scGuiGraphic
{
public:
	scGuiEllipseAnnulus();
	virtual ~scGuiEllipseAnnulus();

	virtual GraphicType GetType() const {return eGuiEllipseAnnulus;};

	////////////////////////////////////////////////////////////
	//		         *  *  *   
	//	        *	            *
	//                 ***       
	//    *        *         *        *
	//   *       *              *      *
	//  *       *                *      *
	//  +RadiusX+      *center   *      + Rotation
	//  *       *                *      *
	//   *       *              *      *
	//    *        *         *        *
	//                 *+*     
	//          *               * 
	//               *  +  *
	//                   RadiusY
	/////////////////////////////////////////////////////////
	enum TrackerHit
	{
		hitNothing		   = -1, 
		hitFiducialRadiusX =  0,	// 基准X方向半轴调节点
		hitFiducialRadiusY =  1,	// 基准Y方向半轴调节点
		hitOtherRadiusX	   =  2,	// 另外X方向半轴调节点
		hitOtherRadiusY	   =  3,	// 另外Y方向半轴调节点
		hitRotation		   =  4,	// 旋转调节点
		//十字中心X边、十字中心Y边
		hitCenterXSeg = 5,
		hitCenterYSeg = 6,
		hitEllipseAnnulus  =  7		// 椭圆环							
	};

	// 自由度
	enum GuiEllipseAnnulusDOF
	{
		eGuiDOFNone	 	        = 0,	// 禁止所有的自由度
		eGuiDOFFiducialSizeX	= 1,	// 可改变基准X轴长度
		eGuiDOFFiducialSizeY    = 2,    // 可改变基准Y轴长度
		eGuiDOFOtherSizeX       = 4,    // 可改变另外X轴长度
        eGuiDOFOtherSizeY       = 8,    // 可改变另外Y轴长度
		eGuiDOFRotation         = 16,   // 可改变旋转角度
		eGuiDOFPosition	        = 32,	// 可改变位置		
		eGuiDOFAll		= eGuiDOFFiducialSizeX|eGuiDOFFiducialSizeY|eGuiDOFOtherSizeX|eGuiDOFOtherSizeY
		                    |eGuiDOFRotation|eGuiDOFPosition, // 所有的自由度
	};


public:

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition）
	void EnableGuiDOF(DWORD dof);

	// 设置椭圆环参数
	void SetCenterRadiiRotationRadialScale(double dCenterX,			// 椭圆环中心的X坐标
											double dCenterY,		// 椭圆环中心的Y坐标
											double dRadiusX,		// 椭圆环基准圆的X方向半轴
											double dRadiusY,		// 椭圆环基准圆的Y方向半轴
											double dRotation,		// 椭圆环的旋转角度
											double dRadialScale);	// 径向缩放比例	

	// 获取椭圆环参数
	void GetCenterRadiiRotationRadialScale(double& dCenterX,		// 椭圆环中心的X坐标
											double& dCenterY,		// 椭圆环中心的Y坐标
											double& dRadiusX,		// 椭圆环基准圆的X方向半轴
											double& dRadiusY,		// 椭圆环基准圆的Y方向半轴
											double& dRotation,		// 椭圆环的旋转角度
											double& dRadialScale)const;	// 径向缩放比例		

	// 设置椭圆环参数
	void SetCenterRadiiRotationRadialScale(const sc2Vector& vCenter,// 椭圆环中心坐标
											double dRadiusX,		// 椭圆环基准圆的X方向半轴
											double dRadiusY,		// 椭圆环基准圆的Y方向半轴
											const scRadian& rRotation,// 椭圆环的旋转角度
											double dRadialScale);	// 径向缩放比例	

	// 获取椭圆环参数
	void GetCenterRadiiRotationRadialScale( sc2Vector& vCenter,		// 椭圆环中心坐标
											double dRadiusX,		// 椭圆环基准圆的X方向半轴
											double dRadiusY,		// 椭圆环基准圆的Y方向半轴
											scRadian& rRotation,	// 椭圆环的旋转角度
											double dRadialScale)const;// 径向缩放比例

	void SetEllipseAnnulus(const scEllipseAnnulus& ellipseAnnulus);	// 设置椭圆环
	scEllipseAnnulus GetEllipseAnnulus() const;						// 获取椭圆环


	void SetCenterCrossExtent(const sc2Vector& vSize = sc2Vector(16,16));	// 设置中心十字线尺寸（宽度和高度）
	sc2Vector GetetCenterCrossExtent() const;								// 获取中心十字线尺寸（宽度和高度）

	virtual BOOL BoundingRect(CRect& rect) const;

	CString  GetTipText() const;
	CString	 GetTipText(int nHandle) const;
	CString	 GetTipText(CPoint point) const;

	BOOL Transform(double dScale, double dOffsetX, double dOffsetY);

	void Draw(CDC* pDC, LPRECT lpRect = NULL);
	void DrawLabel(CDC* pDC, LPRECT lpRect = NULL);
	int  HitTest(CPoint point) const;
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;

	BOOL DragStart(CPoint point);
	void DragAnimate(CPoint ptStart, CPoint ptStop);
	void DragStop();
	void DragCancel();

	void	SetFillEnabled(BOOL bEnable);								// 设置是否允许填充
	BOOL	IsFillEnabled() const;										// 获取是否允许填充

	void    SetFillColor( COLORREF color);								// 设置填充的颜色
	COLORREF GetFillColor() const;										// 获取填充的颜色

	// helper for Implementation
protected:	
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawEllipse(CDC* pDC, scEllipse* pEllipse) const;

	int   HitTestHandles(CPoint point) const;
	CRect GetHandleRect(TrackerHit nHandle) const;
	void  LoadCursor();
	void  InitTipText();

protected:

	scEllipseAnnulus* m_pEllipseAnnulus;
	scEllipseAnnulus* m_pEllipseAnnulusDrag;
	TrackerHit	m_hitResult;
	CPoint		m_ptCur;
	static HCURSOR g_hCursor[8];

	DWORD		    m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			// 
};

#pragma pack(pop)
