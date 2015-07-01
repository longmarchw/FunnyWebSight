
// FunnyWSDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FunnyWS.h"
#include "FunnyWSDlg.h"
#include "afxdialogex.h"
#include "psapi.h"
#include "FWebServer.h"
//#include "dxScreenShot.h"
//#define	ErrorMessage(x)		MessageBox(NULL, x, _T("Error"), MB_OK|MB_ICONERROR)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEvent g_ExitEvent;
extern CRITICAL_SECTION g_csBMPFile;

const DWORD CaptureTime = 1000;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFunnyWSDlg 对话框
CFunnyWSDlg::CFunnyWSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFunnyWSDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFunnyWSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_edtPort, m_edtPort);
	DDX_Control(pDX, IDC_cboWindow, m_cboWindows);
	DDX_Control(pDX, IDC_lblInfo, m_lblInfo);
	DDX_Control(pDX, IDC_picThumbnail, m_picThumbnail);
	DDX_Control(pDX, IDC_edtRefreshInterval, m_edtInterval);
	DDX_Control(pDX, IDC_butRun, m_butRun);
	DDX_Control(pDX, IDC_butStop, m_butStop);
	DDX_Control(pDX, IDC_butRefresh, m_butRefresh);
	DDX_Control(pDX, IDC_butPause2, m_butPause);
}

BEGIN_MESSAGE_MAP(CFunnyWSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_cboWindow, &CFunnyWSDlg::OnCbnSelchangecbowindow)
	ON_BN_CLICKED(IDC_butRun, &CFunnyWSDlg::OnBnClickedbutRun)
	ON_BN_CLICKED(IDC_butStop, &CFunnyWSDlg::OnBnClickedbutStop)
	ON_BN_CLICKED(IDC_butRefresh, &CFunnyWSDlg::OnBnClickedbutRefresh)
	ON_BN_CLICKED(IDC_butPause, &CFunnyWSDlg::OnBnClickedbutpause)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFunnyWSDlg 消息处理程序

BOOL CFunnyWSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CurrentFilePath();
	GetAllVisibledWindows();
	SetDlgInitValue();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFunnyWSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFunnyWSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFunnyWSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*
void CFunnyWSDlg::DXCapture(HWND hWnd, LPCTSTR lpSavePath)
{
	dxScreenShot* dxSS = new dxScreenShot(hWnd);
	if(FAILED(dxSS->ScreenShot(lpSavePath)))
	{
		MessageBox(_T("Unable to ScreenShot "), _T("Error"), MB_OK|MB_ICONERROR);
	}

	delete dxSS;
}
*/
void CFunnyWSDlg::APICapture(HWND hWnd, LPCTSTR lpszPath)
{
	HDC hDC = ::GetWindowDC(hWnd);
	if(hDC == NULL){
		return;
	}
	ASSERT(hDC);
	
	HDC hMemDC = ::CreateCompatibleDC(hDC);
	if(hMemDC == NULL){
		return;
	}
	ASSERT(hMemDC);
	
	RECT rc;
	::GetWindowRect(hWnd, &rc);
	//::GetClientRect(hWnd, &rc);
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);
	//HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rc.right, rc.bottom);
	ASSERT(hBitmap);
	
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hBitmap);
	::PrintWindow(hWnd, hMemDC, 0);

	BITMAP bitmap = {0};
	::GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	BITMAPINFOHEADER bi = {0};
	BITMAPFILEHEADER bf = {0};

	CONST int nBitCount = 24;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bitmap.bmWidth;
	bi.biHeight = bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = nBitCount;
	bi.biCompression = BI_RGB;
	DWORD dwSize = ((bitmap.bmWidth * nBitCount + 31) / 32) * 4 * bitmap.bmHeight;

	HANDLE hDib = GlobalAlloc(GHND, dwSize + sizeof(BITMAPINFOHEADER));
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	::GetDIBits(hMemDC, hBitmap, 0, bitmap.bmHeight, (BYTE*)lpbi + sizeof(BITMAPINFOHEADER), (BITMAPINFO*)lpbi, DIB_RGB_COLORS);
	
	try
	{
		CFile file;
		file.Open(lpszPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite );
		bf.bfType = 0x4d42;
		dwSize += sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		bf.bfSize = dwSize;
		bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		file.Write((BYTE*)&bf, sizeof(BITMAPFILEHEADER));
		file.Write((BYTE*)lpbi, dwSize);
		file.Close();
	}
	catch(CFileException* e)
	{
		e->ReportError();
		e->Delete();
	}

	GlobalUnlock(hDib);
	GlobalFree(hDib);

	::SelectObject(hMemDC, hOldBmp);
	::DeleteObject(hBitmap);	
	::DeleteObject(hMemDC);
	::ReleaseDC(hWnd, hDC);
}

