#pragma once

#include "svModelData.h"

#pragma pack(push,8)

// 图元特殊角度旋转
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesArc* srcArc, CEntitiesArc* dstArc,RotateType type,double dCenterX,double dCenterY);   
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesCircle* srcCircle, CEntitiesCircle* dstCircle,RotateType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesEllipse* srcEllipse, CEntitiesEllipse* dstEllipse,RotateType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesEllipseArc* srcEllipseArc, CEntitiesEllipseArc* dstEllipseArc,RotateType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesLine* srcLine, CEntitiesLine* dstLine,RotateType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesPoint* srcPoint, CEntitiesPoint* dstPoint,RotateType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesRay* srcRay, CEntitiesRay* dstRay,RotateType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesXline* srcXline, CEntitiesXline* dstXline,RotateType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesMText* srcText, CEntitiesMText* dstText,RotateType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesText* srcText, CEntitiesText* dstText,RotateType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesLWPolyline* src, CEntitiesLWPolyline* dst,RotateType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesPolyline* src, CEntitiesPolyline* dst,RotateType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicRotate(const CEntitiesSpline* src, CEntitiesSpline* dst,RotateType type,double dCenterX,double dCenterY); 

// 图元镜像
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesArc* srcArc, CEntitiesArc* dstArc,MirrorType type,double dCenterX,double dCenterY);   
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesCircle* srcCircle, CEntitiesCircle* dstCircle,MirrorType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesEllipse* srcEllipse, CEntitiesEllipse* dstEllipse,MirrorType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesEllipseArc* srcEllipseArc, CEntitiesEllipseArc* dstEllipseArc,MirrorType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesLine* srcLine, CEntitiesLine* dstLine,MirrorType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesPoint* srcPoint, CEntitiesPoint* dstPoint,MirrorType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesRay* srcRay, CEntitiesRay* dstRay,MirrorType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesXline* srcXline, CEntitiesXline* dstXline,MirrorType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesMText* srcText, CEntitiesMText* dstText,MirrorType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesText* srcText, CEntitiesText* dstText,MirrorType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesLWPolyline* src, CEntitiesLWPolyline* dst,MirrorType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesPolyline* src, CEntitiesPolyline* dst,MirrorType type,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicMirror(const CEntitiesSpline* src, CEntitiesSpline* dst,MirrorType type,double dCenterX,double dCenterY); 

// 图元 特殊角度旋转、镜像（一次完成先镜像，后旋转）
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesArc* srcArc, CEntitiesArc* dstArc,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesCircle* srcCircle, CEntitiesCircle* dstCirle,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesEllipse* srcEllipse, CEntitiesEllipse* dstEllipse,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesEllipseArc* srcEllipseArc, CEntitiesEllipseArc* dstEllipseArc,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesLine* srcLine, CEntitiesLine* dstLine,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesPoint* srcPoint, CEntitiesPoint* dstPoint,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesRay* srcRay, CEntitiesRay* dstRay,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesXline* srcXline, CEntitiesXline* dstXline,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesMText* srcText, CEntitiesMText* dstText,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesText* srcText, CEntitiesText* dstText,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesLWPolyline* src, CEntitiesLWPolyline* dst,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesPolyline* src, CEntitiesPolyline* dst,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);
SVDXFFILE_API bool sfGraphicRotateMirror(const CEntitiesSpline* src, CEntitiesSpline* dst,RotateType typeR,MirrorType typeM,double dRotateX,double dRotateY,double dMirrorX,double dMirrorY);

SVDXFFILE_API bool sfModelRotate(const scModelData& srcModel, scModelData& dstModel,RotateType type,double dCenterX,double dCenterY);
SVDXFFILE_API bool sfModelMirror(const scModelData& srcModel, scModelData& dstModel,MirrorType type,double dCenterX,double dCenterY);

// 先镜像，后旋转
SVDXFFILE_API bool sfModelMirrorRotate(const scModelData& srcModel, scModelData& dstModel,MirrorType typeM,RotateType typeR,double dMirrorX,double dMirrorY, double dRotateX,double dRotateY);


