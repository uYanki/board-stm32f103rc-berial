// DES加解密算法.h : main header file for the DES加解密算法 application
//

#if !defined(AFX_DES_H__7FAB0C81_389F_433B_A2BB_27DFF608068E__INCLUDED_)
#define AFX_DES_H__7FAB0C81_389F_433B_A2BB_27DFF608068E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDESApp:
// See DES加解密算法.cpp for the implementation of this class
//

class CDESApp : public CWinApp
{
public:
	CDESApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDESApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDESApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DES_H__7FAB0C81_389F_433B_A2BB_27DFF608068E__INCLUDED_)
