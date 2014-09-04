#include "stdafx.h"
#include "Importer.h"


CImporter::CImporter()
{
	m_bOpened = false;
	m_xmlDocPtr = NULL;
}


CImporter::~CImporter()
{
	if (m_xmlDocPtr != NULL) delete(m_xmlDocPtr);
}

BOOL CImporter::ImportLibrary(LPCTSTR lpszPathName)
{
	//Load raw xml data into object
	DoLoadRawData(lpszPathName);

	CString strTCRootNodeLabel = "Library";
	CString strTCSymbolNodeLabel = "Library";

	if (m_xmlDocPtr->FindElem(strTCRootNodeLabel)){
		if (!m_xmlDocPtr->IntoElem()){
			return FALSE;
		}

		//Iterate symbols
		while (m_xmlDocPtr->FindElem(strTCSymbolNodeLabel))
		{
			if (m_xmlDocPtr->IntoElem()){

				//Do process symbol
				DoProcessNode();

				//Go out of node
				m_xmlDocPtr->OutOfElem();
			}
		}
	}//end if root node

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CImporter implementation

void CImporter::DoProcessNode()
{
	//Create CShapeUnit
}

void CImporter::DoLoadRawData(LPCTSTR lpszPathName)
{
	FILE *f = fopen(lpszPathName, "rb");

	// reading data - zlib will detect if zipped or not...
	TCHAR tInString[4096];
	int nReadChar = 1;
	std::_tstring sLoaded;

	// reading chunk of data and adding to sLoaded
	while (nReadChar != 0 && nReadChar != -1)
	{
		// read zip file
		nReadChar = fread(tInString, 1, 4096 * sizeof(TCHAR), f);

		// testing if load succesful
		if (nReadChar == -1)
		{
			// something went wrong, closing zip file and stopping
			fclose(f);
			m_bOpened = false;
			return;
		}
		// tInString OK, adding
		if (nReadChar != 0)
		{
			sLoaded.append(tInString, nReadChar);
			TRACE(_T("nReadChar = %d, sLoaded size = %d\n"), nReadChar, sLoaded.size());
		}
	}

	// closing zipfile
	fclose(f);

	m_xmlDocPtr = new CMarkup();
	m_bOpened = m_xmlDocPtr->SetDoc(sLoaded.c_str());
}


