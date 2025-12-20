#pragma once

#include "svDef.h"
#include "svImage.h"
#include "svVector.h"
#include <vector>
#include "svRect.h"
#include <string.h>
using namespace std;

#pragma pack(push,8)

enum ImagePlane
{
	Intensity=0,
	Saturation,
	Hue,
	Blue,
	Green,
	Red,
};

enum ImageColorSpace
{
	HSI=0,
	RGB,
};

class SVCOLOR_API scPlaneRange
{
public:
	scPlaneRange(void);
	scPlaneRange(ImagePlane planeVal);
	scPlaneRange(double nominalVal,ImagePlane planeVal);
	scPlaneRange(const scPlaneRange& planeRangeVal);

	~scPlaneRange(void);
	
	double GetNominal(){return Nominal;};
	double GetLowTol(){return LowTolerance;};
	double GetHighTol(){return HighTolerance;};
	double GetSoftness(){return Softness;};
	ImagePlane GetPlane(){return Plane;};

	void Update(double nominalVal,double lowTolVal,double highTolVal,double softnessVal);
	
	void UpdateWithClipping(double nominalVal,double lowTolVal,double highTolVal,double softnessVal);

public:
	double Nominal;
	double LowTolerance;
	double HighTolerance;
	double Softness;
	ImagePlane Plane;
};

class SVCOLOR_API scColorRange
{
public:
	scColorRange(void);
	scColorRange(ImageColorSpace colorSpace,double plane0Val,double plane1Val,double plane2Val);
	scColorRange(const scColorRange& colorRangeVal); 
	~scColorRange(void);

	void SetName(const TCHAR* name){
		size_t length = 0;
		#ifndef _UNICODE
			length = strlen(name);
		#else
			length = wcslen(name);
		#endif

		if (length<256)
		{
			#ifndef _UNICODE
				strcpy(Name,name); 
			#else
				wcscpy(Name,name); 
			#endif
		}
	};
	TCHAR* GetName(){return Name;};

	void SetPlaneRange0(scPlaneRange planeRangeVal){*PlaneRange0 = planeRangeVal;};
	scPlaneRange *GetPlaneRange0(){return PlaneRange0;};

	void SetPlaneRange1(scPlaneRange planeRangeVal){*PlaneRange1 = planeRangeVal;};
	scPlaneRange *GetPlaneRange1(){return PlaneRange1;};

	void SetPlaneRange2(scPlaneRange planeRangeVal){*PlaneRange2 = planeRangeVal;};
	scPlaneRange *GetPlaneRange2(){return PlaneRange2;};

	void SetColorSpace(ImageColorSpace colorspace){ColorSpace = colorspace;};
	ImageColorSpace GetColorSpace(){return ColorSpace;};

	void UpdateUsingColorFromImagePoint(const scImage24& image,sc2Vector point);
	void UpdateUsingColorFromImagePoint(const cpImage& image,sc2Vector point);

	void UpdateUsingColorFromImagePoint(const scImage24& image,sc2Vector point,ImageColorSpace colorspace);
	void UpdateUsingColorFromImagePoint(const cpImage& image,sc2Vector point,ImageColorSpace colorspace);

	void UpdateUsingMeanColorFromImageRegion(const scImage24& image,scRect rect); 
	void UpdateUsingMeanColorFromImageRegion(const cpImage& image,scRect rect); 

	void UpdateUsingMeanColorFromImageRegion(const scImage24& image,scRect rect,ImageColorSpace colorspace); 
	void UpdateUsingMeanColorFromImageRegion(const cpImage& image,scRect rect,ImageColorSpace colorspace); 

	scColorRange& operator=(const scColorRange& colorRangeVal);
public:
	TCHAR Name[256];
	ImageColorSpace ColorSpace;
	scPlaneRange *PlaneRange0;   
	scPlaneRange *PlaneRange1; 
	scPlaneRange *PlaneRange2; 
};

class SVCOLOR_API scColorRangeItem : public scColorRange
{
public:
	scColorRangeItem(void);
	scColorRangeItem(const scColorRangeItem& colorRangeItemVal);
	
	~scColorRangeItem(void);
	scColorRangeItem& operator=(const scColorRangeItem& colorRangeItemVal);

public:
	void SetSelected(bool selected){Selected = selected;};
	bool GetSelected(){return Selected;};
public:
	bool Selected;
};

class SVCOLOR_API scColorRangeCollection
{
public:
	scColorRangeCollection();
	scColorRangeCollection(const scColorRangeCollection& colorRangeCollection);
	
	scColorRangeCollection& operator=(const scColorRangeCollection& colorRangeCollection);
	~scColorRangeCollection(){Items.clear();};

public:
	int Add(scColorRangeItem RangeItemVal){Items.push_back(RangeItemVal); return Items.size();};
	void Clear(){Items.clear();};

	void SetItem(int index,scColorRangeItem RangeItemVal){
		if (index<Items.size())
		{
			Items[index] = RangeItemVal;
		}
	};

	bool GetItem(int index,scColorRangeItem &RangeItemVal){
			if (index<Items.size())
			{
				RangeItemVal = Items[index];
				return true;
			}
			else
			{
				return false;
			}
	};
	
	void RemoveAt(int index){
		if (index<Items.size())
		{
			Items.erase(Items.begin()+index);
		}
	};
	

	void Insert(int index,scColorRangeItem RangeItemVal){
		if (index<Items.size())
		{
			Items.insert(Items.begin()+index,RangeItemVal);
		}
	};
	
	int GetCount(){return Items.size();};

	int GetSelectedCount(){
		int nCount=0;
		for (int i=0;i<Items.size();i++)
		{
			if (Items[i].GetSelected())
			{
				nCount ++;
			}
		}	
		return nCount;
	};
private:
	svStd vector<scColorRangeItem> Items;
};

class SVCOLOR_API scColorSegmenterTool
{
public:
	scColorSegmenterTool(void);
	~scColorSegmenterTool(void);
public:

	void SetColorRangeCollection(const scColorRangeCollection colorRangeCollectionVal);
	bool GetColorRangeCollection(scColorRangeCollection &colorRangeCollection );
	
	bool Execute(const scImage24& image,scRect roi,scImageGray &resultImage);
	bool Execute(const cpImage& image,scRect roi,scImageGray &resultImage);
	bool Execute(const cpImage& image,scRect roi,cpImage &resultImage);
	bool GetResult(scImageGray& image);
	bool GetResult(cpImage& image);

public:
	scColorRangeCollection ColorRangeCollection;
	scImageGray ResultImage;
};

#pragma pack(pop)
