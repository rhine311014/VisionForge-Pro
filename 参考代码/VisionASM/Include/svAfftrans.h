#pragma once

#include "svDef.h"
#include "svError.h"
#include "svAngle.h"
#include "svMath.h"
#include "svImage.h"
#include "vpImage.h"
#include "vpRotateMirror.h"

#pragma pack(push,8)

//旋转、平移
//程序内部为输出图像dstimage分配数据内存，其size与输入图像srcimage一致
//angle				旋转角度
//aboutx、abouty	以（aboutx、abouty）该点为中心进行旋转
//shiftx、shifty	旋转完成以后对输出图像沿水平方向平移shiftx，沿垂直方向平移shifty
//如果srcimage无效，返回HInvalidImage；否则，返回HSuccess
HStatus SVCORE_API sfRotateShift (const scImageGray& srcimage, scImageGray& dstimage, scRadian angle, double aboutx, double abouty, double shiftx, double shifty, unsigned char clipvalue = 0, bool bUseGpu = true);
HStatus SVCORE_API sfRotateShift (cpImage& srcimage, cpImage& dstimage, scRadian angle, double aboutx, double abouty, double shiftx, double shifty, unsigned char clipvalue = 0, bool bUseGpu = true);

//旋转、平移
//在程序外部由用户为输出图像dstimage分配数据内存
//angle				旋转角度
//aboutx、abouty	以（aboutx、abouty）该点为中心进行旋转
//shiftx、shifty	旋转完成以后对输出图像沿水平方向平移shiftx，沿垂直方向平移shifty
//如果srcimage或dstimage无效，返回HInvalidImage；否则，返回HSuccess
HStatus SVCORE_API sfRotateShift2(const scImageGray& srcimage, scImageGray& dstimage, scRadian angle, double aboutx, double abouty, double shiftx, double shifty, unsigned char clipvalue = 0, bool bUseGpu = true);
HStatus SVCORE_API sfRotateShift2(cpImage& srcimage, cpImage& dstimage, scRadian angle, double aboutx, double abouty, double shiftx, double shifty, unsigned char clipvalue = 0, bool bUseGpu = true);

//旋转、平移、缩放
//在程序外部由用户为输出图像dstimage分配数据内存
//angle				旋转角度
//aboutx、abouty	以（aboutx、abouty）该点为中心进行旋转
//shiftx、shifty	旋转完成以后对输出图像沿水平方向平移shiftx，沿垂直方向平移shifty
//scalex、scaley	缩放系数
//如果srcimage或dstimage无效，返回HInvalidImage；如果scale小于等于0，返回HBadParam；否则，返回HSuccess
HStatus SVCORE_API sfRotateShiftScale2(const scImageGray& srcimage, scImageGray& dstimage, scRadian angle, double aboutx, double abouty, double shiftx, double shifty, double scalex, double scaley, unsigned char clipvalue = 0);
HStatus SVCORE_API sfRotateShiftScale2(cpImage& srcimage, cpImage& dstimage, scRadian angle, double aboutx, double abouty, double shiftx, double shifty, double scalex, double scaley, unsigned char clipvalue = 0);

//旋转
//程序内部为输出图像dstimage,weights分配数据内存，其size根据旋转角度参数angle计算得到
//如果srcimage，返回HInvalidImage；否则，返回HSuccess
HStatus SVCORE_API sfRotate(const scImageGray& srcimage, scImageGray& dstimage, scRadian angle, scImageGray& weights, bool& clipped, double& shiftx, double& shifty, bool bNewAlgo = true);
HStatus SVCORE_API sfRotate(cpImage& srcimage, cpImage& dstimage, scRadian angle, cpImage& weights, bool& clipped, double& shiftx, double& shifty, bool bNewAlgo = true);

//特殊角度旋转
//程序内部为输出图像dstimage分配数据内存
HStatus SVCORE_API sfRotate(const scImageGray& srcimage, scImageGray& dstimage, RotateType type);
HStatus SVCORE_API sfRotate(cpImage& srcimage, cpImage& dstimage, RotateType type);

//镜像
//程序内部为输出图像dstimage分配数据内存，其size与输入图像srcimage一致
HStatus SVCORE_API sfMirror(const scImageGray& srcimage, scImageGray& dstimage, MirrorType type);
HStatus SVCORE_API sfMirror(cpImage& srcimage, cpImage& dstimage, MirrorType type);

//特殊角度旋转、镜像（一次完成旋转和镜像）
//程序内部为输出图像dstimage分配数据内存，其size与输入图像srcimage一致
HStatus SVCORE_API sfRotateMirror(const scImageGray& srcimage, scImageGray& dstimage, RotateType typeR, MirrorType typeM);
HStatus SVCORE_API sfRotateMirror(const scImage24& srcimage, scImage24& dstimage, RotateType typeR, MirrorType typeM);
HStatus SVCORE_API sfRotateMirror(const scImage32& srcimage, scImage32& dstimage, RotateType typeR, MirrorType typeM);
HStatus SVCORE_API sfRotateMirror(const cpImage& srcimage, cpImage& dstimage, RotateType typeR, MirrorType typeM);
HStatus SVCORE_API sfRotateMirror3D(const cpImage& srcimage, cpImage& dstimage, RotateType typeR, MirrorType typeM);

#pragma pack(pop)
