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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	COLORREF m_crCurrent;
	CBrush m_brush;

	void SetColor(COLORREF cr);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnDblclickFrontColor();
	virtual BOOL OnInitDialog();
};
