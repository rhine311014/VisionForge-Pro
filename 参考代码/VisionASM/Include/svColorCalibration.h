#pragma once

#include "svDef.h"
#include "svImage.h"
#include "svVector.h"
#include <vector>

#pragma pack(push,8)

class SVCOLOR_API scColorCalibrationResult
{
public:
	friend class scColorCalibration;
	friend class scColorDifference;
	friend class scColorTransform;

	scColorCalibrationResult();
	~scColorCalibrationResult();

	void Clear();
	bool SaveToFile(const TCHAR* strFilePath);					
	bool LoadFromFile(const TCHAR* strFilePath);	

private:
	void	*m_hTransMatrix;
};

class SVCOLOR_API scColorCalibration
{
public:
	scColorCalibration();
	~scColorCalibration();

	void Clear();

	bool LoadTargetDataFromFile(const char* strFilePath);	

	bool GetRGBDataFromImage(const scImage24& image, sc2Vector corner[4]);
	bool GetRGBDataFromImage(const scImage32& image, sc2Vector corner[4]);

	bool Execute();

	bool GetCalibrationResult(scColorCalibrationResult& result);	
	double GetMaxDE();
	double GetMeanDE();
	double GetStDevDE();
	bool   GetArrayDE(svStd vector<double>& de);

private:
	void    *m_hColorCali;
};

#pragma pack(pop)