// 图元一般角度旋转
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesArc* srcArc, CEntitiesArc* dstArc,double dAngle,double dCenterX,double dCenterY);   
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesCircle* srcCircle, CEntitiesCircle* dstCircle,double dAngle,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesEllipse* srcEllipse, CEntitiesEllipse* dstEllipse,double dAngle,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesEllipseArc* srcEllipseArc, CEntitiesEllipseArc* dstEllipseArc,double dAngle,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesLine* srcLine, CEntitiesLine* dstLine,double dAngle,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesPoint* srcPoint, CEntitiesPoint* dstPoint,double dAngle,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesRay* srcRay, CEntitiesRay* dstRay,double dAngle,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesXline* srcXline, CEntitiesXline* dstXline,double dAngle,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesMText* srcText, CEntitiesMText* dstText,double dAngle,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesText* srcText, CEntitiesText* dstText,double dAngle,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesLWPolyline* src, CEntitiesLWPolyline* dst,double dAngle,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesPolyline* src, CEntitiesPolyline* dst,double dAngle,double dCenterX,double dCenterY); 
SVDXFFILE_API bool sfGraphicNormalRotate(const CEntitiesSpline* src, CEntitiesSpline* dst,double dAngle,double dCenterX,double dCenterY); 

SVDXFFILE_API bool sfModelNormalRotate(const scModelData& srcModel, scModelData& dstModel,double dAngle,double dCenterX,double dCenterY);

// 图元平移
SVDXFFILE_API bool sfGraphicShift(const CEntitiesArc* srcArc, CEntitiesArc* dstArc,double dShiftX,double dShiftY);   
SVDXFFILE_API bool sfGraphicShift(const CEntitiesCircle* srcCircle, CEntitiesCircle* dstCircle,double dShiftX,double dShiftY); 
SVDXFFILE_API bool sfGraphicShift(const CEntitiesEllipse* srcEllipse, CEntitiesEllipse* dstEllipse,double dShiftX,double dShiftY); 
SVDXFFILE_API bool sfGraphicShift(const CEntitiesEllipseArc* srcEllipseArc, CEntitiesEllipseArc* dstEllipseArc,double dShiftX,double dShiftY); 
SVDXFFILE_API bool sfGraphicShift(const CEntitiesLine* srcLine, CEntitiesLine* dstLine,double dShiftX,double dShiftY); 
SVDXFFILE_API bool sfGraphicShift(const CEntitiesPoint* srcPoint, CEntitiesPoint* dstPoint,double dShiftX,double dShiftY); 
SVDXFFILE_API bool sfGraphicShift(const CEntitiesRay* srcRay, CEntitiesRay* dstRay,double dShiftX,double dShiftY); 
SVDXFFILE_API bool sfGraphicShift(const CEntitiesXline* srcXline, CEntitiesXline* dstXline,double dShiftX,double dShiftY); 
SVDXFFILE_API bool sfGraphicShift(const CEntitiesMText* srcText, CEntitiesMText* dstText,double dShiftX,double dShiftY); 
SVDXFFILE_API bool sfGraphicShift(const CEntitiesText* srcText, CEntitiesText* dstText,double dShiftX,double dShiftY); 
SVDXFFILE_API bool sfGraphicShift(const CEntitiesLWPolyline* src, CEntitiesLWPolyline* dst,double dShiftX,double dShiftY); 
SVDXFFILE_API bool sfGraphicShift(const CEntitiesPolyline* src, CEntitiesPolyline* dst,double dShiftX,double dShiftY); 
SVDXFFILE_API bool sfGraphicShift(const CEntitiesSpline* src, CEntitiesSpline* dst,double dShiftX,double dShiftY); 

SVDXFFILE_API bool sfModelShift(const scModelData& srcModel, scModelData& dstModel,double dShiftX,double dShiftY);

#pragma pack(pop)