#pragma once

#include "svDef.h"
#include "svImage.h"
#include "svRect.h"
#include <cassert>

#pragma pack(push,8)

enum epPixelFormat
{
	epUnknown = 0,
	epGray8 = 1,
	epGray16 = 2,
	epPackedRGB24 = 3,
	epPackedRGB32 = 4,
};

enum epBayerType
{
	epBayerRG8 = 1,
	epBayerGB8 = 2,
	epBayerGR8 = 3,
	epBayerBG8 = 4,
};

#pragma pack(push,1)

class SVCORE_API cpPackedRGB24Pixel
{

public:
	cpPackedRGB24Pixel(unsigned long ppel=0) 
	{ 
		unsigned char val = (unsigned char) ((ppel & 0xFF0000) >> 16); r(val); 
		val = (unsigned char) ((ppel & 0x00FF00) >> 8); g(val); 
		val = (unsigned char) (ppel & 0x0000FF); b(val);   
	}

	cpPackedRGB24Pixel(unsigned char pr, unsigned char pg, unsigned char pb) 
	{ r(pr); g(pg); b(pb); }

	unsigned char r() const { return r_; }
	unsigned char g() const { return g_; }
	unsigned char b() const { return b_; }
	unsigned char gray() const
	{
		unsigned long s = r();
		s += 2 * g();
		s += b();
		s /= 4;
		return (unsigned char) (s & 0xff);
	}

	void r(unsigned char rval) { r_ = rval; }
	void g(unsigned char gval) { g_ = gval; }
	void b(unsigned char bval) { b_ = bval; }

	void cvt() { unsigned char t_ = r_; r_ = b_; b_ = t_; }

	bool operator==(cpPackedRGB24Pixel v)
	{ return (v.r() == r() && v.g() == g() && v.b() == b()); }
	bool operator!=(cpPackedRGB24Pixel v)
	{ return (v.r() != r() || v.g() != g() || v.b() != b()); }

private:
	unsigned char b_;
	unsigned char g_;
	unsigned char r_;
};

class SVCORE_API cpPackedRGB32Pixel
{

public:
	cpPackedRGB32Pixel(unsigned long ppel=0) : packedPel_(ppel) {}

	cpPackedRGB32Pixel(unsigned char pr, unsigned char pg, unsigned char pb, unsigned char pa = 0) 
		: packedPel_(0) { r(pr); g(pg); b(pb); a(pa); }

	unsigned char a() const { return (unsigned char)((packedPel_ & 0xFF000000) >> 24); }
	unsigned char r() const { return (unsigned char)((packedPel_ & 0x00FF0000) >> 16); }
	unsigned char g() const { return (unsigned char)((packedPel_ & 0x0000FF00) >> 8);  }
	unsigned char b() const { return (unsigned char)(packedPel_ & 0x000000FF);         }
	unsigned char gray() const
	{
		unsigned long s = r();
		s += 2 * g();
		s += b();
		s /= 4;
		return (unsigned char) (s & 0xff);
	}

	void a(unsigned char aval) { *(((unsigned char*)&packedPel_) + 3) = aval; }
	void r(unsigned char rval) { *(((unsigned char*)&packedPel_) + 2) = rval; }
	void g(unsigned char gval) { *(((unsigned char*)&packedPel_) + 1) = gval; }
	void b(unsigned char bval) { *(((unsigned char*)&packedPel_))     = bval; }

	operator const unsigned long& () const { return  packedPel_; };
	operator unsigned long& () { return  packedPel_; };

	bool operator==(cpPackedRGB32Pixel b) { return packedPel_ == b.packedPel_; }
	bool operator!=(cpPackedRGB32Pixel b) { return packedPel_ != b.packedPel_; }

	friend class cpConvertPixel;

private:
	unsigned long packedPel_;
};

#pragma pack(pop)

#define HImageReturnStatus cpImage::ReturnStatus

class SVCORE_API cpImage
{
public:
	enum ReturnStatus
	{
		statusSuccess = 0,
		statusInvalidImage = 1,
		statusInvalidImageFormat = 2,
		statusInvalidImageSize = 3,
		statusInvalidImageRegion = 4,
	};

public:
	//构造函数
	cpImage(void);

	//析构函数
	~cpImage(void);

	//拷贝构造函数
	cpImage(const cpImage& image);

	//根据尺寸、对齐模式构造图像
	cpImage(long width, long height, epPixelFormat pixelFormat, long alignModulus=1);

