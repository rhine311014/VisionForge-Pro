#pragma once

#include "svShape.h"
#include "svShape3D.h"
#include "svDxfFileDef.h"
#include <vector>

#pragma pack(push,8)
class scGuiGraphicData;
class scModelData;
class SVDXFFILE_API scShapeData
{
public:
	scShapeData();
	scShapeData(const scShapeData& shape);
	scShapeData(const std::vector<scShape*>& m_vpShape,const std::vector<scShape3D*>& m_vpShape3D);
	virtual ~scShapeData();

	scShapeData& operator=(const scShapeData& shape);

	// 添加2D图形（可添加2D图形数组或单个2D图形）
	void AddShape(const std::vector<scShape*>& shape);
	void AddShape(scShape* shape);

	// 添加3D图形（可添加3D图形数组或单个3D图形）
	void AddShape3D(const std::vector<scShape3D*>& shape3D);
	void AddShape3D(scShape3D* shape3D);

	// 移除2D图形（可移除单个2D图形或移除一段2D图形）
	void MoveShape(int nIndex);
	void MoveShape(int firstIndex,int endIndex);

	// 移除3D图形（可移除单个3D图形或移除一段3D图形）
	void MoveShape3D(int nIndex);
	void MoveShape3D(int firstIndex,int endIndex);


	// 清空2D图形
	void ClearShape();

	// 清空3D图形
	void ClearShape3D();

	// 设置/获取2D图形数据
	void SetShape(scShape* shape,int nIndex);
	scShape* GetShape(int nIndex)const;
	std::vector<scShape*> GetShape()const;
	std::vector<scShape*> GetShape(int firstIndex,int endIndex)const;

	// 设置/获取3D图形数据
	void SetShape3D(scShape3D* shape3D,int nIndex);
	scShape3D* GetShape3D(int nIndex)const;
	std::vector<scShape3D*> GetShape3D()const;
	std::vector<scShape3D*> GetShape3D(int firstIndex,int endIndex)const;

	// 获取图形数量
	int GetShapeNum()const;

	// 获取3D图形数量
	int GetShape3DNum()const;

	// 图形数据到图形数据的转换
	scGuiGraphicData ShapeToGuiGraphic();

	// 图形数据到模型数据的转换
	scModelData ShapeToModel();

private:
	void DeleteShape(scShape* shape);
	void DeleteShape3D(scShape3D* shape3D);

protected:
	std::vector<scShape*> m_vpShape;		 //  图形数组
	std::vector<scShape3D*> m_vpShape3D;	 //  3D图形数组
};

#pragma pack(pop)