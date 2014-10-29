#pragma once
#include "afxcmn.h"


// CDialogFillShape dialog

#define DRAFTCABLE_FILL_NO			0
#define DRAFTCABLE_FILL_SOLID		1
#define DRAFTCABLE_FILL_GRADIENT	2

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
	int m_blendCount;
	float* m_blendPositions;
	float* m_blendFactors;

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
	int ChangeFill(int nSize);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnDblclickFrontColor();
	virtual BOOL OnInitDialog();
	afx_msg void OnStnDblclickBgndColor();
	afx_msg void OnStnBlclickSwapIcon();
	afx_msg void OnClickedRadioFillNo();
	afx_msg void OnClickedRadioFillSolid();
	afx_msg void OnClickedRadioFillGradient();
	CSliderCtrl m_ctrlSliderGradient;
//	afx_msg void OnOk();
	virtual void OnOK();
};
