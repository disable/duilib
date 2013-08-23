//////////////////////////////////////////////////////////////////////////
//
//类:http 多线程断点续传   
//作者: 王勰(Nature Virus) 
//时间: 2012.10.7 - 2012.10.9
//备注: 如有错误 或 不方便的调用 可以联系我.或自行修改
//QQ: 8035408
//
//////////////////////////////////////////////////////////////////////////



#pragma once


#include <Windows.h>
#include <wininet.h>
#include <iostream>
#include <process.h>
#include <time.h>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>


class _Nv_Download;

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "ws2_32.lib")

//string
using std::string;
using std::vector;
using std::sort;


#define		Msg(msg)		::MessageBox(NULL, msg, "信息", 0)
#define		DegMsg(msg)		OutputDebugStringA(msg)
#define		DegMsgW(msg)	OutputDebugStringW(msg)

//////////////////////////////////////////////////////////////////////////
//
#define		Run_Mode_1								FALSE
#define		Run_Mode_2								TRUE
//返回状态
#define		Http_Return_206							206
#define		Http_Return_200							200

//线程退出
#define		Thread_download_Out						0x110
#define		Thread_download_Ok						0x111
#define		Thread_ThreadTask_Out					0x112
#define		Thread_Download_Fun						0x113

//信号状态
#define		T_X										0x1
#define		T_Y										0x2
#define		T_Z										0x3

//两种状态
#define		Thread_OK								0x01
#define		Thread_ON								0x02


//////////////////////////////////////////////////////////////////////////
//断点传文件
#define		FILE_SAVE_STRCAT_BUF					".NatureVirusDownloadFile"


//断点续传所需的每个线程的状态
struct _Nv_Tag_Download_Data
{
	__int64		n64X_;
	__int64		n64DownloadCnt_;
	__int64		n64TaskCnt_;

	_Nv_Tag_Download_Data()
	{
		n64X_ = n64TaskCnt_ = n64DownloadCnt_ = 0;
	}

	bool operator <   (const _Nv_Tag_Download_Data& rhs )   const 
	{
		return n64X_ < rhs.n64X_;
	}
	bool operator >   (const _Nv_Tag_Download_Data& rhs )   const
	{
		return n64X_ > rhs.n64X_;
	}
};


class _Nv_Download_Task
{
public:
	//构造
	//平均任务下载
	_Nv_Download_Task(
		__in CONST CHAR *strDownloadUrl, 
		__in CONST CHAR *strSaveFilePath, 
		__in __int64 _n64X,  
		__in __int64 _n64Size,
		__out int *nCallCnt,
		__in CONST CHAR *strCoolie,
		__in BOOL bFalg = Run_Mode_2
		);
	//普通构造
	_Nv_Download_Task();
	~_Nv_Download_Task();

public:
	//错误记录
	DWORD		dwError_;

	//http
	HINTERNET	Hinternet_Main_;
	HINTERNET	Hinternet_OpenUrl_;
	HINTERNET	Hinternet_Connect_;
	HINTERNET	Hinternet_Request_;
	
	//解析
	CHAR		strHostName_[1024];
	CHAR		strFileName_[1024];

	//引用数据
	int		*nCallByteCnt_;
	__int64	*n64CallByteCnt_;

	//记录url
	string		strUrl_;
	//记录保存文件路径
	string		strFilePath_;
	//记录断点续传文件
	string		strDownloadFile_;
	//记录文件大小
	__int64		n64FileSize_;

	//记录x y
	__int64		n64X_, n64Y_;

	//目标下载字节
	__int64		n64DownloadSize_;

	//停止下载
	BOOL		bStop_Download_;
	HANDLE		Handle_Stop_Download_;
	//线程句柄
	HANDLE		Handle_Download_Ok;

	//记录当前工作类下载了多少字节
	__int64		n64_Download_Byte_Cnt_;

	//限速
	DWORD		*dwSleep_;
	//已完成任务
	DWORD		dwTaskOk_;

	//是否有Coolie
	BOOL		bCoolie_;
	string		strCoolie_;

	//下载类指针
	_Nv_Download	*Nv_Download_Ptr_;
	//////////////////////////////////////////////////////////////////////////
	//函数
	//销毁句柄
	BOOL	NvInternetCloseHandle	(__in HINTERNET hDelete);
	//下载函数
	BOOL	NvDownload_Task			(__in CONST CHAR *strSaveFilePath);
	//获取管理器类指针
	BOOL	get_DownloadThisPtr		(__in _Nv_Download *nv_this);
	//设置停止信号
	BOOL	stop_Task				(VOID);
	
