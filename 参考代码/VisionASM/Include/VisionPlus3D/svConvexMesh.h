#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include <vector>

#pragma pack(push,8)

// 凸网格形
class SVGEOMETRICS_API_3D scConvexMesh: public scShape3D
{
public:

	enum MeshType
	{
		eMeshEmpty = 0,  // 空
		eMeshPoint,      // 点
		eMeshSegment,    // 线段
		eMeshPolygon,    // 多边形
		eMeshPolyhedron  // 多面体
	};

	scConvexMesh();
	scConvexMesh(const std::vector<scPoint3D>& vectices,const std::vector<unsigned int>& indices,MeshType type = eMeshEmpty);
	scConvexMesh(const std::vector<float>& vectices,const std::vector<unsigned int>& indices,MeshType type = eMeshEmpty);
	scConvexMesh(const scConvexMesh& convexMesh);

	virtual ~scConvexMesh();

	scConvexMesh& operator=(const scConvexMesh& convexMesh);

	bool operator==(const scConvexMesh& convexMesh)const;
	bool operator!=(const scConvexMesh& convexMesh)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const;

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return eConvexMesh;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const std::vector<scPoint3D>& vectices,const std::vector<unsigned int>& indices,MeshType type = eMeshEmpty);
	void Get(std::vector<scPoint3D>& vectices,std::vector<unsigned int>& indices,MeshType& type)const;

	void Set(const std::vector<float>& vectices,const std::vector<unsigned int>& indices,MeshType type = eMeshEmpty);
	void Get(std::vector<float>& vectices,std::vector<unsigned int>& indices,MeshType& type)const;

	std::vector<float> GetFloatVectices()const;
	std::vector<scPoint3D> GetPointVectices()const {return m_Vectices;}
	std::vector<unsigned int> GetIndices()const {return m_Indices;}
	MeshType GetMeshType()const {return m_MeshType;}

	int GetVecticesNum()const {return (int)m_Vectices.size();}
	int GetIndicesNum()const {return (int)m_Indices.size();}

private:
	std::vector<scPoint3D> m_Vectices;   // 顶点
	std::vector<unsigned int> m_Indices; // 顶点索引

	// 确定凸网格形的具体图形以及顶点索引的含义
	// 若为eMeshPoint、eMeshSegment，则顶点索引无意义
	// 若为eMeshPolygon，则每两个顶点索引代表一段线段
	// 若为eMeshPolyhedron，则每三个顶点索引代表一个三角片
	MeshType m_MeshType;
};

#pragma pack(pop)