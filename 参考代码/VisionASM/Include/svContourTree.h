// svContourTree.h

#pragma once

#include "svShapeTree.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                           轮廓图形树
// ---------------------------------------------------------------------
// 轮廓图形树scContourTree是保持相连接图形（开轮廓的）层级的一个矩形实现类,
// 这些相连接图形所组成的图形可以使开的也可以使闭合的。通常基类scShape的方法
// 把这个组合图形当做一个单独轮廓进行操作，就像对待其他基本的轮廓。
//
// 轮廓图形树的子图形可以基本图形也可以图形树，但必须都是开轮廓的。
// 目前唯一一种可以作为轮廓树子图形的非基本图形就是开的轮廓图形树。
//
// 轮廓图形树种基本图形的顺序就是连接的顺序。如果两个图形在链表中是相邻的，
// 则它们就是相连接的。轮廓树认为一个子图形的终点基本上就是下一个子图形的起点


class scContourTree;
smDerivedPtrHdlDcl(scContourTreePtrh,scContourTree,scShapeTreePtrh,scShapeTreePtrh_const);


class SVCORE_API scContourTree : public scShapeTree
{
public:

	// 显示构造函数，构造一个不含子图形的轮廓图形树
	// 如果bClosed为true则构造闭合的轮廓树，否则构造开的轮廓树
	explicit scContourTree(bool bClosed = false);
	// effect      Constructs an open ccContourTree containing no
	//             shapes with given closed-ness.

	// 拷贝构造函数和析构函数都是默认的

	// 拷贝构造函数是浅拷贝,只是拷贝了指针句柄.
	// 因此通过拷贝构造函数构造的图形树,会与给定图形树共享孩子图形

	// 返回当前轮廓树所表示图形是否为闭合的
	// 如果是闭合的则返回true，否则返回false
	bool IsClosed() const;		

	// 设置当前轮廓树所表示图形是否为闭合的
	void IsClosed(bool bClosed);							

	bool operator==(const scContourTree &rhs) const;	// 重载==操作符
	bool operator!=(const scContourTree &rhs) const		// 重载!=操作符
	{ return !(*this == rhs);}


	// 重载scShapeTree的方法
	////////////////////////////////////////////////////////////////
	
	// 在第idx位置插入(拷贝插入)新的孩子图形child;
	// idx应该满足idx>=0且idx<=GetChildrenNum条件,否则插入会不成功
	// 如果child不是开轮廓，则插入会不成功;
	// 如果插入成功则返回true,否则返回false
	virtual bool InsertChild(long idx, const scShape& child);			

	// 在第idx位置插入新的孩子图形child;
	// 若direct==true表示直接插入,否则为拷贝插入;	
	// idx应该满足idx>=0且idx<=GetChildrenNum条件,否则插入会不成功
	// 如果child不是开轮廓，则插入会不成功;
	// 如果插入成功则返回true,否则返回false
	virtual bool InsertChild(long idx, const scShapePtrh_const& child,   
		bool direct = false);

	// 在第idx位置插入一组图形children作为孩子;
	// 若direct==true表示直接插入,否则为拷贝插入;	
	// idx应该满足idx>=0且idx<=GetChildrenNum条件,否则插入会不成功;
	// 如果children中的图形不是开轮廓，则插入会不成功;
	// 如果插入成功则返回true,否则返回false;
	virtual bool InsertChildren(long idx,									
		const svStd vector<scShapePtrh_const> &children,	
		bool direct = false);

	// 在图形树的末尾添加(拷贝)新的图形child作为孩子;
	// 如果child不是开轮廓，则添加会不成功;
	// 如果添加成功则返回true,否则返回false;
	virtual bool AddChild(const scShape &child);								

	// 在图形树的末尾添加新的图形child作为孩子;
	// 若direct为true表示直接添加,否则表示拷贝添加;
	// 如果child不是开轮廓，则添加会不成功;
	// 如果添加成功则返回true,否则返回false;
	virtual bool AddChild(const scShapePtrh_const &child, bool direct = false); 

	// 在图形树的末尾添加一组图形children作为孩子;
	// 若direct为true表示直接添加,否则表示拷贝添加;
	// 如果children中的图形不是开轮廓，则添加会不成功;
	// 如果添加成功则返回true,否则返回false;
	virtual bool AddChildren(const svStd vector<scShapePtrh_const> &children,			
		bool direct = false);

	// 用图形child替换（拷贝）图形树中的第idx个子图形
	// idx应该满足idx>=0且idx<GetChildrenNum条件,否则替换会不成功;
	// 如果child不是开轮廓，则替换会不成功
	// 如果成功则返回true,否则返回false;
	virtual bool ReplaceChild(long idx, const scShape &child);		

	// 用图形child替换图形树中的第idx个子图形;
	// 若direct为true表示直接替换,否则表示拷贝替换；
	// idx应该满足idx>=0且idx<GetChildrenNum条件,否则替换会不成功;
	// 如果child不是开轮廓，则替换会不成功;
	// 如果成功则返回true,否则返回false;
	virtual bool ReplaceChild(long idx, const scShapePtrh_const &child,			
		bool direct = false);

	// 用一组图形children替换图形树中图形;
	// 若direct为true表示直接替换,否则表示拷贝替换;
	// 如果children中的图形不是开轮廓，则替换会不成功;
	// 如果成功则返回true,否则返回false;
	virtual bool ReplaceChildren(const svStd vector<scShapePtrh_const> &children,		
		bool direct = false);


	// 重载scShape的方法
	////////////////////////////////////////////////////////////

	// 返回类型的枚举值
	virtual ShapeType GetType() const{ return eContourTree; };	

	// 复制当前的轮廓图形树结构,得到新的轮廓图形树
	scShapePtrh Clone() const;					

	// 只有当轮廓树是开的，并且至少包含一个基本图形时，才返回true
	bool IsOpenContour() const;					

	// 是否为区域
	// 只有当轮廓树是闭合的且非空的才返回true
	bool IsRegion() const;

	// 是否为有限的,总是返回true
	bool IsFinite() const;						

	// 计算离给定点p最近的点
	sc2Vector NearestPoint(const sc2Vector& p) const;	

	// 判断点p是否在图形内部
//	bool Within(const sc2Vector& p) const;

	bool IsEqual(const scShape& rhs) const;

private:

	bool m_bClosed;

};

#pragma pack(pop)
