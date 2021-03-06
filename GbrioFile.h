// GbrioFile.h: interface for the CGbrioFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GBRIOFILE_H__9D87C2BB_68DF_4420_B245_497C899FA5E1__INCLUDED_)
#define AFX_GBRIOFILE_H__9D87C2BB_68DF_4420_B245_497C899FA5E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define _MAXPOINTBUF_DRAFTCABLE		256

/*
As refererence, for CStatic control:
BOOL Create( LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff );

WS_VISIBLE

SS_BITMAP
SS_BLACKFRAME
SS_BLACKRECT
SS_CENTER
SS_CENTERIMAGE
SS_ENDELLIPSIS 
SS_ENHMETAFILE
SS_ETCHEDFRAME
SS_ETCHEDHORZ
SS_ETCHEDVERT
SS_GRAYFRAME
SS_GRAYRECT
SS_ICON
SS_LEFT
SS_LEFTNOWORDWRAP
SS_NOPREFIX
SS_NOTIFY
SS_OWNERDRAW
SS_PATHELLIPSIS
SS_REALSIZECONTROL
SS_REALSIZEIMAGE
SS_RIGHT
SS_RIGHTJUST
SS_SIMPLE
SS_SUNKEN
SS_WHITEFRAME
SS_WHITERECT
SS_WORDELLIPSIS
*/
typedef struct labeltag{
	CRect *rect;
	CString *slabel; //if vertical store here X\nX\nX\n
	CString *sname;
	CFont *font;
	int iSize;
	BOOL bver;
	//24/03/2005
	DWORD style;
	//09/12/2014
	BOOL resizeOnDraw;

	labeltag(){

		rect=new CRect();
		slabel=new CString();
		sname=new CString();
		font=new CFont();
		iSize=0;
		bver=FALSE;
		//24/03/2005
		style=WS_VISIBLE;//for compatibility with serialized drawings of previous versions
		//09/12/2014
		resizeOnDraw = FALSE;
	};

	//24/03/2005
	//This version does not use pParentWnd and nID is change for label name.
	//For easy of use a default font is creted if necessary.
	labeltag(LPCTSTR lpszText, DWORD dwStyle, const RECT& rc, LPCTSTR lpszName, LOGFONT *lf=NULL){

		rect=new CRect(rc);
		slabel=new CString(lpszText);
		sname=new CString(lpszName);
		font=new CFont();
		if(lf==NULL){
			LOGFONT lf;
			ZeroMemory(&lf, sizeof(LOGFONT));
			lf.lfHeight=-13;
			lf.lfWeight=400;
			lf.lfOutPrecision=3;
			lf.lfClipPrecision=2;
			lf.lfQuality=1;
			lf.lfPitchAndFamily=34;
			//strcpy(lf.lfFaceName,"Arial");
			wcscpy(lf.lfFaceName, _T("Arial"));
			font->CreateFontIndirect(&lf);
			iSize=-lf.lfHeight-3;
		}
		else{
			font->CreateFontIndirect(lf);
			iSize=-lf->lfHeight-3;
		}
		bver=FALSE;
		style=dwStyle;
	};

	labeltag(labeltag& lbl){

		rect=new CRect(lbl.rect);
		slabel=new CString(*lbl.slabel);
		sname=new CString(*lbl.sname);

		font=new CFont();
		LOGFONT lf;
		lbl.font->GetLogFont(&lf);
		font->CreateFontIndirect(&lf);

		iSize=lbl.iSize;
		bver=lbl.bver;
		//24/03/2004
		style=lbl.style;
	};

	labeltag& labeltag::operator=( const labeltag& lbl ){

		*rect=*lbl.rect;
		*slabel=*lbl.slabel;
		*sname=*lbl.sname;

		if(font){
			delete(font);
		}
		font=new CFont();
		LOGFONT lf;
		lbl.font->GetLogFont(&lf);
		font->CreateFontIndirect(&lf);

		iSize=lbl.iSize;
		bver=lbl.bver;
		//24/03/2004
		style=lbl.style;
		return *this;
	};

	void ResizeAuto(CDC *pDC)
	{
		if (rect->IsRectEmpty()){

			//Save font
			CFont *prev_font;
			if (font == NULL){
				/*
				if (iSize <= 10){
					prev_font = pDC->SelectObject(AfxGetFont(AFX_FONT_SMALL));
				}
				else{
					prev_font = pDC->SelectObject(AfxGetFont(AFX_FONT_NORMAL));
				}
				*/
			}
			else{
				prev_font = pDC->SelectObject(font);
			}

			if (bver){

				//Local variables
				CString stlabel = "";
				CSize szac = CSize(0, 0);

				//Check if label has not been initialized
				/*
				if (!m_bIni){
					for (int i = 0; i<m_Label.slabel->GetLength(); i++){

						if (i)stlabel += "\n";
						stlabel += m_Label.slabel[0][i];
					}
					*m_Label.slabel = stlabel;
					//Set label as initialized
					m_bIni = TRUE;
				}
				*/

				//Calculate bounding rectangle
				for (int i = 0; i<slabel->GetLength(); i++){

					CString stlabel1 = slabel[0][i];
					SIZE sz;
					GetTextExtentPoint32(pDC->m_hDC, stlabel1, lstrlen(stlabel1), &sz);

					szac += CSize(0, sz.cy);
					if (szac.cx<sz.cx){
						szac.cx = sz.cx;
					}
				}

				//Set bounding rectangle
				*rect = CRect(rect->TopLeft(), szac);
			}
			else{
				//Check if label has not been initialized
				/*
				if (!m_bIni){
					m_Label.slabel->Replace("\n", "");
					m_bIni = TRUE;
				}
				*/

				//Calculate bounding rectangle
				SIZE sz;
				GetTextExtentPoint32(pDC->m_hDC, *slabel, lstrlen(*slabel), &sz);

				//Set bounding rectangle
				*rect = CRect(rect->TopLeft(), sz);
			}

			//Restore font
			pDC->SelectObject(prev_font);
		}
	};
} label;

class CGbrioFile : public CStdioFile  
{
public:
	CArchive * m_par;
	void WriteMoveTo(CPoint point);
	void WriteLineTo(CPoint point);
	CGbrioFile(CArchive *par);
	virtual ~CGbrioFile();

};

class CDdwioFile : public CStdioFile  
{
public:
	BOOL m_bOrg;
	CPoint m_POrg;
	CString m_stCommandLast;
	CArchiveDb * m_par;
	//void WriteMoveTo(CPoint point);
	//void WriteLineTo(CPoint point);
	void WritePolyline(LPPOINT lpPoints, int nCount );
	void WriteLabel(label *lb);
	void WritePin(POINT point,UINT uiPinnumber,UINT uiPos,DWORD dwStyle,int iPinType = 0);
	void WriteRect(CRect *rect);
	void WriteEllipse(CRect *rect,LPCOLORREF lpcrFill =NULL);
	void WriteUnit(CString name,CRect *rect,LPCTSTR lpszLabel=NULL);
	void WriteUnit(CString name,LPPOINT lpPoints, int nCount,LPCTSTR lpszLabel);//This overloaded function is to write buses DDW files.
	void WriteConnect(int *pConn,int nCount);
	void CDdwioFile::WriteSheet(LPCTSTR lpszLabel);
	CDdwioFile(CArchiveDb *par);
	virtual ~CDdwioFile();

};

#endif // !defined(AFX_GBRIOFILE_H__9D87C2BB_68DF_4420_B245_497C899FA5E1__INCLUDED_)
