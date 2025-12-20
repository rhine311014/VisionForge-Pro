#pragma once

#include "svEntitiesData.h"
#include "svDxfFileDef.h"
#include "svGuiGraphicData.h"
#include "svShapeData.h"
#include <vector>

#pragma pack(push,8)

class SVDXFFILE_API scModelData
{
public:
	scModelData();
	scModelData(const std::vector<CEntitiesBase*>& model);
	scModelData(const scModelData& model);
	virtual ~scModelData();

	scModelData& operator=(const scModelData& model);

	// 添加图元（可添加图元数组或单个图元）
	void AddEntities(const std::vector<CEntitiesBase*>& model);
	void AddEntities(CEntitiesBase* model);


	// 移除图元（可移除单个图元或移除一段图元）
	void MoveEntities(int nIndex);
	void MoveEntities(int firstIndex,int endIndex);

	// 移除3D图元
	void Move3DEntities();

	// 移除2D图元
	void Move2DEntities();

	// 清空图元
	void ClearEntities();

	// 设置/获取图元数据
	void SetEntities(CEntitiesBase* model,int nIndex);
	CEntitiesBase* GetEntities(int nIndex)const;
	std::vector<CEntitiesBase*> GetEntities()const;
	std::vector<CEntitiesBase*> GetEntities(int firstIndex,int endIndex)const;

	// 获取图元数量
	int GetEntitiesNum()const;

	// 模型数据与2D图形数据的转换
	scShapeData ModelToShape();

protected:
	std::vector<CEntitiesBase*> m_vpEntities; // 图元数组

};

#pragma pack(pop)