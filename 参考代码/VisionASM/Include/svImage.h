//该文件中所有接口已废弃使用，请使用vpImage.h中接口

#pragma once

#include "svDef.h"
#include "svError.h"
#include <cassert>

#pragma pack(push,8)

class scRect;

class scImage;
class scImage8;
class scImage24;
class scPackedRGB24Pixel;
class scPackedRGB32Pixel;
class cpImage;

enum svBayerType
{
	eBayerRG8 = 0,
	eBayerGB8 = 1,
	eBayerGR8 = 2,
	eBayerBG8 = 3,
};

class SVCORE_API scPackedRGB24Pixel
{

public:
	scPackedRGB24Pixel(unsigned long ppel=0) 
	{ 
		unsigned char val = (unsigned char) ((ppel & 0xFF0000) >> 16); r(val); 
		val = (unsigned char) ((ppel & 0x00FF00) >> 8); g(val); 
		val = (unsigned char) (ppel & 0x0000FF); b(val);   
	}

	scPackedRGB24Pixel(unsigned char pr, unsigned char pg, unsigned char pb) 
	{ r(pr); g(pg); b(pb); }

	unsigned char r() const { return r_; }
	unsigned char g() const { return g_; }
	unsigned char b() const { return b_; }

	void r(unsigned char rval) { r_ = rval; }
	void g(unsigned char gval) { g_ = gval; }
	void b(unsigned char bval) { b_ = bval; }

	void cvt() { unsigned char t_ = r_; r_ = b_; b_ = t_; }

	bool operator==(scPackedRGB24Pixel v)
	{ return (v.r() == r() && v.g() == g() && v.b() == b()); }
	bool operator!=(scPackedRGB24Pixel v)
	{ return (v.r() != r() || v.g() != g() || v.b() != b()); }

private:
	unsigned char b_;
	unsigned char g_;
	unsigned char r_;
};

class SVCORE_API scPackedRGB32Pixel
{

public:
	scPackedRGB32Pixel(unsigned long ppel=0) : packedPel_(ppel) {}

	scPackedRGB32Pixel(unsigned char pr, unsigned char pg, unsigned char pb, unsigned char pa = 0) 
		: packedPel_(0) { r(pr); g(pg); b(pb); a(pa); }

	unsigned char a() const { return (unsigned char)((packedPel_ & 0xFF000000) >> 24); }
	unsigned char r() const { return (unsigned char)((packedPel_ & 0x00FF0000) >> 16); }
	unsigned char g() const { return (unsigned char)((packedPel_ & 0x0000FF00) >> 8);  }
	unsigned char b() const { return (unsigned char)(packedPel_ & 0x000000FF);         }

	void a(unsigned char aval) { *(((unsigned char*)&packedPel_) + 3) = aval; }
	void r(unsigned char rval) { *(((unsigned char*)&packedPel_) + 2) = rval; }
	void g(unsigned char gval) { *(((unsigned char*)&packedPel_) + 1) = gval; }
	void b(unsigned char bval) { *(((unsigned char*)&packedPel_))     = bval; }

	operator const unsigned long& () const { return  packedPel_; };
	operator unsigned long& () { return  packedPel_; };

	bool operator==(scPackedRGB32Pixel b) { return packedPel_ == b.packedPel_; }
	bool operator!=(scPackedRGB32Pixel b) { return packedPel_ != b.packedPel_; }

	friend inline void sfConvertPixel (scPackedRGB32Pixel &t, const unsigned char& f);
	friend inline void sfConvertPixel (scPackedRGB32Pixel &t, const scPackedRGB24Pixel& f);
	friend inline void sfConvertPixel (scPackedRGB32Pixel &t, const scPackedRGB32Pixel& f);

private:
	unsigned long packedPel_;
};

class SVCORE_API scImage
{
public:
	//图像宽度
	long Width() const { return m_lWidth; }

	//图像高度
	long Height() const { return m_lHeight; }

	//图像行尺寸，即图像的实际宽度（大于等于图像宽度）
	//当alignModulus=1时，图像的实际宽度=图像宽度；当alignModulus>1时，图像的实际宽度>=图像宽度
	long RowUpdate() const { return m_lWidthPadded; }

