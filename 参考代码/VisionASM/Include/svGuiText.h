// svGuiText.h

// ---------------------------------------------------------------------
//                        文本显示控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svText.h"

#pragma pack(push,8)

class SVGUI_API scGuiText : public scGuiGraphic  
{
public:
	scGuiText();
	virtual ~scGuiText();

	virtual GraphicType GetType() const {return eGuiText;};

	void SetXYRotationTextFont(scText text);

	void SetXYRotationText(double dX,					// 文本输出位置的X坐标
						   double dY,					// 文本输出位置的Y坐标
						   double dRotation,			// 文本输出的旋转角度(单位弧度)
						   LPCTSTR lpszText);			// 文本

	void SetXYRotation(double dX,						// 文本输出位置的X坐标
					   double dY,						// 文本输出位置的Y坐标
					   double dRotation);				// 文本输出的旋转角度(单位弧度)

	// 设置文本输出位置pos，旋转角度rot和文本lpszText
	void SetXYRotationText(const sc2Vector& pos, const scRadian& rot, LPCTSTR lpszText);

	// 设置文本输出位置pos，旋转角度rot
	void SetXYRotation(const sc2Vector& pos, const scRadian& rot);

	void GetXYRotation(double& dX,						// 文本输出位置的X坐标
					   double& dY,						// 文本输出位置的Y坐标
					   double& dRotation) const;		// 文本输出的旋转角度

	void GetXYRotation(sc2Vector& pos, scRadian& rot) const;// 文本输出的位置和旋转角度

	sc2Vector GetPos() const;							// 获取位置
	scRadian  GetRotation() const;						// 获取旋转角度


	void	SetText(LPCTSTR lpszText);					// 设置文本
	LPCTSTR GetText() const;							// 获取文本

	void	 SetTextFont(LOGFONT* pLogFont);			// 设置文本的字体
	LOGFONT* GetTextFont() const;						// 获取文本的字体


	BOOL Transform(double dScale, double dOffsetX, double dOffsetY);

	void Draw(CDC* pDC, LPRECT lpRect = NULL);
	void DrawLabel(CDC* pDC, LPRECT lpRect = NULL);
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;

	virtual BOOL BoundingRect(CRect& rect) const;

private:
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;

protected:
	void*	 m_pLShape;
	LOGFONT* m_pTextFont;
private:
	bool	 m_bAutoDeleteFont;
};

#pragma pack(pop)
