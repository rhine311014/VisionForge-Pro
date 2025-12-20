// svTIFF.h

#pragma once

#include "svDef.h"
#include "svError.h"
#include "svImage.h"
#include "vpImage.h"
#include <cassert>

#ifdef SVIMAGEFILETIFF_EXPORTS
#define SVIMAGEFILLTIFF __declspec(dllexport)
#else
#define SVIMAGEFILLTIFF __declspec(dllimport)
#endif

#pragma pack(push,8)

enum CompressMode
{
	eCompressNone = 0,   //无压缩
	eCompressLZW  = 1,   //LZW压缩
	eCompressZip  = 2,   //Zip压缩
	eCompressJPEG  = 3,  //JPEG压缩
};

class SVIMAGEFILLTIFF scTiffLoad2
{
public:
	scTiffLoad2();
	virtual ~scTiffLoad2();
	
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

class SVIMAGEFILLTIFF scTiffSave2
{
public:
	scTiffSave2();
	virtual ~scTiffSave2();
	
	// 从TIFF格式文件读取数据初始化scTIFF对象
	HStatus Init(const TCHAR* strFilePath, bool bAppend = false);

	// 将image图像数据保存到TIFF文件中	
	HStatus AddImage(const scImageGray& image,int nCompressMode=eCompressNone,int nCompressQuality = 100);  //压缩质量0~100，默认为100为无损压缩
	HStatus AddImage(const scImage16& image, int nCompressMode=eCompressNone,int nCompressQuality = 100);
	HStatus AddImage(cpImage& image, int nCompressMode=eCompressNone,int nCompressQuality = 100);

	// 返回是否有有效的TIFF数据
	bool IsValid() const;

	// 关闭TIFF文件
	void Close();

private:
	void* m_pTiffImage;
};

#pragma pack(pop)