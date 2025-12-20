// svGuiGraphicCollection.h

// ---------------------------------------------------------------------
//                        图形容器
// ---------------------------------------------------------------------

#pragma once

#include <afxtempl.h>
#include <vector>
#include<algorithm>
#include "svGuiDef.h"
#include "svGuiGraphic.h"

#pragma pack(push,8)


class SVGUI_API scGuiGraphicCollection  
{
public:
	scGuiGraphicCollection();
	virtual ~scGuiGraphicCollection();

	BOOL	AddItem(scGuiGraphic* pNewItem, int nIndex = -1);	// 向集合中加入图形
	scGuiGraphic* GetItem(int nIndex) const;					// 获取集合中的第nIndex个图形
	int		GetCount() const;									// 获取集合中图形的个数

	int		FindItem(scGuiGraphic* pItem) const;				// 查找图形在集合中的序号
	BOOL	Move(int nFromIndex, int nToIndex);					// 移动集合中的图形:从nFromIndex到nToIndex

	BOOL	Remove(int nIndex);									// 从集合中删除第nIndex个图形
	void	Clear();											// 清空集合中图形


private:
	std::vector<scGuiGraphic*> m_sl;
};

#pragma pack(pop)