void CFunnyWSDlg::OnClose()
{
	StopWebServer();
	DeleteCriticalSection(&g_csBMPFile); 
	CDialogEx::OnClose();
}
/*
BOOL CFunnyWSDlg::GetCurrentProcess()
{
	LPCTSTR retEnmuProc = NULL;
	DWORD dwKey =0;
	CString strName = _T("");

	if (PrintEnumProcess(retEnmuProc, m_ProcessMap))
	{
		POSITION pos = m_ProcessMap.GetStartPosition();
		while(pos)
		{
			m_ProcessMap.GetNextAssoc(pos, dwKey, strName);
			int k = m_cboProcess.GetCount();
			m_cboProcess.InsertString(k, strName);
			m_cboProcess.SetItemData(k, dwKey);  
		}
	}
	else
	{
		m_lblInfo.SetWindowText(retEnmuProc);
		return FALSE;
	}
	return TRUE;
}

BOOL CFunnyWSDlg::PrintEnumProcess(LPCTSTR retInfo, CMap<DWORD, DWORD, CString, LPCTSTR>& procMap)
{
	DWORD arrProIds[MAX_PATH], ByteCnt=0, ProCnt=0;

	if (!EnumProcesses(arrProIds, sizeof(arrProIds), &ByteCnt))
	{
		retInfo = TEXT("获取所有进程的ID失败！\n");
		return FALSE;
	}

	for (unsigned int i = 0; i < ByteCnt / sizeof(DWORD); i++)
	{
		if( arrProIds[i] != 0 )
		{
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
			HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, arrProIds[i]);
			if (NULL != hProcess )
			{
				HMODULE hMod = NULL;
				DWORD ByteCnt = 0;
				if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &ByteCnt) )
				{
					GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
					procMap.SetAt (arrProIds[i], szProcessName);
				}
			}
			CloseHandle( hProcess );
		}
	}

	return TRUE;
}

void CFunnyWSDlg::GetSelectProcModule()
{
	CStringArray moduleAry;
	UINT curSelid = m_cboProcess.GetCurSel();
	DWORD_PTR dwcurrPID = m_cboProcess.GetItemData(curSelid);

	if(PrintModules(dwcurrPID, moduleAry))
	{
		m_cboWindows.ResetContent();
		for(INT_PTR i=0; i<moduleAry.GetCount(); i++)
		{
			m_cboWindows.AddString (moduleAry.GetAt(i));
		}
	}
}

BOOL CFunnyWSDlg::PrintModules( DWORD processID, CStringArray& modAry )
  {
      HMODULE hMods[1024];
      HANDLE hProcess;
      DWORD cbNeeded;
      // Get a list of all the modules in this process.
  
      hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
      if (NULL == hProcess)
	  {
		  CloseHandle( hProcess );
		  return FALSE;
	  }
	  else
	  {
		  if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
		  {
			  for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
			  {
				  TCHAR szModName[MAX_PATH];
  
				  if ( GetModuleFileNameEx( hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
				  {
					  //_tprintf( TEXT("\t%s (0x%08X)\n"), szModName, hMods[i] );
					  modAry.Add (szModName);
				  }
			  }
		  }
	  }  
      CloseHandle( hProcess );
	  return TRUE;
  }
*/
void CFunnyWSDlg::SetDlgInitValue()
{
	m_pWWWThread = NULL;
	m_dwcurrPID = NULL;
	m_edtPort.SetWindowTextW (_T("8080"));
	m_edtInterval.SetWindowTextW (_T("1000"));
	InitializeCriticalSection(&g_csBMPFile);
}

static BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam)

{
	CFunnyWSDlg* pWnd = (CFunnyWSDlg*) lParam;
	PSTR pszMem = (PSTR) VirtualAlloc((LPVOID) NULL, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);
	PSTR pszCls = (PSTR) VirtualAlloc((LPVOID) NULL, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);
	if (pszMem != NULL) 
	{
		::GetWindowText(hwnd, (LPWSTR)pszMem, MAX_PATH);
		::GetClassNameW(hwnd, (LPWSTR)pszCls, MAX_PATH);
		if (_wcsicmp((LPWSTR)pszMem, L"")!=NULL && _wcsicmp((LPWSTR)pszCls, L"")!=NULL && !IsIconic(hwnd) && IsWindowVisible(hwnd))
		{
			int k = pWnd->m_cboWindows.GetCount();
			pWnd->m_cboWindows.InsertString(k, (LPCTSTR)pszMem);
			pWnd->m_cboWindows.SetItemData(k, (DWORD_PTR)hwnd); 
		}
	}
	VirtualFree(pszMem, 0, MEM_RELEASE); 
	VirtualFree(pszCls, 0, MEM_RELEASE); 
	return TRUE;
}

