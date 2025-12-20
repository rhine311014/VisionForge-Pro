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

//一维码解码参数
struct Find1DCodePara
{
	DecodeMethod m_methodDecode;

	//全局采样参数
	int m_nSampleLevelGlobal;

	//vsb、vsc共用参数
	AppMode m_modeApp;
	bool m_bCodaBarFlag;
	bool m_bCode128Flag;
	bool m_bCode39Flag;
	bool m_bCode93Flag;
	bool m_bCodeEANFlag;
	bool m_bCodeITF25Flag;

	int m_nRegionNum;
	int m_nMaxSizeWidth;
	int m_nMinSizeWidth;

	//vsb参数
	bool m_bErrorDetectFlag;
	bool m_nImageMorph;
	int m_nPromodeDistortion;
	int m_nPromodeSpot;
	int m_nPromodeWhiteGap;

	int m_nSampleLevel;
	int m_nSavePicLevel;
	int m_nStaticAreaWidth;
	int m_nWaitingTime;
	int m_nWindowSize;

	//vsc参数
	int m_nMaxSizeHeight;
	int m_nMinSizeHeight;
	int m_nElemMinWidth;	//最小单元尺寸，>1
	int m_nElemMaxWidth;	//最大单元尺寸，>1

	int m_nNumScanlines;	//范围3-50,默认值20

	int m_nBarcodeBalanceLevel;	//范围1-10,默认值5        //已弃用
	SegmentationMethod m_methodSegmentation;              //已弃用
	BoxDisplayMode m_modeBoxDisplay;                      // 已弃用
	bool m_bFrameCheckOut;	//多次校验标志位开关         // 已弃用

	Find1DCodePara()
	{
		m_methodDecode = eDecodeMethod_Vsb;
		m_nSampleLevelGlobal = 0;

		m_modeApp = eAppMode_Normal;

		m_bCodaBarFlag = true;
		m_bCode128Flag = true;
		m_bCode39Flag = true;
		m_bCode93Flag = true;
		m_bCodeEANFlag = true;
		m_bCodeITF25Flag = true;

		m_nRegionNum = 1;
		m_nMaxSizeWidth = 2400;
		m_nMinSizeWidth = 30;

		m_bErrorDetectFlag = 1;
		m_nImageMorph = 0;
		m_nPromodeDistortion = 0;
		m_nPromodeSpot = 0;
		m_nPromodeWhiteGap = 0;

		m_nSampleLevel = 1;
		m_nSavePicLevel = 1;
		m_nStaticAreaWidth = 30;
		m_nWaitingTime = 500;
		m_nWindowSize = 4;

		m_nMaxSizeHeight = 2000;
		m_nMinSizeHeight = 10;
		m_nElemMinWidth = 1;
		m_nElemMaxWidth = 1000;

		m_nNumScanlines = 20;
		m_nBarcodeBalanceLevel = 5;
		m_methodSegmentation = eSegmentationMethod_Global;
		m_modeBoxDisplay = eBoxDisplayMode_Accurate;      
		m_bFrameCheckOut = false;
	}
};

//一维码解码结果
struct Code1DResult
{
	sc2Vector m_pt[4];	//位置

	char m_OutputData[MAX_CODE_LENGTH];	//内容

	CodeType1D m_typeCode;	//类型

	double m_dAngle;		//角度

	Code1DVerifyResult m_Code1DVerifyResult;   // 评价结果

	Code1DResult()
	{
		m_pt[0] = sc2Vector(0,0);
		m_pt[1] = sc2Vector(1,0);
		m_pt[2] = sc2Vector(1,1);
		m_pt[3] = sc2Vector(0,1);

		memset(m_OutputData, 0, MAX_CODE_LENGTH*sizeof(char));
	}

	Code1DResult& operator=(const Code1DResult& ret)
	{
		if (this != &ret)
		{
			m_pt[0] = ret.m_pt[0];
			m_pt[1] = ret.m_pt[1];
			m_pt[2] = ret.m_pt[2];
			m_pt[3] = ret.m_pt[3];

			memcpy(m_OutputData, ret.m_OutputData, MAX_CODE_LENGTH*sizeof(char));

			m_typeCode = ret.m_typeCode;

			m_dAngle = ret.m_dAngle;

			m_Code1DVerifyResult = ret.m_Code1DVerifyResult;
		}

		return *this;
	}
};


class SVCODE_API sc1DCodeTool
{
public:
	sc1DCodeTool();
	~sc1DCodeTool();

	bool SetFind1DCodePara(Find1DCodePara para);
	Find1DCodePara GetFind1DCodePara();

	void Set1DCodeVerifyEnable(bool bEnable);
	bool Get1DCodeVerifyEnable();

	bool Set1DCodeVerifyPara(Code1DVerifyPara para);
	Code1DVerifyPara Get1DCodeVerifyPara();

	bool Execute(const scImage8& image, const scRect& roi, svStd vector<Code1DResult>& resultarray);
	bool Execute(cpImage& image, const scRect& roi, svStd vector<Code1DResult>& resultarray);

	// 按行按列排序
	bool SortByCoordXY(svStd vector<Code1DResult>& resultarray, double dGap = 64, CoordSortMethod method = eCoordYFirst, SortOrder orderX = eAscending, SortOrder orderY = eAscending, BOOL bImageCoord = TRUE);

protected:
	Find1DCodePara m_para;
	Code1DVerifyPara m_verifyPara;
	bool m_bCodeVerifyEnable;

	void* m_pCode1DReaderTool;
	void* m_pCode1DVerifyTool;

};

#pragma pack(pop)