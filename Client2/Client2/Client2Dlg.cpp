// Client2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client2.h"
#include "Client2Dlg.h"
#include "ClientSocket.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CClientSocket m_ClientSocket;
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


// CClient2Dlg dialog




CClient2Dlg::CClient2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClient2Dlg::IDD, pParent)
	, m_strIPAdress(_T(""))
	, m_strMessage(_T(""))
	, m_iPort(0)
	, m_strName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClient2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strIPAdress);
	DDX_Text(pDX, IDC_EDIT3, m_strMessage);
	DDX_Text(pDX, IDC_EDIT2, m_iPort);
	DDX_Text(pDX, IDC_EDIT4, m_strName);
}

BEGIN_MESSAGE_MAP(CClient2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CClient2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(btnSend, &CClient2Dlg::OnBnClickedbtnsend)
	ON_BN_CLICKED(btnClose, &CClient2Dlg::OnBnClickedbtnclose)
	ON_BN_CLICKED(IDC_BUTTON1, &CClient2Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CClient2Dlg message handlers

BOOL CClient2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	if(m_ClientSocket.Create())
	{
		TRACE("456");
	}
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
	
	SetDlgItemText(IDC_EDIT1,(CString)"127.0.0.1");
	SetDlgItemText(IDC_EDIT2,(CString)"6666");
	SetDlgItemText(IDC_EDIT4,(CString)"Sam");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClient2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClient2Dlg::OnPaint()
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
HCURSOR CClient2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClient2Dlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	
	if(m_ClientSocket.Connect(m_strIPAdress,m_iPort))
	{
		//AfxMessageBox("connection Successful");
		m_ClientSocket.Send(m_strName.GetBuffer(m_strName.GetLength()),m_strName.GetLength());

	}
	else
	{
		//AfxMessageBox("connection fail");
		TRACE("%d",GetLastError());
	}
	DWORD error = GetLastError();
}

void CClient2Dlg::OnBnClickedbtnsend()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	if(m_ClientSocket.Send(m_strMessage.GetBuffer(m_strMessage.GetLength()),m_strMessage.GetLength()))
	{

	}
	else
	{
		//AfxMessageBox("failed to send Message");
	}
}

void CClient2Dlg::OnBnClickedbtnclose()
{
	// TODO: Add your control notification handler code here
	m_ClientSocket.Close();
}

void CClient2Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if(m_ClientSocket.Create())
	{
		TRACE("456");
	}

}
