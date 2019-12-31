// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__60826132_FEBD_4632_B081_E3F4BA4730F7__INCLUDED_)
#define AFX_STDAFX_H__60826132_FEBD_4632_B081_E3F4BA4730F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//BUG: C4786 or C1067 Caused by Long Identifier and /Zi 
//Q122539
//"warning C4786: 'identifier' : identifier was truncated to '255' characters in the debug information"
//This warning is produced compiling with C++ standard libraries.
//The following pragma directive disables that warning to reduce output text.
#pragma warning(disable:4786)

//Disable conversion from this to that, possible lost of data
#pragma warning(disable:4244)

//Disable warning C4305: '=' : truncation from 'double' to 'float'
#pragma warning(disable:4305)

//Disable warning C4996: 'strcmpi': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _strcmpi. See online help for details.
#pragma warning(disable:4996)

//Disable warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable:4800)

//Remove the precompiler secure warnings that come up when using functions like scanf()
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers






#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdb.h>			// MFC support for database access
#include <afxole.h>			// MFC support for OLE
#include <afxpriv.h>		// macros for MBCS/Unicode conversion (like USES_CONVERSION)


//std template objects
#include <afxtempl.h>
#include <string>

#include <deque>
#include <list>
#include <map>
#include <vector>



//Program globals
#include "Globals.h"

#include "ArchiveDb.h"

#include "ShapeUnit.h"

//GDI+ interface
#include "DIBAPI.H"
#include <gdiplus.h>




#include "COMItfCore.h"//Com Interface to call VBasic ActiveX DLL's from VC

//Include class for drawing execell tabs in views
#include "FolderFrame.h"





using namespace std ;

using namespace Gdiplus;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__60826132_FEBD_4632_B081_E3F4BA4730F7__INCLUDED_)
