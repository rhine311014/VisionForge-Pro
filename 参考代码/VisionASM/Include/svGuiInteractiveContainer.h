// svGuiInteractiveContainer.h

// ---------------------------------------------------------------------
//                          交互图形控件容器
// ---------------------------------------------------------------------

#pragma once

#include <afxtempl.h>
#include "svGuiDef.h"
#include "svGuiGraphic.h"
#include "svGuiStringCollection.h"
#include "svGuiGraphicCollection.h"

#pragma pack(push,8)

class SVGUI_API scGuiInteractiveContainer  
{
public:
	scGuiInteractiveContainer();
	virtual ~scGuiInteractiveContainer();

	typedef CMap<CString, LPCTSTR, scGuiGraphic*, scGuiGraphic*> GraphicMap;

	//	向容器中加入交互图形指针，pGraphic：交互图形指针；lpszKey：交互图形关键字
	BOOL	AddItem(scGuiGraphic* pGraphic, LPCTSTR lpszKey);	 

	int		GetCount() const;									// 获取已加入容器的交互图形数量
	CString	GetItemKey(int nIndex) const;						// 获取Z轴顺序的第nIndex个交互图形对象关键字
	scGuiGraphic*	   GetItem(int nIndex) const;	// 获取Z轴顺序的第nIndex个交互图形对象指针	
	const scGuiStringCollection* GetZOrder()	const;			// 获取交互图形的Z轴顺序

	BOOL	MoveBackward(LPCTSTR lpszKey);						// 将交互图形的Z轴顺序向后移一位
	BOOL	MoveForward(LPCTSTR lpszKey);						// 将交互图形的Z轴顺序向前移一位
	BOOL	MoveToBack(LPCTSTR lpszKey);						// 将交互图形的Z轴顺序移到最后
	BOOL	MoveToFront(LPCTSTR lpszKey);						// 将交互图形的Z轴顺序移到最前

	BOOL	RemoveItem(LPCTSTR lpszKey);						// 将从容器中删除交互图形
	void	Clear();											// 清空容器中的交互图形

protected:
	scGuiStringCollection m_sc;
	scGuiGraphicCollection m_gm;
};

#pragma pack(pop)
