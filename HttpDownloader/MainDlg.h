#pragma once

#include "_Nv_Http_Download.h"
#include <vector>

using std::vector;



//DLL 要导出的函数
typedef BOOL (*DLL_SetUrL)			(CHAR *strFileUrl);
typedef BOOL (*DLL_SetClassStr)		(CHAR *strClassStr);
typedef BOOL (*DLL_SetCoolie)		(CHAR *strCoolie);

//类
class _Nv_Download_Ex;

//////////////////////////////////////////////////////////////////////////
//宏定义
#define WM_USER_PROG			WM_USER+100
#define WM_USER_TEXTPRINT		WM_USER+101
#define WM_USER_DELETE_ITEM		WM_USER+102


#define	Thread_OutCode_ProgRess	0x110
#define	Thread_OutCode_Print	0x120
#define Thread_OutCode_Sleep	0x130



//任务发布器传送的结构体
struct tagDownloadAttriBute
{
	__int64		int64FileSize;
	CHAR		strFileName[1024];
	CHAR		strURL[4096];
	CHAR		strFilePath[1024];
	CHAR		strCollie[4096];
	BOOL		bIsOpen;
	INT			nNameType;

	tagDownloadAttriBute()
	{
		int64FileSize = 0;
		memset(strFilePath, 0, sizeof(strFilePath));
		memset(strURL, 0, sizeof(strURL));
		memset(strFileName, 0, sizeof(strFileName));
		memset(strCollie, 0, sizeof(strCollie));
		bIsOpen	= FALSE;
		nNameType = -1;
	}

	~tagDownloadAttriBute()
	{

	}
};


//记录下载文件进度的结构体
struct tag_Donwload_File_Ini_Data 
{
	string strUrl;
	string strPath;
	string strFileName;
	string strDataPrint;
	string strCoolie;
	__int64 n64ProgRessPos;
	__int64 n64FileSize;
	BOOL	bOpenFile;

	tag_Donwload_File_Ini_Data()
	{
		strUrl =
		strPath = 
		strFileName = 
		strCoolie = 
		strDataPrint = "";

		n64ProgRessPos = 0;
		bOpenFile = FALSE;
	}
};


//线程任务数据结构体
struct tag_Download_Data 
{
	CProgressUI		*pProgress;
	_Nv_Download	*Nv_Download_;
	CTextUI			*pTextPrint;
	INT				nPos;
	CListUI			*pList;
	_Nv_Download_Ex	*pDownloadEx;
	BOOL			bMode;

