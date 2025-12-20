#pragma once

#include "svDef.h"
#include "svCodeCalibrateDef.h"
#include "svImage.h"
#include "vpImage.h"
#include "svVector.h"
#include <vector>
#include "svCodeVerifyDef.h"

#pragma pack(push,8)

#define MAX_CODE_LENGTH	512

//QR解码参数
struct FindQRCodePara
{
	DecodeMethod m_methodDecode;

	//QR码类型
	int m_nQRCodeType;	//1-QR，2-MQR，3-QR+MQR

	//全局采样参数
	int m_nSampleLevelGlobal;

	AppMode m_modeApp;
	DiscreteFlag m_flagDiscrete;
	DistortionFlag m_flagDistortion;
	MirrorMode m_modeMirror;
	PolarityMode m_modePolarity;
	RectangleFlag m_flagRectangle;
	int m_nMaxCodeNum;
	int m_nMinBarSize;
	int m_nMaxBarSize;
	int m_nSampleLevel;
	int m_nWaitingTime;

	int m_nDebugFlag;			// 已弃用
	int m_nAdvanceParam;		// 已弃用
	int m_nAdvanceParam2;		// 已弃用

	//vsc参数
	int m_nElemMinWidth;	//最小单元尺寸，1~200    //已弃用
	int m_nElemMaxWidth;	//最大单元尺寸，1~200    //已弃用

	FindQRCodePara()
	{
		m_methodDecode = eDecodeMethod_Vsb;

		m_nQRCodeType = 1;	

		m_nSampleLevelGlobal = 0;

		m_modeApp = eAppMode_Normal;
		m_flagDiscrete = eDiscreteFlag_Continuous;
		m_flagDistortion = eDistortionFlag_Close;
		m_modeMirror = eMirrorMode_Both;
		m_modePolarity = ePolarityMode_Both;
		m_flagRectangle = eRectangleFlag_Square;
		m_nMaxCodeNum = 1;
		m_nMinBarSize = 20;
		m_nMaxBarSize = 500;
		m_nSampleLevel = 1;
		m_nWaitingTime = 1000;

		m_nDebugFlag = 0;
		m_nAdvanceParam = 0;
		m_nAdvanceParam2 = 0;

		m_nElemMinWidth = 3;	       //已弃用
		m_nElemMaxWidth = 200;	       //已弃用
	}
};

//QR解码结果
struct DecodeQRResult
{
	sc2Vector m_pt[4];

	char m_OutputData[MAX_CODE_LENGTH];

	int	 m_nQRCodeType;	//1-QR，2-MQR

	Code2DVerifyResult m_Code2DVerifyResult;   // 评价结果

	DecodeQRResult()
	{
		m_pt[0] = sc2Vector(0,0);
		m_pt[1] = sc2Vector(1,0);
		m_pt[2] = sc2Vector(1,1);
		m_pt[3] = sc2Vector(0,1);

		memset(m_OutputData, 0, MAX_CODE_LENGTH*sizeof(char));

		m_nQRCodeType = 1;
	}

	DecodeQRResult& operator=(const DecodeQRResult& ret)
	{
		if (this != &ret)
		{
			m_pt[0] = ret.m_pt[0];
			m_pt[1] = ret.m_pt[1];
			m_pt[2] = ret.m_pt[2];
			m_pt[3] = ret.m_pt[3];

			memcpy(m_OutputData, ret.m_OutputData, MAX_CODE_LENGTH*sizeof(char));

			m_nQRCodeType = ret.m_nQRCodeType;

			m_Code2DVerifyResult = ret.m_Code2DVerifyResult;
		}

		return *this;
	}
};



class SVCODE_API scQRCodeTool
{
public:
	scQRCodeTool();
	~scQRCodeTool();

	bool SetFindQRCodePara(FindQRCodePara para);
	FindQRCodePara GetFindQRCodePara();

	void SetQRCodeType(int nQRCodeType);
	int GetCodeType();

	void SetSampleLevelGlobal(int nSampleLevelGlobal);
	int GetSampleLevelGlobal();

	bool SetDecodeMethod(DecodeMethod method);
	DecodeMethod GetDecodeMethod();

	void SetAppMode(AppMode mode);
	AppMode GetAppMode();

	void SetDiscreteFlag(DiscreteFlag flag);
	DiscreteFlag GetDiscreteFlag();

	void SetDistortionFlag(DistortionFlag flag);
	DistortionFlag GetDistortionFlag();

	void SetMirrorMode(MirrorMode mode);
	MirrorMode GetMirrorMode();

	void SetPolarityMode(PolarityMode mode);
	PolarityMode GetPolarityMode();

	void SetRectangleFlag(RectangleFlag flag);
	RectangleFlag GetRectangleFlag();

	void SetMaxCodeNum(int num);
	int GetMaxCodeNum();

	bool SetBarSize(int min, int max);
	void GetBarSize(int& min, int& max);

	void SetSampleLevel(int nSampleLevel);
	int GetSampleLevel();

	void SetWaitingTime(int millisecond);
	int GetWaitingTime();

	void SetDebugFlag(int flag);
	int GetDebugFlag();

	void SetAdvanceParam(int param);
	int GetAdvanceParam();

	void SetAdvanceParam2(int param);
	int GetAdvanceParam2();

	void SetElemMinWidth(int nElemMinWidth);
	int GetElemMinWidth();

	void SetElemMaxWidth(int nElemMaxWidth);
	int GetElemMaxWidth();

	void SetQRCodeVerifyEnable(bool bEnable);
	bool GetQRCodeVerifyEnable();

	bool SetQRCodeVerifyPara(Code2DVerifyPara para);
	Code2DVerifyPara GetQRCodeVerifyPara();

	bool Execute(const scImage8& image, const scRect& roi, svStd vector<DecodeQRResult>& resultarray);
	bool Execute(cpImage& image, const scRect& roi, svStd vector<DecodeQRResult>& resultarray);

	// 按行按列排序
	bool SortByCoordXY(svStd vector<DecodeQRResult>& resultarray, double dGap = 64, CoordSortMethod method = eCoordYFirst, SortOrder orderX = eAscending, SortOrder orderY = eAscending, BOOL bImageCoord = TRUE);

protected:
	DecodeMethod m_methodDecode;

	int m_nQRCodeType;	//0-保留，1-QR，2-MQR，3-QR+MQR

	int m_nSampleLevelGlobal;

	AppMode m_modeApp;
	DiscreteFlag m_flagDiscrete;
	DistortionFlag m_flagDistortion;
	MirrorMode m_modeMirror;
	PolarityMode m_modePolarity;
	RectangleFlag m_flagRectangle;
	int m_nMaxCodeNum;
	int m_nMinBarSize;
	int m_nMaxBarSize;
	int m_nSampleLevel;
	int m_nWaitingTime;

	int m_nDebugFlag;			// 已弃用
	int m_nAdvanceParam;		// 已弃用
	int m_nAdvanceParam2;		// 已弃用

	int m_nElemMinWidth;       //已弃用
	int m_nElemMaxWidth;       //已弃用

	void* m_pCode2DReaderTool;
	void* m_pCode2DVerifyTool;

	bool m_bCodeVerifyEnable;
	Code2DVerifyPara m_QRVerifyPara;

};

#pragma pack(pop)
