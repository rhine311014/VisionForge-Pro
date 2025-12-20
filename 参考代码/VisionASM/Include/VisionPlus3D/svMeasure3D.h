#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include "svLine3D.h"
#include "svVector3D.h"
#include "svRay3D.h"
#include "svSegment3D.h"
#include "svTriangle3D.h"
#include "svRectangle3D.h"
#include "svCircle3D.h"
#include "svPlane.h"
#include "svBox.h"
#include "svSphere.h"
#include "svCylinder.h"
#include "svEllipse3D.h"
#include "svEllipsoid.h"
#include "svTetrahedron.h"
#include "svConvexPolyhedron.h"
#include "svFrustum.h"
#include "svLCPSolver.h"
#include "svRootsPolynomial.h"
#include "svPolynomial.h"

#pragma pack(push,8)

#ifndef svStd
#define svStd std::
#endif


// 点到点距离测量
SVMEASURE_API_3D bool sfDistance3DPointToPoint(scPoint3D point1, scPoint3D point2, double& distance);

//点到线段距离测量
SVMEASURE_API_3D bool sfDistance3DPointToSegment(scPoint3D point, scSegment3D segment, double& distance, scPoint3D& pointSegment);

// 点到射线距离测量
SVMEASURE_API_3D bool sfDistance3DPointToRay(scPoint3D point, scRay3D ray, double& distance, scPoint3D& pointRay);

// 点到直线距离测量
SVMEASURE_API_3D bool sfDistance3DPointToLine(scPoint3D point, scLine3D line, double& distance, scPoint3D& pointline);

// 点到三角形距离测量
SVMEASURE_API_3D bool sfDistance3DPointToTriangle(scPoint3D point, scTriangle3D triangle, double&distance, scPoint3D& pointTriangle);

// 点到矩形距离测量
SVMEASURE_API_3D bool sfDistance3DPointToRectangle(scPoint3D point, scRectangle3D rectangle, double& distance, scPoint3D& pointRectangle);

// 点到圆距离测量
// 当bCalculateCurve为true时，计算点到圆曲线的距离，否则计算点到圆面的距离。
SVMEASURE_API_3D bool sfDistance3DPointToCircle(scPoint3D point, scCircle3D circle, double& distance, scPoint3D& pointCircle, bool bCalculateCurve = false);

// 点到面距离测量
SVMEASURE_API_3D bool sfDistance3DPointToPlane(scPoint3D point, scPlane plane, double& distance, scPoint3D& pointPlane);

// 点到长方体
SVMEASURE_API_3D bool sfDistance3DPointToBox(scPoint3D point, scBox box, double& distance, scPoint3D& pointBox);

// 点到球体
SVMEASURE_API_3D bool sfDistance3DPointToSphere(scPoint3D point, scSphere sphere, double& distance, scPoint3D& pointSphere);

// 点到圆柱体
SVMEASURE_API_3D bool sfDistance3DPointToCylinder(scPoint3D point, scCylinder cylinder, double& distance, scPoint3D& pointCylinder);

// 点到椭圆
// 当bCalculateCurve为true时，计算点到椭圆曲线的距离，否则计算点到椭圆面的距离。
SVMEASURE_API_3D bool sfDistance3DPointToEllipse(scPoint3D point, scEllipse3D ellipse, double& distance, scPoint3D& pointEllipse, bool bCalculateCurve = false);

// 点到椭球体
SVMEASURE_API_3D bool sfDistance3DPointToEllipsoid(scPoint3D point, scEllipsoid ellipsoid, double& distance, scPoint3D& pointEllipsoid);

// 点到四面体
SVMEASURE_API_3D bool sfDistance3DPointToTetrahedron(scPoint3D point, scTetrahedron tetrahedron, double& distance, scPoint3D& pointTetrahedron);

// 点到凸多面体
SVMEASURE_API_3D bool sfDistance3DPointToConvexPolyhedron(scPoint3D point, scConvexPolyhedron polyhedron, double& distance, scPoint3D& pointPolyhedron, LCPSolver::Result* res = nullptr);

// 点到视锥
SVMEASURE_API_3D bool sfDistance3DPointToFrustum(scPoint3D point, scFrustum frustum, double& distance, scPoint3D& pointFrustum);

// 线段到线段
SVMEASURE_API_3D bool sfDistance3DSegmentToSegment(scSegment3D segment1, scSegment3D segment2, double& distance, scPoint3D& pointSegment1, scPoint3D& pointSegment2);

// 直线到直线
SVMEASURE_API_3D bool sfDistance3DLineToLine(scLine3D line1, scLine3D line2, double& distance, scPoint3D& pointLine1, scPoint3D& pointLine2);

// 直线到射线
SVMEASURE_API_3D bool sfDistance3DLineToRay(scLine3D line, scRay3D ray, double& distance, scPoint3D& pointLine, scPoint3D& pointRay);

