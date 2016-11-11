
// GUIDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CGUIDlg dialog
class CGUIDlg : public CDialogEx
{
// Construction
public:
	CGUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GUI_DIALOG };
#endif

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
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
//	CProgressCtrl m_ctlProgress;
	afx_msg void OnBnClickedButton1();
	CListBox ctl_ListBox;
};
