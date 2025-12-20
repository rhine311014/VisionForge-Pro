#pragma once
#include "Layout.h"

class CHorizontalLayout: public CLayout
{
public:
	CHorizontalLayout(CLayout* pParent=NULL);
	~CHorizontalLayout(void);

	virtual void OnSize();
};