	//获取文件大小
	__int64	get_UrlFileSizeAndFileName(
		__in	CONST CHAR *strUrl,
		OUT CHAR *strFileName, 
		__in	CONST CHAR *strCoolie
		);

	//测试是否支持多线程
	INT		Detece_ThreadMode		( 
		__in CONST CHAR *strUrl, 
		__in __int64 dwMinPos, 
		__in __int64 dwMaxPos, 
		__in CONST CHAR *strCoolie 
		);

	//单线程下载
	__int64 SinglethreadDwondLoad	( 
		__in CONST CHAR *strDownloadUrl, 
		__in CONST CHAR *strSaveFilePath, 
		__out __int64 *n64CallCnt, 
		__in CONST CHAR *strCoolie
		);

protected:
private:
	//打开INET
	BOOL NvInternetOpen		(__in CONST CHAR *strHttpName, __in INT nFalg);
	//直接打开URL
	BOOL NvInternetOpenUrl	(__in CONST CHAR *strUrl, __in INT nFalg);
	//解析URL
	BOOL NvAnalyzeURL		(__in CONST CHAR *strUrl);
	//连接HTTP
	BOOL NvInternetConnect	(__in CHAR *strHostName);
	//添加信息头
	BOOL NvHttpOpenRequest	(__in CONST CHAR *strAcceptTypes, __in CHAR *strFileName, __in CONST CHAR *strHttpType, __in DWORD dwFalg);
	//发送请求
	BOOL NvHttpSendRequest	(VOID);
	//设置断点续传位置
	BOOL NvHttpAddRequestHeaders(__in HINTERNET hRequest, __in __int64 dwMinPos, __in __int64 dwMaxPos, __in INT nFalg);
	//测试状态 等
	__int64 NvHttpQueryInfo	(__in HINTERNET hRequest, __in INT nFalg);
	VOID	NvHttpQueryInfo	(__in HINTERNET hRequest, __in INT nFalg, OUT CHAR *strFileName);
	
	//////////////////////////////////////////////////////////////////////////
	//初始化
	VOID	Init(VOID);
	//////////////////////////////////////////////////////////////////////////
	//测试返回值
	DWORD	GetHttpReturnData	(
		__in	CONST CHAR	*strUrl, 
		__in	__int64 dwMinPos, 
		__in	__int64 dwMaxPos, 
		__in	CONST CHAR	*strCoolie
		);

	BOOL	Thread_Task				(VOID);

	//停止下载
	VOID	stop_Download			(VOID);
	__int64 get_Download_Byte_Cnt	(VOID);
	VOID	set_Coolie				(__in CONST CHAR *strCoolie);
};




class _Nv_Download
{
public:
	//////////////////////////////////////////////////////////////////////////
	//构造
	_Nv_Download();
	_Nv_Download( 
		__in CONST CHAR *strDownloadUrl, 
		__in CONST CHAR *strSaveFilePath, 
		__in CONST CHAR *strCoolie, 
		__in BOOL bModeFalg  = Run_Mode_2
		);
	~_Nv_Download();

public:
	//////////////////////////////////////////////////////////////////////////
	//对象
	CRITICAL_SECTION chitical_section_;
	CRITICAL_SECTION chitical_section_Add;

	BOOL		bCoolie_;
	string		strCoolie_;

	//该任务最大线程量
	DWORD		dwThreadCnt_;
	//每个任务下载量
	__int64		n64ThreadDownloadCnt_;
	//下一个任务的起始位置
	__int64		n64ThreadTaskSion_;
	//文件大小
	__int64		n64FileSize_;

	//下载线程列表
	vector<_Nv_Download_Task*>		vtThread_Task_;

	//记录当前任务中的url
	string		strUrl_;
	string		strFileSavePath_;
	//续传文件
	string		strDownloadFile_;

	//下载量
	__int64		n64DownloadCount_;
	int			nDataCount_;
	int			*nDownloadCount_;
	DWORD		dwDownloadVectorSize_;

	//检查任务线程安全信号
	HANDLE		Handle_DetectTask_;
	//检查任务线程安全信号强制退出
	HANDLE		Handle_DetectTask_Out_;
	//线程管理器安全信号
	HANDLE		Handle_ThreadTask_;
	//线程管理器强制退出信号
	HANDLE		Handle_ThreadTask_Out_;

	//以前下载量
	__int64		n64JiDonwloadCnt_;

	//时间差
	__int64		n64TimeDownloadCnt_;

