// svJpeg.h

#pragma once

#include "svDef.h"
#include "svError.h"
#include "svImage.h"
#include "vpImage.h"
#include <cassert>

#pragma pack(push,8)

class scJpegPro;
class SVCORE_API scJpeg
{
public:
	scJpeg();
	virtual ~scJpeg();
	
	// 从Jpeg格式文件读取图像
	BOOL Read(const TCHAR* strFilePath, scImageGray& image);
	BOOL Read(const TCHAR* strFilePath, scImageRGB& image);
	BOOL Read(const TCHAR* strFilePath, cpImage& image);
	BOOL Read(FILE *input_file, cpImage& image);

	// 将图像数据保存到Jpeg格式的文件中
	BOOL Write(const TCHAR* strFilePath, const scImageGray& image, int nQuality = 100) const;	
	BOOL Write(const TCHAR* strFilePath, const scImageRGB& image, int nQuality = 100) const;	
	BOOL Write(const TCHAR* strFilePath, cpImage& image, int nQuality = 100) const;	
	BOOL Write(FILE *output_file, cpImage& image, int nQuality = 100) const;

private:
	scJpegPro* m_pJpegPro;
};

#pragma pack(pop)