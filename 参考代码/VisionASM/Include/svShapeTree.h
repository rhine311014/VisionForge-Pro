// svShapeTree.h

#pragma once

#include "svShape.h"
#include <vector>

#pragma pack(push,8)


// ---------------------------------------------------------------------
//                            图形树基类
// ---------------------------------------------------------------------
// 图形树可以用来表示派生自图形基类scShape的图形构成的树状结构。图形树不
// 支持环形结构，在程序中也不对此进行检查。图形树基类scShapeTree也是派生自
// 图形基类scShape,同时它又是一般图形树scGenShapeTree、轮廓图形树scContourTree
// 和区域图形树scRegionTree的基类。

class scShapeTree;
smDerivedShapePtrhDecl(scShapeTree);


class SVCORE_API scShapeTree : public scShape
{

public:
	// 构造函数，构造没有子图形的图形树
	scShapeTree();										

	// 拷贝构造函数和析构函数都是默认的

	// 拷贝构造函数是浅拷贝,只是拷贝了指针句柄.
	// 因此通过拷贝构造函数构造的图形树,会与给定图形树共享孩子图形


	// 返回图形树的孩子数量
	long GetChildrenNum() const;							

	// 返回图形树是否为叶子结点
	bool IsLeaf() const;								

	// 根据给定值primitivesOnly,返回图形树后代的数量;
	// 如果primitivesOnly为true,则只返回图形树中基本图形的数量,否则返回图形树所有结点的数量.
	// 基本图形如圆ccCircle、矩形等.
	long GetSize(bool primitivesOnly = false) const;		

	// 返回图形树的深度
	// 从根结点到叶子结点最长路径的长度,叶子结点的深度为0
	long GetHeight() const;									

	// 返回图形树的第idx个孩子结点;
	// idx应该满足idx>=0且idx<GetChildrenNum条件,
	// 否则,返回值会不正常,在调试版时会报断言错误
	const scShape& GetChild(long idx) const;				

	// 返回图形树所有的孩子图形
	const svStd vector<scShapePtrh_const>& GetChildren() const;	

	// 在第idx位置插入(拷贝插入)新的孩子图形child;
	// 如果插入成功则返回true,否则返回false
	// idx应该满足idx>=0且idx<=GetChildrenNum条件,否则插入会不成功
	virtual bool InsertChild(long idx, const scShape& child);			
	
	// 在第idx位置插入新的孩子图形child;
	// 若direct==true表示直接插入,否则为拷贝插入;
	// 如果插入成功则返回true,否则返回false
	// idx应该满足idx>=0且idx<=GetChildrenNum条件,否则插入会不成功
	virtual bool InsertChild(long idx, const scShapePtrh_const& child,   
		bool direct = false);

	// 在第idx位置插入一组图形children作为孩子;
	// 若direct==true表示直接插入,否则为拷贝插入;
	// 如果插入成功则返回true,否则返回false;
	// idx应该满足idx>=0且idx<=GetChildrenNum条件,否则插入会不成功;
	virtual bool InsertChildren(long idx,									
		const svStd vector<scShapePtrh_const> &children,	
		bool direct = false);

	// 在图形树的末尾添加(拷贝)新的图形child作为孩子;
	// 总是返回true;
	virtual bool AddChild(const scShape &child);								
	
	// 在图形树的末尾添加新的图形child作为孩子;
	// 若direct为true表示直接添加,否则表示拷贝添加
	// 总是返回true;
	virtual bool AddChild(const scShapePtrh_const &child, bool direct = false); 

	// 在图形树的末尾添加一组图形children作为孩子;
	// 若direct为true表示直接添加,否则表示拷贝添加;
	// 总是返回true;
	virtual bool AddChildren(const svStd vector<scShapePtrh_const> &children,			
		bool direct = false);
	
	// 用图形child替换（拷贝）图形树中的第idx个子图形
	// idx应该满足idx>=0且idx<GetChildrenNum条件,否则替换会不成功;
	// 如果成功则返回true,否则返回false;
	virtual bool ReplaceChild(long idx, const scShape &child);		

	// 用图形child替换图形树中的第idx个子图形;
	// 若direct为true表示直接替换,否则表示拷贝替换；
	// idx应该满足idx>=0且idx<GetChildrenNum条件,否则替换会不成功;
	// 如果成功则返回true,否则返回false;
	virtual bool ReplaceChild(long idx, const scShapePtrh_const &child,			
		bool direct = false);

	// 用一组图形children替换图形树中图形;
	// 若direct为true表示直接替换,否则表示拷贝替换；
	// 总是返回true;
	virtual bool ReplaceChildren(const svStd vector<scShapePtrh_const> &children,		
		bool direct = false);

	// 删除图形树的第idx个子图形
	// idx应该满足idx>=0且idx<GetChildrenNum条件,否则删除会不成功;
	// 如果成功则返回true,否则返回false;
	virtual bool RemoveChild(long idx);											

	// 删除图形树中的所有子图形
	virtual void RemoveChildren();												

//	scShapeTreePtrh Flatten(bool preserveContours = false) const;				// 展开图形树

	bool operator==(const scShapeTree& rhs) const;								// 重载==操作符

	bool operator!=(const scShapeTree& rhs) const								// 重载!=操作符
	{ return !(*this == rhs);}

	////////////////////////////////////////////////////////////
	// 重载基类ccShape的方法

	// 返回类型的枚举值
	ShapeType GetType() const{ return eShapeTree; };		

	// 返回是否为空;
	// 如果图形树中所有基本图形都为空则返回true,否则返回false
	bool IsEmpty() const;											

	// 获取图形树所表示图形的最小外接矩形
	scRect BoundingBox() const;									

//	double Perimeter() const;													// 获取图形树所表示图形的边缘长


protected:
	scShapeTree& operator=(const scShapeTree &rhs);

private:

	svStd vector<scShapePtrh_const>		m_vKids;								// 子图形序列						
};

#pragma pack(pop)
