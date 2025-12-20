// svGenShapeTree.h

#pragma once

#include "svShapeTree.h"

#pragma pack(push,8)


// ---------------------------------------------------------------------
//                            一般图形树
// ---------------------------------------------------------------------
// 一般图形树是保持图形层级的一个具体实现。
// 一般图形树对于加入其中的子结点的类型和数量没有限制。
// 一般图形树scGenShapeTree是scShapeTree的派生类

class scGenShapeTree;
smDerivedPtrHdlDcl(scGenShapeTreePtrh,scGenShapeTree,scShapeTreePtrh,scShapeTreePtrh_const);


class SVCORE_API scGenShapeTree : public scShapeTree
{

public:
	// 构造函数,构造没有子图形的一般图形树
	scGenShapeTree();

	// 拷贝构造函数和析构函数都是默认的

	// 拷贝构造函数是浅拷贝,只是拷贝了指针句柄.
	// 因此通过拷贝构造函数构造的图形树,会与给定图形树共享孩子图形

	// 根据距离容限tolerance,连接相近的孩子图形
//	scGenShapeTreePtrh Connect(double tolerance) const;				

	////////////////////////////////////////////////////////////
	//重载ccShape的方法

	// 返回类型的枚举值
	ShapeType GetType() const{ return eGenShapeTree; };

	// 复制图形树形结构,得到新的图形树
	scShapePtrh Clone() const;										

	// 是否开轮廓
	// 总是返回false
	bool IsOpenContour() const;										

	// 是否为区域
	// 总是返回false
	bool IsRegion() const;											

	// 返回是否有限
	// 如果一般图形树中所有基本图形都是有限的则返回true，否则返回false
	bool IsFinite() const;											

	// 计算图形树上离给定点p最近的点
	sc2Vector NearestPoint(const sc2Vector& p) const;				

	bool IsEqual(const scShape& rhs) const;

	bool IsMultipleContours() const;
};

#pragma pack(pop)
