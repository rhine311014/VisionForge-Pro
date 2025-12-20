// svRegionTree.h

#pragma once

#include "svShapeTree.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            区域图形树
// ---------------------------------------------------------------------
// 区域图形树类scRegionTree是一种定义了平面上区域的层级结构的具体实现类。
// 这些区域可以相连，也可以不相连，还可以有孔洞。孔洞可以包含多个实体区域，
// 实体区域又可以包含多个孔洞区域。区域图形树类scRegionTree派生自图形树
// 基类scShapeTree。
// 
// 区域图形树scRegionTree唯一允许的子图形是scRegionTree类型的图形。所有的
// 区域图形树scRegionTree都有一个边界。边界可以是除了scRegionTree外的任意图形，
// 这些图形需要满足IsRegion为true的条件。
//

class scRegionTree;
smDerivedPtrHdlDcl(scRegionTreePtrh,scRegionTree,scShapeTreePtrh,scShapeTreePtrh_const);

class SVCORE_API scRegionTree : public scShapeTree
{
public:
	
	// 构造函数，构造一个实体的区域图形树，边界是退化的圆形（半径为0，中心在原点）
	scRegionTree();			
	
	// 用图形boundary来显式构造区域图形树；
	// 新构造区域图形树的实体/孔洞状态有hole指定，如果hole为true则为孔洞，否则为实体；
	// 图形boundary的IsRegion应该为true，但不能是scRegionTree类型的，
	// 如果不满足上述条件，则会设置失败；
	// 图形boundary是使用Clone方法拷贝作为当前区域图形树的边界的
	explicit scRegionTree(const scShape& boundary, bool hole = false);			
	
	// 用图形boundary来显式构造区域图形树；
	// 新构造区域图形树的实体/孔洞状态有hole指定，如果hole为true则为孔洞，否则为实体；
	// 图形boundary的IsRegion应该为true，但不能是scRegionTree类型的，
	// 如果不满足上述条件，则会设置失败；
	// 图形boundary是直接赋值作为当前区域图形树的边界的
	explicit scRegionTree(const scShapePtrh_const& boundary, bool hole = false);

	// 拷贝构造函数，拷贝构造是浅拷贝的；
	// 新构造的区域图形树与原区域图形树所包的图形是一样的,实体和孔洞状态也一样
	// 但是新构造的区域图形树的IsRoot总是为true；
	scRegionTree(const scRegionTree& orig);								

	// 赋值函数和析构函数都是默认的

	// 赋值函数是浅拷贝,只是拷贝了指针句柄.
	// 因此赋值后的当前图形树,会与给定图形树共享孩子图形


	// 返回第idx个孩子区域图形树
	// idx应该满足idx>=0且idx<GetChildrenNum条件,
	// 否则,返回值会不正常,在调试版时会报断言错误
	const scRegionTree& GetChild(long idx) const;							

	// 返回是否为孔洞,以显示区域图形树的边界所包含区域是实体还是孔洞；
	// 如果是孔洞则返回true，否则返回false；
	bool IsHole() const;											

	// 返回是否为图形树的根结点
	bool IsRoot() const;												

	// 用图形shape来设置（拷贝设置）区域图形树的轮廓
	// 该图形shape的IsRegion应该为true，但不能是scRegionTree类型的，
	// 如果不满足上述条件，则会设置失败
	// 如果成功则返回true，否则返回false
	bool SetBoundary(const scShape& shape);				

	// 用图形shape来设置（直接设置）区域图形树的轮廓
	// 该图形shape的IsRegion应该为true，但不能是scRegionTree类型的，
	// 如果不满足上述条件，则会设置失败
	// 如果成功则返回true，否则返回false
	bool SetBoundary(const scShapePtrh_const& shape);						

	// 返回区域图形树的根节点的轮廓图形，
	// 该轮廓图形的IsRegion为true，可能是基本图形也可能是轮廓树图形
	const scShape& GetBoundary() const;									

	// 返回一个与除了实体/孔洞状态当前区域图形树相反,其他一样的新区域图形树
	scRegionTreePtrh Flip() const;										

	bool operator==(const scRegionTree &rhs) const;						// 重载==操作符
	bool operator!=(const scRegionTree &rhs) const						// 重载!=操作符
	{ return !(*this == rhs);}


	// 重载scShapeTree的方法
	////////////////////////////////////////////////////////////////

	// 在第idx位置插入(拷贝插入)新的孩子图形child;
	// idx应该满足idx>=0且idx<=GetChildrenNum条件,否则插入会不成功
	// 如果child不是区域图形树，则插入会不成功;
	// 如果child的实体/孔洞状态与当前图形树一样，插入也不会成功;
	// 如果插入成功则返回true,否则返回false
	virtual bool InsertChild(long idx, const scShape& child);			

