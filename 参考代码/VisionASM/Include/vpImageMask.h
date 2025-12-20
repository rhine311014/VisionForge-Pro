#pragma once

#include "svDef.h"
#include "svShape.h"
#include "svRect.h"
#include "vpImage.h"
#include <cassert>

#pragma pack(push,8)

#define HImageMaskReturnStatus cpImageMask::ReturnStatus

class SVCORE_API cpImageMask
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
	cpImageMask(void);

	//析构函数
	~cpImageMask(void);

	//拷贝构造函数
	cpImageMask(const cpImageMask& imagemask);

	//根据尺寸、对齐模式构造图像
	cpImageMask(long offsetx, long offsety, long width, long height, unsigned char ucMaskValue);

	//创建掩模图像
	bool CreateImageMask(long offsetx, long offsety, long width, long height, unsigned char ucMaskValue);
	bool CreateImageMask(long offsetx, long offsety, long width, long height, void* pImageBuffer);
	bool CreateImageMask(const scShape* roi,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX);

	//复制当前掩模图像到imagemask
	//如果图像无效，则返回statusInvalidImage；否则，返回statusSuccess
	HImageMaskReturnStatus Clone(cpImageMask& imagemask) const;

	//重载运算符=
	cpImageMask& operator=(const cpImageMask& imagemask);

	//释放图像数据段，返回初始状态
	void Release();

	//掩模图像起始X
	long OffsetX() const { return m_lOffsetX; }

	//掩模图像起始Y
	long OffsetY() const { return m_lOffsetY; }

	//掩模图像宽度
	long Width() const { return m_ImageMask.Width(); }

	//掩模图像高度
	long Height() const { return m_ImageMask.Height(); }

	//掩模区域
	scRect GetMaskRect() const { return scRect(m_lOffsetX,m_lOffsetY,m_ImageMask.Width(),m_ImageMask.Height()); }

	//掩模图像数据
	const cpImage& GetImageMask() const { return m_ImageMask; }

	//判断图像数据是否有效
	//如果图像数据为空或者尺寸小于等于0，则返回false；否则，返回true
	bool IsValid() const { return m_ImageMask.IsValidGray8(); }

	//与另一副掩模图像做与操作,两幅图像同时有效(255)的地方最终判定为有效（区域默认变成两个区域的并集；bIntersectRegion=true则变成两个区域的交集）
	bool And(const cpImageMask& imagemask, cpImageMask& result, bool bIntersectRegion = false);

	//与另一副掩模图像做或操作,两幅图像任一有效(255)的地方最终判定为有效（区域变成两个区域的并集）
	bool Or(const cpImageMask& imagemask, cpImageMask& result);

	//与另一副掩模图像做异或操作,两幅图像一个有效(255)一个无效(0)的地方最终判定为有效（区域默认变成两个区域的并集；bIntersectRegion=true则变成两个区域的交集）
	bool Xor(const cpImageMask& imagemask, cpImageMask& result, bool bIntersectRegion = false);

	//取反操作
	bool Inverse(cpImageMask& result);

	//旋转平移操作
	bool Translation(double offsetx, double offsety, cpImageMask& result);
	bool Rotate(double centerx, double centery, scRadian rRotate, cpImageMask& result);
	bool RotateTranslation(double centerx, double centery, scRadian rRotate, double offsetx, double offsety, cpImageMask& result);

public:
	long	m_lOffsetX;
	long	m_lOffsetY;

	cpImage m_ImageMask;
};

#pragma pack(pop)