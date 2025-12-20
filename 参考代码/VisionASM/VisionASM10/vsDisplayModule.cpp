// vsDisplayModule.cpp: implementation of the vcDisplayModule class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "vsDisplayModule.h"
#include "svAfftrans.h"
#include "svDIB.h"
#include "svTimer.h"
//#include "_Record.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define _USE_CUSTOM_ROTATION

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

vcDisplayModule::vcDisplayModule()
{
	m_eRotationType = eRotate0;
	m_eMirrorType = (MirrorType)(-1);
	m_dWidth = 0.0;
	m_dHeight = 0.0;
	m_rRotate = 0.0;

	m_bGammaCorrectionEnable = false; 

	m_bBayerFilterEnable = false;
	m_eBayerFilterType = epBayerRG8;
}

vcDisplayModule::~vcDisplayModule()
{

}
void vcDisplayModule::SetGammaCorrectionEnable(bool bEnable)
{
	m_bGammaCorrectionEnable = bEnable;
}

bool vcDisplayModule::GetGammaCorrectionEnable() const
{
	return m_bGammaCorrectionEnable;
}

void vcDisplayModule::SetGammaPara(double dGamma)
{
	m_toolGammaCorrection.SetGammaPara(dGamma);
}

double vcDisplayModule::GetGammaPara()
{
	return m_toolGammaCorrection.GetGammaPara();
}


void vcDisplayModule::SetBayerFilterEnable(bool bEnable)
{
	m_bBayerFilterEnable = bEnable;
}

bool vcDisplayModule::GetBayerFilterEnable() const
{
	return m_bBayerFilterEnable;
}

void vcDisplayModule::SetBayerFilterType(epBayerType type)
{
	m_eBayerFilterType = type;
}

epBayerType vcDisplayModule::GetBayerFilterType() const
{
	return m_eBayerFilterType;
}


void vcDisplayModule::SetRotationType( RotateType rotateType )
{
	m_eRotationType = rotateType;
	switch (rotateType)
	{
	case eRotate0:
		m_rRotate = 0.0;
		break;
	case eRotate90:
		m_rRotate = skPI_2;
		break;
	case eRotate180:
		m_rRotate = skPI;
		break;
	case eRotate270:
		m_rRotate = skPI + skPI_2;
		break;
	default:
		break;
	}
}

void vcDisplayModule::GetRotationType( RotateType& rotateType ) const
{
	rotateType = m_eRotationType;
}

void vcDisplayModule::SetMirrorType( MirrorType mirrorType )
{
	m_eMirrorType = mirrorType;
}

void vcDisplayModule::GetMirrorType( MirrorType& mirrorType ) const
{
	mirrorType = m_eMirrorType;
}

bool vcDisplayModule::SetROI(long x, long y, long w, long h)
{
	if ((x<0) || (y<0) || (w<0) || y<0)
	{
		return false;
	}
	m_rcROI = scRect((double)x, (double)y, (double)w, (double)h);
	return true;
}

bool vcDisplayModule::SetROI( scRect rcROI )
{
	long x = (long)rcROI.GetUL().GetX();
	long y = (long)rcROI.GetUL().GetY();
	long w = (long)rcROI.GetWidth();
	long h = (long)rcROI.GetHeight();
	return SetROI(x, y, w, h);
}

void vcDisplayModule::GetROI( scRect& rcROI ) const
{
	rcROI = m_rcROI;
}

void vcDisplayModule::GetROI(long &x, long &y, long &w, long &h) const
{
	x = (long)m_rcROI.GetUL().GetX();
	y = (long)m_rcROI.GetUL().GetY();
	w = (long)m_rcROI.GetWidth();
	h = (long)m_rcROI.GetHeight();
}

