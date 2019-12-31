#if !defined(AFX_ARCHIVEDB_H__A6A5B983_B442_41DC_8BB5_8A5D7977E381__INCLUDED_)
#define AFX_ARCHIVEDB_H__A6A5B983_B442_41DC_8BB5_8A5D7977E381__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArchiveDb.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CArchiveDb window

class CArchiveDb : public CArchive
{
// Construction
public:
	CArchiveDb(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL );

// Attributes
public:
	std::string buffer;
	std::stringstream *m_sout;
	CString m_nNamePart;
	BOOL m_bFlagDb;		//if FALSE(by default) behaves as normal CArchive

// Operations
public:
	void WriteString( LPCTSTR lpsz );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArchiveDb)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CArchiveDb();

	// Generated message map functions
protected:
	//{{AFX_MSG(CArchiveDb)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARCHIVEDB_H__A6A5B983_B442_41DC_8BB5_8A5D7977E381__INCLUDED_)
