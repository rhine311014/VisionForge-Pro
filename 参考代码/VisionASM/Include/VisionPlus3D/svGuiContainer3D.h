#pragma once

#include "svGuiGraphic3D.h"
#include "svMatrix4.h"
#include <map>

#pragma pack(push,8)

class scGuiStringCollection3D;
class SVGEOMETRICSGUI_API_3D scGuiContainer3D
{
public:
	scGuiContainer3D();
	~scGuiContainer3D();

	bool AddItem(scGuiGraphic3D* pGuiGraphic3D,CString strLabel);

	bool Remove(CString strLabel);
	void clear();

	int GetCount()const;
	scGuiGraphic3D* GetItem(int nIndex)const;

	void SetGlobalToWindowMatrix(const scMatrix4& m) {m_matGlobalToWindow = m;}
	scMatrix4 GetGlobalToWindowMatrix()const {return m_matGlobalToWindow;}

private:
	std::map<CString,scGuiGraphic3D*>m_gm;
	scGuiStringCollection3D* m_pSc;

	scMatrix4 m_matGlobalToWindow;
};

#pragma pack(pop)