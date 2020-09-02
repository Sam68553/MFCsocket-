// Client2Dlg.h : header file
//

#pragma once
#include "ClientSocket.h"

// CClient2Dlg dialog
class CClient2Dlg : public CDialog
{
// Construction
public:
	CClient2Dlg(CWnd* pParent = NULL);	// standard constructor
	CClientSocket m_ClientSocket;
// Dialog Data
	enum { IDD = IDD_CLIENT2_DIALOG };

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
	afx_msg void OnBnClickedOk();
	CString m_strIPAdress;
	CString m_strMessage;
	UINT m_iPort;
	afx_msg void OnBnClickedbtnsend();
	afx_msg void OnBnClickedbtnclose();

	afx_msg void OnBnClickedButton1();
	CString m_strName;
};
