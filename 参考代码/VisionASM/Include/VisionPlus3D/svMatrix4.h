#pragma once
#include "svDef3D.h"
#include "svVector4D.h"

#pragma pack(push,8)

class SVGEOMETRICS_API_3D scMatrix4
{
public:
	scMatrix4();
	scMatrix4(const scVector4D& v1,const scVector4D& v2,const scVector4D& v3,const scVector4D& v4);
	scMatrix4(const scMatrix4& m);
	~scMatrix4() {}

	scMatrix4& operator=(const scMatrix4& m);

	bool operator==(const scMatrix4& m)const;
	bool operator!=(const scMatrix4& m)const;

	scMatrix4 operator+(const scMatrix4& m)const;
	scMatrix4& operator+=(const scMatrix4& m);

	scMatrix4 operator-(const scMatrix4& m)const;
	scMatrix4& operator-=(const scMatrix4& m);
	scMatrix4 operator-()const;

	scMatrix4 operator*(const scMatrix4& m)const;
	scMatrix4& operator*=(const scMatrix4& m);

	scVector4D operator*(const scVector4D& v)const;

	scMatrix4 operator*(float scale)const;
	scMatrix4& operator*=(float scale);

	scMatrix4 operator/(float scale)const;
	scMatrix4& operator/=(float scale);

	scVector4D operator[](int nIndex)const;
	scVector4D& operator[](int nIndex);

public:
	// 生成单位矩阵
	static scMatrix4 Identity();

    // 生成元素均为0的矩阵
	static scMatrix4 Zero();

	// 获取数据首地址
	float* GetValuePtr();

	// 转置
	scMatrix4 Transpose()const;

	// 行列式的值
	float Det()const;

	// 余子式的值
	float MinorDet(int i,int j)const;

	// 伴随矩阵
	scMatrix4 Adjugate()const;

	// 逆矩阵
	scMatrix4 Inverse()const;

	// 生成视点矩阵
	static scMatrix4 LookAt(const scPoint3D& viewPoint,const scPoint3D& center,const scVector3D& up);

	// 生成透视投影矩阵
	static scMatrix4 Perspective(float radians,float ratio,float zNear,float zFar);

	// 生成正视投影矩阵
	static scMatrix4 Ortho(float left,float right,float bottom,float top,float zNear,float zFar);

	// 根据角度与平移向量生成矩阵
	// z-y-x
	static scMatrix4 Transform(float alpha,float belta,float gamma,float dx,float dy,float dz);

private:
	scVector4D m_Vec[4];
};

#pragma pack(pop)
