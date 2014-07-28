// DraftDraw.h : main header file for the DRAFTDRAW application
//

#if !defined(AFX_DRAFTDRAW_H__3CBB047B_BBB2_4C74_8216_4CB1AB579696__INCLUDED_)
#define AFX_DRAFTDRAW_H__3CBB047B_BBB2_4C74_8216_4CB1AB579696__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "DraftDrawDoc.h"
#include "OMParser.h"		//Object model parser


#define _DOCTYPE_SCHEMATIC						0	//schematic document type
#define _DOCTYPE_RACK							1	//rack document type
#define _DOCTYPE_PATCHPANEL						2	//patch panel document type


//Display patch panel properties
void SDisplayPatchPProp(UINT nID);
void SDisplayPatchPProp(CString strID);
//Serialize patch panel
void SSerializePatchP(UINT nId);

/////////////////////////////////////////////////////////////////////////////
// CDraftDrawApp:
// See DraftDraw.cpp for the implementation of this class
//

class CDraftDrawApp : public CWinApp
{
public:
	int GetActiveTool();
	int GetActiveMode();
	int GetActiveDocType();
	CDraftDrawDoc *GetActiveDocument();
	CSize GetActiveViewProp();
	void SetActiveViewProp(CSize szGrid);
	void OnUpdateBtEditPart(CCmdUI *pCmdUI);
	void OnUpdateBtEditRack(CCmdUI *pCmdUI);
	CDraftDrawApp();
	CMultiDocTemplate* pDocTemplate;
	CMultiDocTemplate* pMDTPatchP;
	OMParser m_OMParser;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDraftDrawApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CDraftDrawApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnFileNewRack();
	afx_msg void OnEditRack();
	afx_msg void OnFileNewPatchP();
	afx_msg void OnFileNewSheet();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	DECLARE_MESSAGE_MAP()
};


extern CDraftDrawApp theApp;

extern CObArray g_aShBuffer;
extern int g_aShBufIndex;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAFTDRAW_H__3CBB047B_BBB2_4C74_8216_4CB1AB579696__INCLUDED_)
