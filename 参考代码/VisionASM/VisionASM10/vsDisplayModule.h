// vsDisplayModule.h: interface for the vcDisplayModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVDISPLAYMODULE_H__68D2F18D_7570_4B84_BB00_3DAD5D14AFA5__INCLUDED_)
#define AFX_SVDISPLAYMODULE_H__68D2F18D_7570_4B84_BB00_3DAD5D14AFA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #ifdef SV_IMAGETRANSFORM_EXPORTS
// #define SV_IMAGETRANSFORM_API _declspec(dllexport)
// #else
// #define SV_IMAGETRANSFORM_API _declspec(dllimport)
// #endif 

#include "svDef.h"
#include "svImage.h"
#include "svVector.h"
#include "svRect.h"
#include "vpGammaCorrection.h"

#pragma pack(push,8)

class /*SV_IMAGETRANSFORM_API*/ vcDisplayModule  
{
public:
	vcDisplayModule();
	virtual ~vcDisplayModule();

	// 设置/获取是否启用Gamma校正
	void SetGammaCorrectionEnable(bool bEnable);
	bool GetGammaCorrectionEnable() const;

	// 设置/获取Gamma校正参数
	void SetGammaPara(double dGamma);
	double GetGammaPara();

	// 设置/获取是否Bayer滤波
	void SetBayerFilterEnable(bool bEnable);
	bool GetBayerFilterEnable() const;

	// 设置/获取Bayer滤波参数
	void SetBayerFilterType(epBayerType type);
	epBayerType GetBayerFilterType() const;

	// 设置/获取旋转角度
	void SetRotationType(RotateType rotateType);
	void GetRotationType (RotateType& rotateType) const;
	
	// 设置/获取镜像变换方式
	void SetMirrorType(MirrorType mirrorType = (MirrorType)(0));
	void GetMirrorType (MirrorType& mirrorType) const;
	
	// 设置/获取ROI裁剪区域
	bool SetROI(long x, long y=0, long w=0, long h=0);	// SetROI(0)表示不设置ROI区域，即全图
	bool SetROI(scRect rcROI = scRect());	// SetROI()表示不设置ROI区域，即全图
	void GetROI (scRect & rcROI) const;
	void GetROI(long &x, long &y, long &w, long &h) const;
	
	// 获取处理后的图像数据
	void GetDisplayImage (cpImage& imgSource, cpImage& imgDisplay);
	void GetDisplayImage (const scImageRGB& imgSource, scImageRGB& imgDisplay);
	
	// 获取指定点处理后的对应坐标值
	bool GetProcessPt(const sc2Vector& inputPt, sc2Vector& outputPt);

private:
	bool RotateMirror(	const cpImage& srcImage, 
						cpImage& dstImage, 
						const RotateType rotateType, 
						const MirrorType mirrorType = (MirrorType)(-1));
	bool RotateMirror(	const scImageRGB& srcImage, 
						scImageRGB& dstImage, 
						const RotateType rotateType, 
						const MirrorType mirrorType = (MirrorType)(-1));
	RotateType		m_eRotationType;
	MirrorType		m_eMirrorType;
	scRect			m_rcROI;
	scRadian		m_rRotate;
	double			m_dWidth;
	double			m_dHeight;

	cpImage		m_imgTempRotateMirror;
	
	bool			m_bGammaCorrectionEnable;
	cpGammaCorrection m_toolGammaCorrection;

	bool			m_bBayerFilterEnable;
	epBayerType		m_eBayerFilterType;
	cpImage			m_ImageTemp;
};

#pragma pack(pop)

#endif // !defined(AFX_SVDISPLAYMODULE_H__68D2F18D_7570_4B84_BB00_3DAD5D14AFA5__INCLUDED_)
