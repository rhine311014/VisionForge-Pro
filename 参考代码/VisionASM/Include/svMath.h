#pragma once

#include "svDef.h"
#include <cmath>
#include <float.h>

#pragma pack(push,8)

//常用的常量定义
#define skPI           3.14159265358979323846
#define sk2PI          (skPI*2.0)
#define skPI_2         1.57079632679489661923
#define skPI_4         0.78539816339744830962
#define sk1_PI         0.31830988618379067154
#define sk2_PI         0.63661977236758134308
#define sk2_SQRTPI     1.12837916709551257390

#define skE            2.7182818284590452354
#define skLN2          0.69314718055994530942
#define skLN10         2.30258509299404568402
#define skLOG2E        1.4426950408889634074
#define skLOG10E       0.43429448190325182765

#define skSQRT2        1.41421356237309504880
#define skSQRT1_2      0.70710678118654752440

//平方
template <class T> inline T sfSquare(T x) {return x*x;}

//立方
template <class T> inline T sfCube(T x) {return x*x*x;}

//判断两个数是否相等
inline bool sfRealEqual(double x,double y,double epsilon = 1.e-15)
{
	return fabs(x - y) <= epsilon;
}

//复制sign的符号到val
inline double sfCopySign(double val, double sign)
{
	if ((val < 0 && sign > 0) || (val > 0 && sign < 0))
		val = -val;
	return val;
}

//向下取整
inline int sfFloor(double x, double epsilon = 1.e-11)
{
	return (int) floor(x * (1 + sfCopySign(epsilon, x)));
}

//向上取整
inline int sfCeil(double x, double epsilon = 1.e-11)
{
	return (int) ceil(x * (1 - sfCopySign(epsilon, x)));
}

#pragma pack(pop)
