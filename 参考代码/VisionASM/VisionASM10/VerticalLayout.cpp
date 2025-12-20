#include "StdAfx.h"
#include "VerticalLayout.h"


CVerticalLayout::CVerticalLayout(CLayout* pParentLayout/*=NULL*/):CLayout(pParentLayout)
{
	SetLayoutType(CLayout::eLayout_Vertical);
}


CVerticalLayout::~CVerticalLayout(void)
{
}


void CVerticalLayout::OnSize()
{
	// 先判断窗口指针和句柄是否有效
	if (!m_pParentWnd || !m_pParentWnd->GetSafeHwnd())
	{
		return;
	}

	// 判断使用大小, 如果父Layout不为空, 则使用Layout的大小, 如果为NULL 使用窗口大小
	RECT rtLayout;

	if (m_pParentLayout)
	{
		if (!m_bUseParentLayoutSize)
		{
			rtLayout = m_rtLayout;
		}
		else
		{
			rtLayout = m_pParentLayout->GetLayoutRect();
		}

	}
	else
	{
		m_pParentWnd->GetClientRect(&rtLayout);
	}

	//  前一个区域位置
	int nChildCount = m_vpChildPtr.size();
	RECT rtPreChild = { 0, 0, 0, 0 };
	std::vector<RECT> vrtChilds;
	vrtChilds.resize(nChildCount);

	// 计算expand宽度
	int nFixed = 0;
	int nExpandLong = 0;
	int nExpandCnt = 0;

	int nPreferredLong = 0;
	int nPreferredCnt = 0;

	for (int i = 0; i < nChildCount; i++)
	{
		switch (m_vpChildPtr[i].eVertSizePolicy)
		{
		case eSizePolicy_Fixed:
			{
				if (m_vpChildPtr[i].bIsLayout)
				{
					if (((CLayout*)m_vpChildPtr[i].pPtr)->IsVisible())
					{
						nFixed += m_vpChildPtr[i].nheight;
					}
				}
				else
				{
					LONG lStyle =  GetWindowLong(((CWnd*)m_vpChildPtr[i].pPtr)->GetSafeHwnd(), GWL_STYLE);
					if(lStyle & WS_VISIBLE)
					{
						nFixed += m_vpChildPtr[i].nheight;
					}
				}
				
			}
			break;
		case eSizePolicy_Preferred:
			{
				if (m_vpChildPtr[i].bIsLayout)
				{
					if (((CLayout*)m_vpChildPtr[i].pPtr)->IsVisible())
					{
						nPreferredCnt += 1;
					}
				}
				else
				{
					LONG lStyle =  GetWindowLong(((CWnd*)m_vpChildPtr[i].pPtr)->GetSafeHwnd(), GWL_STYLE);
					if(lStyle & WS_VISIBLE)
					{
						nPreferredCnt += 1;
					}
				}

			}
			
			break;
		case eSizePolicy_Expand:
			{
				if (m_vpChildPtr[i].bIsLayout)
				{
					if (((CLayout*)m_vpChildPtr[i].pPtr)->IsVisible())
					{
						nExpandCnt += 1;
					}
				}
				else
				{
					LONG lStyle =  GetWindowLong(((CWnd*)m_vpChildPtr[i].pPtr)->GetSafeHwnd(), GWL_STYLE);
					if(lStyle & WS_VISIBLE)
					{
						nExpandCnt += 1;
					}
				}

			}
			break;
		}
	}

	if (nExpandCnt == 0 && nPreferredCnt > 0)
	{
		nPreferredLong =  (rtLayout.bottom-rtLayout.top - nFixed) / nPreferredCnt;
	}

	if (nExpandCnt > 0 && nFixed < (rtLayout.bottom - rtLayout.top))
	{
		nExpandLong = (rtLayout.bottom-rtLayout.top - nFixed) / nExpandCnt;
	}

	rtPreChild.left = rtLayout.left;
	rtPreChild.right = rtLayout.right;
	rtPreChild.bottom = rtLayout.top;

	//  为每个Layout规划新的大小
	for (int i = 0; i < nChildCount; i++)
	{
		switch (m_vpChildPtr[i].eVertSizePolicy)
		{
		case eSizePolicy_Fixed:
			{
				bool bIsVisible = false;

				if (m_vpChildPtr[i].bIsLayout)
				{
					if(((CLayout*)m_vpChildPtr[i].pPtr)->IsVisible())
					{
						bIsVisible = true;
					}
				}
				else
				{
					LONG lStyle =  GetWindowLong(((CWnd*)m_vpChildPtr[i].pPtr)->GetSafeHwnd(), GWL_STYLE);
					if(lStyle & WS_VISIBLE)
					{
						bIsVisible = true;
					}
				}

				if (bIsVisible)
				{
					vrtChilds[i].left	= rtPreChild.left;
					if (m_vpChildPtr[i].nWidth == 0)
					{
						vrtChilds[i].right	= rtPreChild.right;
					}
					else
					{
						vrtChilds[i].right	= vrtChilds[i].left + m_vpChildPtr[i].nWidth;
					}
				
					vrtChilds[i].top	= rtPreChild.bottom;
					vrtChilds[i].bottom = vrtChilds[i].top + m_vpChildPtr[i].nheight;
					rtPreChild = vrtChilds[i];
				}
				else
				{
					 vrtChilds[i].left = 0;
					 vrtChilds[i].right = 0;
					 vrtChilds[i].top = 0;
					 vrtChilds[i].bottom = 0;
				}
			}
			break;
		case eSizePolicy_Preferred:
			{
				bool bIsVisible = false;
				if (m_vpChildPtr[i].bIsLayout)
				{
					if(((CLayout*)m_vpChildPtr[i].pPtr)->IsVisible())
					{
						bIsVisible = true;
					}
				}
				else
				{
					LONG lStyle =  GetWindowLong(((CWnd*)m_vpChildPtr[i].pPtr)->GetSafeHwnd(), GWL_STYLE);
					if(lStyle & WS_VISIBLE)
					{
						bIsVisible = true;
					}
				}

				if (bIsVisible)
				{
					vrtChilds[i].left	= rtLayout.left;
					vrtChilds[i].right	= rtLayout.right;
					vrtChilds[i].top	= rtPreChild.bottom;
					vrtChilds[i].bottom = vrtChilds[i].top + nPreferredLong;

					rtPreChild = vrtChilds[i];
				}
				else
				{
					vrtChilds[i].left = 0;
					vrtChilds[i].right = 0;
					vrtChilds[i].top = 0;
					vrtChilds[i].bottom = 0;
				}
				
			}
			break;
		case eSizePolicy_Expand:
			{
				bool bIsVisible = false;
				if (m_vpChildPtr[i].bIsLayout)
				{
					if(((CLayout*)m_vpChildPtr[i].pPtr)->IsVisible())
					{
						bIsVisible = true;
					}
				}
				else
				{
					LONG lStyle =  GetWindowLong(((CWnd*)m_vpChildPtr[i].pPtr)->GetSafeHwnd(), GWL_STYLE);
					if(lStyle & WS_VISIBLE)
					{
						bIsVisible = true;
					}
				}

				if (bIsVisible)
				{
					vrtChilds[i].left	= rtLayout.left;
					vrtChilds[i].right	= rtLayout.right;
					vrtChilds[i].top	= rtPreChild.bottom;
					vrtChilds[i].bottom = vrtChilds[i].top + nExpandLong;
					rtPreChild = vrtChilds[i];
				}
				else
				{
					vrtChilds[i].left = 0;
					vrtChilds[i].right = 0;
					vrtChilds[i].top = 0;
					vrtChilds[i].bottom = 0;
				}
				
			}
			break;
		}
	}


	//  为每个Layout设置新的大小
	for (int i = 0; i < nChildCount; i++)
	{
		if (m_vpChildPtr[i].bIsLayout)
		{
			vrtChilds[i].left	+= m_vpChildPtr[i].nLeftAlignOffset;
			vrtChilds[i].right	-= m_vpChildPtr[i].nRightAlignOffset;
			vrtChilds[i].top	+= m_vpChildPtr[i].nTopAlignOffset;
			vrtChilds[i].bottom -= m_vpChildPtr[i].nBottomAlignOffset;
			((CLayout*)m_vpChildPtr[i].pPtr)->m_rtLayout = vrtChilds[i];
			((CLayout*)m_vpChildPtr[i].pPtr)->OnSize();
		}
		else
		{
			vrtChilds[i].left	+= m_vpChildPtr[i].nLeftAlignOffset;
			vrtChilds[i].right	-= m_vpChildPtr[i].nRightAlignOffset;
			vrtChilds[i].top	+= m_vpChildPtr[i].nTopAlignOffset;
			vrtChilds[i].bottom -= m_vpChildPtr[i].nBottomAlignOffset;
			CWnd* pOldParentWnd = ((CWnd*)m_vpChildPtr[i].pPtr)->GetParent();
			if (pOldParentWnd!= m_pParentWnd)
			{
				((CWnd*)m_vpChildPtr[i].pPtr)->SetParent(m_pParentWnd);
			}
			((CWnd*)m_vpChildPtr[i].pPtr)->MoveWindow(&vrtChilds[i], FALSE);
		}
	}

	m_pParentWnd->Invalidate();
}