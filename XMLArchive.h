#pragma once
#include "afx.h"
#include "Markup.h"

#include <stack>

#ifndef _UNICODE
namespace std
{
	typedef string			_tstring;
	typedef ifstream		_tifstream;
	typedef ofstream		_tofstream;
	typedef istringstream	_tistringstream;
	typedef ostringstream	_tostringstream;
}
#else
namespace std
{
	typedef wstring			_tstring;
	typedef wifstream		_tifstream;
	typedef wofstream		_tofstream;
	typedef wistringstream	_tistringstream;
	typedef wostringstream	_tostringstream;
};
#endif

#define XMLCLASSNODE { CXMLArchive& xmlArchive = static_cast<CXMLArchive&>(ar); \
						xmlArchive.GetNode(GetActualClass()->m_lpszClassName);
#define XMLDATA(attrName) { CXMLArchiveNode* nodePtr = xmlArchive.GetCurrentNode();\
	if (nodePtr != NULL) {nodePtr->DataNode(#attrName, attrName);}}
#define XMLINTDATA(attrName) { CXMLArchiveNode* nodePtr = xmlArchive.GetCurrentNode();\
	if (nodePtr != NULL) {nodePtr->DataNode(#attrName, (int&)(attrName));}}
#define XMLENDNODE { CXMLArchiveNode* nodePtr = xmlArchive.GetCurrentNode();\
	if (nodePtr != NULL) {nodePtr->Close(); }}}


class CXMLArchiveNode;

class CXMLArchive :
	public CArchive
{
	std::stack  <CXMLArchiveNode*> m_nodeList;
	static CFile	m_dummyFile;
	IStream*		m_streamPtr;
public:
	CXMLArchive(const CString& fileName, UINT nMode,
		IStream* streamPtr = NULL,
		CDocument* docPtr = NULL);
	CXMLArchive(
		CFile* pFile,
		UINT nMode,
		int nBufSize = 4096,
		void* lpBuf = NULL
		);
	~CXMLArchive();
	void Close();
	CXMLArchiveNode*	GetNode(LPCTSTR nodeNameStr);
	void				RemoveNode(CXMLArchiveNode* xmlArchiveNodePtr);
	CXMLArchiveNode*	GetCurrentNode();

	// Attributes
	const CString	m_fileName;
protected:
	//! true if already opened
	bool m_bOpened;
	CMarkup* m_pMarkup;
};

class CXMLArchiveNode
{
	friend class CXMLArchive;

	// Private constructor, can only be constructed by CXMLArchive
	CXMLArchiveNode(CXMLArchive* archivePtr, CMarkup* newNodePtr, CMarkup* fatherNodePtr);
protected:
	// Cache of RUNTIME_CLASS created, to speed up createion
	static CMapStringToPtr			m_classMap;
	CMarkup* const					m_nodePtr;
	CMarkup* const					m_fatherNodePtr;
	CXMLArchive* const				m_archivePtr;
	//MSXML::IXMLDOMNodeListPtr		m_childNodeListPtr;
	CMarkup*		m_childNodeListPtr;
	int								m_childIndex;

public:
	virtual ~CXMLArchiveNode();
//	MSXML::IXMLDOMNodePtr CreateDataNode(LPCTSTR attrName, CString attrValue);
//	MSXML::IXMLDOMNodePtr CXMLArchiveNode::GetDataNode(LPCTSTR nodeName, CString& nodeText);
//	int		GetNoChildren();
//	CString GetChildName(int childIndex);
//	int		GetNextChildIndex() { return m_childIndex++; };
	void	Close();
//	void DataNode(LPCTSTR attrName, CTime& attrValue);

	// Loads into existing objects
	void DataNode(LPCTSTR attrName, CObject& object);

	// Creates new object when loading
	void DataNode(LPCTSTR attrName, CObject*& objectPtr);

	// Creates new object from node name
	static CObject*  CreateObject(const CString& className);
};

