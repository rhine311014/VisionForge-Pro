#pragma once

#include "svDef.h"
#include "svMath.h"
#include <vector>

#pragma pack(push,8)

class scRadian;
class scDegree;
class scAngle8;
class scAngle16;

// ---------------------------------------------------------------------
//                            角度表示
// ---------------------------------------------------------------------
// 提供角度scDegree、弧度scRadian、8位表示scAngle8、16位表示scAngle16等
// 角度的表示方法，及其相互之间的转换。
// 提供角度相关的三角函数等定义。

#define smMakeUnit(T, U)                                        \
public:                                                         \
  U () {}                                                       \
  explicit U (T a) : v(a) {}                                    \
                                                                \
  U operator + (U a) const { return U (v + a.v);}               \
  U operator - (U a) const { return U (v - a.v);}               \
                                                                \
  U operator * (T a) const { return U (v * a);}                 \
  U operator / (T a) const { return U (v / a);}                 \
                                                                \
  U operator - () const { return U (-v);}                       \
                                                                \
  T operator * (U a) const { return v * a.v;}                   \
  T operator / (U a) const { return v / a.v;}                   \
                                                                \
  friend U operator * (T a, U b) { return U (a * b.v);}         \
                                                                \
  double ToDouble () const { return (double) v;}                \
  T      Plain    () const { return          v;}                \
                                                                \
  bool operator == (U a) const { return v == a.v;}              \
  bool operator != (U a) const { return v != a.v;}              \
  bool operator <  (U a) const { return v <  a.v;}              \
  bool operator <= (U a) const { return v <= a.v;}              \
  bool operator >  (U a) const { return v >  a.v;}              \
  bool operator >= (U a) const { return v >= a.v;}              \
                                                                \
  U& operator  = (T a) { v  = a; return *this;}                 \
  U& operator *= (T a) { v *= a; return *this;}                 \
  U& operator /= (T a) { v /= a; return *this;}                 \
                                                                \
  U& operator *= (U a) { v *= a.v; return *this;}               \
  U& operator /= (U a) { v /= a.v; return *this;}               \
                                                                \
  U& operator += (U a) { v += a.v; return *this;}               \
  U& operator -= (U a) { v -= a.v; return *this;}               \
                                                                \
private:                                                        \
  T v

//弧度
class SVCORE_API scRadian
{
	smMakeUnit (double, scRadian);

public:
	inline scRadian (scDegree x);
	inline scRadian (scAngle8 x);
	inline scRadian (scAngle16 x);

	scRadian Norm () const;        // 规范化到[0, 2PI)
	scRadian SignedNorm () const;  // 规范化到[-PI, PI)
	scRadian SignedNorm (scRadian ref) const;  // 规范化到[ref-PI, ref+PI)
	scRadian NormMod180 () const;  // 规范化到[0, PI)
	scRadian SignedNormMod180 () const;  // 规范化到[-PI/2, PI/2)
	scRadian SignedNormMod180 (scRadian ref) const;  // 规范化到[ref-PI/2, ref+PI/2)
};

SVCORE_API bool sfRealEqual (scRadian x, scRadian y, scRadian epsilon = scRadian(1.e-15));

//角度
class SVCORE_API scDegree
{
	smMakeUnit (double, scDegree);

public:
	inline scDegree (scRadian x);
	inline scDegree (scAngle8 x);
	inline scDegree (scAngle16 x);

	scDegree Norm () const;        // 规范化到[0, 360)
	scDegree SignedNorm () const;  // 规范化到[-180, 180)
	scDegree SignedNorm (scDegree ref) const;  // 规范化到[ref-180, ref+180)
	scDegree NormMod180 () const;  // 规范化到[0, 180)
	scDegree SignedNormMod180 () const;  // 规范化到[-90, 90)
	scDegree SignedNormMod180 (scDegree ref) const;  // 规范化到[ref-90, ref+90)
};

SVCORE_API bool sfRealEqual (scDegree x, scDegree y, scDegree epsilon = scDegree(1.e-15));

//8位角度表示
class SVCORE_API scAngle8
{
	smMakeUnit (unsigned char, scAngle8);

public:
	inline scAngle8 (scRadian x);
	inline scAngle8 (scDegree x);
	inline scAngle8 (scAngle16 x);
};

//16位角度表示
class SVCORE_API scAngle16
{
	smMakeUnit (unsigned short, scAngle16);

public:
	inline scAngle16 (scRadian x);
	inline scAngle16 (scDegree x);
	inline scAngle16 (scAngle8 x);
};

