// svDIB.h

#pragma once

#include "svDef.h"
#include "svError.h"
#include "svImage.h"
#include "vpImage.h"
#include <cassert>

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            图像文件操作
// ---------------------------------------------------------------------
// 图像文件操作类scDIB主要是读写windows位图文件。目前支持８位、24位和
// 32位的非压缩windows位图文件读写。
// 

#define smDIB_HEADER_MARKER   ((unsigned short) ('M' << 8) | 'B')
#define smWIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

#pragma pack(push,1)

typedef struct tagSCRGBQUAD {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} SCRGBQUAD, *PSCRGBQUAD;

typedef struct tagSCBITMAPFILEHEADER {
	unsigned short bfType;
	unsigned long  bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long  bfOffBits;
} SCBITMAPFILEHEADER, *PSCBITMAPFILEHEADER;

#pragma pack(2)

typedef struct tagSCBITMAPINFOHEADER{
	unsigned long	biSize;
	long			biWidth;
	long			biHeight;
	unsigned short	biPlanes;
	unsigned short	biBitCount;
	unsigned long	biCompression;
	unsigned long	biSizeImage;
	long			biXPelsPerMeter;
	long			biYPelsPerMeter;
	unsigned long	biClrUsed;
	unsigned long	biClrImportant;
} SCBITMAPINFOHEADER, *PSCBITMAPINFOHEADER;


typedef struct tagSCBITMAPINFO {
	SCBITMAPINFOHEADER    bmiHeader;
	SCRGBQUAD             bmiColors[1];
} SCBITMAPINFO, *PSCBITMAPINFO;

#pragma pack(pop)

class SVCORE_API scDIB  
{
public:
	// 构造函数
	scDIB();

	// 析构函数
	~scDIB();

	// 从BMP格式文件读取数据初始化scDIB对象
	HStatus Init(const TCHAR* strFilePath);

	// 从文件初始化scDIB对象
	HStatus Init(FILE *fp);

	// 用scImageGray对象image中的图像数据初始化scDIB对象
	HStatus Init(const scImageGray& image);	

	// 用scImage16对象image中的图像数据初始化scDIB对象,默认为16位灰度图
	HStatus Init(const scImage16& image);	

	// 用scImageRGB对象image中的图像数据初始化scDIB对象
	HStatus Init(const scImageRGB& image);

	// 用scImage32对象image中的图像数据初始化scDIB对象
	HStatus Init(const scImage32& image);

	// 用image中的图像数据初始化scGuiDIB对象
	HStatus Init(const scImage* pImage);
	HStatus Init(const cpImage& image);

	// 由DIB类型结构的数据（包括位图信息和数据）初始化scDIB对象
	HStatus Init(const unsigned char* pbyDib, long len);		

	// 由图像数据初始化scDIB对象，pixelsize为单个像素所占字节数，alignModulus为图像数据按几个像素进行内存取齐
	HStatus Init(unsigned char* pImage, long width, long height, long pixelsize, long alignModulus = 1);	

	// 将图像数据保存到BMP格式的文件中
	HStatus Write(const TCHAR* strFilePath, bool bQuickWrite = false) const;	

	// 将图像数据保存到句柄中
	HStatus Write(HANDLE hFile) const;

	// 将cpImage保存到句柄中
	HStatus Write(HANDLE hFile, cpImage& image);
	
	// 按照alignModulus像素对齐，将图像数据保存到image结构中	
	HStatus GetImage(scImageGray& image, long alignModulus = 1) const;

	// 按照alignModulus像素对齐，将图像数据保存到image结构中	
	HStatus GetImage(scImage16& image, long alignModulus = 1) const;
	
	// 按照alignModulus像素对齐，将图像数据保存到image结构中	
	HStatus GetImage(scImageRGB& image, long alignModulus = 1) const;	

	// 按照alignModulus像素对齐，将图像数据保存到image结构中	
	HStatus GetImage(scImage32& image, long alignModulus = 1) const;

	// 按照alignModulus像素对齐，将图像数据保存到image结构中	
	HStatus GetImage(scImage* pImage, long alignModulus = 1) const;
	HStatus GetImage(cpImage& image, long alignModulus = 1) const;

	// 返回是否有有效的DIB数据
	bool IsValid() const { return m_pBMI != 0; }
	
	// 返回图像的宽
	long Width() const{ return IsValid() ? m_pBMI->bmiHeader.biWidth : 0;}
	
	// 返回图像的高
	long Height() const{ return IsValid() ? m_pBMI->bmiHeader.biHeight : 0;}

	// 返回表示图像中一个像素所需的位数
	unsigned int BitCount() const{ return IsValid() ? m_pBMI->bmiHeader.biBitCount : 0;}	

	// 设置水平分辨率，单位pixels/meter
	HStatus SetXPelsPerMeter(long lXPelsPerMeter);

	// 获取水平分辨率，单位pixels/meter
	long GetXPelsPerMeter() const;

	// 设置垂直分辨率，单位pixels/meter
	HStatus SetYPelsPerMeter(long lYPelsPerMeter);

	// 获取垂直分辨率，单位pixels/meter
	long GetYPelsPerMeter() const;

	// 设置/获取文件头保留信息1
	void SetBFHReserved1(unsigned short usValue);
	unsigned short GetBFHReserved1();

	// 设置/获取文件头保留信息2
	void SetBFHReserved2(unsigned short usValue);
	unsigned short GetBFHReserved2();

protected:
	void Free();
	unsigned long PaletteSize()const;
	unsigned long NumColors()const;

private:	
	PSCBITMAPINFO  m_pBMI;
	unsigned char* m_pBits;

	unsigned short m_usReserved1;
	unsigned short m_usReserved2;
};

#pragma pack(pop)