	//当前速率
	DWORD		dwIsSpeed_;

	//保存断点续传每个线程的数据
	vector<_Nv_Tag_Download_Data>	vtTagTaskData;
	//未完成下载 需要重新下载
	vector<_Nv_Tag_Download_Data>	vtDownloadNo_;

	//停止任务下载
	BOOL		bTaskStop_;

	//下载任务管理器已经退出
	BOOL		bDownloadTaskCmd_;

	//单线程下载
	_Nv_Download_Task				*Nv_One_Download_Task_;
	//标记是单线程下载
	BOOL							bThreadMode_;
	//单线程下载的计数器
	__int64							n64ThreadOneCnt_;

public:
	//////////////////////////////////////////////////////////////////////////
	//函数
	VOID	Init(VOID);
	//获取文件大小
	__int64	get_TaskFileSize(__in CONST CHAR *strDownloadUrl);

	//获取当前已经下载多少字节
	__int64 get_DownloadCount		(VOID);
	//设置速度差
	VOID	set_TimeCount			(VOID);
	//获取下载速率
	__int64 get_DownloadSpeed		(VOID);
	//获取完成下载还剩余多少时间 返回字符串
	CHAR*	get_SurplusDownloadTime	(VOID);
	//设置多线程数量
	VOID	set_ThreadCount			(__in DWORD dwThreadCount);
	//设置每个线程需要下载多少字节
	VOID	set_DownloadTaskCnt		(__in DWORD dwDownloadTaskCnt);
	//检查任务是否完成
	BOOL	detect_TaskComplete		(VOID);
	//安全关闭下载
	BOOL	thread_SecurityCloseTask(VOID);
	//检查线程下载任务是否完成
	VOID	detect_vtThread_Task	(VOID);
	//获取完成下载还剩余多少时间 返回秒速
	DOUBLE	get_SurplusDownloadTime_Dword(VOID);
	//获取是否关闭下载
	BOOL	get_TaskStop			(VOID);
	//设置连接数
	VOID	set_HttpLinkCount		(__in DWORD dwCount);
	//获取线程管理器是否已经退出
	BOOL	get_DownloadTaskCmdOut	(VOID);
	//供外部显示进度代码判断是否可以停止显示进度
	BOOL	get_StopPrintf			(VOID);
	//是否可以销毁该类
	BOOL	detect_DownlaodIsClose	(VOID);
	//单线程函数
	VOID	detect_vtThreadOne_Task	(VOID);
	//检查是多线程下载还是单线程下载
	BOOL	deetct_ThreadMode		(VOID);
	
	
	//获取工作线程状态
	VOID	get_ThreadTaskData(
		__in __int64 n64X, 
		__in __int64 n64TaskCnt, 
		__in __int64 n64DownloadCnt
		);

	//任务
	BOOL	thread_Task( 
		__in CONST CHAR *strDownloadUrl, 
		__in CONST CHAR *strSaveFilePath, 
		__in CONST CHAR *strCoolie, 
		__in BOOL bModeFalg  = Run_Mode_2
		);

	//单线程任务
	BOOL thread_OneTask( 
		__in CONST CHAR *strDownloadUrl, 
		__in CONST CHAR *strSaveFilePath, 
		__in CONST CHAR *strCoolie, 
		__in BOOL bModeFalg  = Run_Mode_2
		);

	//多线程下载调用
	BOOL	Download(
		__in CONST CHAR *strUrl, 
		__in CONST CHAR *strFileSavePath, 
		__in CONST CHAR *strCoolie, 
		__in BOOL bModeFalg = Run_Mode_2
		);

protected:
private:
	//分配任务
	BOOL	call_AssignTasks		(__in __int64 &n641, __in __int64 &n642, __in __int64 &n643);
	//停止检查下载任务
	VOID	stop_DetectTask		(VOID);
	//停止线程下载任务
	BOOL	stop_ThreadTask		(VOID);
	//创建断点续传文件
	BOOL	create_DwonloadFile	(VOID);
	//读写断点续传文件
	BOOL	read_DwonloadFile	(VOID);

	//断点续传线程布置
	__int64	set_ThreadTaskData(
		__out __int64 &n64X, 
		__out __int64 &n64TaskCnt,
		__out __int64 &n64Count
		);

	//修补出错线程
	BOOL decete_TaskFailure( 
		__in CONST CHAR *strDownloadUrl, 
		__in CONST CHAR *strSaveFilePath, 
		__in CONST CHAR *strCoolie 
		);
};