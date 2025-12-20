#pragma once

#include "svDef3D.h"
#include "svVector3D.h"
#include "svPoint3D.h"
#include "svSegment3D.h"
#include "svRay3D.h"
#include "svLine3D.h"
#include "svTriangle3D.h"
#include "svRectangle3D.h"
#include "svCircle3D.h"
#include "svEllipse3D.h"
#include "svCircleAnnulus3D.h"
#include "svEllipseAnnulus3D.h"
#include "svPlane.h"
#include "svBox.h"
#include "svTetrahedron.h"
#include "svSphere.h"
#include "svCylinder.h"
#include "svEllipsoid.h"
#include "svFrustum.h"
#include "svCone.h"
#include "svTorus.h"
#include "svCapsule.h"
#include "svLozenge.h"
#include "svConvexPolyhedron.h"
#include "svPolyhedron.h"
#include "svConvexMesh.h"

#include <vector>

#ifndef svStd
#define svStd std::
#endif

#pragma pack(push,8)

// 线段与线段的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DSegment3D(const scSegment3D& seg1,const scSegment3D& seg2,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与射线的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DRay3D(const scSegment3D& seg,const scRay3D& ray,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与直线的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DLine3D(const scSegment3D& seg,const scLine3D& line,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与射线的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DRay3D(const scRay3D& ray1,const scRay3D& ray2,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与直线的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DLine3D(const scRay3D& ray,const scLine3D& line,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与直线的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DLine3D(const scLine3D& line1,const scLine3D& line2,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与三角形的交点
// 当bSamePlaneAnalysis为true时，对于线段在三角形平面上这一情形具体分析交点情况，否则，认为当线段与三角形在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DTriangle3D(const scSegment3D& seg,const scTriangle3D& triangle,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 射线与三角形的交点
// 当bSamePlaneAnalysis为true时，对于射线在三角形平面上这一情形具体分析交点情况，否则，认为当射线与三角形在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectRay3DTriangle3D(const scRay3D& ray,const scTriangle3D& triangle,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 直线与三角形的交点
// 当bSamePlaneAnalysis为true时，对于直线在三角形平面上这一情形具体分析交点情况，否则，认为当直线与三角形在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectLine3DTriangle3D(const scLine3D& line,const scTriangle3D& triangle,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 线段与矩形的交点
// 当bSamePlaneAnalysis为true时，对于线段在矩形形平面上这一情形具体分析交点情况，否则，认为当线段与矩形在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DRectangle3D(const scSegment3D& seg,const scRectangle3D& rect,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 射线与矩形的交点
// 当bSamePlaneAnalysis为true时，对于射线在矩形形平面上这一情形具体分析交点情况，否则，认为当射线与矩形在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectRay3DRectangle3D(const scRay3D& ray,const scRectangle3D& rect,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 直线与矩形的交点
// 当bSamePlaneAnalysis为true时，对于直线在矩形形平面上这一情形具体分析交点情况，否则，认为当直线与矩形在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectLine3DRectangle3D(const scLine3D& line,const scRectangle3D& rect,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 线段与圆的交点
// 当bSamePlaneAnalysis为true时，对于线段在圆平面上这一情形具体分析交点情况，否则，认为当线段与圆在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DCircle3D(const scSegment3D& seg,const scCircle3D& circle,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 射线与圆的交点
// 当bSamePlaneAnalysis为true时，对于射线在圆平面上这一情形具体分析交点情况，否则，认为当射线与圆在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectRay3DCircle3D(const scRay3D& ray,const scCircle3D& circle,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 直线与圆的交点
// 当bSamePlaneAnalysis为true时，对于直线在圆平面上这一情形具体分析交点情况，否则，认为当直线与圆在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectLine3DCircle3D(const scLine3D& line,const scCircle3D& circle,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 线段与椭圆的交点
// 当bSamePlaneAnalysis为true时，对于线段在椭圆平面上这一情形具体分析交点情况，否则，认为当线段与椭圆在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DEllipse3D(const scSegment3D& seg,const scEllipse3D& ellipse,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 射线与椭圆的交点
// 当bSamePlaneAnalysis为true时，对于射线在椭圆平面上这一情形具体分析交点情况，否则，认为当射线与椭圆在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectRay3DEllipse3D(const scRay3D& ray,const scEllipse3D& ellipse,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 直线与椭圆的交点
// 当bSamePlaneAnalysis为true时，对于直线在椭圆平面上这一情形具体分析交点情况，否则，认为当直线与椭圆在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectLine3DEllipse3D(const scLine3D& line,const scEllipse3D& ellipse,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 线段与圆环的交点
// 当bSamePlaneAnalysis为true时，对于线段在圆环平面上这一情形具体分析交点情况，否则，认为当线段与圆环在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DCircleannulus3D(const scSegment3D& seg,const scCircleAnnulus3D& annulus,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 射线与圆环的交点
// 当bSamePlaneAnalysis为true时，对于射线在圆环平面上这一情形具体分析交点情况，否则，认为当射线与圆环在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectRay3DCircleannulus3D(const scRay3D& ray,const scCircleAnnulus3D& annulus,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 直线与圆环的交点
// 当bSamePlaneAnalysis为true时，对于直线在圆环平面上这一情形具体分析交点情况，否则，认为当直线与圆环在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectLine3DCircleannulus3D(const scLine3D& line,const scCircleAnnulus3D& annulus,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 线段与椭圆环的交点
// 当bSamePlaneAnalysis为true时，对于线段在椭圆环平面上这一情形具体分析交点情况，否则，认为当线段与椭圆环在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DEllipseannulus3D(const scSegment3D& seg,const scEllipseAnnulus3D& annulus,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 射线与椭圆环的交点
// 当bSamePlaneAnalysis为true时，对于射线在椭圆环平面上这一情形具体分析交点情况，否则，认为当射线与椭圆环在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectRay3DEllipseannulus3D(const scRay3D& ray,const scEllipseAnnulus3D& annulus,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 直线与椭圆环的交点
// 当bSamePlaneAnalysis为true时，对于直线在椭圆环平面上这一情形具体分析交点情况，否则，认为当直线与椭圆环在同一平面时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectLine3DEllipseannulus3D(const scLine3D& line,const scEllipseAnnulus3D& annulus,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 线段与平面的交点
// 当bSamePlaneAnalysis为true时，对于线段在平面上这一情形具体分析交点情况，否则，认为当线段在平面上时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DPlane(const scSegment3D& seg,const scPlane& plane,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 射线与平面的交点
// 当bSamePlaneAnalysis为true时，对于射线在平面上这一情形具体分析交点情况，否则，认为当射线在平面上时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectRay3DPlane(const scRay3D& ray,const scPlane& plane,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 直线与平面的交点
// 当bSamePlaneAnalysis为true时，对于直线在平面上这一情形具体分析交点情况，否则，认为当直线在平面上时，一律没有交点
SVINTERSECTION_API_3D bool sfIntersectLine3DPlane(const scLine3D& line,const scPlane& plane,bool& intersect,svStd vector<scPoint3D>& points,bool bSamePlaneAnalysis = false);