void vcDisplayModule::GetDisplayImage(cpImage& imgSource, cpImage& imgDisplay)
{

	if (!imgSource.IsValid())
	{
		return;
	}
	m_dWidth = imgSource.Width();
	m_dHeight = imgSource.Height();

	
	scTimer timer;	
	if (m_bGammaCorrectionEnable)
	{
		m_toolGammaCorrection.ExecuteGammaCorrection(imgSource);
	}

	if (m_bBayerFilterEnable)
	{
		m_ImageTemp.CreateImageBuffer(imgSource.Width(), imgSource.Height(), epPackedRGB24);

		cpBayerFilter toolBayerFilter;
		toolBayerFilter.Bayer2RGB24(imgSource, m_ImageTemp, m_eBayerFilterType);

		MirrorType mtMirrorTypeTemp;
		mtMirrorTypeTemp = m_eMirrorType;
		if (mtMirrorTypeTemp < eNoMirror || mtMirrorTypeTemp > eHorVerMirror)
		{
			mtMirrorTypeTemp = eNoMirror;
		}

		sfRotateMirror(m_ImageTemp, imgDisplay, m_eRotationType, mtMirrorTypeTemp);
	}
	else
	{
		MirrorType mtMirrorTypeTemp;
		mtMirrorTypeTemp = m_eMirrorType;
		if (mtMirrorTypeTemp < eNoMirror || mtMirrorTypeTemp > eHorVerMirror)
		{
			mtMirrorTypeTemp = eNoMirror;
		}

		sfRotateMirror(imgSource, imgDisplay, m_eRotationType, m_eMirrorType);
	}
	
	double time = timer.GetTimeMilli(TRUE);
	CString str;
	str.Format(_T("%.2f ms\n"), time);

// 	// ROI
// 	if (m_rcROI.IsNull())
// 	{
// 		imgDisplay = m_imgTempRotateMirror;
// 	}
// 	else
// 	{
// 		HStatus hSub = m_imgTempRotateMirror.GetSubImage(imgDisplay, m_rcROI);
// 		if (hSub != HSuccess)
// 		{
// 			return;
// 		}
// 	}

}

void vcDisplayModule::GetDisplayImage(const scImageRGB& imgSource, scImageRGB& imgDisplay)
{
// 	if (imgDisplay.IsValid())
// 	{
// 		imgDisplay.Release();
// 	}
	if (!imgSource.IsValid())
	{
		return;
	}
	m_dWidth = imgSource.Width();
	m_dHeight = imgSource.Height();
	
	scImageRGB imgTemp;
	RotateMirror(imgSource, imgTemp, m_eRotationType, m_eMirrorType);

	if (m_bGammaCorrectionEnable)
	{
		m_toolGammaCorrection.ExecuteGammaCorrection(imgTemp);
	}

	// ²Ã¼ô
	if (m_rcROI.IsNull())
	{
		imgDisplay = imgTemp;
	}
	else
	{
		HStatus hSub = imgTemp.GetSubImage(imgDisplay, m_rcROI);
		if (hSub != HSuccess)
		{
			return;
		}
	}
}

bool vcDisplayModule::GetProcessPt( const sc2Vector& inputPt, sc2Vector& outputPt )
{
	scRect rectImage = scRect(0, 0, m_dWidth, m_dHeight);
	if (rectImage.IsNull() || !rectImage.Within(inputPt))
	{
		return false;
	}
	else
	{
		double dCenterX = m_dWidth / 2.0;
		double dCenterY = m_dHeight / 2.0;

		// Ðý×ª±ä»»
		double tx, ty;
		if (m_eRotationType == eRotate0)
		{
			tx = inputPt.GetX();
			ty = inputPt.GetY();
		}
		else
		{
			double inX = inputPt.GetX();
			double inY = inputPt.GetY();
			double cosAngle = cos(m_rRotate);
			double sinAngle = sin(m_rRotate);
 		   	tx = cosAngle*(inX - dCenterX) + sinAngle*(inY - dCenterY) + dCenterX;
 		   	ty = -sinAngle*(inX - dCenterX) + cosAngle*(inY - dCenterY) + dCenterY;

			if ((m_eRotationType == eRotate90) || (m_eRotationType == eRotate270))
			{
				tx = tx + dCenterY - dCenterX;
				ty = ty + dCenterX - dCenterY;
			}
		}

		// ¾µÏñ±ä»»
		sc2Vector tempPt;
		if ((m_eRotationType == eRotate90) || (m_eRotationType == eRotate270))
		{
			switch (m_eMirrorType)
			{
			case eHorMirror:
				tempPt.SetX(m_dHeight - tx);
				tempPt.SetY(ty);
				break;
			case eVerMirror:
				tempPt.SetX(tx);
				tempPt.SetY(m_dWidth - ty);
				break;
			case eHorVerMirror:
				tempPt.SetX(m_dHeight - tx);
				tempPt.SetY(m_dWidth - ty);
				break;
			default:
				tempPt.SetX(tx);
				tempPt.SetY(ty);
				break;
			}
		} 
		else
		{
			// ¾µÏñ±ä»»
			switch (m_eMirrorType)
			{
			case eHorMirror:
				tempPt.SetX(m_dWidth - tx);
				tempPt.SetY(ty);
				break;
			case eVerMirror:
				tempPt.SetX(tx);
				tempPt.SetY(m_dHeight - ty);
				break;
			case eHorVerMirror:
				tempPt.SetX(m_dWidth - tx);
				tempPt.SetY(m_dHeight - ty);
				break;
			default:
				tempPt.SetX(tx);
				tempPt.SetY(ty);
				break;
			}
		}
		
		// ROI
		if (m_rcROI.IsNull())
		{
			outputPt = tempPt;
		}
		else
		{
			outputPt.SetX(tempPt.GetX() - m_rcROI.GetUL().GetX());
			outputPt.SetY(tempPt.GetY() - m_rcROI.GetUL().GetY());
		}

		return true;
	}// end of else
}

