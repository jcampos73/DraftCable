// Tipography.cpp: implementation of the CTipography class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
/*#include "Sacim.h"*/
#include "Tipography.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CTipography afxTipography;



CFont *AfxGetFont(int index){


	return afxTipography.GetFont(index);

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int CTipography::MAX_FONT=10;

CTipography::CTipography()
{
	for(int i=0;i<MAX_FONT;i++){

		m_pFont[i]=NULL;
	}
}

CTipography::~CTipography()
{
	for(int i=0;i<MAX_FONT;i++) if(m_pFont[i]){

		delete(m_pFont[i]);
	}
}

CFont * CTipography::GetFont(int index)
{

	if(m_pFont[index]==NULL){

		Initialize();
/*
		m_pFont[index]=new CFont();


		m_pFont[index]->CreateFont(
		   16,                        // nHeight
		   0,                         // nWidth
		   0,                         // nEscapement
		   0,                         // nOrientation
		   FW_NORMAL,                 // nWeight
		   FALSE,                     // bItalic
		   FALSE,                     // bUnderline
		   0,                         // cStrikeOut
		   ANSI_CHARSET,              // nCharSet
		   OUT_DEFAULT_PRECIS,        // nOutPrecision
		   CLIP_DEFAULT_PRECIS,       // nClipPrecision
		   DEFAULT_QUALITY,           // nQuality
		   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		   _T("Arial"));                 // lpszFacename
*/

	}

	return m_pFont[index];
}

void CTipography::Initialize()
{
	m_pFont[0]=new CFont();
	m_pFont[0]->CreateFont(
		16,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
		_T("Arial"));					// lpszFacename

	m_pFont[1]=new CFont();
	m_pFont[1]->CreateFont(
		10,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
		_T("Arial"));					// lpszFacename

	m_pFont[2]=new CFont();
	m_pFont[2]->CreateFont(
		16,							// nHeight
		0,							// nWidth
		900,						// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
		_T("Arial"));					// lpszFacename

	m_pFont[3]=new CFont();
	m_pFont[3]->CreateFont(
		10,							// nHeight
		0,							// nWidth
		900,						// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
		_T("Arial"));					// lpszFacename

}