	//根据尺寸、对齐模式构造图像，并根据buffer初始化图像数据段（buffer为连续的原始图像数据，不存在对齐问题）
	cpImage(const void* buffer, long width, long height, epPixelFormat pixelFormat, long alignModulus=1);

	//根据尺寸、对齐模式构造图像，根据单个像素值初始化图像(单个像素类型决定了图像像素格式PixelFormat)
	cpImage(unsigned char value8, long width, long height, long alignModulus=1);
	cpImage(unsigned short value16, long width, long height, long alignModulus=1);
	cpImage(cpPackedRGB24Pixel value24, long width, long height, long alignModulus=1);
	cpImage(cpPackedRGB32Pixel value32, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存
	void CreateImageBuffer(long width, long height, epPixelFormat pixelFormat, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存，并根据buffer初始化图像数据段（buffer为连续的原始图像数据，不存在对齐问题）
	void CreateImageBuffer(const void* buffer, long width, long height, epPixelFormat pixelFormat, long alignModulus=1);
	
	//根据尺寸、对齐模式构造图像，根据单个像素值初始化图像(单个像素类型决定了图像像素格式PixelFormat)
	void CreateImageBuffer(unsigned char value8, long width, long height, long alignModulus=1);
	void CreateImageBuffer(unsigned short value16, long width, long height, long alignModulus=1);
	void CreateImageBuffer(cpPackedRGB24Pixel value24, long width, long height, long alignModulus=1);
	void CreateImageBuffer(cpPackedRGB32Pixel value32, long width, long height, long alignModulus=1);

	//复制当前图像到image
	//如果图像无效，则返回statusInvalidImage；否则，返回statusSuccess
	HImageReturnStatus Clone(cpImage& image) const;

	//重载运算符=
	cpImage& operator=(const cpImage& image);

	//释放图像数据段，返回初始状态
	void Release();

	//图像宽度
	long Width() const { return m_lWidth; }

	//图像高度
	long Height() const { return m_lHeight; }

	//图像行尺寸，即图像的实际宽度（大于等于图像宽度）
	//当alignModulus=1时，图像的实际宽度=图像宽度；当alignModulus>1时，图像的实际宽度>=图像宽度
	long RowUpdate() const { return m_lWidthPadded; }

	//对齐模式
	long AlignModulus() const { return m_lAlignModulus; }

	//图像像素类型
	epPixelFormat PixelFormat() const { return m_PixelFormat; }

	//单个像素占用的内存字节数
	long PixelSize() const { return m_lPixelSize; }

	//一行占用的内存字节数
	long RowSize() const { return m_lWidthPadded*m_lPixelSize; }

	//图像数据所占内存字节数
	long ImageBufferSize() const { return m_lWidthPadded*m_lHeight*m_lPixelSize; }

	//判断图像是否有效
	//如果图像数据为空或者尺寸小于等于0，则返回false；否则，返回true
	bool IsValid() const { return ( (m_PixelFormat != epUnknown) && (m_pRawStorage != NULL) && (m_lWidthPadded > 0) && (m_lWidth > 0) && (m_lHeight > 0) && (m_lPixelSize >= 1) && (m_lPixelSize <= 4) ); }
	bool IsGray8() const { return ( (m_PixelFormat == epGray8) ); }
	bool IsGray16() const { return ( (m_PixelFormat == epGray16) ); }
	bool IsRGB24() const { return ( (m_PixelFormat == epPackedRGB24) ); }
	bool IsRGB32() const { return ( (m_PixelFormat == epPackedRGB32) ); }
	bool IsValidGray8() const { return ( (IsGray8()) && (IsValid()) ); }
	bool IsValidGray16() const { return ( (IsGray16()) && (IsValid()) ); }
	bool IsValidRGB24() const { return ( (IsRGB24()) && (IsValid()) ); }
	bool IsValidRGB32() const { return ( (IsRGB32()) && (IsValid()) ); }

	//拷贝子图像
	HImageReturnStatus GetSubImage(cpImage& image, const scRect& rect) const;

	//获取灰度图像
	HImageReturnStatus GetGrayImage(cpImage& image) const;

	//获取彩色图像通道
	HImageReturnStatus ChannelR(cpImage& image) const;
	HImageReturnStatus ChannelG(cpImage& image) const;
	HImageReturnStatus ChannelB(cpImage& image) const;

	//新旧两种图像类转换
	HImageReturnStatus GetImage8(scImage8& image8) const;
	HImageReturnStatus GetImage16(scImage16& image16) const;
	HImageReturnStatus GetImage24(scImage24& image24) const;
	HImageReturnStatus GetImage32(scImage32& image32) const;
	cpImage(scImage8& image8);
	void CreateImageBuffer(scImage8& image8);
	cpImage& operator=(const scImage8& image8);
	
	//图像的行首指针
	inline void* PointToRow(long row=0) const;

	//图像中(x, y)像素对应的指针
	inline void* PointToPixel(long x, long y) const;

	//设置/获取(x, y)像素灰度值
	inline unsigned char GetPixelGray8(long x, long y) const;
	inline void SetPixelGray8(long x, long y, unsigned char value);
	inline unsigned short GetPixelGray16(long x, long y) const;
	inline void SetPixelGray16(long x, long y, unsigned short value);
	inline cpPackedRGB24Pixel GetPixelRGB24(long x, long y) const;
	inline void SetPixelRGB24(long x, long y, cpPackedRGB24Pixel value);
	inline cpPackedRGB32Pixel GetPixelRGB32(long x, long y) const;
	inline void SetPixelRGB32(long x, long y, cpPackedRGB32Pixel value);

	//与数据绑定或解绑
	bool SwitchForm(scImage8& image8);
	bool SwitchForm(scImage16& image16);
	bool SwitchForm(scImage24& image24);
	bool SwitchForm(scImage32& image32);
	void Attach(void* pBuffer);
	unsigned char* Disattach();

protected:
	void*	m_pRawStorage;			// 图像数据
	long	m_lWidth;				// 图像宽度
	long	m_lHeight;				// 图像高度
	long	m_lAlignModulus;		// 图像对齐模式
	long	m_lWidthPadded;			// 图像实际宽度（由于对齐的需要，可能对图像行数据进行扩展，因此，图像实际宽度 >= 图像宽度）
	long	m_lPixelSize;			// 图像单像素占用的内存字节数（Gray:1，RGB:3）
	epPixelFormat m_PixelFormat;	// 像素格式
};

//图像的行首指针
inline void* cpImage::PointToRow(long row) const
{
	if (!IsValid())
	{
		return NULL;
	}

	assert( (row >= 0) && (row < m_lHeight) );

	return ( ((unsigned char*)m_pRawStorage)  + row * m_lWidthPadded * m_lPixelSize );
}

//图像中(x, y)像素对应的指针
inline void* cpImage::PointToPixel(long x, long y) const
{
	if (!IsValid())
	{
		return NULL;
	}

	assert( (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	return ( ((unsigned char*)m_pRawStorage) + (y * m_lWidthPadded + x) * m_lPixelSize );
}

inline unsigned char cpImage::GetPixelGray8(long x, long y) const
{
	if (!IsValidGray8())
	{
		return 0;
	}

	assert( (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	unsigned char* pPixel = ( ((unsigned char*)m_pRawStorage) + (y * m_lWidthPadded + x) * m_lPixelSize );

	return *pPixel;
}

inline void cpImage::SetPixelGray8(long x, long y, unsigned char value)
{
	if (!IsValidGray8())
	{
		return;
	}

	assert( (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	unsigned char* pPixel = ( ((unsigned char*)m_pRawStorage) + (y * m_lWidthPadded + x) * m_lPixelSize );
	
	*pPixel = value;

	return;
}

inline unsigned short cpImage::GetPixelGray16(long x, long y) const
{
	if (!IsValidGray16())
	{
		return 0;
	}

	assert( (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	unsigned short* pPixel = (unsigned short*)( ((unsigned char*)m_pRawStorage) + (y * m_lWidthPadded + x) * m_lPixelSize );

	return *pPixel;
}

inline void cpImage::SetPixelGray16(long x, long y, unsigned short value)
{
	if (!IsValidGray16())
	{
		return;
	}

	assert( (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	unsigned short* pPixel = (unsigned short*)( ((unsigned char*)m_pRawStorage) + (y * m_lWidthPadded + x) * m_lPixelSize );

	*pPixel = value;

	return;
}

inline cpPackedRGB24Pixel cpImage::GetPixelRGB24(long x, long y) const
{
	if (!IsValidRGB24())
	{
		return 0;
	}

	assert( (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	cpPackedRGB24Pixel* pPixel = (cpPackedRGB24Pixel*)( ((unsigned char*)m_pRawStorage) + (y * m_lWidthPadded + x) * m_lPixelSize );

	return *pPixel;
}

inline void cpImage::SetPixelRGB24(long x, long y, cpPackedRGB24Pixel value)
{
	if (!IsValidRGB24())
	{
		return;
	}

	assert( (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	cpPackedRGB24Pixel* pPixel = (cpPackedRGB24Pixel*)( ((unsigned char*)m_pRawStorage) + (y * m_lWidthPadded + x) * m_lPixelSize );

	*pPixel = value;

	return;
}

inline cpPackedRGB32Pixel cpImage::GetPixelRGB32(long x, long y) const
{
	if (!IsValidRGB32())
	{
		return 0;
	}

	assert( (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	cpPackedRGB32Pixel* pPixel = (cpPackedRGB32Pixel*)( ((unsigned char*)m_pRawStorage) + (y * m_lWidthPadded + x) * m_lPixelSize );

	return *pPixel;
}

inline void cpImage::SetPixelRGB32(long x, long y, cpPackedRGB32Pixel value)
{
	if (!IsValidRGB32())
	{
		return;
	}

	assert( (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	cpPackedRGB32Pixel* pPixel = (cpPackedRGB32Pixel*)( ((unsigned char*)m_pRawStorage) + (y * m_lWidthPadded + x) * m_lPixelSize );

	*pPixel = value;

	return;
}

class SVCORE_API cpConvertPixel
{
public:
	inline void ConvertPixel (const unsigned char& f, unsigned char& t) 
	{
		t = f;
	};

	inline void ConvertPixel (const cpPackedRGB24Pixel& f, unsigned char& t) 
	{ 
		unsigned long s = f.r();
		s += 2 * f.g();
		s += f.b();
		s /= 4;
		t = (unsigned char) (s & 0xff);
	};

	inline void ConvertPixel (const cpPackedRGB32Pixel& f, unsigned char& t)
	{ 
		unsigned long s = f.r();
		s += 2 * f.g(); 
		s += f.b();
		s /= 4;
		t = (unsigned char) (s & 0xff);
	};

	inline void ConvertPixel (const unsigned char& f, cpPackedRGB24Pixel &t) 
	{
		t.r(f); t.g(f); t.b(f);
	};

	inline void ConvertPixel (const cpPackedRGB24Pixel& f, cpPackedRGB24Pixel& t) 
	{
		t = f;
	};

	inline void ConvertPixel (const cpPackedRGB32Pixel& f, cpPackedRGB24Pixel &t) 
	{
		t.r(f.r()); t.g(f.g()); t.b(f.b());
	};

	inline void ConvertPixel (const unsigned char& f, cpPackedRGB32Pixel &t)
	{
		unsigned long q = f;
		t.packedPel_ = (q << 24) | (q << 16) | (q << 8) | q;
	};

	inline void ConvertPixel (const cpPackedRGB24Pixel& f, cpPackedRGB32Pixel &t)
	{
		t.packedPel_ = (f.r() << 16) | (f.g() << 8) | (f.b());
	};

	inline void ConvertPixel (const cpPackedRGB32Pixel& f, cpPackedRGB32Pixel &t) 
	{
		t = f;
	};

	inline void ConvertPixel(const unsigned char& f, unsigned short &t)
	{
		unsigned short q = f;
		t = q << 8;
	};

	inline void ConvertPixel(const cpPackedRGB24Pixel& f, unsigned short &t)
	{
		unsigned long s = f.r();
		s += 2 * f.g();
		s += f.b();
		s /= 4;
		t = (unsigned short) (s & 0xffff);
	};

	inline void ConvertPixel(const cpPackedRGB32Pixel& f, unsigned short &t)
	{
		unsigned long s = f.r();
		s += 2 * f.g(); 
		s += f.b();
		s /= 4;
		t = (unsigned short) (s & 0xffff);
	};

	inline void ConvertPixel(const unsigned short& f, unsigned char &t)
	{
		t = f >> 8;
	};

	inline void ConvertPixel(const unsigned short& f, cpPackedRGB24Pixel &t)
	{
		unsigned char c = ((f & 0xff00) >> 8);
		t.r(c); t.g(c); t.b(c);
	};

	inline void ConvertPixel(const unsigned short& f, cpPackedRGB32Pixel &t)
	{
		unsigned long s = ((f & 0xff00) >> 8);
		t.packedPel_ = (s << 24) | (s << 16) | (s << 8) | s;
	};
};

class SVCORE_API cpBayerFilter
{
public:
	bool Bayer2Gray8 (const cpImage& src, cpImage& dst, epBayerType type);

	bool Bayer2RGB24 (const cpImage& src, cpImage& dst, epBayerType type);
};

bool SVCORE_API CombineImage(cpImage imgHeight, cpImage imgLuminance, cpImage& imgCombine);
bool SVCORE_API SeparateImage(cpImage& imgCombine, cpImage imgHeight, cpImage imgLuminance);

#pragma pack(pop)