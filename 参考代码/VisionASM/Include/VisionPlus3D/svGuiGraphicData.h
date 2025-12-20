#pragma once

#include "svGuiGraphic.h"
#include "svGuiGraphic3D.h"
#include "svDxfFileDef.h"
#include "svShapeData.h"
#include <vector>

#pragma pack(push,8)

class SVDXFFILE_API scGuiGraphicData
{
public:
	scGuiGraphicData();
	scGuiGraphicData(const scGuiGraphicData& guiGraphic);
	scGuiGraphicData(const std::vector<scGuiGraphic*>& vpGuiGraphic,const std::vector<scGuiGraphic3D*>& vpGuiGraphic3D);
	virtual ~scGuiGraphicData();

	scGuiGraphicData& operator=(const scGuiGraphicData& guiGraphic);

	// 添加Gui2D图形（可添加Gui2D图形数组或单个Gui2D图形）
	void AddGuiGraphic(const std::vector<scGuiGraphic*>& guiGraphic);
	void AddGuiGraphic(scGuiGraphic* guiGraphic);

	// 添加Gui3D图形（可添加Gui3D图形数组或单个Gui3D图形）
	void AddGuiGraphic3D(const std::vector<scGuiGraphic3D*>& guiGraphic3D);
	void AddGuiGraphic3D(scGuiGraphic3D* guiGraphic3D);

	// 移除Gui2D图形（可移除单个Gui2D图形或移除一段Gui2D图形）
	void MoveGuiGraphic(int nIndex);
	void MoveGuiGraphic(int firstIndex,int endIndex);

	// 移除Gui3D图形（可移除单个Gui3D图形或移除一段Gui3D图形）
	void MoveGuiGraphic3D(int nIndex);
	void MoveGuiGraphic3D(int firstIndex,int endIndex);


	// 清空Gui2D图形
	void ClearGuiGraphic();

	// 清空Gui3D图形
	void ClearGuiGraphic3D();

	// 设置/获取Gui2D图形数据
	void SetGuiGraphic(scGuiGraphic* guiGraphic,int nIndex);
	scGuiGraphic* GetGuiGraphic(int nIndex)const;
	std::vector<scGuiGraphic*> GetGuiGraphic()const;
	std::vector<scGuiGraphic*> GetGuiGraphic(int firstIndex,int endIndex)const;

	// 设置/获取Gui3D图形数据
	void SetGuiGraphic3D(scGuiGraphic3D* guiGraphic3D,int nIndex);
	scGuiGraphic3D* GetGuiGraphic3D(int nIndex)const;
	std::vector<scGuiGraphic3D*> GetGuiGraphic3D()const;
	std::vector<scGuiGraphic3D*> GetGuiGraphic3D(int firstIndex,int endIndex)const;

	// 获取Gui图形数量
	int GetGuiGraphicNum()const;

	// 获取Gui3D图形数量
	int GetGuiGraphic3DNum()const;

	// Gui图形数据到图形数据的转换
	scShapeData GuiGraphicToShape();

private:
	void DeleteGuiGraphic(scGuiGraphic* guiGraphic);
	void DeleteGuiGraphic3D(scGuiGraphic3D* guiGraphic3D);

protected:
	std::vector<scGuiGraphic*> m_vpGuiGraphic;		 //  Gui图形数组
	std::vector<scGuiGraphic3D*> m_vpGuiGraphic3D;	 //  Gui3D图形数组
};

#pragma pack(pop)