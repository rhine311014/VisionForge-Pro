#pragma once

#include "svDef.h"
#include "svGuiDef.h"
#include "svError.h"
#include "svImage.h"
#include "vpImage.h"

#pragma pack(push,8)

#define smGUIDIBHEADERMARKER		((unsigned short) ('M' << 8) | 'B')
#define smGUIWIDTHBYTES(bits)		(((bits) + 31) / 32 * 4)

#define smGUIRECTWIDTH(lpRect)		((lpRect)->right - (lpRect)->left)
#define smGUIRECTHEIGHT(lpRect)		((lpRect)->bottom - (lpRect)->top)

#define smGUIPALVERSION				0x300

SVGUI_API extern COLORREF g_clrJetMap[256];
SVGUI_API extern COLORREF g_clrRGB332Map[256];
SVGUI_API extern COLORREF g_clrThalMap[256];

class SVGUI_API scGuiDIBBase
{
public:
	scGuiDIBBase();
	virtual ~scGuiDIBBase();

public:

	// 从BMP格式文件读取数据初始化scGuiDIBBase对象
	virtual HStatus Init(LPCTSTR strFilePath) {return HInvalidImage;}

	// 用scImageGray对象image中的图像数据初始化scGuiDIBBase对象
	virtual HStatus Init(const scImageGray& image) {return HInvalidImage;}

	// 用scImageRGB对象image中的图像数据初始化scGuiDIBBase对象
	virtual HStatus Init(const scImageRGB& image){return HInvalidImage;}		

	// 用image中的图像数据初始化scGuiDIB对象
	virtual HStatus Init(const scImage* pImage){return HInvalidImage;}	
	virtual HStatus Init(const cpImage& image){return HInvalidImage;}

	// 由DIB类型结构的数据（包括位图信息和数据）初始化scGuiDIBBase对象
	virtual HStatus Init(const unsigned char* pbyDib, long len){return HInvalidImage;}		

	// 由图像数据初始化scGuiDIB对象
	virtual HStatus Init(unsigned char* pImage, long width, long height, int bitCount, bool bRaw = true){return HInvalidImage;}	

	// 将图像数据保存到BMP格式的文件中
	virtual HStatus Write(const TCHAR* strFilePath, bool bQuickWrite = false) const{return HInvalidImage;}

	// 按照alignModulus像素对齐，将图像数据保存到image结构中	
	virtual HStatus GetImage(scImageGray& image, long alignModulus = 32) const{return HInvalidImage;}

	// 按照alignModulus像素对齐，将图像数据保存到image结构中	
	virtual HStatus GetImage(scImageRGB& image, long alignModulus = 32) const{return HInvalidImage;}	

	// 获取图像中（x，y）处的像素值
	virtual HStatus GetPixelValue(long x, long y, BYTE& r, BYTE& g, BYTE& b){return HInvalidImage;}

	// 返回是否有有效的DIB数据
	virtual bool IsValid() const { return false;}

	// 返回图像的宽
	virtual long Width() const{return 0;}

	// 返回图像的高
	virtual long Height() const{return 0;}

	// 返回表示图像中一个像素所需的位数
	virtual unsigned int BitCount() const{return 0;}

	// 获取图像像素数据地址
	virtual unsigned char* GetImageAddr() {return NULL;}

	// 在hDC所指定的DC上绘制图像，显示区域为lpDCRect，图像区域为lpDIBRect
	// 注：图像区域原点的规定参考widows的拉伸显示图像函数的相关规定
	virtual BOOL  Paint(HDC hDC, LPRECT lpDCRect, LPRECT lpDIBRect) const{return FALSE;}

	//	更新透明位图
	virtual BOOL  UpdateBlend(){return FALSE;}

	//	在指定的DC上绘制图像,针对RGB8888类型的图像,其中包含一个Alpha通道,可以用来控制透明度
	virtual BOOL  PaintBlend(CDC*, LPRECT, LPRECT){return FALSE;}

	// 重载绘制函数,用双缓冲方式绘制以避免闪烁，但会降低速度和增加系统资源占用率
	virtual BOOL Paint(CDC* pDC, LPRECT lpDCRect, LPRECT lpViewRect, LPRECT lpDIBRect) const{return FALSE;}

	// 设置调色板映射模式
	// 0 - 灰度,1 - Jet,2 - RGB332,3 - Thal
	void SetPaletteMapMode(int nMode) {m_nPaletteMapMode = nMode;}

	// 设置自定义调色板
	virtual void SetCustomPalette(std::vector<COLORREF>& clr);

public:
	static void RGB565ToRGB888(unsigned short rgb565,BYTE& r,BYTE& g,BYTE& b);
	static void RGB888ToRGB565(BYTE r,BYTE g,BYTE b,unsigned short& rgb565);
	static void RGB555ToRGB888(unsigned short rgb555,BYTE& r,BYTE& g,BYTE& b);
	static void RGB888ToRGB555(BYTE r,BYTE g,BYTE b,unsigned short& rgb555);

protected:
	int m_nPaletteMapMode;
	COLORREF m_clrCustomPalette[256];
};

#pragma pack(pop)