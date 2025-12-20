#pragma once

#include "svDef.h"
#include "svCodeCalibrateDef.h"
#include "svImage.h"
#include "vpImage.h"
#include "svVector.h"
#include <vector>

#pragma pack(push,8)

#define MAX_CODE_LENGTH	512

//VCR解码参数
struct FindVCRCodePara
{
	DecodeMethod m_methodDecode;

	//全局采样参数
	int m_nSampleLevelGlobal;
	// vsb、vsc共用参数
	int m_nTotalTimeOut;

	// vsb参数
	int m_nCPUFreq;
	SymbolType m_symbolType;
	int m_nAutoParam;           //0-关闭，1-开启
	int m_nContrastIndex;
	int m_nSampleWidth;
	BitsPerCell m_bitsPerCell;
	EdacLevel m_edacLevel;
	CompressionMode m_compressionMode;
	int m_nPrefiltering;       //0-关闭，1-开启
	int m_nExtendLen;
	
	//outline码、normal码特有的公共参数
	int m_nMinSize;
	int m_nMaxSize;
	int m_nMinModule;
	int m_nMaxModule;
	int m_nEdgeContrast;

	//normal码特有参数
	int m_nNormalLocRadioMin;
	int m_nNormalLocRadioMax;
	int m_nNormalLocFineEdgeMode;    //0-关闭，1-开启


	//Vsc 参数
	PolarityMode m_modePolarity;
	int m_nMaxCodeNum;
	int m_nMinBarSize;
	int m_nMaxBarSize;
	DiscreteFlag m_flagDiscrete;
	AppMode m_decoderCapacity;

	FindVCRCodePara()
	{
		m_methodDecode = eDecodeMethod_Vsc;
		
		m_nSampleLevelGlobal = 0;
		m_nTotalTimeOut = 0;

		m_nCPUFreq = 2000;
		m_symbolType = eSymbolType_WhiteOnBlack;
		m_nAutoParam = 1;
		m_nContrastIndex = 0;
		m_nSampleWidth = 4;
		m_bitsPerCell = eBitsPerCell_PerCell4;
		m_edacLevel = eEdacLevel_Auto;
		m_compressionMode = eCompressionMode_None;
		m_nPrefiltering = 0;
		m_nExtendLen = 30;
		

		//outline码、normal码特有的公共参数
		m_nMinSize = 10;
		m_nMaxSize = 48;
		m_nMinModule = 3;
		m_nMaxModule = 50; 
		m_nEdgeContrast = 15;

		//normal码特有参数
		m_nNormalLocRadioMin = 2;
		m_nNormalLocRadioMax = 4;
		m_nNormalLocFineEdgeMode = 0;

		// Vsc参数
		m_modePolarity = ePolarityMode_Both;
		m_nMaxCodeNum = 1;
		m_nMinBarSize = 20;
		m_nMaxBarSize = 500;
		m_flagDiscrete = eDiscreteFlag_Continuous;
		m_decoderCapacity = eAppMode_Normal;
	}
};


//VCR解码结果
struct DecodeVCRResult
{
	sc2Vector m_pt[4];

	char m_OutputData[MAX_CODE_LENGTH];

	DecodeVCRResult()
	{
		m_pt[0] = sc2Vector(0,0);
		m_pt[1] = sc2Vector(1,0);
		m_pt[2] = sc2Vector(1,1);
		m_pt[3] = sc2Vector(0,1);

		memset(m_OutputData, 0, MAX_CODE_LENGTH*sizeof(char));
	}

	DecodeVCRResult& operator=(const DecodeVCRResult& ret)
	{
		if (this != &ret)
		{
			m_pt[0] = ret.m_pt[0];
			m_pt[1] = ret.m_pt[1];
			m_pt[2] = ret.m_pt[2];
			m_pt[3] = ret.m_pt[3];

			memcpy(m_OutputData, ret.m_OutputData, MAX_CODE_LENGTH*sizeof(char));
		}

		return *this;
	}
};


class SVCODE_API scVCRCodeTool
{
public:
	scVCRCodeTool();
	~scVCRCodeTool();

