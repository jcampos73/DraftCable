// COMItfCore.h : header file
//

#if !defined(AFX_COMITFCORE_H__INCLUDED_)
#define AFX_COMITFCORE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COMItfCore


#define MAX_DISP_ARGS 32
#define DISPARG_NOFREEVARIANT 1

extern VARIANTARG    g_aVargs[MAX_DISP_ARGS];

void ClearAllArgs();
void ReleaseVariant(VARIANTARG *pvarg);
BOOL AddArgumentCString(LPOLESTR lpszArgName, WORD wFlags,LPOLESTR lpsz);

void LaunchWord(CString sPathWord,CString sPathOutput);

#endif // !defined(AFX_COMITFCORE_H__INCLUDED_)