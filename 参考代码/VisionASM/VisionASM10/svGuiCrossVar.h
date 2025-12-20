// svGuiCrossVar.h
// ---------------------------------------------------------------------
//                           十字可编辑标记交点Gui控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svGuiDisplay.h"
#include "svGuiLineSeg.h"
#include "svIntersect.h"
#include "svLineSeg.h"
#include "svGuiCoordCross.h"
#include "svGuiText.h"

#pragma pack(push,8)

/////////////////////////////////////////////////////////////////////////////
// Handles:
//					top
//					 *
//					 |
//			  left*--*--*right
//					 |center
//					 *
//				  bottom
//
/////////////////////////////////////////////////////////////////////////////

class svGuiCrossVar {

public:
	svGuiCrossVar();
	virtual ~svGuiCrossVar();

	bool SetDisplay(scGuiDisplay *pGuiDisplay);					// 设置显示控件指针	
	void GuiCrossToLineseg();									// 将交点十字转换成直线

	//直线拖动、拉伸后的交点判断
	void Dragging();
	
	//设置文本显示
	void SetText(double dX,					// 文本输出位置的X坐标
				 double dY,					// 文本输出位置的Y坐标
				 double dRotation,			// 文本输出的旋转角度(单位弧度)
				LPCTSTR lpszText);			// 文本

	//固定显示文本
	void SetFixText(LPCTSTR lpszText);

	void SetCenterXYWidthHeight(double dCenterX,				// 中心点的X坐标
		double dCenterY,										// 中心点的Y坐标
		double dWidth  = 16,									// 宽度
		double dHeight = 16);									// 高度

	void SetCrossCenter(const sc2Vector& vCenter);				// 设置中心点坐标值

	void GetCenterXY(double& dCenterX,double& dCenterY)const;	// 获取中心点的X坐标和Y坐标
	sc2Vector GetCenter() const;								// 获取中心点

public:
	scGuiDisplay* m_vpGuiDisplay;								// 显示图形display
	scGuiInteractiveContainer*	m_vpGuiInteractiveContainer;	

	scGuiCoordCross*			m_vGuiCoorCross;				   // 交点显示
	scGuiText*					m_pGuiTextShow;

protected:
	BOOL m_bVisionVerExLine;
	BOOL m_bVisionHorExLine;

	sc2Vector		m_vCenter;				// 
	sc2Vector		m_vTop;					// 
	sc2Vector		m_vBottom;				// 
	sc2Vector		m_vLeft;				// 
	sc2Vector		m_vRight;				// 

	scLineSeg	m_vLineSegVer;
	scLineSeg	m_vLineSegHor;

	scGuiLineSeg* m_vGuiLineSegVer;								// 竖直线段显示
	scGuiLineSeg* m_vGuiLineSegHor;								// 水平线段显示
	scGuiLineSeg* m_vGuiLineSegVerEx;							// 竖直延长线段显示
	scGuiLineSeg* m_vGuiLineSegHorEx;							// 水平延长线段显示

	//转换前设置位置状态
	void CrossMoveSet(sc2Vector scCenter, double dWidth, double dHeight,double dAngel);
	void CrossMoveSet90(sc2Vector scCenter, double dWidth, double dHeight,double dAngel);

	//得到线段交点及延长线
	bool GetLineCross();
	//延长线与线段的接点的判断
	sc2Vector GetLinesegContinePos(sc2Vector scPoint,scLineSeg scCheckLine);
};

#pragma pack(pop)