bool vcDisplayModule::RotateMirror(	const cpImage& srcImage, cpImage& dstImage, const RotateType rotateType, const MirrorType mirrorType)
{
	if (dstImage.IsValid())
	{
		dstImage.Release();
	}

//	scImageGray tmpImage;

#if 1


	MirrorType mtMirrorTypeTemp;
	mtMirrorTypeTemp = mirrorType;
	if (mtMirrorTypeTemp < eNoMirror || mtMirrorTypeTemp > eHorVerMirror)
	{
		mtMirrorTypeTemp = eNoMirror;
	}
	

	if (sfRotateMirror(srcImage, dstImage, rotateType, mtMirrorTypeTemp) != HSuccess)
	{
		return false;
	}

// 
// 	if (sfRotate(srcImage, tmpImage, rotateType) != HSuccess)
// 	{
// 		return false;
// 	}
// 	if ((mirrorType == eHorMirror) || (mirrorType == eVerMirror) || (mirrorType == eHorVerMirror))
// 	{
// 		if (sfMirror(tmpImage, dstImage, mirrorType) != HSuccess)
// 		{
// 			return false;
// 		}
// 	}
// 	else
// 	{
// 		dstImage = tmpImage;
// 	}
#else
		if (rotateType == eRotate0)
		{
			tmpImage = srcImage;
		}
		else
		{
			unsigned char *pDstImg = NULL;
			long width = srcImage.Width();
			long height = srcImage.Height();
			int pixelSize = srcImage.PixelSize();
			long i, j, y, offsetY;
			
			pDstImg = new unsigned char[width * height * pixelSize];
			switch (rotateType)
			{
			case eRotate90:
				for (j=0; j<width; j++)
				{
					y = width-j-1;
					offsetY = j*height;
					for (i=0; i<height; i++)
					{
						*(pDstImg + offsetY + i) = srcImage.GetPixelValue(y, i);
					}
				}
				tmpImage.CreateImageBuffer(pDstImg, height, width);
				break;
			case eRotate180:
				for (j=0; j<height; j++)
				{
					y = height-j-1;
					offsetY = j*width;
					for (i=0; i<width; i++)
					{
						*(pDstImg + offsetY + i) = srcImage.GetPixelValue(width-i-1, y);
					}
				}
				tmpImage.CreateImageBuffer(pDstImg, width, height);
				break;
			case eRotate270:
				for (j=0; j<width; j++)
				{
					offsetY = j*height;
					for (i=0; i<height; i++)
					{
						*(pDstImg + offsetY + i) = srcImage.GetPixelValue(j, height-i-1);
					}
				}
				tmpImage.CreateImageBuffer(pDstImg, height, width);
				break;
			default:
				break;
			}
			delete[] pDstImg;
			pDstImg = NULL;
		}
		if ((mirrorType == eHorMirror) || (mirrorType == eVerMirror) || (mirrorType == eHorVerMirror))
		{
			if (sfMirror(tmpImage, dstImage, mirrorType) != HSuccess)
			{
				return false;
		}
	}
	else
	{
		dstImage = tmpImage;	
	}
#endif

	return true;
}

