// svPng.h

#pragma once

#include "svDef.h"
#include "svError.h"
#include "svImage.h"
#include "vpImage.h"
#include <cassert>

#pragma pack(push,8)

class SVCORE_API scPng
{
public:
	scPng();
	virtual ~scPng();
	
	// 从Png格式文件读取图像
	BOOL Read(const TCHAR* strFilePath, scImageGray& image);
	BOOL Read(const TCHAR* strFilePath, scImage16& image);
	BOOL Read(const TCHAR* strFilePath, scImageRGB& image);
	BOOL Read(const TCHAR* strFilePath, cpImage& image);
	BOOL Read(FILE *input_file, cpImage& image);

	// 将图像数据保存到Png格式的文件中
	BOOL Write(const TCHAR* strFilePath, const scImageGray& image) const;	
	BOOL Write(const TCHAR* strFilePath, const scImage16& image) const;
	BOOL Write(const TCHAR* strFilePath, const scImageRGB& image) const;	
	BOOL Write(const TCHAR* strFilePath, cpImage& image) const;	
	BOOL Write(FILE *output_file, cpImage& image) const;
};

#pragma pack(pop)