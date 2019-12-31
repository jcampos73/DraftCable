// Tipography.h: interface for the CTipography class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIPOGRAPHY_H__79BD7861_7E55_477E_A81A_C50E78518670__INCLUDED_)
#define AFX_TIPOGRAPHY_H__79BD7861_7E55_477E_A81A_C50E78518670__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


CFont *AfxGetFont(int index);

#define AFX_FONT_NORMAL_DEFAULT_HEIGHT	-13

#define AFX_FONT_NORMAL		0
#define AFX_FONT_SMALL		1
#define AFX_FONT_NORMALVER	2
#define AFX_FONT_SMALLVER	3

class CTipography  
{
public:
	void Initialize();
	CFont * GetFont(int index);
	CTipography();
	virtual ~CTipography();

protected:
	static const int MAX_FONT;
	CFont *m_pFont[10];
};



#endif // !defined(AFX_TIPOGRAPHY_H__79BD7861_7E55_477E_A81A_C50E78518670__INCLUDED_)
