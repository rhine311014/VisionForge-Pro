#pragma once
#include "afxwin.h"
#include "svCodeCalibrate.h"
#include "svGuiPolyline.h"
#include "svPolyline.h"
#include "svGuiText.h"
#include "vsBaseAlignTool.h"
#include "vsVisionAlignDef.h"
class CDmCodeCalibrate
{
public:
    CDmCodeCalibrate(void);
    virtual ~CDmCodeCalibrate(void);
private:
    //二维码关联工具
    scDMCodeCalibrate m_scDmCodeCalibrate;
    //二维码关联参数
    scDMCodeParam m_scDmCodeParam;
    scRect m_roiRectCalibrate;
	scRect m_roiRectDecode;
    BOOL m_bImageValid;
    //结果
public:
    std::vector<sc2Vector>   m_vImgDecodeCorners;//二维码角点图像坐标
    std::vector<sc2Vector>   m_vImgDecodeCornersLabel;//二维码角点位置数字标记
    std::vector<sc2Vector>  m_vImgDecodecornersResult; //关联交集结果

	//图像
	cpImage m_imageInput;
private:
    std::vector<Code2DResult> m_vArrayDMCodesResult;//二维码结果

    //GUI
	CGuiGraphics* m_pGuiDmCodeGraphics;
    //scGuiDisplay* m_pGuiDisplay;
    //scGuiStaticContainer*		m_pStaticContainer;

    //scGuiPolyline* m_GuiPolyline;	// 二维码区域
    //scGuiText*  m_GuiQRCodeText;//二维码string

    //std::vector<scGuiCross*> m_vGuiCodeCross;				 // 搜索二维码十字
    //std::vector<scGuiCoordCross*> m_vpResultGuiCodeCross;				 // 关联交集结果十字

    CString m_strError;//错误
    BOOL m_bIsSearchValid;

    BOOL UpdateSearchParam();
	bool SetRoiByImage(double nWidth,double nHeight);
    
public:
	void VisionAlignLogRecord(CString strPathFileName, CString strLog, BOOL bLogTimeEnable = TRUE);
	void SetSearchValid(BOOL bValid);
    BOOL SetSearchImage(cpImage &image);
    //BOOL SetGuiDisplay(scGuiDisplay* pGuiDisplay);
	BOOL SetGuiGraphics(CGuiGraphics* pGuiGraphics);
    BOOL SetDmCodeParam(scDMCodeParam& dmCodeParam);
    BOOL SearchDmCode();
    BOOL IsSearchValid();

    //BOOL ShowGuiSearchResult(BOOL bShow);
    //BOOL ShowGuiRelationResult(BOOL bShow);

    bool ClearSearchResult();
    //bool ClearSearchGui();
    bool UpdateSearchGuiData();

    bool ClearRelationResult();
    //bool ClearRelationGui();
    bool UpdateRelationGuiData();

    CString GetErrorString();

    int GetCornersNum();
	bool GetCornersImagePos(std::vector<sc2Vector>& vCornersImagePos);
	bool GetCornersLabelPos(std::vector<sc2Vector>& vCornersLabelPos);
    BOOL GetDmCodeResult(Code2DResult& result);
    BOOL RelatedTo(CDmCodeCalibrate& dmCodeTo);
};

