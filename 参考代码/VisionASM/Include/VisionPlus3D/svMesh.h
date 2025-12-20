#pragma once

#ifdef SV_POINT_CLOUD_PROC_EXPORTS
#define SV_MESH_API_3D __declspec(dllexport)
#else
#define SV_MESH_API_3D __declspec(dllimport)
#endif

#pragma pack(push,1)

typedef struct tagVertex
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};

		float coord[3];
	};

	union
	{
		struct
		{
			float r;
			float g;
			float b;
		};

		float color[3];
	};

	union
	{
		struct
		{
			float nx;
			float ny;
			float nz;
		};

		float normal[3];
	};

	tagVertex()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
		r = 128.f;
		g = 128.f;
		b = 255.f;
		nx = 0.f;
		ny = 0.f;
		nz = 0.f;
	}

}Vertex,*PVertex;

typedef struct tagTriangle
{
	union
	{
		struct
		{
			unsigned int index0;
			unsigned int index1;
			unsigned int index2;
		};

		unsigned int index[3];
	};

	union
	{
		struct
		{
			float nx;
			float ny;
			float nz;
		};

		float normal[3];
	};

	tagTriangle()
	{
		index0 = 0;
		index1 = 0;
		index2 = 0;
		nx     = 0.f;
		ny     = 0.f;
		nz     = 0.f;
	}
}Triangle,*PTriangle;

#pragma pack(pop)

#include <vector>

#pragma pack(push,8)
class  SV_MESH_API_3D scMesh
{
public:
	scMesh();
	virtual ~scMesh();

	enum VertexColor
	{
		eClrDefault  = 0,
		eClrCoord_x  = 1,
		eClrCoord_y  = 2,
		eClrCoord_z  = 3,
		eClrDistance = 4
	};

	enum FilterDimension
	{
		eFilterX = 0,
		eFilterY = 1,
		eFilterZ = 2
	};

	scMesh(const scMesh& mesh);
	scMesh& operator=(const scMesh& mesh);

	/************** 无序点云数据处理 *******************/
	void addVertex(const Vertex& vertex);       // 添加顶点
	void addTriangle(const Triangle& triangle); // 添加三角形

	void SetVertex(const std::vector<Vertex>& vertices); // 设置顶点数组
	void SetTriangle(std::vector<Triangle>& triangles);  // 设置三角形数组

	void SetVertex(const Vertex& vertex,int nIndex); // 按索引设置顶点
	void SetTriangle(const Triangle& triangle,int nIndex); // 按索引设置三角形

	const Vertex& GetVertex(int nIndex) const;     // 按索引获取顶点
	const Triangle& GetTriangle(int nIndex) const; // 按索引获取三角形

	std::vector<Vertex>& GetVertices();  // 获取顶点数组
	std::vector<Triangle>& GetTriangles(); // 获取三角形数组
	
	int VerticesNum() const;  // 获取顶点数量
	int TrianglesNum() const;  // 获取三角形数量

	float GetMinX() const {return minX;}
	float GetMaxX() const {return maxX;}
	float GetMinY() const {return minY;}
	float GetMaxY() const {return maxY;}
	float GetMinZ() const {return minZ;}
	float GetMaxZ() const {return maxZ;}

	void UpdateMaxMinValue();  // 更新最大值最小值

	void GetCenter(float& centerX,float& centerY,float& centerZ) const ; // 获取中心点

	void CalculateNormal(); // 根据拓扑结构计算法向量
	
	// 渲染打包
	void PackageVertex(std::vector<float>& v);
	void PackageTriangleIndex(std::vector<unsigned int>& v);
	void PackageCuboid(std::vector<float>& v);
	void PackageCuboidIndex(std::vector<unsigned int>& v);
	void PackageAxis(std::vector<float>& v);
	void PackageAxisIndex(std::vector<unsigned int>& v);
	void PackageNormal(std::vector<float>& v,float normalLength,COLORREF color);
	void PackageNormalIndex(std::vector<unsigned int>& v);

private:
	std::vector<Vertex>   m_Vertices;   // 顶点属性数组
	std::vector<Triangle> m_Triangles;  // 三角形索引数组

	float   minX;      // x最小值
	float   maxX;      // x最大值
	float   minY;      // y最小值
	float   maxY;      // y最大值
	float   minZ;      // z最小值
	float   maxZ;      // z最大值
};

void PCLColorTrans(float value,float& r,float& g,float& b); // PCL
COLORREF RGBToHSL(COLORREF color);
COLORREF HSLToRGB(COLORREF color);

#pragma pack(pop)
