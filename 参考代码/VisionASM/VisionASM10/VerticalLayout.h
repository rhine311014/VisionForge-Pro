#pragma once
#include "layout.h"
class CVerticalLayout :	public CLayout
{
public:
	CVerticalLayout(CLayout* pParentLayout=NULL);
	virtual ~CVerticalLayout(void);

	virtual void OnSize();
};

