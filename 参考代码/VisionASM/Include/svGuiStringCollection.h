// svGuiStringCollection.h

// ---------------------------------------------------------------------
//                        字符串容器
// ---------------------------------------------------------------------

#pragma once

#include <afxtempl.h>
#include <vector>
#include<algorithm>
#include "svGuiDef.h"

#pragma pack(push,8)


class SVGUI_API scGuiStringCollection  
{
public:
	scGuiStringCollection();
	virtual ~scGuiStringCollection();

	BOOL	AddItem(LPCTSTR strNewItem, int nIndex = -1);	// 向集合中加入字符串
	CString GetItem(int nIndex) const;						// 获取集合中的第nIndex个字符串
	int		GetCount() const;								// 获取集合中字符串的个数

	int		FindItem(LPCTSTR strItem) const;				// 查找字符串在集合中的序号
	BOOL	Move(int nFromIndex, int nToIndex);				// 移动集合中的字符串:从nFromIndex到nToIndex

	BOOL	Remove(int nIndex);								// 从集合中删除第nIndex个字符串
	void	Clear();										// 清空集合中字符串


private:
	std::vector<CString> m_sl;
};

#pragma pack(pop)
