#include "stdafx.h"
#include "XMLArchive.h"
#include <io.h>

CFile CXMLArchive::m_dummyFile;

CXMLArchive::CXMLArchive(const CString& fileName, UINT nMode,
	IStream* streamPtr /* = NULL*/,
	CDocument* docPtr /*= NULL*/) :
	CArchive(&m_dummyFile, nMode, 0, NULL),
	m_fileName(fileName),
	m_streamPtr(streamPtr)
{
	m_pDocument = docPtr;
	m_bForceFlat = FALSE;

	ASSERT(!fileName.IsEmpty() || streamPtr != NULL);

	//// Get reference of XML document	
	//try
	//{
	//	m_xmlDocPtr = MSXML::IXMLDOMDocumentPtr(__uuidof(MSXML::DOMDocument));
	//}

	//catch (_com_error e)
	//{

	//}

	//if (m_xmlDocPtr == NULL)
	//{
	//	::AfxMessageBox(_T("Can't get reference to XML parser, Ensure that msxml.dll is installed"));
	//	return;
	//}

	// Don't want async
	//m_xmlDocPtr->put_async(VARIANT_FALSE);

	if (IsLoading())
	{
		VARIANT_BOOL varResult = VARIANT_FALSE;

		try
		{
			if (m_streamPtr == NULL)
			{
				//varResult = m_xmlDocPtr->load(_variant_t(fileName));

				FILE *f = fopen(m_fileName, "rb");

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

				m_pMarkup = new CMarkup();
				m_bOpened = m_pMarkup->SetDoc(sLoaded.c_str());
			}
			else
			{
				//LARGE_INTEGER largeZero;

				//largeZero.QuadPart = 0;

				//m_streamPtr->Seek(largeZero, 0, NULL);
				//varResult = m_xmlDocPtr->load(_variant_t(m_streamPtr));
			}

			//// reading data - zlib will detect if zipped or not...
			//TCHAR tInString[4096];
			//int nReadChar = 1;
			//std::_tstring sLoaded;

			//// reading chunk of data and adding to sLoaded
			//while (nReadChar != 0 && nReadChar != -1)
			//{
			//	// read zip file
			//	nReadChar = gzread(gzf, tInString, 4096 * sizeof(TCHAR));

			//	// testing if load succesful
			//	if (nReadChar == -1)
			//	{
			//		// something went wrong, closing zip file and stopping
			//		gzclose(gzf);
			//		return m_bOpened = false;
			//	}
			//	// tInString OK, adding
			//	if (nReadChar != 0)
			//	{
			//		sLoaded.append(tInString, nReadChar);
			//		TRACE(_T("nReadChar = %d, sLoaded size = %d\n"), nReadChar, sLoaded.size());
			//	}
			//}
			//// closing zipfile
			//gzclose(gzf);

			//m_bOpened = SetDoc(sLoaded.c_str());
		}

		catch (CException* e)
		{
			varResult = VARIANT_FALSE;
		}
	}
}

