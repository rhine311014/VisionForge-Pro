#include "StdAfx.h"
#include "Layout.h"


CLayout::CLayout(CLayout* pParentLayout/*=NULL*/): m_pParentLayout(pParentLayout)
{
	m_pParentWnd = NULL;      // 布局所在窗口 
	m_vpChildPtr.clear();
	memset(&m_rtLayout, 0, sizeof(RECT));
	m_bIsVisible = true;
	m_backGroundColor = RGB(255, 255, 255);
	m_bPaintBkColor = false;
	m_bUseParentLayoutSize = false;
	m_bAutoDeleteChild = true;
}


CLayout::~CLayout(void)					  
{
	if (m_bAutoDeleteChild)
	{
		for (int i = 0; i < m_vpChildPtr.size(); i++)
		{
			if (m_vpChildPtr[i].bIsLayout)
			{
				delete (CLayout*)m_vpChildPtr[i].pPtr;
			}
		}
	}

	m_vpChildPtr.clear();
}


void CLayout::SetParent(CWnd* pParent)
{ 
	for (int i = 0; i < m_vpChildPtr.size(); i++)
	{
		if (m_vpChildPtr[i].bIsLayout)
		{
		    ((CLayout*)m_vpChildPtr[i].pPtr)->SetParent(pParent);
		}
	}

	m_pParentWnd = pParent;
}


void CLayout::SetParentLayout(CLayout* pParentLayout)
{
	m_pParentLayout = pParentLayout;
	m_bUseParentLayoutSize = true;
	if (pParentLayout->GetParent())
	{
		SetParent(pParentLayout->GetParent());
	}
}

void CLayout::AddWnd(CWnd* pWnd, int w, int h, SizePolicy eSizePolicy, int nLeftOffset, int nTopOffset, int nRightOffset, int nBottomOffset, bool bAreaInvisible/*=false*/)
{ 
	_Child_Ptr chdPtr;
	chdPtr.bIsLayout = false;
	chdPtr.pPtr = pWnd;
	chdPtr.eHoriSizePolicy = eSizePolicy;
	chdPtr.eVertSizePolicy = eSizePolicy;
	chdPtr.nheight = h;
	chdPtr.nWidth = w;
	chdPtr.nLeftAlignOffset = nLeftOffset;
	chdPtr.nTopAlignOffset = nTopOffset;
	chdPtr.nRightAlignOffset = nRightOffset;
	chdPtr.nBottomAlignOffset = nBottomOffset;
	chdPtr.bAreaInvisible = bAreaInvisible;

	m_vpChildPtr.push_back(chdPtr); 
}


void CLayout::AddLayout(CLayout* pLayout, int w, int h, SizePolicy eSizePolicy, int nLeftOffset, int nTopOffset, int nRightOffset, int nBottomOffset)
{
	_Child_Ptr chdPtr;
	chdPtr.bIsLayout = true;
	chdPtr.pPtr = pLayout;
	pLayout->m_pParentLayout = this;
	chdPtr.eHoriSizePolicy = eSizePolicy;
	chdPtr.eVertSizePolicy = eSizePolicy;
	chdPtr.nheight = h;
	chdPtr.nWidth = w;

	chdPtr.nLeftAlignOffset = nLeftOffset;
	chdPtr.nTopAlignOffset = nTopOffset;
	chdPtr.nRightAlignOffset = nRightOffset;
	chdPtr.nBottomAlignOffset = nBottomOffset;

	m_vpChildPtr.push_back(chdPtr);
}

