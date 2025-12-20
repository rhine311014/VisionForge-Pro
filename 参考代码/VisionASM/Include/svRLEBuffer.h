#pragma once

#include "svDef.h"
#include "svError.h"
#include "svMath.h"
#include "svPair.h"
#include "svImage.h"
#include <cassert>

#pragma pack(push,8)

class	scRect;

struct	scRLERun;
class	scRLEBufferTemp;

class SVCORE_API scRLEBuffer  
{
public:
	scRLEBuffer();

	scRLEBuffer(const scRLEBuffer& rlebuffer);

	scRLEBuffer(const scRLEBuffer& rlebuffer, scRect rect);

	scRLEBuffer(long width, long height, unsigned char background);

	~scRLEBuffer();

	scRLEBuffer& operator= (const scRLEBuffer& rlebuffer);

	void HintNumRuns(long hintnum);

	bool IsDegenerate() const;
	void Degenerate();

	bool Encode(const scImageGray& image, scRect roi);
	bool Encode(const scImageGray& image, scRect roi, const unsigned char ucMapArray[256]);
	bool Encode(const scImageGray& image, scRect roi, const unsigned char ucPerMapArray[256], const scImageGray& subimage, const unsigned char ucPostMapArray[256]);
	bool Encode(const scImageGray& image, scRect roi, unsigned char thresh, bool invert=false);
	bool Encode(const scImageGray& image, scRect roi, unsigned char lothresh, unsigned char hithresh, int softness, bool invert=false);
	bool EncodePercent(const scImageGray& image, scRect roi, int nLowTailPercent, int nHighTailPercent, int nThreshPercent, bool invert=false);
	bool EncodePercent(const scImageGray& image, scRect roi, int nLowTailPercent, int nHighTailPercent, int nThreshLowPercent, int nThreshHighPercent, int softness, bool invert=false);

	bool IsBinary() const;

	void SetOffset(int left, int top);
	void GetOffset(int& left, int& top) const;
	void SetOffset(const scIPair& offset);
	scIPair	GetOffset() const;

	scIPair GetSize() const;
	long GetWidth() const;
	long GetHeight() const;

	bool GetImage(scImageGray& image) const;

	unsigned char GetPixelValue(const scIPair& position) const;
	unsigned char GetPixelValue(long x, long y) const;

	int NumRuns() const;
	int NumRunsRow(long rownum) const;

	const scRLERun* const * PointToRat() const;
	const scRLERun* PointToRow(long rownum) const;
	const scRLERun* PointToRun() const;

	void Mask(const scRLEBuffer& mask, unsigned char match, unsigned char replace, bool invert = false);

	enum Shape { eHoriz=0, eVert, eSquare };
	void GmorphMin(Shape shape = eSquare);
	void GmorphMax(Shape shape = eSquare);

private:
	void CalPixelMapBySoftThre(unsigned char map[256], unsigned char lothresh, unsigned char hithresh, int softness, bool invert);

	void GmorphMinHoriz();
	void GmorphMinVert();
	void GmorphMinSquare();
	void GmorphMaxHoriz();
	void GmorphMaxVert();
	void GmorphMaxSquare();

	void Min2Row(scRLERun* pRow1, scRLERun* pRow2, scRLERun* pRowResult);
	long Min2Row(scRLERun* pRow1, scRLERun* pRow2, scRLEBufferTemp* pRowResult);
	void Max2Row(scRLERun* pRow1, scRLERun* pRow2, scRLERun* pRowResult);
	long Max2Row(scRLERun* pRow1, scRLERun* pRow2, scRLEBufferTemp* pRowResult);

	long Mask2Row(scRLERun* pRowSrc, scRLERun* pRowMask, scRLEBufferTemp* pRowDst, unsigned char match, unsigned char replace);
	long Mask2Row_Invert(scRLERun* pRowSrc, scRLERun* pRowMask, scRLEBufferTemp* pRowDst, unsigned char match, unsigned char replace);

	long m_lHintNumRuns;
	long m_lOffsetX;
	long m_lOffsetY;
	long m_lWidth;
	long m_lHeight;
	long m_lRunNum;
	bool m_bBinary;

	long*		m_pRowRunNum;
	scRLERun*	m_pRLEBufferRun;
	scRLERun**	m_pRLEBufferRow;
};

struct SVCORE_API scRLERun
{
	unsigned char	value;
	unsigned long	length;

	scRLERun()
	{
		value  = 0;
		length = 0;
	}
};

#pragma pack(pop)
