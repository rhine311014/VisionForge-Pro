// svShapeGroup.h


#pragma once

#include "svShape.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            图形组类
// ---------------------------------------------------------------------
// 

class scShape;
class scRect;

class scShapeGroup;
smDerivedShapePtrhDecl(scShapeGroup);

class SVCORE_API scShapeGroup : public scShape
{
public:
	// 构造函数
	scShapeGroup();	
	scShapeGroup(const scShapeGroup& group);
	scShapeGroup(const std::vector<scShape*>& vpShape);	//默认使用LineColor绘制
	scShapeGroup(const std::vector<sc2Vector>& vPoint, const std::vector<bool>& vbUsed);//vbUsed为true，使用LineColor绘制；vbUsed为false，使用LineColor2绘制
	scShapeGroup(const std::vector<scShape*>& vpShape, const std::vector<int>& viShowProperty);//viShowProperty为0，使用LineColor绘制；viShowProperty为1，使用LineColor2绘制
	virtual ~scShapeGroup();
public:
	// 增加图形项
	bool	AddItem(const scShapeGroup& group);
	bool	AddItem(const std::vector<scShape*>& vpShape);
	bool	AddItem(scShape* pShape, int nShowProperty = 0);
	bool	AddItem(ShapeType type, int nCount, int nShowProperty = 0);
	bool	AddItem(const std::vector<sc2Vector>& vPoint, const std::vector<bool>& vbUsed);
	bool	AddItem(const std::vector<scShape*>& vpShape, const std::vector<int>& viShowProperty);
	
	// 插入图形项
	bool	InsertItem(int nIndex, const scShapeGroup& group);
	bool	InsertItem(int nIndex, const std::vector<scShape*>& vpShape);
	bool	InsertItem(int nIndex, scShape* pShape, int nShowProperty = 0);
	bool	InsertItem(int nIndex, ShapeType type, int nCount, int nShowProperty = 0);
	bool	InsertItem(int nIndex, const std::vector<sc2Vector>& vPoint, const std::vector<bool>& vbUsed);
	bool	InsertItem(int nIndex, const std::vector<scShape*>& vpShape, const std::vector<int>& viShowProperty);
	
	// 移除图形项
	bool	RemoveItem(int nIndex);
	bool	RemoveItem(int nBegin, int nEnd);

	// 设置图形
	bool	SetShape(int nIndex, scShape* pShape);

	// 获取图形
	scShape* GetShape(int nIndex) const;
	std::vector<scShape*> GetShape(int nBegin, int nEnd) const;
	std::vector<scShape*> GetShapeAll() const;

	// 设置显示属性
	bool	SetShowProperty(int nIndex, int nShowProperty);

	// 获取显示属性
	int		GetShowProperty(int nIndex) const;
	std::vector<int> GetShowProperty(int nBegin, int nEnd) const;
	std::vector<int> GetShowPropertyAll() const;

	// 清空图形容器
	void	Clear();

	// 获取图形容器大小
	int		GetCount() const;

	scShapeGroup& operator=(const scShapeGroup &rhs);				// 重载=操作符
	bool operator==(const scShapeGroup &rhs) const;					// 重载==操作符
	bool operator!=(const scShapeGroup &rhs) const					// 重载!=操作符
	{ return !(*this == rhs);}

public:
	virtual ShapeType GetType() const { return eShapeGroup; }		// 返回类型的枚举值
	virtual scShapePtrh Clone() const;								// 复制得到新的图形

	virtual bool IsOpenContour() const;								// 图形组是否为开的轮廓
	virtual bool IsRegion() const;									// 图形组是否为区域
	virtual bool IsFinite() const;									// 图形组是否为有限的
	virtual bool IsEmpty() const;									// 图形组是否为空
	virtual scRect BoundingBox() const;								// 返回图形的最小外接矩形
	virtual bool IsEqual(const scShape& rhs) const;					// 判断给定图形rhs是否与当前图形相等，仅供内部使用

protected:
	void FreeShape();
	scShape* CreateShapeFromType(ShapeType type);
	scShape* CreateShapeFromShape(scShape* pShape);
private:
	std::vector<scShape*>	m_vpShape;
	std::vector<int>		m_viShowProperty;
};

#pragma pack(pop)