bool vcDisplayModule::RotateMirror( const scImageRGB& srcImage, scImageRGB& dstImage, const RotateType rotateType, const MirrorType mirrorType )
{
	if (dstImage.IsValid())
	{
		dstImage.Release();
	}
	
	scImageRGB tmpImage;
	unsigned char r, g, b;
	unsigned char *pDstImg = NULL;
	if (rotateType == eRotate0)
	{
		tmpImage = srcImage;
	}
	else
	{

		long width = srcImage.Width();
		long height = srcImage.Height();
		int pixelSize = srcImage.PixelSize();
		long i, j, y, offsetY;
		
		pDstImg = new unsigned char[width * height * pixelSize];
		switch (rotateType)
		{
		case eRotate90:
			for (j=0; j<width; j++)
			{
				y = width-j-1;
				offsetY = j*height;
				for (i=0; i<height; i++)
				{
					srcImage.GetPixelValue(y, i, r, g, b);
					*(pDstImg + (offsetY + i)*3    ) = b;
					*(pDstImg + (offsetY + i)*3 + 1) = g;
					*(pDstImg + (offsetY + i)*3 + 2) = r;
				}
			}
			tmpImage.CreateImageBuffer(pDstImg, height, width);
			break;
		case eRotate180:
			for (j=0; j<height; j++)
			{
				y = height-j-1;
				offsetY = j*width;
				for (i=0; i<width; i++)
				{
					srcImage.GetPixelValue(width-i-1, y, r, g, b);
					*(pDstImg + (offsetY + i)*3    ) = b;
					*(pDstImg + (offsetY + i)*3 + 1) = g;
					*(pDstImg + (offsetY + i)*3 + 2) = r;
				}
			}
			tmpImage.CreateImageBuffer(pDstImg, width, height);
			break;
		case eRotate270:
			for (j=0; j<width; j++)
			{
				offsetY = j*height;
				for (i=0; i<height; i++)
				{
					srcImage.GetPixelValue(j, height-i-1, r, g, b);
					*(pDstImg + (offsetY + i)*3    ) = b;
					*(pDstImg + (offsetY + i)*3 + 1) = g;
					*(pDstImg + (offsetY + i)*3 + 2) = r;
				}
			}
			tmpImage.CreateImageBuffer(pDstImg, height, width);
			break;
		default:
			break;
		}
	}

	if ((mirrorType == eHorMirror) || (mirrorType == eVerMirror) || (mirrorType == eHorVerMirror))
	{
		long width = tmpImage.Width();
		long height = tmpImage.Height();
		long pixelSize = tmpImage.PixelSize();
		long i, j, offsetY, y;
		if (pDstImg == NULL)
		{
			pDstImg = new unsigned char[width * height * pixelSize];
		}
		switch (mirrorType)
		{
		case eHorMirror:
			for (j=0; j<height; j++)
			{
				offsetY = j*width;
				for (i=0; i<width; i++)
				{
					tmpImage.GetPixelValue(width-i-1, j, r, g, b);
					*(pDstImg + (offsetY + i)*3    ) = b;
					*(pDstImg + (offsetY + i)*3 + 1) = g;
					*(pDstImg + (offsetY + i)*3 + 2) = r;
				}
			}
			break;
		case eVerMirror:
			for (j=0; j<height; j++)
			{
				y = height-j-1;
				offsetY = j*width;
				for (i=0; i<width; i++)
				{
					tmpImage.GetPixelValue(i, y, r, g, b);
					*(pDstImg + (offsetY + i)*3    ) = b;
					*(pDstImg + (offsetY + i)*3 + 1) = g;
					*(pDstImg + (offsetY + i)*3 + 2) = r;
				}
			}
			break;
		case eHorVerMirror:
			for (j=0; j<height; j++)
			{
				y = height-j-1;
				offsetY = j*width;
				for (i=0; i<width; i++)
				{
					tmpImage.GetPixelValue(width-i-1, y, r, g, b);
					*(pDstImg + (offsetY + i)*3    ) = b;
					*(pDstImg + (offsetY + i)*3 + 1) = g;
					*(pDstImg + (offsetY + i)*3 + 2) = r;
				}
			}
			break;
		default:
			break;
		}
		dstImage.CreateImageBuffer(pDstImg, width, height);
		if (pDstImg != NULL)
		{
			delete[] pDstImg;
			pDstImg = NULL;
		}
	}
	else
	{
		dstImage = tmpImage;
	}
	return true;
}