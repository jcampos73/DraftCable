#pragma once


// CDialogFillShape dialog

class CDialogFillShape : public CDialog
{
	DECLARE_DYNAMIC(CDialogFillShape)

public:
	CDialogFillShape(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogFillShape();

// Dialog Data
	enum { IDD = IDD_FILLSHAPE };
	COLORREF m_crCurrent;
	COLORREF m_crCurrentBgnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	CBrush m_brush;
	CBrush m_brushBgnd;

	void SetColor(COLORREF cr);
	void SetColorBgnd(COLORREF cr);
	void _DoSwapColor();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnDblclickFrontColor();
	virtual BOOL OnInitDialog();
	afx_msg void OnStnDblclickBgndColor();
	afx_msg void OnStnBlclickSwapIcon();
};
