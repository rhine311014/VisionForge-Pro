#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"

class scPlane;

#pragma pack(push,8)

// 矩形
// 由矩形中心、姿态轴、矩形长宽定义
class SVGEOMETRICS_API_3D scRectangle3D: public scShape3D
{
public:
	scRectangle3D();
	scRectangle3D(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float uLength,float vLenght);
	scRectangle3D(const scRectangle3D& rect);

	virtual ~scRectangle3D();

	scRectangle3D& operator=(const scRectangle3D& rect);
	bool operator==(const scRectangle3D& rect)const;
	bool operator!=(const scRectangle3D& rect)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const {return 0.f;}

	virtual bool IsFinite() const;
	virtual ShapeType3D GetType3D()const { return eRectangle3D;}

	virtual scSphere GetExternalSphere()const;

public:
	// U轴正方向为右，负方向为左
	// V轴正方向为上，负方向为下
	scPoint3D GetLT()const; // 获取左上角点
	scPoint3D GetRT()const; // 获取右上角点
	scPoint3D GetLB()const; // 获取左下角点
	scPoint3D GetRB()const; // 获取右下角点

	void Set(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float uLength,float vLenght);
	void Get(scPoint3D& center,scVector3D& u,scVector3D& v,scVector3D& w,float& uLength,float& vLenght)const;

	void SetCenter(const scPoint3D& p) {m_Center = p;}
	scPoint3D GetCenter()const {return m_Center;}

	void SetAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetAxis(scVector3D& u,scVector3D& v,scVector3D& w)const;

	scVector3D GetAxisU()const {return m_AxisU;}
	scVector3D GetAxisV()const {return m_AxisV;}
	scVector3D GetAxisW()const {return m_AxisW;}

	void SetLength(float uLength,float vLength);
	void GetLength(float& uLength,float& vLength)const;

	void SetLengthU(float uLength) {m_fLengthU = uLength;}
	void SetLengthV(float vLength) {m_fLengthV = vLength;}

	float GetLengthU()const {return m_fLengthU;}
	float GetLengthV()const {return m_fLengthV;}

	// 施密特正交化,使u、v、w三轴互相垂直,以u轴为基准
	void SchmidtOrthogonalization(); 

	// 获取矩形所在平面
	scPlane GetPlane()const;

private:
	scPoint3D  m_Center;
	scVector3D m_AxisU;
	scVector3D m_AxisV;
	scVector3D m_AxisW;
	float      m_fLengthU;
	float      m_fLengthV;
};

#pragma pack(pop)