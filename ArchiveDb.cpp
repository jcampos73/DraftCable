// ArchiveDb.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "ArchiveDb.h"



//std template objects
#include <ios>
#include <sstream>





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArchiveDb

CArchiveDb::CArchiveDb(CFile* pFile, UINT nMode, int nBufSize, void* lpBuf ):CArchive(pFile, nMode, nBufSize, lpBuf)
{
	m_bFlagDb=FALSE;
	buffer="hello";

	m_sout=new std::stringstream(ios_base::out);
	m_nNamePart=pFile->GetFileName();
}

CArchiveDb::~CArchiveDb()
{

	m_sout->flush();

	if (m_bFlagDb){


		TCHAR sConnect[1024];

		g_GetConnectString(sConnect,1024);


		//Connect to database
		CDatabase db;
		db.OpenEx(sConnect);

		CRecordset rsConnector(&db);

		rsConnector.m_strFilter="nNamePart='"+m_nNamePart+"'";
		rsConnector.Open(CRecordset::forwardOnly,"SELECT * FROM tbPart");


		if(rsConnector.IsEOF()){

			CString strQuery="INSERT INTO tbPart (nNamePart,nTextPart,bTextBin) VALUES ('"+m_nNamePart+"','"+CString(m_sout->str().c_str())+"',1)";

			db.ExecuteSQL(strQuery);

		}
		else{





			CString strQuery="UPDATE tbPart SET nTextPart='"+CString(m_sout->str().c_str())+"',bTextBin=1 WHERE nNamePart='"+m_nNamePart+"'";


			//AfxMessageBox(strQuery);

			FILE *fout=fopen("log_debugdb.txt","w");
			std::string str(strQuery);
			fwrite(str.c_str(),sizeof(char),str.length(),fout);
			fclose(fout);



			db.ExecuteSQL(strQuery);


		}


		db.Close();
	}

	delete(m_sout);
}


//BEGIN_MESSAGE_MAP(CArchiveDb, CWnd)
	//{{AFX_MSG_MAP(CArchiveDb)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CArchiveDb message handlers

void CArchiveDb::WriteString( LPCTSTR lpsz ){


	CArchive::WriteString(lpsz);

	if (m_bFlagDb){
		*m_sout<<lpsz;

	}

}