	//对齐模式
	long AlignModulus() const { return m_lAlignModulus; }

	//单个像素占用的内存字节数
	long PixelSize() const { return m_lPixelSize; }

	//一行占用的内存字节数
	long RowSize() const { return m_lWidthPadded*m_lPixelSize; }

	//图像数据所占内存字节数
	long ImageBufferSize() const { return m_lWidthPadded*m_lHeight*m_lPixelSize; }

	//图像的行首指针
	inline unsigned char* PointToRow(long row=0) const;

	//图像中(x, y)像素对应的指针
	inline unsigned char* PointToPixel(long x, long y) const;

	//判断图像是否有效
	//如果图像数据为空或者尺寸小于等于0，则返回false；否则，返回true
	bool IsValid() const { return ( (m_pRawStorage != NULL) && (m_lWidthPadded > 0) && (m_lWidth > 0) && (m_lHeight > 0) ); }

	//释放图像数据段，返回初始状态
	virtual void Release() = 0;

	//与数据绑定或解绑
	inline void Attach(unsigned char* pBuffer);
	inline void* Disattach();

protected:
	unsigned char* m_pRawStorage;	// 图像数据
	long	m_lWidth;				// 图像宽度
	long	m_lHeight;				// 图像高度
	long	m_lAlignModulus;		// 图像对齐模式
	long	m_lWidthPadded;			// 图像实际宽度（由于对齐的需要，可能对图像行数据进行扩展，因此，图像实际宽度 >= 图像宽度）
	long	m_lPixelSize;			// 图像单像素占用的内存字节数（Gray:8，RGB:24）

	bool	m_bSwitch;
};

inline unsigned char* scImage::PointToRow(long row) const
{
	assert( (m_pRawStorage != NULL) && (row >= 0) && (row < m_lHeight) );

	return (m_pRawStorage + row * m_lWidthPadded * m_lPixelSize);
}

inline unsigned char* scImage::PointToPixel(long x, long y) const
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	return (m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize);
}

inline void scImage::Attach(unsigned char* pBuffer)
{
	if (m_pRawStorage == NULL)
	{
		m_pRawStorage = pBuffer;
	}
}

inline void* scImage::Disattach()
{
	void* pReturn = (void*)m_pRawStorage;

	m_pRawStorage = NULL;
	m_bSwitch = false;

	return pReturn;
}

class SVCORE_API scImage8 : public scImage
{
	friend class scImage24;
public:
	scImage8(void);

	~scImage8(void);

	//拷贝构造函数
	scImage8(const scImage8& image);

