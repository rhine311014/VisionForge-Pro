#pragma once

#include "svDef.h"
#include "svError.h"
#include "vpImage.h"

#pragma pack(push,8)

enum epConvertMode
{
	epGray8ToGray16 = 1,
	epGray8ToGray24,
	epGray8ToGray32,
	epGray8ToRGB16_555,
	epGray8ToRGB16_565,
	epGray8ToRGB24,
	epGray8ToRGB32,

	epGray16ToGray8,
	epGray16ToGray24,
	epGray16ToGray32,
	epGray16ToRGB16_555,
	epGray16ToRGB16_565,
	epGray16ToRGB24,
	epGray16ToRGB32,

	epRGB16_555ToGray8,
	epRGB16_555ToGray16,
	epRGB16_555ToGray24,
	epRGB16_555ToGray32,
	epRGB16_555ToRGB24,
	epRGB16_555ToRGB32,

	epRGB16_565ToGray8,
	epRGB16_565ToGray16,
	epRGB16_565ToGray24,
	epRGB16_565ToGray32,
	epRGB16_565ToRGB24,
	epRGB16_565ToRGB32,

	epRGB24ToGray8,
	epRGB24ToGray16,
	epRGB24ToGray24,
	epRGB24ToGray32,
	epRGB24ToRGB16_555,
	epRGB24ToRGB16_565,
	epRGB24ToRGB32,

	epRGB32ToGray8,
	epRGB32ToGray16,
	epRGB32ToGray24,
	epRGB32ToGray32,
	epRGB32ToRGB16_555,
	epRGB32ToRGB16_565,
	epRGB32ToRGB24
};

class SVCORE_API scImageConvert
{
public:
	scImageConvert(void);
	virtual ~scImageConvert(void);

	// vpImage×ªÎª8¡¢16¡¢24¡¢32Î»Í¼
	static HStatus Convert(const cpImage& Src, cpImage& Dst, epConvertMode mode, long alignModulus=1);

private:
	static HStatus Gray8Convert(const cpImage& Src, cpImage& Dst, epConvertMode mode, long alignModulus=1);

	static HStatus Gray16Convert(const cpImage& Src, cpImage& Dst, epConvertMode mode, long alignModulus=1);

	static HStatus RGB555Convert(const cpImage& Src, cpImage& Dst, epConvertMode mode, DWORD rMask = 0x7C00, DWORD gMask = 0x03E0, DWORD bMask = 0x001F, long alignModulus=1);

	static HStatus RGB565Convert(const cpImage& Src, cpImage& Dst, epConvertMode mode, DWORD rMask = 0xF800, DWORD gMask = 0x03E0, DWORD bMask = 0x001F, long alignModulus=1);

	static HStatus RGB24Convert(const cpImage& Src, cpImage& Dst, epConvertMode mode, long alignModulus=1);

	static HStatus RGB32Convert(const cpImage& Src, cpImage& Dst, epConvertMode mode, long alignModulus=1);


};

#pragma pack(pop)