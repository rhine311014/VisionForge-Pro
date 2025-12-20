#pragma once

#ifdef VS_FTPCLIENT_EXPORTS 
#define VS_FTPCLIENT_API _declspec(dllexport)
#else
#define VS_FTPCLIENT_API _declspec(dllimport)
#endif

#include "afxinet.h"
#include <vector>

#pragma pack(push,8)

typedef struct FileInformation
{
	CString strFileName;			   //文件名
	CString strFileType;			   //文件类型 <DIR>,文件夹;<FIL>,文件
	CString strFileLastWriteTime;      //最后修改时间
	CString strFileLastAccessTime;     //最后访问时间
	CString strFileCreationTime;       //创建时间
	CString strFileSize;			   //文件大小 文件夹为空
}FileInfo;

class CInternetSessionEx;

class VS_FTPCLIENT_API CFTPClient
{
public:
	CFTPClient();
	~CFTPClient();

	enum ClearFloderType
	{
		eCurDirFile = 0,
		eAllFile,
		eAllFileAndFloder
	};

	//状态回调
	void EnableStatusCallback(BOOL bEnable);                                                                                        //是否开启状态回调
	void SetCallBack(void* pOwner, INTERNET_STATUS_CALLBACK funcCallback);                                                       //状态回调设置

	//连接
	BOOL Connect(CString strIP,CString strUserName,CString strPassword,int nPortNum,CString &strErrorInfo,BOOL bPassive = FALSE);   //连接FTP         参数：服务端IP，用户名，密码，服务端端口号,错误类型,是否采用被动模式(数据传输)
	void DisConnect();																											    //断开连接FTP
	void Destroy();																												    //销毁定义的FTP相关对象

	//工作目录
	BOOL GetCurDirectory(CString &strCurDirPathName);                                                                               //获取当前服务器路径
	BOOL SetCurDirectory(CString strCurDirPathName);																			    //设置当前服务器路径
	BOOL SetSubDirectory(CString strDirName);                                                                                       //进入下一级目录   参数：文件夹名称
	BOOL SetPreDirectory();                                                                                                         //进入上一级目录  
	void GetListContent(CString strDirPathNmae,std::vector<FileInfo> &vecFileInfo);												    //获取目录下文件列表

	//文件夹和文件相关操作
	BOOL CreateFloder(CString strFloderPathName);                                                                                       //新建文件夹        参数：新文件夹路径及名称
	BOOL RemoveFloder(CString strFloderPathName,BOOL bIteration = TRUE);															    //删除文件夹        迭代删除当前文件夹以及文件夹下所有文件  参数：文件夹路径及名称                       
	BOOL ClearFloder(CFTPClient::ClearFloderType cfType);                                                                                                          //清空文件夹        清空文件夹下所有文件
	BOOL CreateFile(CString strFileName,BOOL bFailIfExists = TRUE);                                                                     //创建文件          参数：新建文件路径及名称 是否存在重名文件(为TRUE时，若存在则创建失败，为FALSE时，若存在则覆盖)
	BOOL RemoveFile(CString strFilePathName);                                                                                           //删除文件          参数：要删除的文件路径及名称
	BOOL ReName(CString strOldName,CString strNewName);                                                                                 //重命名            参数：文件新名称

	BOOL IsDirectoryExist(CString strFloderPathName);


	//本地文件操作
	BOOL UploadFile(CString strLocalFile, CString strRemoteFile,
		            DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY, DWORD_PTR dwContext = 1);                                             //上传文件           参数：本地文件名，服务端文件名,数据类型，上下文标识
	BOOL DownloadFile(CString strRemoteFile, CString strLocalFile,
		              BOOL bFailIfExists = TRUE,DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL,                                         //下载文件           参数：服务端文件名，本地文件名，
			          DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY, DWORD_PTR dwContext = 1);  											//                         是否已存在同名文件(为TRUE时，若存在则下载失败，为FALSE时，若存在则覆盖原文件)，文件属性，数据类型，上下文标识
	void BackupLocalFile(CString strLocalFilePath,CString strBackupFilePath);                                                       //本地文件备份       参数：本地上传文件路径及文件名，备份路径及文件名
	void DeleteLocalFile(CString strLocalFilePath);                                                                                 //本地文件删除       参数：本地上传文件路径及文件名

	//内存文件操作
	CInternetFile* InternetFileOpen(CString strFileName,DWORD dwAccess = GENERIC_WRITE,DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY, DWORD_PTR dwContext = 1);		//新建或打开InternetFile文件
	BOOL InternetFileWrite(const void* lpBuf, UINT nCount);                                                                                                     //向文件内写数据  参数为指向要写入的第一个字节的指针， 数据量
	BOOL InternetFileRead(void* lpBuf, UINT nCount,UINT &nRealCount);                                                                                           //读取数据到内存中。参数：指向读取文件数据的内存地址的指针，要写入的字节数，实际写入的字节数。nRealCount<=nCount
	void InternetFileClose();                                                                                                                                   //关闭InternetFile文件

	//本地文件夹下文件上传并删除(备份)
	BOOL UploadFileInLocalFloder(BOOL bBackup,CString strLocalPath,CString strFileType ,CString strFTPPath,CString strBackupPath = _T(""),BOOL bSubfoldersUpload = FALSE);                                 //上传本地夹内文件   参数：是否备份，上传路径，备份路径
	BOOL UploadFileOneDirectory(BOOL bBackup,CString strLocalPath,CString strFileType ,CString strFTPPath,CString strBackupPath);

private:
	CInternetSessionEx* m_pInetSession;	//会话对象
	CFtpConnection* m_pFtpConnection;	//连接对象
	CFtpFileFind* m_pRemoteFinder;		//远程查找文件对象
	CInternetFile* m_pInternetFile;     //文件创建、内存文件上传
	HANDLE m_hEvent;                    //线程是否结束事件
	BOOL m_bThread;                     //线程开启标志
	int m_nStatus;                      //线程中是否发送指令标志(1发送，0不发送)
	
	CWinThread* m_cwThread;
	
	BOOL OnDeleteFloder(CString strFloderPathName);																					//递归删除文件夹函数
	BOOL OnDeleteFile(CString strFloderPathName);																					//递归删除文件函数
	CString OnGetErrorStr(DWORD dwErrorCode);                                                                                       //获取错误描述
	static UINT MonitorThread( LPVOID pParam );	//网络监测线程函数

public:
	CString m_strFTPError;
	BOOL m_bEnbaleCallback;						//开启链接状态监控
	unsigned long m_lConnectTimeOut;			//连接超时（ms）
	unsigned long m_lControlSendTimeOut;		//控制通道命令发送超时（ms）
	unsigned long m_lControlReceiveTimeOut;		//控制通道命令接收超时（ms）
	unsigned long m_lDataSendTimeOut;			//数据通道命令发送超时（ms）
	unsigned long m_lDataReceiveTimeOut;		//数据通道命令接收超时（ms）

	CRITICAL_SECTION m_cs;						//临界区
};

#pragma pack(pop)