// 线段与长方体的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DBox(const scSegment3D& seg,const scBox& box,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与长方体的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DBox(const scRay3D& ray,const scBox& box,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与长方体的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DBox(const scLine3D& line,const scBox& box,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与四面体的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DTetrahedron(const scSegment3D& seg,const scTetrahedron& tedron,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与四面体的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DTetrahedron(const scRay3D& ray,const scTetrahedron& tedron,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与四面体的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DTetrahedron(const scLine3D& line,const scTetrahedron& tedron,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与球体的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DSphere(const scSegment3D& seg,const scSphere& sphere,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与球体的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DSphere(const scRay3D& ray,const scSphere& sphere,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与球体的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DSphere(const scLine3D& line,const scSphere& sphere,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与圆柱的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DCylinder(const scSegment3D& seg,const scCylinder& cylinder,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与圆柱的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DCylinder(const scRay3D& ray,const scCylinder& cylinder,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与圆柱的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DCylinder(const scLine3D& line,const scCylinder& cylinder,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与椭球体的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DEllipsoid(const scSegment3D& seg,const scEllipsoid& ellipsoid,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与椭球体的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DEllipsoid(const scRay3D& ray,const scEllipsoid& ellipsoid,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与椭球体的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DEllipsoid(const scLine3D& line,const scEllipsoid& ellipsoid,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与视锥体的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DFrustum(const scSegment3D& seg,const scFrustum& frustum,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与视椎体的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DFrustum(const scRay3D& ray,const scFrustum& frustum,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与视椎体的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DFrustum(const scLine3D& line,const scFrustum& frustum,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与圆锥的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DCone(const scSegment3D& seg,const scCone& cone,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与圆锥的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DCone(const scRay3D& ray,const scCone& cone,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与圆锥的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DCone(const scLine3D& line,const scCone& cone,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与圆环面的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DTorus(const scSegment3D& seg,const scTorus& torus,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与圆环面的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DTorus(const scRay3D& ray,const scTorus& torus,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与圆环面的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DTorus(const scLine3D& line,const scTorus& torus,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与胶囊体的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DCapsule(const scSegment3D& seg,const scCapsule& capsule,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与胶囊体的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DCapsule(const scRay3D& ray,const scCapsule& capsule,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与胶囊体的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DCapsule(const scLine3D& line,const scCapsule& capsule,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与长圆体的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DLozenge(const scSegment3D& seg,const scLozenge& lozenge,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与长圆体的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DLozenge(const scRay3D& ray,const scLozenge& lozenge,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与长圆体的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DLozenge(const scLine3D& line,const scLozenge& lozenge,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与凸多面体的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DConvexPolyhedron(const scSegment3D& seg,const scConvexPolyhedron& convex,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与凸多面体的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DConvexPolyhedron(const scRay3D& ray,const scConvexPolyhedron& convex,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与凸多面体的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DConvexPolyhedron(const scLine3D& line,const scConvexPolyhedron& convex,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与多面体的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DPolyhedron(const scSegment3D& seg,const scPolyhedron& polyhedron,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与多面体的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DPolyhedron(const scRay3D& ray,const scPolyhedron& polyhedron,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与多面体的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DPolyhedron(const scLine3D& line,const scPolyhedron& polyhedron,bool& intersect,svStd vector<scPoint3D>& points);

// 线段与凸网格形的交点
SVINTERSECTION_API_3D bool sfIntersectSegment3DConvexMesh(const scSegment3D& seg,const scConvexMesh& mesh,bool& intersect,svStd vector<scPoint3D>& points);

// 射线与凸网格形的交点
SVINTERSECTION_API_3D bool sfIntersectRay3DConvexMesh(const scRay3D& ray,const scConvexMesh& mesh,bool& intersect,svStd vector<scPoint3D>& points);

// 直线与凸网格形的交点
SVINTERSECTION_API_3D bool sfIntersectLine3DConvexMesh(const scLine3D& line,const scConvexMesh& mesh,bool& intersect,svStd vector<scPoint3D>& points);

#pragma pack(pop)
