// svGuiObject.h

// ---------------------------------------------------------------------
//                        交互基类
// ---------------------------------------------------------------------

#pragma once

#include "svGuiDef.h"

#pragma pack(push,8)


class SVGUI_API scGuiObject
{
public:
	~scGuiObject();			// 析构函数

protected:
	scGuiObject();			// 构造函数	

protected:
	// 虚函数
	virtual void OnMouseDownEvent(UINT nFlags, CPoint point){};			// 鼠标按下
	virtual void OnMouseUpEvent(UINT nFlags, CPoint point){};			// 鼠标弹起
	virtual void OnMouseMoveEvent(UINT nFlags, CPoint point){};			// 鼠标移动
	virtual void OnClickedEvent(UINT nFlags, CPoint point){};			// 单击
	virtual void OnDblClickedEvent(UINT nFlags, CPoint point){};		// 双击

	virtual void OnSelectedEvent(BOOL bSelected = TRUE){};				// 选中或被放弃；有被选中的或有被放弃的；	
	virtual void OnDragStartEvent(){};									// 拖拽开始
	virtual void OnDragStopEvent(){};									// 拖拽停止

};

#pragma pack(pop)