	tag_Download_Data()
	{
		pProgress = NULL;
		Nv_Download_ = NULL;
		pTextPrint = NULL;
		nPos = 0;
		pList = NULL;
		pDownloadEx = NULL;
		bMode = FALSE;
	}
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CMainDlg : public CWindowWnd, public INotifyUI
{
public:

	//构造
	CMainDlg();

	LPCTSTR GetWindowClassName() CONST { return _T("UIMainFrame"); };
	UINT GetClassStyle() CONST { return CS_DBLCLKS; };
	VOID OnFinalMessage(HWND /*hWnd*/) { delete this; };

	VOID OnPrepare() {
	}

	VOID Notify(TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::PostQuitMessage(0L);

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( ::IsIconic(*this) ) bHandled = FALSE;
		return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCopyData(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//////////////////////////////////////////////////////////////////////////
	//自定义消息
	LRESULT OnUserCloseListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserProg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserPrint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//////////////////////////////////////////////////////////////////////////
	//设置 移动 控件
	VOID	set_WidthPos	(IN CRichEditUI *pdlg, IN INT nWidth);
	VOID	set_WidthPos	(IN CButtonUI *pdlg, IN INT nWidth);
	VOID	set_WidthPos	(IN CTextUI *pdlg, IN INT nWidth);
	VOID	set_Width		(IN CRichEditUI *pdlg, IN INT nWidth);
	VOID	set_Width		(IN CHorizontalLayoutUI *pdlg, IN INT nWidth);

public:

	//绑定控件
	VOID init_BindControls(VOID);
	//初始化数据
	VOID init_ControlsData(VOID);
	//添加一个list item
	BOOL add_ListItem(CONST CHAR *strUrl, CONST CHAR *strFilePath, BOOL bOpenMode, _Nv_Download_Ex *pdlg);
	//获取list pos
	INT  get_DownloadPtrPos(CControlUI* cContrCmp);
	//获取当前应用程序路径
	CHAR *get_ProcessPath(VOID);
	
	//添加记录文件
	BOOL add_DownloadFileIni( 
		__in CONST CHAR *strUrl,
		__in CONST CHAR *strSaveFilePath, 
		__in CONST CHAR *strFileName,
		__in __int64 n64FileSize,
		__in CONST CHAR *strCoolie = NULL, 
		__in __int64 n64ProgRessPos = 0, 
		__in CONST CHAR *strPrint = "正在等待下载..", 
		__in bool bOpenFile = false
		);
	//修改记录文件
	BOOL mod_DownloadFileIni(
		__in INT nPos,
		__in CONST CHAR *strUrl,
		__in CONST CHAR *strSaveFilePath, 
		__in CONST CHAR *strFileName, 
		__in CONST CHAR *strCoolie,
		__in __int64 n64FileSize,
		__in __int64 n64ProgRessPos, 
		__in CONST CHAR *strPrint, 
		__in bool bOpenFile
		);
	
	//读取配置记录文件
	BOOL rand_DownloadFileIni(VOID);
	//创建一个list item
	BOOL create_ListItem(tag_Donwload_File_Ini_Data tag_File_Data);
	//剔除字符串中\n
	VOID set_StrSign(__in CHAR *strData);
	BOOL null_DownloadFileIni(VOID);
public:
	//数据区域
	CPaintManagerUI m_pm;

	//退出按钮
	CButtonUI		*m_pBtnClose;
	CButtonUI		*m_pBtnMin;
	//添加任务
	CButtonUI		*m_pBtnAddTask;

	//插件导出函数
	DLL_SetUrL			m_FunSetUrL;
	DLL_SetCoolie		m_FunSetCoolie;
	DLL_SetClassStr		m_FunSetClassStr;
	
	//插件句柄
	HINSTANCE		m_hinstance;
	
	//任务追踪容器
	vector<_Nv_Download_Ex*>	m_vt_Nv_Download;
	
	//连接数累计
	DWORD			m_dwLinkCount;

	//任务引用计数
	INT				m_nTaskCount;
	
	//记录配置文件容器
	vector<tag_Donwload_File_Ini_Data>	m_vtDownLoadFileData;
	
};



//任务类
class _Nv_Download_Ex
{
public:
	//初始化
	_Nv_Download_Ex(){ 
		m_strUrl=""; m_strFileSavePath=""; m_strCoolie=""; m_ListItemPos=-1;
		m_Handle_ProgRess = NULL;
		m_Handle_Print = NULL;
		m_bClose = FALSE;
		Nv_Download_ = NULL;
		m_bOpenFile = NULL;
	};
	~_Nv_Download_Ex(){};
	
	//////////////////////////////////////////////////////////////////////////
	//list item的所有指针
	CListUI						*m_pList;
	CListContainerElementUI		*m_pListElement;
	CVerticalLayoutUI			*m_pVerticalLayout;
	CButtonUI					*m_pBtnIco;
	CTextUI						*m_pTextFile;
	CTextUI						*m_pTextPrint;
	CTextUI						*m_pTextJdtBk;
	CProgressUI					*m_pProgRess;
	CButtonUI					*m_pBtnStop;
	CButtonUI					*m_pBtnGo;
	CButtonUI					*m_pBtnOK;
	CButtonUI					*m_pBtnClose;
	
	//父窗口指针
	CMainDlg					*m_pWndPtr;
	
	//下载工作类
	_Nv_Download				*Nv_Download_;

	string						m_strUrl;
	string						m_strFileSavePath;
	string						m_strCoolie;
	
	//当前list的pos
	INT							m_ListItemPos;
	
	//线程退出代码
	HANDLE						m_Handle_ProgRess;
	HANDLE						m_Handle_Print;
	
	//是否销毁list
	BOOL						m_bClose;
	//是否直接打开文件
	BOOL						m_bOpenFile;
	
	//获取父窗口指针
	BOOL	get_WndPtr(CMainDlg *pdlg)
	{
		if (pdlg != NULL)
		{
			m_pWndPtr = pdlg;
			return TRUE;
		}

		return FALSE;
	}
protected:
private:
};