// 直线到线段
SVMEASURE_API_3D bool sfDistance3DLineToSegment(scLine3D line, scSegment3D segment, double& distance, scPoint3D& pointLine, scPoint3D& pointSegment);

// 射线到射线
SVMEASURE_API_3D bool sfDistance3DRayToRay(scRay3D ray1, scRay3D ray2, double& distance, scPoint3D& pointRay1, scPoint3D& pointRay2);

// 射线到线段
SVMEASURE_API_3D bool sfDistance3DRayToSegment(scRay3D ray, scSegment3D segment, double& distance, scPoint3D& pointRay, scPoint3D& pointSegment);

// 直线到三角形
SVMEASURE_API_3D bool sfDistance3DLineToTriangle(scLine3D line, scTriangle3D triangle, double& distance, scPoint3D& pointLine, scPoint3D& pointTriangle);

// 射线到三角形
SVMEASURE_API_3D bool sfDistance3DRayToTriangle(scRay3D ray, scTriangle3D triangle, double& distance, scPoint3D& pointRay, scPoint3D& pointTriangle);

// 线段到三角形
SVMEASURE_API_3D bool sfDistance3DSegmentToTriangle(scSegment3D segment, scTriangle3D triangle, double& distance, scPoint3D& pointSegment, scPoint3D& pointTriangle);

// 直线到矩形
SVMEASURE_API_3D bool sfDistance3DLineToRectangle(scLine3D line, scRectangle3D rectangle, double& distance, scPoint3D& pointLine, scPoint3D& pointRectangle);

// 射线到矩形
SVMEASURE_API_3D bool sfDistance3DRayToRectangle(scRay3D ray, scRectangle3D rectangle, double& distance, scPoint3D& pointRay, scPoint3D& pointRectangle);

// 线段到矩形
SVMEASURE_API_3D bool sfDistance3DSegmentToRectangle(scSegment3D segment, scRectangle3D rectangle, double& distance, scPoint3D& pointSegment, scPoint3D& pointRectangle);

// 三角形到三角形
SVMEASURE_API_3D bool sfDistance3DTriangleToTriangle(scTriangle3D triangle1, scTriangle3D triangle2, double& distance, scPoint3D& pointTriangle1, scPoint3D& pointTriangle2);

// 三角形到矩形
SVMEASURE_API_3D bool sfDistance3DTriangleToRectangle(scTriangle3D triangle, scRectangle3D rectangle, double& distance, scPoint3D& pointTriangle, scPoint3D& pointRectangle);

// 矩形到矩形
SVMEASURE_API_3D bool sfDistance3DRectangleToRectangle(scRectangle3D rectangle1, scRectangle3D rectangle2, double& distance, scPoint3D& pointRectangle1, scPoint3D& pointRectangle2);

// 直线到立方体
SVMEASURE_API_3D bool sfDistance3DLineToBox(scLine3D line, scBox box, double& distance, scPoint3D& pointLine, scPoint3D& pointBox);

// 线段到立方体
SVMEASURE_API_3D bool sfDistance3DSegmentToBox(scSegment3D segment, scBox box, double& distance, scPoint3D& pointSegment, scPoint3D& pointBox);

// 三角形到立方体
SVMEASURE_API_3D bool sfDistance3DTriangleToBox(scTriangle3D triangle, scBox box, double& distance, scPoint3D& pointTriangle, scPoint3D& pointBox, LCPSolver::Result* res = nullptr);

// 矩形到立方体
SVMEASURE_API_3D bool sfDistance3DRectangleToBox(scRectangle3D rectangle, scBox box, double& distance, scPoint3D& pointRectangle, scPoint3D& pointBox, LCPSolver::Result* res = nullptr);

// 立方体到立方体
SVMEASURE_API_3D bool sfDistance3DBoxToBox(scBox box1, scBox box2, double& distance, scPoint3D& pointBox1, scPoint3D& pointBox2, LCPSolver::Result* res = nullptr);

// 直线到圆
SVMEASURE_API_3D bool sfDistance3DLineToCircle(scLine3D line, scCircle3D circle, double& distance, scPoint3D& pointLine, scPoint3D& pointCircle, bool bCalculateCurve = false);

// 圆线到圆线、圆线到圆面、圆面到圆面
SVMEASURE_API_3D bool sfDistance3DCircleToCircle(scCircle3D circle1, scCircle3D circle2, double& distance, scPoint3D& pointCircle1, scPoint3D& pointCircle2, bool bCurve1 = false, bool bCurve2 = false);

// 圆线到平面
SVMEASURE_API_3D bool sfDistance3DCircleToPlane(scCircle3D circle, scPlane plane, double& distance, scPoint3D& pointCircle, scPoint3D& pointPlane);

#pragma pack(pop)