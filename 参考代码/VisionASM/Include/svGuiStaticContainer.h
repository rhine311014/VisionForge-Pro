// svGuiStaticContainer.h: interface for the scGuiStaticContainer class.

// ---------------------------------------------------------------------
//                        静态交互图形容器
// ---------------------------------------------------------------------

#pragma once

#include <afxtempl.h>
#include "svGuiDef.h"
#include "svGuiGraphic.h"
#include "svGuiStringCollection.h"
#include "svGuiGraphicCollection.h"

#pragma pack(push,8)

class SVGUI_API scGuiStaticContainer  
{
public:
	scGuiStaticContainer();
	virtual ~scGuiStaticContainer();

	typedef CMap<CString, LPCTSTR,scGuiGraphic*, scGuiGraphic*> GraphicMap;

	//	向容器中加入静态图形指针，pGraphic：静态图形指针；lpszKey：静态图形关键字
	BOOL	AddItem(scGuiGraphic* pGraphic, LPCTSTR lpszKey);

	int		GetCount() const;								// 获取已加入容器的静态图形数量
	CString	GetItemKey(int nIndex) const;					// 获取Z轴顺序的第nIndex个静态图形对象关键字
	scGuiGraphic*	GetItem(int nIndex) const;				// 获取静态图形对象指针
	const scGuiStringCollection* GetZOrder() const;		// 获取静态图形的Z轴顺序

	BOOL	MoveBackward(LPCTSTR lpszKey);					// 将静态图形的Z轴顺序向后移一位
	BOOL	MoveForward(LPCTSTR lpszKey);					// 将静态图形的Z轴顺序向前移一位
	BOOL	MoveToBack(LPCTSTR lpszKey);					// 将静态图形的Z轴顺序移到最后
	BOOL	MoveToFront(LPCTSTR lpszKey);					// 将静态图形的Z轴顺序移到最前

	BOOL	RemoveItem(LPCTSTR lpszKey);					// 将静态图形从容器中删除
	void	Clear();										// 从容器中删除所有静态图像

protected:
	scGuiStringCollection m_sc;
	scGuiGraphicCollection m_gm;
};

#pragma pack(pop)