	// 在第idx位置插入新的孩子图形child;
	// 若direct==true表示直接插入,否则为拷贝插入;	
	// idx应该满足idx>=0且idx<=GetChildrenNum条件,否则插入会不成功
	// 如果child不是区域图形树，则插入会不成功;
	// 如果child的实体/孔洞状态与当前图形树一样，插入也不会成功;
	// 如果插入成功则返回true,否则返回false
	virtual bool InsertChild(long idx, const scShapePtrh_const& child,   
		bool direct = false);

	// 在第idx位置插入一组图形children作为孩子;
	// 若direct==true表示直接插入,否则为拷贝插入;	
	// idx应该满足idx>=0且idx<=GetChildrenNum条件,否则插入会不成功;
	// 如果children中的图形不是区域图形树，则插入会不成功;
	// 如果children中的图形的实体/孔洞状态与当前图形树一样，插入也不会成功;
	// 如果插入成功则返回true,否则返回false;
	virtual bool InsertChildren(long idx,									
		const svStd vector<scShapePtrh_const> &children,	
		bool direct = false);

	// 在图形树的末尾添加(拷贝)新的图形child作为孩子;
	// 如果child不是区域图形树，则添加会不成功;
	// 如果child的实体/孔洞状态与当前图形树一样，添加也不会成功;
	// 如果添加成功则返回true,否则返回false;
	virtual bool AddChild(const scShape &child);								

	// 在图形树的末尾添加新的图形child作为孩子;
	// 若direct为true表示直接添加,否则表示拷贝添加;
	// 如果child不是区域图形树，则添加会不成功;
	// 如果child的实体/孔洞状态与当前图形树一样，添加也不会成功;
	// 如果添加成功则返回true,否则返回false;
	virtual bool AddChild(const scShapePtrh_const &child, bool direct = false); 

	// 在图形树的末尾添加一组图形children作为孩子;
	// 若direct为true表示直接添加,否则表示拷贝添加;
	// 如果children中的图形不是区域图形树，则添加会不成功;
	// 如果children中的图形的实体/孔洞状态与当前图形树一样，添加也不会成功;
	// 如果添加成功则返回true,否则返回false;
	virtual bool AddChildren(const svStd vector<scShapePtrh_const> &children,			
		bool direct = false);

	// 用图形child替换（拷贝）图形树中的第idx个子图形
	// idx应该满足idx>=0且idx<GetChildrenNum条件,否则替换会不成功;
	// 如果child不是区域图形树，则替换会不成功;
	// 如果child的实体/孔洞状态与当前图形树一样，替换也不会成功;
	// 如果成功则返回true,否则返回false;
	virtual bool ReplaceChild(long idx, const scShape &child);		

	// 用图形child替换图形树中的第idx个子图形;
	// 若direct为true表示直接替换,否则表示拷贝替换；
	// idx应该满足idx>=0且idx<GetChildrenNum条件,否则替换会不成功;
	// 如果child不是区域图形树，则替换会不成功;
	// 如果child的实体/孔洞状态与当前图形树一样，替换也不会成功;
	// 如果成功则返回true,否则返回false;
	virtual bool ReplaceChild(long idx, const scShapePtrh_const &child,			
		bool direct = false);

	// 用一组图形children替换图形树中图形;
	// 若direct为true表示直接替换,否则表示拷贝替换;
	// 如果children中的图形不是区域图形树，则替换会不成功;
	// 如果children中的图形的实体/孔洞状态与当前图形树一样，替换也不会成功;
	// 如果成功则返回true,否则返回false;
	virtual bool ReplaceChildren(const svStd vector<scShapePtrh_const> &children,		
		bool direct = false);

	// 重载scShape的方法
	////////////////////////////////////////////////////////////

	// 返回类型的枚举值
	ShapeType GetType() const{ return eRegionTree; };						

	// 复制当前区域图形树,得到新的区域图形树
	scShapePtrh Clone() const;									

	// 是否为区域,总是返回true
	bool IsRegion() const;										

	// 是否为有限的,总是返回true
	bool IsFinite() const;										

	// 是否为空的
	// 如果是孔洞并且没有子图形则返回true，否则返回true
	bool IsEmpty() const;										

	// 是否为开轮廓,总是返回false
	bool IsOpenContour() const;	

	// 计算区域图形树所表示的轮廓上离给定点p距离最近的点
	sc2Vector NearestPoint(const sc2Vector& p) const;			

	// 获取区域图形树所表示区域的最小外接矩形
	scRect BoundingBox() const;								

	// 获取区域图形树所表示图形的边长,phantom hole的边缘不包括在内
	//double Perimeter() const;								

	// 判断给定点p是否在实体区域内
	//（注：当区域树图形中有轮廓树图形时，此函数不适用）
	bool Within(const sc2Vector& p) const;

	bool IsEqual(const scShape& rhs) const;

	bool IsMultipleContours() const;

private:
	scShapePtrh_const m_shBoundary;
	bool m_bHole;
	mutable bool m_bRoot;
};

#pragma pack(pop)
