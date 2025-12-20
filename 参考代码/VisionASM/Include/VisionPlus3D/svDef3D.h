#pragma once

#pragma pack(push,8)

#pragma warning(disable:4800)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4267)

enum ShapeType3D
{
	eShape3DUnknown   = 0,     // 未知形状
	ePoint3D,                  // 三维点
	eSegment3D,                // 三维线段
	eRay3D,                    // 三维射线
	eLine3D,                   // 三维直线
	eTriangle3D,               // 三维三角形
	eRectangle3D,              // 三维矩形
	eCircle3D,                 // 三维圆
	eEllipse3D,                // 三维椭圆
	eCircleAnnulus3D,          // 三维圆环
	eEllipseAnnulus3D,         // 三维椭圆环
	ePlane,                    // 平面
	eBox,                      // 立方体
	eTetrahedron,              // 四面体
	eSphere,                   // 球体
	eCylinder,                 // 圆柱体
	eEllipsoid,                // 椭球体
	eFrustum,                  // 视椎体
	eCone,                     // 圆锥
	eTorus,                    // 圆环面
	eCapsule,                  // 胶囊体
	eLozenge,                  // 长圆体
	eConvexPolyhedron,         // 凸多面体
	ePolyhedron,               // 多面体
	eConvexMesh                // 凸网格形
};

#ifdef SVGEOMETRICS_EXPORTS_3D
#define SVGEOMETRICS_API_3D __declspec(dllexport)
#else
#define SVGEOMETRICS_API_3D __declspec(dllimport)
#endif

#ifdef SVMEASURE_EXPORTS_3D
#define SVMEASURE_API_3D __declspec(dllexport)
#else
#define SVMEASURE_API_3D __declspec(dllimport)
#endif

#ifdef SVINTERSECTION_EXPORTS_3D
#define SVINTERSECTION_API_3D __declspec(dllexport)
#else
#define SVINTERSECTION_API_3D __declspec(dllimport)
#endif

#pragma pack(pop)
