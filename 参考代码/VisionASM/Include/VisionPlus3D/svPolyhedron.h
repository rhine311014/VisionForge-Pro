#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include <vector>
#include "float.h"

#pragma pack(push,8)

// 多面体
// 由顶点数组、顶点索引数组定义
// 每三个顶点索引构成一个三角片
class SVGEOMETRICS_API_3D scPolyhedron : public scShape3D
{
public:
	scPolyhedron();
	scPolyhedron(const std::vector<scPoint3D>& vectices,const std::vector<unsigned int>& indices);
	scPolyhedron(const std::vector<float>& vectices,const std::vector<unsigned int>& indices);
	scPolyhedron(const scPolyhedron& polyhedron);

	virtual ~scPolyhedron();

	scPolyhedron& operator=(const scPolyhedron& polyhedron);

	bool operator==(const scPolyhedron& polyhedron)const;
	bool operator!=(const scPolyhedron& polyhedron)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const;

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return ePolyhedron;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const std::vector<scPoint3D>& vectices,const std::vector<unsigned int>& indices);
	void Get(std::vector<scPoint3D>& vectices,std::vector<unsigned int>& indices)const;

	void Set(const std::vector<float>& vectices,const std::vector<unsigned int>& indices);
	void Get(std::vector<float>& vectices,std::vector<unsigned int>& indices)const;

	void SetVectices(const std::vector<scPoint3D>& vectices);
	std::vector<scPoint3D> GetPointVectices()const;

	void SetVectices(const std::vector<float>& vectices);
	std::vector<float> GetFloatVectices()const;

	void SetIndices(const std::vector<unsigned int>& indices);
	std::vector<unsigned int> GetIndices()const;

	scPoint3D GetCenter()const;

	int GetVecticesNum()const {return (int)m_Vectices.size();}
	int GetTrianglesNum()const {return (int)m_Indices.size() / 3;}

private:
	std::vector<scPoint3D> m_Vectices;
	std::vector<unsigned int> m_Indices;
};

#pragma pack(pop)