void CLayout::ResetWnd(CWnd* pWnd, int w, int h, SizePolicy eSizePolicy, int nLeftOffset, int nTopOffset, int nRightOffset, int nBottomOffset)
{
	for (int i = 0; i < m_vpChildPtr.size(); i++)
	{
		if (m_vpChildPtr[i].pPtr == pWnd)
		{
			m_vpChildPtr[i].eHoriSizePolicy = eSizePolicy;
			m_vpChildPtr[i].eVertSizePolicy = eSizePolicy;
			m_vpChildPtr[i].nheight = h;
			m_vpChildPtr[i].nWidth = w;
			m_vpChildPtr[i].nLeftAlignOffset = nLeftOffset;
			m_vpChildPtr[i].nTopAlignOffset = nTopOffset;
			m_vpChildPtr[i].nRightAlignOffset = nRightOffset;
			m_vpChildPtr[i].nBottomAlignOffset = nBottomOffset;
			break;
		}
	}
}


void CLayout::ResetLayout(CLayout* pLayout, int w, int h, SizePolicy eSizePolicy, int nLeftOffset, int nTopOffset, int nRightOffset, int nBottomOffset)
{
	for (int i = 0; i < m_vpChildPtr.size(); i++)
	{
		if (m_vpChildPtr[i].pPtr == pLayout)
		{
			m_vpChildPtr[i].eHoriSizePolicy = eSizePolicy;
			m_vpChildPtr[i].eVertSizePolicy = eSizePolicy;
			m_vpChildPtr[i].nheight = h;
			m_vpChildPtr[i].nWidth = w;
			m_vpChildPtr[i].nLeftAlignOffset = nLeftOffset;
			m_vpChildPtr[i].nTopAlignOffset = nTopOffset;
			m_vpChildPtr[i].nRightAlignOffset = nRightOffset;
			m_vpChildPtr[i].nBottomAlignOffset = nBottomOffset;
			break;
		}
	}
}



 int CLayout::GetChildCount()
 {
	 return m_vpChildPtr.size(); 
 }


 void CLayout::OnDraw()
 {
	 if (!m_pParentWnd)
	 {
		return;
	 }

	 if (m_bPaintBkColor)
	 {
// 		 Gdiplus::Graphics graphics(m_pParentWnd->GetSafeHwnd());
// 		 Gdiplus::Rect rect(m_rtLayout.left, m_rtLayout.top, m_rtLayout.right-m_rtLayout.left, m_rtLayout.bottom - m_rtLayout.top);
// 		 Gdiplus::Color bkClr;
// 		 bkClr.SetFromCOLORREF(m_backGroundColor);
// 		 Gdiplus::SolidBrush brush(bkClr);
// 		 Gdiplus::Color penClr;
// 		 penClr.SetFromCOLORREF(RGB(255,255,255));
// 		 Gdiplus::Pen pen(penClr);
// 		 graphics.DrawRectangle(&pen, rect);
// 		 graphics.FillRectangle(&brush, rect);
	 }

	 for (int i = 0; i < m_vpChildPtr.size(); i++)
	 {
		 if (m_vpChildPtr[i].bIsLayout)
		 {
			 ((CLayout*)m_vpChildPtr[i].pPtr)->OnDraw();
		 }
	 }
 }

 void CLayout:: SetAllChildVisible(bool bVisile)
 {
	 for (int i = 0; i < m_vpChildPtr.size(); i++)
	 {
		 if (m_vpChildPtr[i].bIsLayout)
		 {
			 ((CLayout*)m_vpChildPtr[i].pPtr)->SetVisible(bVisile);
		 }
		 else
		 {
			 ((CWnd*)m_vpChildPtr[i].pPtr)->ShowWindow(bVisile?SW_SHOW:SW_HIDE);
		 }
	 }
 }


 void CLayout:: SetChildVisible(int nIndex, bool bVisile)
 {
	 if (nIndex < 0 || nIndex >= m_vpChildPtr.size())
	 {
		 return;
	 }

	 if (m_vpChildPtr[nIndex].bIsLayout)
	 {
		 ((CLayout*)m_vpChildPtr[nIndex].pPtr)->SetVisible(bVisile);
	 }
	 else
	 {
		 ((CWnd*)m_vpChildPtr[nIndex].pPtr)->ShowWindow(bVisile?SW_SHOW:SW_HIDE);
	 }
 }