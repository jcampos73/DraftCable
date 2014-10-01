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

				m_xmlDocPtr = new CMarkup();
				m_bOpened = m_xmlDocPtr->SetDoc(sLoaded.c_str());
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

			m_xmlDocPtr = new CMarkup();
			m_bOpened = m_xmlDocPtr->SetDoc(sLoaded.c_str());


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
#ifdef USE_MSXML
		BSTR nodeNameBSTR = nodeName.AllocSysString();
		MSXML::IXMLDOMNodePtr fatherNodePtr;
#else
		CMarkup* fatherNodePtr;
#endif

		if (m_nodeList.size() == 0)
		{
			fatherNodePtr = m_xmlDocPtr;
		}
		else
		{
			fatherNodePtr = m_nodeList.top()->m_nodePtr;
		}

		if (fatherNodePtr == NULL)
		{
			return NULL;
		}


		if (IsStoring())
		{
#ifdef USE_MSXML
			// Archive is storing
			CXMLArchiveNode* xmlArchiveNodePtr = new CXMLArchiveNode(this, m_xmlDocPtr->createElement(nodeNameBSTR), fatherNodePtr);

			::SysFreeString(nodeNameBSTR);

			m_nodeList.push(xmlArchiveNodePtr);

			return xmlArchiveNodePtr;
#endif
			// Archive is storing
			m_xmlDocPtr->AddChildElem(nodeName);
			CMarkup* childnodep = new CMarkup(m_xmlDocPtr->GetDoc());
			childnodep = m_xmlDocPtr;
			m_xmlDocPtr->ResetChildPos();
			CXMLArchiveNode* xmlArchiveNodePtr = new CXMLArchiveNode(this, 
				childnodep,
				//m_xmlDocPtr->createElement(nodeNameBSTR),
				fatherNodePtr);

			m_nodeList.push(xmlArchiveNodePtr);

			return xmlArchiveNodePtr;
		}


		// Archive is Loading
		//MSXML::IXMLDOMNodeListPtr	nodeListPtr;
		//MSXML::IXMLDOMNodePtr		nodePtr;
		CMarkup*	nodeListPtr;
		CMarkup*		nodePtr;

		// If child node list is not empty, we are loading using the tags to
		// create CObject derived objects or collections (like CArray<Cobject* CObject*>, use child list

#ifdef USE_MSXML
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

					CXMLArchiveNode* xmlArchiveNodePtr = new CXMLArchiveNode(this, nodePtr, m_xmlDocPtr);

					m_nodeList.push(xmlArchiveNodePtr);

					::SysFreeString(nodeNameBSTR);

					return xmlArchiveNodePtr;
				}

				ASSERT(FALSE);
			}
		}
#else
		if (m_nodeList.size() > 0)
		{
			CXMLArchiveNode* xmlNodePtr = m_nodeList.top();
			nodeListPtr = xmlNodePtr->m_childNodeListPtr;

			//Block added to calcule length when using CMarkup
			int length = 0;
			if (nodeListPtr != NULL){
				nodeListPtr->ResetMainPos();
				while (nodeListPtr->FindElem())
				{
					length++;
				}
				nodeListPtr->ResetMainPos();
			}

			if (nodeListPtr != NULL && length/*nodeListPtr->length*/ > 0)
			{
				int childIndex = xmlNodePtr->m_childIndex;

				if (childIndex < length/*nodeListPtr->length*/)
				{
					//nodeListPtr->get_item(childIndex, &nodePtr);
					int index = 0;

					if (nodeListPtr->FindChildElem())
					{
						while (nodeListPtr->FindElem() && index < childIndex)
						{
							index++;
						}

						nodePtr = new CMarkup();
						nodePtr = nodeListPtr;
						//nodeListPtr->ResetChildPos();
					}

					CXMLArchiveNode* xmlArchiveNodePtr = new CXMLArchiveNode(this, nodePtr, m_xmlDocPtr);// m_xmlDocPtr);

					m_nodeList.push(xmlArchiveNodePtr);

					//::SysFreeString(nodeNameBSTR);

					return xmlArchiveNodePtr;
				}

				ASSERT(FALSE);
			}
		}
#endif

