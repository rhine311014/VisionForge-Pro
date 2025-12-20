#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"

#include <cassert>

class scRectangle3D;

#pragma pack(push,8)

// 长方体
// 由中心、姿态轴、三轴方向长度确定
class SVGEOMETRICS_API_3D scBox:public scShape3D
{
public:
	scBox();
	scBox(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float uLength,float vLength,float wLength);
	scBox(const scBox& box);

	virtual ~scBox();

	scBox& operator=(const scBox& box);

	bool operator==(const scBox& box)const;
	bool operator!=(const scBox& box)const;

	virtual float GetPerimeter() const {return 4.f*(m_fLengthU + m_fLengthV + m_fLengthW);}
	virtual float GetArea() const {return 2.f*(m_fLengthU*m_fLengthV + m_fLengthV*m_fLengthW + m_fLengthU*m_fLengthW);}
	virtual float GetVolume() const {return (m_fLengthU*m_fLengthV*m_fLengthW);}

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return eBox;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float uLength,float vLength,float wLength);
	void Get(scPoint3D& center,scVector3D& u,scVector3D& v,scVector3D& w,float& uLength,float& vLength,float& wLength)const;

	void SetCenter(const scPoint3D& center) {m_Center = center;}
	scPoint3D GetCenter()const {return m_Center;}

	void SetAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetAxis(scVector3D& u,scVector3D& v,scVector3D& w)const;

	scVector3D GetAxisU() const {return m_AxisU;}
	scVector3D GetAxisV() const {return m_AxisV;}
	scVector3D GetAxisW() const {return m_AxisW;}

	void SetLengthU(float uLength) {assert(uLength >= 0.f);m_fLengthU = uLength;}
	float GetLengthU()const {return m_fLengthU;}

	void SetLengthV(float vLength) {assert(vLength >= 0.f);m_fLengthV = vLength;}
	float GetLengthV() const {return m_fLengthV;}

	void SetLengthW(float wLength) {assert(wLength >= 0.f);m_fLengthW = wLength;}
	float GetLengthW() const {return m_fLengthW;}

	// 施密特正交化,使u、v、w三轴互相垂直,以u轴为基准
	void SchmidtOrthogonalization(); 

	// U轴正方向为右，负方向为左
	// V轴正方向为上，负方向为下
	// W轴正方向为前，负方向为后
	scRectangle3D GetLeftRectangle3D()const;
	scRectangle3D GetRightRectangle3D()const;
	scRectangle3D GetFrontRectangle3D()const;
	scRectangle3D GetBackRectangle3D()const;
	scRectangle3D GetTopRectangle3D()const;
	scRectangle3D GetBottomRectangle3D()const;

private:
	scPoint3D  m_Center;   // 长方体中心
	scVector3D m_AxisU;    // u,v,w表示长方体的姿态，三轴相互垂直
	scVector3D m_AxisV;
	scVector3D m_AxisW;
	float      m_fLengthU; // u轴方向长度
	float      m_fLengthV; // v轴方向长度
	float      m_fLengthW; // w轴方向长度
};

#pragma pack(pop)