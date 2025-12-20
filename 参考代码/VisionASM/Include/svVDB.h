#pragma once

#include "svDef.h"
#include "svGuiDef.h"
#include "svError.h"
#include "vpImage.h"
#include "svGuiGraphic.h"
#include "svDIB.h"
#include "svPng.h"
#include "svJpeg.h"
#include "svTIFF2.h"
#include <vector>

#pragma pack(push,8)

enum ImageType{
	eImgUnknown  = 1000,			// 未知图像
	eImgBmp      = 1001,			// bmp图像
	eImgPng,						// png图像
	eImgJpeg,						// Jpg图像
};

typedef struct tagVdbElementHeaderInfo
{
	long			type;
	long			bytes;
}VdbElementHeaderInfo,*PVdbElementHeaderInfo;

class SVGUI_API scVDB
{
public:
	scVDB();
	~scVDB();

	HStatus Init(const TCHAR* strFilePath);				// 读取数据文件
	HStatus Write(const TCHAR* strFilePath, int nJpegQuality = 100, bool bQuickWrite = false) const;	// 保存数据文件

	HStatus WriteToImageFile(CDC* pDC,const TCHAR* strFilePath, bool bCustomSave = false, CompressMode nCompressMode = eCompressNone, int nCompressQuality = 100);			   // 保存为图像文件

	HStatus AddImage(const cpImage& image, ImageType type = eImgPng);		   // 添加图像
	cpImage* GetImage(int nIndex)const;                                        // 获取图像
	ImageType GetImageType(int nIndex)const;								   // 获取图像类型

	HStatus AddGraphic(const scGuiGraphic* pGuiGraphic);					   // 添加图形
	scGuiGraphic* GetGraphic(int nIndex)const;                                 // 获取图形
	GraphicType GetGraphicType(int nIndex)const;                               // 获取图形类型

	int GetImageCount()const;                                                  // 获取图像数量
	int GetGraphicCount()const;                                                // 获取图形数量
	int GetElementCount()const;                                                // 获取图像+图形数量

	HStatus DeleteImage(int nIndex);                                           // 删除图像
	HStatus DeleteGraphic(int nIndex);                                         // 删除图形

	void EnableCustomGraphicParam(bool bEnable);							   // 使用自定义图形参数
	bool IsCurtomGraphicParamEnable() const;

	void SetCustomGraphicParam(int nLineWidth, int nTextFontSize, int nLabelFontSize);				  // 设置保存图形字号和线宽
	void GetCustomGraphicParam(int& nLineWidth, int& nTextFontSize, int& nLabelFontSize);
	
private:
	void Free();
	VdbElementHeaderInfo* GetGraphicHeaderInfo(int nIndex)const;
	VdbElementHeaderInfo* GetImageHeaderInfo(int nIndex)const;
	
	HStatus WriteGraphic(HANDLE hFile,const scGuiGraphic* pGraphic,const PVdbElementHeaderInfo info)const;
	HStatus WriteImage(HANDLE hFile,FILE* pFile,cpImage* pImage,const PVdbElementHeaderInfo info, int nJpegQuality = 100)const;

	cpImage* ReadImageBmp(FILE* fp,const PVdbElementHeaderInfo info);
	cpImage* ReadImageJpeg(FILE* fp,const PVdbElementHeaderInfo info);
	cpImage* ReadImagePng(FILE* fp,const PVdbElementHeaderInfo info);
	scGuiGraphic* ReadGraphic(FILE* fp,const PVdbElementHeaderInfo info);

	scShape* CreateShapeFromGraphic(scGuiGraphic* pGuiGraphic);
	HStatus CopyGuiGraphicParam(scGuiGraphic* pFrom, scGuiGraphic* pTo);
	HStatus GetGraphicHeaderInfo(scGuiGraphic* pGraphic, PVdbElementHeaderInfo info) const;
private:
	std::vector<cpImage*> m_vpImage;
	std::vector<PVdbElementHeaderInfo> m_vpImageHeaderInfo;

	std::vector<scGuiGraphic*> m_vpGraphic;
	std::vector<PVdbElementHeaderInfo> m_vpGraphicHeaderInfo;

	bool m_bEnableCustomParam;
	int	 m_nCustomLineWidth;
	int	 m_nCustomTextFontSize;
	int	 m_nCustomLabelFontSize;
};

#pragma pack(pop)