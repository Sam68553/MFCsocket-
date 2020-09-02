// Server3Dlg.cpp : implementation file
//
#pragma comment(lib,"ws2_32.lib")

#include "stdafx.h"
#include "Server3.h"
#include "Server3Dlg.h"
#include "Winsock2.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <crtdefs.h>
#include <process.h>

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CServer3Dlg dialog




CServer3Dlg::CServer3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServer3Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServer3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listbox_msg);
}

BEGIN_MESSAGE_MAP(CServer3Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(btnListen, &CServer3Dlg::OnBnClickedbtnlisten)
	ON_BN_CLICKED(btnClose, &CServer3Dlg::OnBnClickedbtnclose)
	ON_BN_CLICKED(btnRvc, &CServer3Dlg::OnBnClickedbtnrvc)
END_MESSAGE_MAP()


// CServer3Dlg message handlers

BOOL CServer3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	
	hThread= (HANDLE)_beginthreadex(NULL,
		0,
		&CServer3Dlg::ListenSocketThread,
		(void *)this,
		0,
		&ThreadID);
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
unsigned __stdcall CServer3Dlg::ConnectSocketThread(void *arvg)
{
	CServer3Dlg *pSocket = (CServer3Dlg*)arvg;
	pSocket->index += 1;
	int conindex = pSocket->index;
	char username[50];
	CString userMsg;
	pSocket->ClientAddrLen = sizeof(SOCKADDR);
	if((pSocket->NewConnection[conindex] = accept(pSocket->ListeningSocket,(SOCKADDR*)&(pSocket->ClientAddr),&(pSocket->ClientAddrLen)))== INVALID_SOCKET)
	{
		TRACE("accept failed error = %d\n",WSAGetLastError());
		closesocket(pSocket->ListeningSocket);
		WSACleanup();
		return TRUE;
	}
	TRACE("We successfully got a connection from %s:%d \n",inet_ntoa(pSocket->ClientAddr.sin_addr),ntohs(pSocket->ClientAddr.sin_port));
	
	pSocket->connectThread[pSocket->index]= (HANDLE)_beginthreadex(NULL,
		0,
		&CServer3Dlg::ConnectSocketThread,
		(void *)arvg,
		0,
		&pSocket->connectThreadID[pSocket->index]);
	pSocket->Ret =recv(pSocket->NewConnection[conindex],username,sizeof(username),0);
	username[pSocket->Ret] = '\0';
	pSocket->m_listbox_msg.InsertString(pSocket->m_listbox_msg.GetCount(),(CString)username+(CString)"¶i¤J²á¤Ñ«Ç");
	while(1){
		TRACE("we are waiting rcv data\n");
		if((pSocket->Ret = recv(pSocket->NewConnection[conindex],pSocket->DataBuffer,sizeof(pSocket->DataBuffer),0)) == SOCKET_ERROR)
		{
			TRACE("recv failed error =  %d\n",WSAGetLastError());
			closesocket(pSocket->NewConnection[conindex]);
			WSACleanup();
			return  TRUE;
		}

		TRACE("we successfully received %d bytes \n",pSocket->Ret);
		pSocket->DataBuffer[pSocket->Ret] = '\0';
		TRACE("%s\n",pSocket->DataBuffer);
		userMsg =(CString)username+(CString)": " +(CString)pSocket->DataBuffer;
		pSocket->m_listbox_msg.InsertString(pSocket->m_listbox_msg.GetCount(),userMsg);
	}
}
unsigned __stdcall CServer3Dlg::ListenSocketThread(void* arvg)
{
	//CTimerDlg *pTimer = reinterpret_cast<CTimerDlg *>(arvg);
	CServer3Dlg *pSocket = (CServer3Dlg*)arvg;
	pSocket->Port = 6666;

	if((pSocket->Ret = WSAStartup(MAKEWORD(2,2),&(pSocket->wsaData)))!=0)
	{
		TRACE("Wsa is error = %d\n",WSAGetLastError());
		return TRUE;
	}
	if(((pSocket->ListeningSocket) = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==INVALID_SOCKET)
	{
		TRACE("socket error = %d\n",WSAGetLastError());
		WSACleanup();
		return TRUE;
	}
	pSocket->ServerAddr.sin_family = AF_INET;
	pSocket->ServerAddr.sin_port = htons(pSocket->Port);
	pSocket->ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if((bind((pSocket->ListeningSocket),(SOCKADDR*)&(pSocket->ServerAddr),sizeof((pSocket->ServerAddr))))==SOCKET_ERROR)
	{
		TRACE("bind error = %d\n",WSAGetLastError());
		closesocket((pSocket->ListeningSocket));
		WSACleanup();
		return TRUE;
	}
	if((listen(pSocket->ListeningSocket,5))==SOCKET_ERROR)
	{
		TRACE("listen fail = %d\n",WSAGetLastError());
		closesocket(pSocket->ListeningSocket);
		WSACleanup();
		return TRUE;
	}
	TRACE("we are waiting connection on Port: %d\n",pSocket->Port);
	TRACE("Listen()...");
	pSocket->index = 0;
	pSocket->connectThread[pSocket->index]= (HANDLE)_beginthreadex(NULL,
		0,
		&CServer3Dlg::ConnectSocketThread,
		(void *)arvg,
		0,
		&pSocket->connectThreadID[pSocket->index]);
	return TRUE;
}
void CServer3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServer3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServer3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServer3Dlg::OnBnClickedbtnlisten()
{
	// TODO: Add your control notification handler code here
	if((ListeningSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==INVALID_SOCKET)
	{
		TRACE("socket error = %d\n",WSAGetLastError());
		WSACleanup();
		return ;
	}
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port);
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if((bind(ListeningSocket,(SOCKADDR*)&ServerAddr,sizeof(ServerAddr)))==SOCKET_ERROR)
	{
		TRACE("bind error = %d\n",WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return ;
	}
	if((listen(ListeningSocket,5))==SOCKET_ERROR)
	{
		TRACE("listen fail = %d\n",WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return ;
	}
	TRACE("we are waiting connection on Port: %d\n",Port);
	TRACE("Listen()...");
	onAccept();
	
}

void CServer3Dlg::OnBnClickedbtnclose()
{
	/*// TODO: Add your control notification handler code here
	TRACE("We are now close Client connetion \n");
	closesocket(NewConnection);
	WSACleanup();*/
}
void CServer3Dlg::onAccept()
{
	/*==ClientAddrLen = sizeof(SOCKADDR);
	if((NewConnection = accept(ListeningSocket,(SOCKADDR*)&ClientAddr,&ClientAddrLen))== INVALID_SOCKET)
	{
		TRACE("accept failed error = %d\n",WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return ;
	}
	TRACE("We successfully got a connection from %s:%d \n",inet_ntoa(ClientAddr.sin_addr),ntohs(ClientAddr.sin_port));

	closesocket(ListeningSocket);
	TRACE("we are waiting rcv data\n");
	if((Ret = recv(NewConnection,DataBuffer,sizeof(DataBuffer),0)) == SOCKET_ERROR)
	{
		TRACE("recv failed error =  %d\n",WSAGetLastError());
		closesocket(NewConnection);
		WSACleanup();
		return ;
	}

	TRACE("we successfully received %d bytes \n",Ret);
	DataBuffer[Ret] = '\0';
	TRACE("%s\n",DataBuffer);*/
}
void CServer3Dlg::OnBnClickedbtnrvc()
{
	// TODO: Add your control notification handler code here
	/*ClientAddrLen = sizeof(SOCKADDR);
	if((NewConnection = accept(ListeningSocket,(SOCKADDR*)&ClientAddr,&ClientAddrLen))== INVALID_SOCKET)
	{
		TRACE("accept failed error = %d\n",WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return ;
	}
	TRACE("We successfully got a connection from %s:%d \n",inet_ntoa(ClientAddr.sin_addr),ntohs(ClientAddr.sin_port));

	TRACE("we are waiting rcv data\n");
	if((Ret = recv(NewConnection,DataBuffer,sizeof(DataBuffer),0)) == SOCKET_ERROR)
	{
		TRACE("recv failed error =  %d\n",WSAGetLastError());
		closesocket(NewConnection);
		WSACleanup();
		return;
	}

	TRACE("we successfully received %d bytes \n",Ret);
	DataBuffer[Ret] = '\0';
	TRACE("%s\n",DataBuffer);*/
}