	//根据尺寸、对齐模式构造图像
	scImage8(long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式构造图像，并初始化图像数据段值为value
	scImage8(unsigned char value, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式构造图像，并根据buffer初始化图像数据段（buffer为连续的原始图像数据，不存在对齐问题）
	scImage8(const unsigned char* buffer, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存
	void CreateImageBuffer(long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存，并初始化图像数据段值为value
	void CreateImageBuffer(unsigned char value, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存，并根据buffer初始化图像数据段（buffer为连续的原始图像数据，不存在对齐问题）
	void CreateImageBuffer(const unsigned char* buffer,long width, long height, long alignModulus=1);

	//设置图像数据段值为value
	//如果图像无效，则返回HInvalidImage；否则，返回HSuccess
	HStatus SetImageValue(unsigned char value);

	//复制当前图像到image
	//如果图像无效，则返回HInvalidImage；否则，返回HSuccess
	HStatus Clone(scImage8& image) const;

	//拷贝子图像
	HStatus GetSubImage(scImage8& image, const scRect& rect) const;

	scImage8& operator=(const scImage8& image);

	//释放图像数据段，返回初始状态
	virtual void Release();

	//设置/获取(x, y)像素灰度值
	inline unsigned char GetPixelValue(long x, long y) const;
	inline void SetPixelValue(long x, long y, unsigned char value);
	inline unsigned char GetPixel(long x, long y) const {return GetPixelValue(x,y);};
	inline void SetPixel(long x, long y, unsigned char value) {return SetPixelValue(x,y,value);};

	//转换
	bool SwitchForm(cpImage& image);
};

inline unsigned char scImage8::GetPixelValue(long x, long y) const
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	return *(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize);
}

inline void scImage8::SetPixelValue(long x, long y, unsigned char value)
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	*(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize) = value;
}

class SVCORE_API scImage16 : public scImage
{
public:
	scImage16(void);

	~scImage16(void);

	//拷贝构造函数
	scImage16(const scImage16& image);

	//根据尺寸、对齐模式构造图像
	scImage16(long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式构造图像，并初始化图像数据段值为value
	scImage16(unsigned short value, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式构造图像，并根据buffer初始化图像数据段（buffer为连续的原始图像数据，不存在对齐问题）
	scImage16(const unsigned short* buffer, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存
	void CreateImageBuffer(long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存，并初始化图像数据段值为value
	void CreateImageBuffer(unsigned short value, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存，并根据buffer初始化图像数据段（buffer为连续的原始图像数据，不存在对齐问题）
	void CreateImageBuffer(const unsigned short* buffer, long width, long height, long alignModulus=1);

	//设置图像数据段值为value
	//如果图像无效，则返回HInvalidImage；否则，返回HSuccess
	HStatus SetImageValue(unsigned short value);

	//复制当前图像到image
	//如果图像无效，则返回HInvalidImage；否则，返回HSuccess
	HStatus Clone(scImage16& image) const;

	//拷贝子图像
	HStatus GetSubImage(scImage16& image, const scRect& rect) const;

	scImage16& operator=(const scImage16& image);

	//释放图像数据段，返回初始状态
	virtual void Release();

	//设置/获取(x, y)像素灰度值
	inline unsigned short GetPixelValue(long x, long y) const;
	inline void SetPixelValue(long x, long y, unsigned short value);

	//转换
	bool SwitchForm(cpImage& image);
};

inline unsigned short scImage16::GetPixelValue(long x, long y) const
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	return *((unsigned short*)(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize));
}

inline void scImage16::SetPixelValue(long x, long y, unsigned short value)
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	*((unsigned short*)(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize)) = value;
}

class SVCORE_API scImage24 : public scImage
{
public:
	scImage24(void);

	~scImage24(void);

	//拷贝构造函数
	scImage24(const scImage24& image);

	//根据尺寸、对齐模式构造图像
	scImage24(long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式构造图像，并初始化图像数据段值为value
	scImage24(unsigned char r, unsigned char g, unsigned char b, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式构造图像，并根据buffer初始化图像数据段（buffer为连续的原始图像数据，不存在对齐问题）
	scImage24(const unsigned char* buffer, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存
	void CreateImageBuffer(long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存，并初始化图像数据段值为value
	void CreateImageBuffer(unsigned char r, unsigned char g, unsigned char b, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存，并根据buffer初始化图像数据段（buffer为连续的原始图像数据，不存在对齐问题）
	void CreateImageBuffer(const unsigned char* buffer, long width, long height, long alignModulus=1);

	//设置图像数据段值为value
	//如果图像无效，则返回HInvalidImage；否则，返回HSuccess
	HStatus SetImageValue(unsigned char r, unsigned char g, unsigned char b);

	//复制当前图像到image
	//如果图像无效，则返回HInvalidImage；否则，返回HSuccess
	HStatus Clone(scImage24& image) const;

	//拷贝子图像
	HStatus GetSubImage(scImage24& image, const scRect& rect) const;

	//获取灰度图像
	HStatus GetGrayImage(scImage8& image) const;

	scImage24& operator=(const scImage24& image);

	//释放图像数据段，返回初始状态
	virtual void Release();

	//拷贝单通道数据为灰度图像
	HStatus ChannelR(scImage8& image) const;
	HStatus ChannelG(scImage8& image) const;
	HStatus ChannelB(scImage8& image) const;

	//设置/获取(x, y)像素灰度值
	inline void GetPixelValue(long x, long y, unsigned char& r, unsigned char& g, unsigned char& b) const;
	inline void SetPixelValue(long x, long y, unsigned char r, unsigned char g, unsigned char b);
	inline scPackedRGB24Pixel GetPixel(long x, long y) const;
	inline void SetPixel(long x, long y, scPackedRGB24Pixel p);

	//RGB<->BGR
	inline void CVT();

	//转换
	bool SwitchForm(cpImage& image);
};

inline void scImage24::GetPixelValue(long x, long y, unsigned char& r, unsigned char& g, unsigned char& b) const
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	b =  *(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize);
	g =  *(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize + 1);
	r =  *(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize + 2);
}

inline void scImage24::SetPixelValue(long x, long y, unsigned char r, unsigned char g, unsigned char b)
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	*(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize) = b;
	*(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize + 1) = g;
	*(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize + 2) = r;
}

inline scPackedRGB24Pixel scImage24::GetPixel(long x, long y) const
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	unsigned char b =  *(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize);
	unsigned char g =  *(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize + 1);
	unsigned char r =  *(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize + 2);

	return scPackedRGB24Pixel(r,g,b);
}

inline void scImage24::SetPixel(long x, long y, scPackedRGB24Pixel p)
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	*(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize) = p.b();
	*(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize + 1) = p.g();
	*(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize + 2) = p.r();

}

inline void scImage24::CVT()
{
	if (!IsValid())
	{
		return;
	}

	for (long y = 0; y<m_lHeight; y++)
	{
		for (long x = 0; x<m_lWidth; x++)
		{
			unsigned char t = *(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize);
			*(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize) = *(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize + 2);
			*(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize + 2) = t;
		}
	}
}

class SVCORE_API scImage32 : public scImage
{
public:
	scImage32(void);

	~scImage32(void);

	//拷贝构造函数
	scImage32(const scImage32& image);

	//根据尺寸、对齐模式构造图像
	scImage32(long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式构造图像，并初始化图像数据段值为value
	scImage32(unsigned long value, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式构造图像，并根据buffer初始化图像数据段（buffer为连续的原始图像数据，不存在对齐问题）
	scImage32(const unsigned long* buffer, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存
	void CreateImageBuffer(long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存
	void CreateImageBuffer(unsigned long value, long width, long height, long alignModulus=1);

	//根据尺寸、对齐模式创建图像内存
	void CreateImageBuffer(const unsigned long* buffer, long width, long height, long alignModulus=1);

	//设置图像数据段值为value
	//如果图像无效，则返回HInvalidImage；否则，返回HSuccess
	HStatus SetImageValue(unsigned long value);

	//复制当前图像到image
	//如果图像无效，则返回HInvalidImage；否则，返回HSuccess
	HStatus Clone(scImage32& image) const;

	//拷贝子图像
	HStatus GetSubImage(scImage32& image, const scRect& rect) const;

	scImage32& operator=(const scImage32& image);

	//释放图像数据段，返回初始状态
	virtual void Release();

	//设置/获取(x, y)像素灰度值
	inline unsigned long GetPixelValue(long x, long y) const;
	inline void SetPixelValue(long x, long y, unsigned long value);
	inline scPackedRGB32Pixel GetPixel(long x, long y) const;
	inline void SetPixel(long x, long y, scPackedRGB32Pixel p);

	//转换
	bool SwitchForm(cpImage& image);
};

inline unsigned long scImage32::GetPixelValue(long x, long y) const
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	return *((unsigned long*)(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize));
}

inline void scImage32::SetPixelValue(long x, long y, unsigned long value)
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	*((unsigned long*)(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize)) = value;
}

inline scPackedRGB32Pixel scImage32::GetPixel(long x, long y) const
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	return scPackedRGB32Pixel(*((unsigned long*)(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize)));
}

inline void scImage32::SetPixel(long x, long y, scPackedRGB32Pixel p)
{
	assert( (m_pRawStorage != NULL) && (x >= 0) && (x < m_lWidth) && (y >= 0) && (y < m_lHeight) );

	*((unsigned long*)(m_pRawStorage + (y * m_lWidthPadded + x) * m_lPixelSize)) = (p);
}

inline void sfConvertPixel (unsigned char& t, const unsigned char& f) 
{
	t = f;
} 

inline void sfConvertPixel (unsigned char& t, const scPackedRGB24Pixel& f) 
{ 
	unsigned long s = f.r();
	s += 2 * f.g();
	s += f.b();
	s /= 4;
	t = (unsigned char) (s & 0xff);
}

inline void sfConvertPixel (unsigned char& t, const scPackedRGB32Pixel& f)
{ 
	unsigned long s = f.r();
	s += 2 * f.g(); 
	s += f.b();
	s /= 4;
	t = (unsigned char) (s & 0xff);
}

inline void sfConvertPixel (scPackedRGB24Pixel &t, const unsigned char& f) 
{
	t.r(f); t.g(f); t.b(f);
}

inline void sfConvertPixel (scPackedRGB24Pixel& t, const scPackedRGB24Pixel& f) 
{
	t = f;
}

inline void sfConvertPixel (scPackedRGB24Pixel &t, const scPackedRGB32Pixel& f) 
{
	t.r(f.r()); t.g(f.g()); t.b(f.b());
}

inline void sfConvertPixel (scPackedRGB32Pixel &t, const unsigned char& f)
{
	unsigned long q = f;
	t.packedPel_ = (q << 24) | (q << 16) | (q << 8) | q;
}

inline void sfConvertPixel (scPackedRGB32Pixel &t, const scPackedRGB24Pixel& f)
{
	t.packedPel_ = (f.r() << 16) | (f.g() << 8) | (f.b());
}

inline void sfConvertPixel (scPackedRGB32Pixel &t, const scPackedRGB32Pixel& f) 
{
	t = f;
}

inline void sfConvertPixel (unsigned char &t, const unsigned short& f) 
{
	t = (f & 0xFF00) >> 8;
}

inline void sfConvertPixel (unsigned short &t, const unsigned char& f) 
{
	unsigned short s = f;
	t = s << 8 | s;
}

inline void sfConvertPixel (unsigned short &t, const scPackedRGB24Pixel& f) 
{
	unsigned long s = f.r();
	s += 2 * f.g();
	s += f.b();
	s /= 4;
	t = (unsigned short) (s & 0xFFFF);
}

inline void sfConvertPixel (scPackedRGB24Pixel &t, const unsigned short& f) 
{
	unsigned char s = (f & 0xFF00) >> 8;
	t.b(s); t.g(s); t.r(s);
}

inline void sfCopyImage(scImage* pDst, const scImage* pSrc)
{
	if ( (pDst == NULL) || (pSrc == NULL) || (pDst == pSrc) )
	{
		return;
	}

	if (pDst->PixelSize() != pSrc->PixelSize())
	{
		return;
	}

	switch (pDst->PixelSize())
	{
	case 1:
		{
			scImage8* pDst8 = (scImage8*)pDst;
			scImage8* pSrc8 = (scImage8*)pSrc;
			*pDst8 = *pSrc8;
		}
		break;
	case 2:
		{
			scImage16* pDst16 = (scImage16*)pDst;
			scImage16* pSrc16 = (scImage16*)pSrc;
			*pDst16 = *pSrc16;
		}
		break;
	case 3:
		{
			scImage24* pDst24 = (scImage24*)pDst;
			scImage24* pSrc24 = (scImage24*)pSrc;
			*pDst24 = *pSrc24;
		}
		break;
	case 4:
		{
			scImage32* pDst32 = (scImage32*)pDst;
			scImage32* pSrc32 = (scImage32*)pSrc;
			*pDst32 = *pSrc32;
		}
		break;
	default:
		break;
	}
}

typedef scImage8 scImageGray;
typedef scImage24 scImageRGB;

bool SVCORE_API sfBayer2RGB24 (const scImage8& src, scImage24& dst, svBayerType type);

#pragma pack(pop)
