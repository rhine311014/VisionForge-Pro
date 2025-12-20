#pragma once

#ifdef SV_POINT_CLOUD_PROC_EXPORTS
#define SV_FILE_API_3D __declspec(dllexport)
#else
#define SV_FILE_API_3D __declspec(dllimport)
#endif

#include <vector>
#include <string>

#include "svMesh.h"

#pragma pack(push,8)

enum FileType
{
	eMesh   = 0,
	ePLY    = 1,
	ePCD    = 2,
	eSTL    = 3,
	eXYZ    = 4,
	ePTS    = 5
};

class SV_FILE_API_3D scFile3D
{
public:
	scFile3D();
	~scFile3D();

	bool Read(const scMesh& mesh);    // 从mesh读取模型
	bool Read(const TCHAR* filename); // 读取模型文件
	bool Write(const TCHAR* filname,bool bAscii = true,bool bcompressed = true) const; // 保存模型

	bool GetMesh(scMesh& mesh) const; // 获取mesh
	FileType GetFileType() const;  // 获取模型文件类型

	bool isValid() const {return (m_Mesh != NULL);}

	scMesh* GetMesh() {return m_Mesh;}

private:
	void Free();

    bool ReadPLY(FILE* file);
	bool ReadPCD(FILE* file);
	bool ReadSTL(FILE* file);
	bool ReadPTS(FILE* file);
	bool ReadXYZ(FILE* file);
	
	bool WritePLY(FILE* file,bool bAscii)const;
	bool WritePCD(FILE* file,bool bAscii,bool bcompressed)const;
	bool WriteSTL(FILE* file,bool bAscii)const;
	bool WritePTS(FILE* file) const;
	bool WriteXYZ(FILE* file) const;

private:
	FileType       m_Type;
	scMesh*        m_Mesh;
};

#pragma pack(pop)