void CFunnyWSDlg::GetAllVisibledWindows()
{
	m_cboWindows.ResetContent();
	EnumDesktopWindows(NULL, lpEnumFunc, (LPARAM) this); 
}

void CFunnyWSDlg::OnCbnSelchangecbowindow()
{
	CreatImageFolder(m_strExePath+_T("image"));

	UINT curSelid = m_cboWindows.GetCurSel();
	m_dwcurrPID = m_cboWindows.GetItemData(curSelid);

	EnterCriticalSection(&g_csBMPFile);
	TRACE("APICapture enter...");
	APICapture((HWND)m_dwcurrPID, m_strBmpPath);
	TRACE("APICapture leave...");
	LeaveCriticalSection(&g_csBMPFile);

	CRect rc;
	m_picThumbnail.GetClientRect(&rc);
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, m_strBmpPath, IMAGE_BITMAP, rc.Height(), rc.Width(), 
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_picThumbnail.ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
	m_picThumbnail.SetBitmap(hBitmap);
	
	m_butRefresh.EnableWindow (TRUE);
	m_butRun.EnableWindow (TRUE);
}

void CFunnyWSDlg::CreatImageFolder(CString& path)
{
	CFileFind cff;
	if(!cff.FindFile(path)){
		if(CreateDirectory(path, NULL)){
			BOOL ret = SetFileAttributes(path, FILE_ATTRIBUTE_HIDDEN );
		}
	}
}

void CFunnyWSDlg::CurrentFilePath()
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);      
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_strExePath = szFilePath;
	m_strBmpPath = m_strExePath + _T("image\\Temp.bmp");
}

void CFunnyWSDlg::OnBnClickedbutRefresh()
{
	GetAllVisibledWindows();
	m_picThumbnail.SetBitmap(NULL);
}

void CFunnyWSDlg::OnBnClickedbutRun()
{
	SetTimer(1, CaptureTime, NULL);
	RunWebServer();
	SetControlStatus(TRUE);
}

void CFunnyWSDlg::OnBnClickedbutStop()
{
	KillTimer(1);
	StopWebServer();
	SetControlStatus(FALSE);
}

void CFunnyWSDlg::RunWebServer()
{
	if(!m_pWWWThread)
	{
		m_pWWWThread = AfxBeginThread(WebServerThreadFunc, this, 0, 0, CREATE_SUSPENDED, NULL);
		m_pWWWThread->m_bAutoDelete = FALSE;
		m_pWWWThread->ResumeThread();
	}
	else
	{
		m_pWWWThread->ResumeThread();
	}
}

void CFunnyWSDlg::PauseWebServer()
{
	if(m_pWWWThread){
		m_pWWWThread->SuspendThread();

		m_butPause.EnableWindow(FALSE);
		m_butRun.EnableWindow(TRUE);
	} 
}
void CFunnyWSDlg::StopWebServer()
{
	if(m_pWWWThread){
		m_pWWWThread->ResumeThread();
		g_ExitEvent.SetEvent();
		WaitForSingleObject(m_pWWWThread->m_hThread, INFINITE);

		delete m_pWWWThread;
		m_pWWWThread = NULL;
	}
}
void CFunnyWSDlg::SetControlStatus(BOOL flag)
{
	m_cboWindows.EnableWindow(!flag); 
	m_edtPort.EnableWindow(!flag);
	m_butRefresh.EnableWindow(!flag);
	m_butRun.EnableWindow(!flag);
	m_butPause.EnableWindow(flag);
	m_butStop.EnableWindow(flag);
}

void CFunnyWSDlg::OnBnClickedbutpause()
{
	PauseWebServer();
}

UINT CFunnyWSDlg::WebServerThreadFunc(LPVOID lpParam)
{
	CFunnyWSDlg* obj = (CFunnyWSDlg*)lpParam;
	CFWebServer* fws = new CFWebServer(obj->m_strExePath);
	if(fws != NULL) {
		fws->InitWWW();
		do{
			fws->RunWWW ();
		} 
		while(WaitForSingleObject(g_ExitEvent.m_hObject, 0) != WAIT_OBJECT_0);
	}

	fws->StopWWW();
	
	delete fws;
	fws = NULL;

	return 0;
}


void CFunnyWSDlg::OnTimer(UINT_PTR nIDEvent)
{
	//EnterCriticalSection(&g_csBMPFile);
	TRACE("APICapture enter...");
	APICapture((HWND)m_dwcurrPID, m_strBmpPath);
	TRACE("APICapture leave...");
	//LeaveCriticalSection(&g_csBMPFile);

	CDialogEx::OnTimer(nIDEvent);
}
