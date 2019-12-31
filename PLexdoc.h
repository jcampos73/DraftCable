#if !defined(AFX_LEXDOC_H__855C2DCE_96BF_4E12_B0C7_8514E4B7D8EF__INCLUDED_)
#define AFX_LEXDOC_H__855C2DCE_96BF_4E12_B0C7_8514E4B7D8EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PLexdoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPLexdoc document

class CPLexdoc : public CDocument
{
protected:
	CPLexdoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPLexdoc)

// Attributes
public:
	CObArray m_obaShape;
	CObArray *m_pObArray;
	CDocument *m_pParent;

// Operations
public:
	static int GetCodeIndex(int group_code);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLexdoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPLexdoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPLexdoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLEXDOC_H__855C2DCE_96BF_4E12_B0C7_8514E4B7D8EF__INCLUDED_)