//构造函数
inline scRadian::scRadian (scDegree x)
{
	v = x.ToDouble() * (sk2PI / 360.);
}

inline scRadian::scRadian (scAngle8 x)
{
	v = x.ToDouble() * (sk2PI / 256.);
}

inline scRadian::scRadian (scAngle16 x)
{
	v = x.ToDouble() * (sk2PI / 65536.);
}

inline scDegree::scDegree (scRadian x)
{
	v = x.ToDouble() * (360. / sk2PI);
}

inline scDegree::scDegree (scAngle8 x)
{
	v = x.ToDouble() * (360. / 256.);
}

inline scDegree::scDegree (scAngle16 x)
{
	v = x.ToDouble() * (360. / 65536.);
}

inline scAngle8::scAngle8 (scRadian x)
{
	v = (unsigned char) (int) floor(x.ToDouble() * (256./sk2PI) + 0.5);
}

inline scAngle8::scAngle8 (scDegree x)
{
	v = (unsigned char) (int) floor(x.ToDouble() * (256./360.) + 0.5);
}

inline scAngle8::scAngle8 (scAngle16 x)
{
  v = (unsigned char) ((x.Plain() + (1 << 7) - 1) >> 8); 
}

inline scAngle16::scAngle16 (scRadian x)
{
	v = (unsigned char) (int) floor(x.ToDouble() * (65536./sk2PI) + 0.5);
}

inline scAngle16::scAngle16 (scDegree x)
{
	v = (unsigned char) (int) floor(x.ToDouble() * (65536./360.) + 0.5);
}

inline scAngle16::scAngle16 (scAngle8 x)
{
	v = (unsigned short) (x.Plain() << 8);
}

//三角函数
inline scRadian arccos (double x) { return scRadian (acos (x));}
inline scRadian arcsin (double x) { return scRadian (asin (x));}
inline scRadian arctan (double x) { return scRadian (atan (x));}
inline scRadian arctan (double y, double x) { return scRadian(atan2(y, x));}

inline double sin (scRadian x) { return sin (x.ToDouble());}
inline double cos (scRadian x) { return cos (x.ToDouble());}
inline double tan (scRadian x) { return tan (x.ToDouble());}

inline scRadian abs (scRadian x) { return scRadian (fabs (x.ToDouble()));}
inline scDegree abs (scDegree x) { return scDegree (fabs (x.ToDouble()));}


//弧度平均, 返回结果取值范围[rad1-PI, rad1+PI)
inline scRadian sfRadianAverage (scRadian x1, scRadian x2);
SVCORE_API scRadian sfRadianAverage (scRadian x1, scRadian x2, scRadian x3);
SVCORE_API scRadian sfRadianAverage (scRadian x1, scRadian x2, scRadian x3, scRadian x4);
SVCORE_API scRadian sfRadianAverage (svStd vector<scRadian> arrayRad);

//弧度平均, 返回结果取值范围[rad1-PI/2, rad1+PI/2)
SVCORE_API scRadian sfRadianAverageMod180 (scRadian x1, scRadian x2);
SVCORE_API scRadian sfRadianAverageMod180 (scRadian x1, scRadian x2, scRadian x3);
SVCORE_API scRadian sfRadianAverageMod180 (scRadian x1, scRadian x2, scRadian x3, scRadian x4);
SVCORE_API scRadian sfRadianAverageMod180 (svStd vector<scRadian> arrayRad);

//角度平均, 返回结果取值范围[deg1-180, deg1+180)
SVCORE_API scDegree sfDegreeAverage (scDegree x1, scDegree x2);
SVCORE_API scDegree sfDegreeAverage (scDegree x1, scDegree x2, scDegree x3);
SVCORE_API scDegree sfDegreeAverage (scDegree x1, scDegree x2, scDegree x3, scDegree x4);
SVCORE_API scDegree sfDegreeAverage (svStd vector<scDegree> arrayDeg);

//角度平均, 返回结果取值范围[deg1-90, deg1+90)
SVCORE_API scDegree sfDegreeAverageMod180 (scDegree x1, scDegree x2);
SVCORE_API scDegree sfDegreeAverageMod180 (scDegree x1, scDegree x2, scDegree x3);
SVCORE_API scDegree sfDegreeAverageMod180 (scDegree x1, scDegree x2, scDegree x3, scDegree x4);
SVCORE_API scDegree sfDegreeAverageMod180 (svStd vector<scDegree> arrayDeg);


#pragma pack(pop)

