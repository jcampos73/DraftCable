#pragma once
#include "afxwin.h"


// CDialogShell dialog

class CDialogShell : public CDialog
{
	DECLARE_DYNAMIC(CDialogShell)

public:
	CDialogShell(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogShell();

// Dialog Data
	enum { IDD = IDD_SHELL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	CEdit m_ecShell;
	virtual BOOL OnInitDialog();
};