#ifdef USE_MSXML
		// Get all nodes with this name
		if (MSXML::IXMLDOMDocumentPtr(fatherNodePtr) != NULL)
		{
			// First level node in document
			ASSERT(!nodeName.IsEmpty());
			nodeListPtr = MSXML::IXMLDOMDocumentPtr(fatherNodePtr)->getElementsByTagName(nodeNameBSTR);
		}
		else
		{
			// Get node with desired name
			nodeListPtr = MSXML::IXMLDOMElementPtr(fatherNodePtr)->getElementsByTagName(nodeNameBSTR);
		}

		::SysFreeString(nodeNameBSTR);
#else

		bool bResult=fatherNodePtr->FindElem(nodeName);
		nodeListPtr = new CMarkup();
		nodeListPtr = fatherNodePtr;
		//fatherNodePtr->ResetMainPos();
#endif
		//Get child index from m_nodeList
		int childIndex = 0;
		if (m_nodeList.size() > 0)
		{
			childIndex = m_nodeList.top()->m_childIndex;
		}

#ifdef USE_MSXML
		if (childIndex < nodeListPtr->length)
		{
			nodeListPtr->get_item(childIndex, &nodePtr);
		}
#else
		//Block added to calcule length when using CMarkup
		int length = 0;
		nodeListPtr->ResetMainPos();
		while (nodeListPtr->FindElem())
		{
			length++;
		}
		nodeListPtr->ResetMainPos();

		if (childIndex < length)
		{
			int index = 0;

			//Check if it has child elements and go inside if so
			if (nodeListPtr->FindChildElem())
			{
				while (nodeListPtr->FindElem() && index < childIndex)
				{
					index++;
				}

				nodeListPtr->IntoElem();

				nodePtr = new CMarkup();
				nodePtr = nodeListPtr;

				nodeListPtr->ResetMainPos();
			}
			else
			{
				while (nodeListPtr->FindElem() && index < childIndex)
				{
					index++;
				}

				nodePtr = new CMarkup();
				nodePtr = nodeListPtr;
			}
		}
#endif
		CXMLArchiveNode* xmlArchiveNodePtr = new CXMLArchiveNode(this, nodePtr, m_xmlDocPtr);

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
	m_childNodeListPtr = NULL;
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

CString CXMLArchiveNode::GetChildName(int childIndex)
{
	ASSERT(m_nodePtr != NULL);

	if (m_nodePtr == NULL)
	{
		return CString();
	}

	ASSERT(m_childNodeListPtr != NULL /*&& childIndex < m_childNodeListPtr->length*/);

	//MSXML::IXMLDOMNodePtr nodePtr;
	CMarkup* nodePtr;
	//BSTR bstr;

	//m_childNodeListPtr->get_item(childIndex, &nodePtr);

	//nodePtr->get_nodeName(&bstr);

	//CString childName(bstr);

	CString childName;
	int index = 0;

	if (m_nodePtr->FindChildElem())
	{
		while (m_nodePtr->FindElem() && index < childIndex)
		{
			index++;
		}

		m_nodePtr->ResetChildPos();
	}

	//::SysFreeString(bstr);

	return childName;
}

int CXMLArchiveNode::GetNoChildren()
{
	int length = 0;

	if (m_nodePtr == NULL)
	{
		return 0;
	}

	if (m_childNodeListPtr == NULL)
	{
		// Get all nodes with this name
		//m_childNodeListPtr = m_nodePtr->childNodes;
		if (m_nodePtr->FindChildElem())
		{
			m_childNodeListPtr = new CMarkup(m_nodePtr->GetDoc());
			*m_childNodeListPtr = *m_nodePtr;
			m_childNodeListPtr->IntoElem();

			//Now count
			while (m_childNodeListPtr->FindElem())
			{
				length++;
			}

			m_childNodeListPtr->ResetChildPos();

			return length;
		}
	}

	while (m_nodePtr->FindElem())
	{
		length++;
	}

	m_nodePtr->ResetMainPos();

#ifdef USE_MSXML
	return m_childNodeListPtr->length;
#else
	return length;
#endif
}

// Loads into existing objects
void CXMLArchiveNode::DataNode(LPCTSTR attrName, CObject& object)
{
	m_archivePtr->GetNode(attrName);
	if (object.IsKindOf(RUNTIME_CLASS(CShapePolyline))){
		CObject* pObject = &object;
		((CShapePolyline*)pObject)->SerializeXml(*m_archivePtr);
	}
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


