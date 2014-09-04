#pragma once
/*
The objetive of this class is to encapsulate of the logic to import mic information
from outside
*/
class CImporter
{
public:
	CImporter();
	~CImporter();
	//Import library from external system
	BOOL ImportLibrary(LPCTSTR lpszPathName);
	CMarkup* m_xmlDocPtr;
protected:
	//! true if already opened
	bool m_bOpened;
	void DoLoadRawData(LPCTSTR lpszPathName);
	void DoProcessNode();
};