	bool SetFindVCRCodePara(FindVCRCodePara para);
	FindVCRCodePara GetFindVCRCodePara();

	void SetSampleLevelGlobal(int nSampleLevelGlobal);
	int GetSampleLevelGlobal();

	bool SetDecodeMethod(DecodeMethod method);
	DecodeMethod GetDecodeMethod();

	void SetCPUFreq(int nCPUFreq);
	int GetCPUFreq();

	void SetSymbolType(SymbolType type);
	SymbolType GetSymbolType();

	void SetAutoParam(int autoParam);
	int GetAutoParam();

	void SetContrastIndex(int index);
	int GetContrastIndex();

	void SetSampleWidth(int SampleWidth);
	int GetSampleWidth();

	void SetBitsPerCell(BitsPerCell perCell);
	BitsPerCell GetBitsPerCell();

	void SetEdacLevel(EdacLevel level);
	EdacLevel GetEdacLevel();

	void SetCompressionMode(CompressionMode mode);
	CompressionMode GetCompressionMode();

	void SetPrefiltering(int prefiltering);
	int GetPrefiltering();

	void SetExtendLen(int extendLen);
	int GetExtendLen();

	void SetTotalTimeOut(int totalTimeOut);
	int GetTotalTimeOut();

	bool SetLocCodeSizeModule(int MinSize, int MaxSize,int MinModule, int MaxModule);
	void GetLocCodeSizeModule(int &MinSize, int &MaxSize,int &MinModule, int &MaxModule);

	void SetLocEdgeContrast(int EdgeContrast);
	int GetLocEdgeContrast();

	bool SetNormalLocRadio(int min, int max);
	void GetNormalLocRadio(int &min, int &max);

	void SetNormalLocFineEdgeMode(int mode);
	int GetNormalLocFineEdgeMode();

	void SetDiscreteFlag(DiscreteFlag flag);
	DiscreteFlag GetDiscreteFlag();

	void SetPolarityMode(PolarityMode mode);
	PolarityMode GetPolarityMode();

	void SetDecoderCapacity(AppMode mode);
	AppMode GetDecoderCapacity();

	void SetMaxCodeNum(int num);
	int GetMaxCodeNum();

	bool SetBarSize(int min, int max);
	void GetBarSize(int& min, int& max);

	bool Execute(const scImage8& image, const scRect& roi, svStd vector<DecodeVCRResult>& resultarray);
	bool Execute(cpImage& image, const scRect& roi, svStd vector<DecodeVCRResult>& resultarray);

	// 按行按列排序
	bool SortByCoordXY(svStd vector<DecodeVCRResult>& resultarray, double dGap = 64, CoordSortMethod method = eCoordYFirst, SortOrder orderX = eAscending, SortOrder orderY = eAscending, BOOL bImageCoord = TRUE);

protected:

	DecodeMethod m_methodDecode;

	//全局采样参数
	int m_nSampleLevelGlobal;
	// vsb、vsc共用参数
	int m_nTotalTimeOut;

	// vsb参数
	int m_nCPUFreq;
	SymbolType m_symbolType;
	int m_nAutoParam;
	int m_nContrastIndex;
	int m_nSampleWidth;
	BitsPerCell m_bitsPerCell;
	EdacLevel m_edacLevel;
	CompressionMode m_compressionMode;
	int m_nPrefiltering;
	int m_nExtendLen;
	

	//outline码、normal码特有的公共参数
	int m_nMinSize;
	int m_nMaxSize;
	int m_nMinModule;
	int m_nMaxModule;
	int m_nEdgeContrast;

	//normal码特有参数

	int m_nNormalLocRadioMin;
	int m_nNormalLocRadioMax;
	int m_nNormalLocFineEdgeMode;


	//Vsc 参数
	PolarityMode m_modePolarity;
	int m_nMaxCodeNum;
	int m_nMinBarSize;
	int m_nMaxBarSize;
	DiscreteFlag m_flagDiscrete;
	AppMode m_decoderCapacity;

	void* m_pCodeVCRReaderTool;
};

#pragma pack(pop)
