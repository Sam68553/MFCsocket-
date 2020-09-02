// Server3Dlg.h : header file
//
#pragma comment(lib,"ws2_32.lib")
#pragma once
#include "afxwin.h"
#include "Winsock2.h"
#define  MAXCONNECT 20
// CServer3Dlg dialog
class CServer3Dlg : public CDialog
{
// Construction
public:
	CServer3Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SERVER3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listbox_msg;
	afx_msg void OnBnClickedbtnlisten();
	WSADATA wsaData;
	SOCKET ListeningSocket;
	SOCKET NewConnection[MAXCONNECT];
	SOCKADDR_IN ServerAddr;
	SOCKADDR_IN ClientAddr;
	int ClientAddrLen;
	int Ret;
	char DataBuffer[1024];
	int Port;
	afx_msg void OnBnClickedbtnclose();
	void onAccept();
	HANDLE hThread;
	unsigned ThreadID;
	HANDLE connectThread[MAXCONNECT];
	unsigned connectThreadID[MAXCONNECT];
	static unsigned __stdcall ListenSocketThread(void *arvg);
	static unsigned __stdcall ConnectSocketThread(void *arvg);
	afx_msg void OnBnClickedbtnrvc();
	int index;
};
