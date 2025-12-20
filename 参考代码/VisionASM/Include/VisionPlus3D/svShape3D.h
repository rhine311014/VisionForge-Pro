#pragma once

#include "svDef3D.h"

class scSphere;
#pragma pack(push,8)

class SVGEOMETRICS_API_3D scShape3D
{
public:
	virtual ~scShape3D();

public:
	virtual float GetPerimeter() const = 0; // 获取图形周长
	virtual float GetArea() const = 0;      // 获取图形面积
	virtual float GetVolume() const = 0;    // 获取图形体积

	virtual bool IsFinite() const = 0; 
	virtual ShapeType3D GetType3D()const = 0;

	virtual scSphere GetExternalSphere()const = 0; // 外接球
};

#pragma pack(pop)