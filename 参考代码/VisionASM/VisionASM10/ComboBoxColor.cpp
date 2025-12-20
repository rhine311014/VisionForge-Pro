// ComboBoxColor.cpp : 实现文件
//

#include "stdafx.h"
#include "ComboBoxColor.h"


// CComboBoxColor

IMPLEMENT_DYNAMIC(CComboBoxColor, CComboBox)

CComboBoxColor::CComboBoxColor()
{

}

CComboBoxColor::~CComboBoxColor()
{
}


BEGIN_MESSAGE_MAP(CComboBoxColor, CComboBox)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CComboBoxColor 消息处理程序




void CComboBoxColor::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	// This function of course does all the work.

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	CRect rect(&(lpDrawItemStruct->rcItem));

	// This switch statement draws the item in the combo box based on the itemID.
	// The itemID corresponds to the index in the COLORREF array.
	switch(lpDrawItemStruct->itemID)
	{
		// The automatic case
	case 16:
		{
			// Create the brush
			CBrush brush(RGB(128,128,128));
			CRect rect(&(lpDrawItemStruct->rcItem));
			rect.InflateRect(-2, -2);
			// Color the area
			dc.FillRect(rect, &brush);
			// Draw teh focus rect if the mosue is either over the item, or if the item
			// is selected
			if (lpDrawItemStruct->itemState & ODS_SELECTED)
				dc.DrawFocusRect(rect);

			//// Draw the text
			//CString strColor = "Automatic";
			//CSize textSize = dc.GetOutputTextExtent(strColor);
			//dc.SetBkMode(TRANSPARENT);
			//dc.SetTextColor(RGB(255, 255, 255));
			//dc.DrawText(strColor, rect, DT_SINGLELINE | DT_CENTER);

		}
		break;
	default:
		{
			// Drawing code for items accept the automatic color

			// Create the brush
			CString strItemText;
			GetLBText(lpDrawItemStruct->itemID,strItemText);
			COLORREF itemColor;
			std::map<CString,COLORREF>::iterator it =colorItem.find(strItemText);
			if (it== colorItem.end())
			{
				itemColor = RGB(128,128,128);
			}
			else
			{
				itemColor = it->second;
			}
			CBrush brush(itemColor);
			CRect rect(&(lpDrawItemStruct->rcItem));
			rect.InflateRect(-2, -2);
			// Color the area
			dc.FillRect(rect, &brush);
			// Draw the focus rect if the mouse is either over the item, or if the item
			// is selected
			if (lpDrawItemStruct->itemState & ODS_SELECTED)
				dc.DrawFocusRect(rect);
		}


	}

	// This draws the black frame around each of the colors so that they
	// do not look like they are kind of blended together
	CBrush frameBrush(RGB(0, 0, 0));
	dc.FrameRect(rect, &frameBrush);
	rect.InflateRect(-1, -1);
	CString strText;
	GetLBText(lpDrawItemStruct->itemID,strText);
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(strText,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	dc.Detach();
}
int CComboBoxColor::AddString(LPCTSTR lpszString,COLORREF nColor)
{
	colorItem.insert(make_pair(CString(lpszString),nColor));
	return CComboBox::AddString(lpszString);	

}
int CComboBoxColor::InsertString(int nIndex, LPCTSTR lpszString,COLORREF nColor)
{
	colorItem.insert(make_pair(CString(lpszString),nColor));
	return CComboBox::InsertString(nIndex,lpszString);
}

void CComboBoxColor::ResetContent()
{
	colorItem.clear();
	CComboBox::ResetContent();
	//PreSubclassWindow();
}
void CComboBoxColor::OnDestroy()
{
	CComboBox::OnDestroy();
	ResetContent();
	// TODO: 在此处添加消息处理程序代码
}


void CComboBoxColor::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	//CComboBox::AddString(_T("磁盘占用率"));
	//colorItem.push_back(RGB(128,128,128));
	//SetCurSel(0);
	CComboBox::PreSubclassWindow();
}
