// DES¼Ó½âÃÜËã·¨Dlg.h : header file
//

#if !defined(AFX_DESDLG_H__AA5C6DAB_B7DF_49AF_90F6_6EEDB177467C__INCLUDED_)
#define AFX_DESDLG_H__AA5C6DAB_B7DF_49AF_90F6_6EEDB177467C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDESDlg dialog

class CDESDlg : public CDialog
{
// Construction
public:
	int t1[65];
	void DESJIEMI();
	void DESDIEDAI();
	int K1[49],K2[49],K3[49],K4[49],K5[49],K6[49],K7[49],K8[49],K9[49],K10[49],K11[49],K12[49],K13[49],K14[49],K15[49],K16[49];
	int R0[33];
	int L0[33];
	void initial();
	void MIYAOERJINZHI();
	void Mingwenerjinzhi();
	CDESDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDESDlg)
	enum { IDD = IDD_DES_DIALOG };
	CString	m_MINGWEN;
	CString	m_MEWEN;
	CString	m_YUANWEN;
	CString	m_MIYAO;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDESDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDESDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnJiemibutton();
	afx_msg void OnJmbutton();
	afx_msg void OnTchbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESDLG_H__AA5C6DAB_B7DF_49AF_90F6_6EEDB177467C__INCLUDED_)
