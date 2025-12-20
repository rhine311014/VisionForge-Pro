// svTIFF.h

#pragma once

#include "svDef.h"
#include "svError.h"
#include "svImage.h"
#include "vpImage.h"
#include <cassert>

#pragma pack(push,8)

class SVCORE_API scTiffLoad
{
public:
	scTiffLoad();
	virtual ~scTiffLoad();
	
	// 从TIFF格式文件读取数据初始化scTIFF对象
	HStatus Init(const TCHAR* strFilePath);
	
	// 获取当前TIFF数据的图像数量
	long GetNumberOfImage() const;

	// 将第lImageIndex幅图像数据保存到image结构中	
	HStatus GetImage(scImageGray& image, long lImageIndex = 0) const;
	HStatus GetImage(scImage16& image, long lImageIndex = 0) const;
	HStatus GetImage(cpImage& image, long lImageIndex = 0) const;

	// 返回是否有有效的TIFF数据
	bool IsValid() const;

	// 关闭TIFF文件
	void Close();

private:

	void* m_pTiffImage;
	long  m_lNumOfImage;
};

class SVCORE_API scTiffSave
{
public:
	scTiffSave();
	virtual ~scTiffSave();
	
	// 从TIFF格式文件读取数据初始化scTIFF对象
	HStatus Init(const TCHAR* strFilePath, bool bAppend = false);

	// 将image图像数据保存到TIFF文件中	
	HStatus AddImage(const scImageGray& image);
	HStatus AddImage(const scImage16& image);
	HStatus AddImage(cpImage& image);

	// 返回是否有有效的TIFF数据
	bool IsValid() const;

	// 关闭TIFF文件
	void Close();

private:
	void* m_pTiffImage;
};

#pragma pack(pop)