CXMLArchive::CXMLArchive(
	CFile* pFile,
	UINT nMode,
	int nBufSize,
	void* lpBuf
	) : CArchive(pFile, nMode, nBufSize, lpBuf)
{
	m_bForceFlat = FALSE;

	if (IsLoading())
	{
		VARIANT_BOOL varResult = VARIANT_FALSE;

		try
		{

			//varResult = m_xmlDocPtr->load(_variant_t(fileName));

			// reading data - zlib will detect if zipped or not...
			TCHAR tInString[4096];
			int nReadChar = 1;
			std::_tstring sLoaded;

			// reading chunk of data and adding to sLoaded
			while (nReadChar != 0 && nReadChar != -1)
			{
				// read zip file
				nReadChar = pFile->Read(tInString, 4096 * sizeof(TCHAR));

				// testing if load succesful
				if (nReadChar == -1)
				{
					// something went wrong, closing zip file and stopping
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

			m_pMarkup = new CMarkup();
			m_bOpened = m_pMarkup->SetDoc(sLoaded.c_str());


			//// reading data - zlib will detect if zipped or not...
			//TCHAR tInString[4096];
			//int nReadChar = 1;
			//std::_tstring sLoaded;

			//// reading chunk of data and adding to sLoaded
			//while (nReadChar != 0 && nReadChar != -1)
			//{
			//	// read zip file
			//	nReadChar = gzread(gzf, tInString, 4096 * sizeof(TCHAR));

			//	// testing if load succesful
			//	if (nReadChar == -1)
			//	{
			//		// something went wrong, closing zip file and stopping
			//		gzclose(gzf);
			//		return m_bOpened = false;
			//	}
			//	// tInString OK, adding
			//	if (nReadChar != 0)
			//	{
			//		sLoaded.append(tInString, nReadChar);
			//		TRACE(_T("nReadChar = %d, sLoaded size = %d\n"), nReadChar, sLoaded.size());
			//	}
			//}
			//// closing zipfile
			//gzclose(gzf);

			//m_bOpened = SetDoc(sLoaded.c_str());
		}

		catch (CException* e)
		{
			varResult = VARIANT_FALSE;
		}
	}
}

CXMLArchive::~CXMLArchive()
{
}

void CXMLArchive::Close()
{

	//HRESULT hr;

	//try
	//{
	//	if (IsStoring())
	//	{
	//		if (m_streamPtr == NULL)
	//		{
	//			BSTR fileNameBSTR = m_fileName.AllocSysString();
	//			hr = m_xmlDocPtr->save(fileNameBSTR);
	//			::SysFreeString(fileNameBSTR);
	//		}
	//		else
	//		{
	//			hr = m_xmlDocPtr->save(_variant_t(m_streamPtr));
	//		}
	//	}
	//}

	//catch (_com_error e)
	//{
	//	ASSERT(FALSE);
	//}
}

CXMLArchiveNode* CXMLArchive::GetCurrentNode()
{
	if (m_nodeList.size() <= 0)
	{
		return NULL;
	}

	return m_nodeList.top();
};

void CXMLArchive::RemoveNode(CXMLArchiveNode* xmlArchiveNodePtr)
{
	ASSERT(m_nodeList.size() > 0);
	ASSERT(xmlArchiveNodePtr == m_nodeList.top());

	delete m_nodeList.top();
	m_nodeList.pop();
}

CXMLArchiveNode* CXMLArchive::GetNode(LPCTSTR nodeNameStr)
{
	CString nodeName(nodeNameStr);

	try
	{
		//BSTR nodeNameBSTR = nodeName.AllocSysString();

		CMarkup* fatherNodePtr;

		if (m_nodeList.size() == 0)
		{
			fatherNodePtr = m_pMarkup;
		}
		else
		{
			fatherNodePtr = m_nodeList.top()->m_nodePtr;
		}

		if (fatherNodePtr == NULL)
		{
			return NULL;
		}

		/*
		if (IsStoring())
		{
			// Archive is storing
			CMarkup* childnodep = m_pMarkup->AddChildElem(nodeName);
			CXMLArchiveNode* xmlArchiveNodePtr = new CXMLArchiveNode(this, 
				childnodep,
				//m_xmlDocPtr->createElement(nodeNameBSTR),
				fatherNodePtr);

			//::SysFreeString(nodeNameBSTR);

			m_nodeList.push(xmlArchiveNodePtr);

			return xmlArchiveNodePtr;
		}
		*/

		// Archive is Loading
		//MSXML::IXMLDOMNodeListPtr	nodeListPtr;
		//MSXML::IXMLDOMNodePtr		nodePtr;
		CMarkup*	nodeListPtr;
		CMarkup*		nodePtr;

		// If child node list is not empty, we are loading using the tags to
		// create CObject derived objects (CArray<Cobject* CObject*>, use child list

		/*
		if (m_nodeList.size() > 0)
		{
			CXMLArchiveNode* xmlNodePtr = m_nodeList.top();
			nodeListPtr = xmlNodePtr->m_childNodeListPtr;

			if (nodeListPtr != NULL && nodeListPtr->length > 0)
			{
				int childIndex = xmlNodePtr->m_childIndex;

				if (childIndex < nodeListPtr->length)
				{
					nodeListPtr->get_item(childIndex, &nodePtr);

					CXMLArchiveNode* xmlArchiveNodePtr = new CXMLArchiveNode(this, nodePtr, 0);// m_xmlDocPtr);

					m_nodeList.push(xmlArchiveNodePtr);

					//::SysFreeString(nodeNameBSTR);

					return xmlArchiveNodePtr;
				}

				ASSERT(FALSE);
			}
		}
		*/

		// Get all nodes with this name
		//if (MSXML::IXMLDOMDocumentPtr(fatherNodePtr) != NULL)
		//{
		//	// First level node in document
		//	ASSERT(!nodeName.IsEmpty());
		//	nodeListPtr = MSXML::IXMLDOMDocumentPtr(fatherNodePtr)->getElementsByTagName(nodeNameBSTR);
		//}
		//else
		//{
		//	// Get node with desired name
		//	nodeListPtr = MSXML::IXMLDOMElementPtr(fatherNodePtr)->getElementsByTagName(nodeNameBSTR);
		//}

		/*nodeListPtr = fatherNodePtr->FindElem(nodeName);*/

		//::SysFreeString(nodeNameBSTR);

		/*
		int childIndex = 0;
		if (m_nodeList.size() > 0)
		{
			childIndex = m_nodeList.top()->m_childIndex;
		}

		if (childIndex < nodeListPtr->length)
		{
			nodeListPtr->get_item(childIndex, &nodePtr);
		}

		*/
		CXMLArchiveNode* xmlArchiveNodePtr = new CXMLArchiveNode(this, nodePtr, m_pMarkup);// m_xmlDocPtr);

		m_nodeList.push(xmlArchiveNodePtr);

		return xmlArchiveNodePtr;
	}

	catch (...)
	{

	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class CXMLArchiveNode
//

// Cache of objects created
CMapStringToPtr		CXMLArchiveNode::m_classMap;

CXMLArchiveNode::CXMLArchiveNode(CXMLArchive* archivePtr, CMarkup* newNodePtr, CMarkup* fatherNodePtr) :
m_archivePtr(archivePtr),
m_nodePtr(newNodePtr),
m_fatherNodePtr(fatherNodePtr)
{
	m_childIndex = 0;
}

CXMLArchiveNode::~CXMLArchiveNode()
{
}

void CXMLArchiveNode::Close()
{
	CXMLArchive* archivePtr = m_archivePtr;

	/*
	if (archivePtr->IsStoring())
	{
		m_fatherNodePtr->appendChild(m_nodePtr);
	}
	*/
	archivePtr->RemoveNode(this);
}

// Loads into existing objects
void CXMLArchiveNode::DataNode(LPCTSTR attrName, CObject& object)
{
	m_archivePtr->GetNode(attrName);
	object.Serialize(*m_archivePtr);
	m_archivePtr->GetCurrentNode()->Close();
}

// Creates new object when loading
void CXMLArchiveNode::DataNode(LPCTSTR attrName, CObject*& objectPtr)
{
	if (m_archivePtr->IsStoring() && objectPtr == NULL)
	{
		return;
	}

	CXMLArchiveNode* curNodePtr = m_archivePtr->GetNode(attrName);

	if (curNodePtr == NULL)
	{
		return;
	}

	if (m_archivePtr->IsStoring())
	{
		objectPtr->Serialize(*m_archivePtr);
	}
	else
	{
		objectPtr = NULL;

		do
		{
			// Dummy loop, executes only once
			/*
			int numberObjects = curNodePtr->GetNoChildren();

			if (numberObjects == 0)
			{
				break;
			}

			CString childNodeName = curNodePtr->GetChildName(0);

			objectPtr = CreateObject(childNodeName);

			if (objectPtr == NULL)
			{
				ASSERT(FALSE);
				break;
			}
			*/
			objectPtr->Serialize(*m_archivePtr);

			break;
		} while (FALSE); // C4702 OK
	}

	m_childIndex = 0;
	curNodePtr->Close();
}

CObject* CXMLArchiveNode::CreateObject(const CString& className)
{
	CRuntimeClass* pClass = NULL;

	// First see if the CRuntimeClass object is cached
	if (!m_classMap.Lookup(className, (void*&)pClass))
	{
		// Search app specific classes
		AFX_MODULE_STATE* pModuleState = AfxGetModuleState();

		for (pClass = pModuleState->m_classList; pClass != NULL; pClass = pClass->m_pNextClass)
		{
			if (className == pClass->m_lpszClassName)
			{
				m_classMap[className] = (void*&)pClass;
				break;
			}
		}
	}

	if (pClass == NULL)
	{
		TRACE1("You did not declare the class %s DECLARE_XMLSERIAL", (const char *)className);
		ASSERT(FALSE);
		return NULL;
	}

	return pClass->CreateObject();
}


