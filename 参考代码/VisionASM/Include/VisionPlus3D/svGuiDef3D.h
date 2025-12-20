#pragma once

#pragma pack(push,8)

enum GuiGraphicType3D
{
	eGuiUnknown3D = 0,          // 未知图形
	eGuiPoint3D,              // 三维点
	eGuiSegment3D,            // 三维线段
	eGuiRay3D,                // 三维射线
	eGuiLine3D,               // 三维直线
	eGuiTriangle3D,           // 三维三角形
	eGuiRectangle3D,          // 三维矩形
	eGuiCircle3D,             // 三维圆
	eGuiEllipse3D,            // 三维椭圆
	eGuiCircleAnnulus3D,      // 三维圆环
	eGuiEllipseAnnulus3D,     // 三维椭圆环
	eGuiPlane,                // 三维平面
	eGuiBox,                  // 长方体
	eGuiTetrahedron,          // 四面体
	eGuiSphere,               // 球体
	eGuiCylinder,             // 圆柱体
	eGuiEllipsoid,            // 椭球体
	eGuiFrustum,              // 视椎体
	eGuiCone,                 // 圆锥体
	eGuiTorus,                // 圆环面
	eGuiCapsule,              // 胶囊体
	eGuiLozenge,              // 长圆体
	eGuiConvexPolyhedron,     // 凸多面体
	eGuiPolyhedron,           // 多面体
	eGuiConvexMesh            // 凸网格形
};

enum GuiGraphicFillType
{
	eGuiWireFrame,
	eGuiSolid
};

/* Display3D Message Define */
#define WM_DISPLAY3D_LBUTTONDOWN    WM_USER + 100
#define WM_DISPLAY3D_LBUTTONUP      WM_USER + 101
#define WM_DISPLAY3D_RBUTTONDOWN    WM_USER + 102
#define WM_DISPLAY3D_RBUTTONUP      WM_USER + 103
#define WM_DISPLAY3D_MBUTTONDOWN    WM_USER + 104
#define WM_DISPLAY3D_MBUTTONUP      WM_USER + 105
#define WM_DISPLAY3D_MOUSEMOVE      WM_USER + 106
#define WM_DISPLAY3D_MOUSEWHEEL     WM_USER + 107
#define WM_DISPLAY3D_FITVIEW        WM_USER + 108
#define WM_DISPLAY3D_MIRROR			WM_USER + 109

#ifdef SVGUI3D_EXPORTS
#define SVGUI_API_3D __declspec(dllexport)
#else
#define SVGUI_API_3D __declspec(dllimport)
#endif

#ifdef SVGEOMETRICS_EXPORTS_3D
#define SVGEOMETRICSGUI_API_3D __declspec(dllexport)
#else
#define SVGEOMETRICSGUI_API_3D __declspec(dllimport)
#endif

#pragma pack(pop)