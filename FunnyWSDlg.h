
// FunnyWSDlg.h : ͷ�ļ�
//
#pragma once
#include "afxwin.h"
#include "afxtempl.h" 

// CFunnyWSDlg �Ի���
class CFunnyWSDlg : public CDialogEx
{
// ����
public:
	CFunnyWSDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FUNNYWS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strExePath;
	CString m_strBmpPath;
	DWORD_PTR m_dwcurrPID;
	CEdit m_edtPort;
	CEdit m_edtInterval;
	CComboBox m_cboWindows;
	CStatic m_lblInfo;
	CStatic m_picThumbnail;
	CButton m_butRun;
	CButton m_butStop;
	CButton m_butRefresh;
	CButton m_butPause;
	CMap<DWORD, DWORD, CString, LPCTSTR>m_ProcessMap;
	CWinThread* m_pWWWThread;

private:
	void DXCapture(HWND, LPCTSTR);
	void APICapture(HWND, LPCTSTR);
	BOOL PrintEnumProcess(LPCTSTR, CMap<DWORD, DWORD, CString, LPCTSTR>&);
	BOOL PrintModules( DWORD , CStringArray& );
	void CurrentFilePath();
	void SetControlStatus(BOOL);
	void StopWebServer();
	void RunWebServer();
	void PauseWebServer();
	void CreatImageFolder(CString&);
public:
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangecboprocess();
	afx_msg void OnCbnSelchangecbowindow();
	afx_msg void OnBnClickedbutRun();
	afx_msg void OnBnClickedbutStop();
	afx_msg void OnBnClickedbutRefresh();

	//BOOL GetCurrentProcess();
	//void GetSelectProcModule();
	void SetDlgInitValue();
	void GetAllVisibledWindows();
	static UINT WebServerThreadFunc(LPVOID);
	afx_msg void OnBnClickedbutpause();

	afx_msg void OnBnClickedbutpause2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
