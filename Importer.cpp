#include "stdafx.h"
#include "Importer.h"

#include "DraftDrawDoc.h"

CImporter::CImporter()
{
	m_bOpened = false;
	m_xmlDocPtr = NULL;
	m_scale = m_scaleDefault;
}


CImporter::~CImporter()
{
	if (m_xmlDocPtr != NULL) delete(m_xmlDocPtr);
}

BOOL CImporter::ImportLibrary(LPCTSTR lpszPathName, CObArray *pObArray)
{
	//Load raw xml data into object
	DoLoadRawData(lpszPathName);

	if (m_xmlDocPtr->FindElem(m_strTCRootNodeLabel)){
		if (!m_xmlDocPtr->IntoElem()){
			return FALSE;
		}

		int count = 0;
		//Debug: set count_max to other value than 0, to limit number of processed shapes
		int count_max = 2;

		//Iterate symbols
		while (m_xmlDocPtr->FindElem(m_strTCSymbolNodeLabel))
		{
			if (m_xmlDocPtr->IntoElem()){

				//Unit to be created
				CShapeUnit* pShUnit = NULL;

				//Do process symbol
				DoProcessNode(pShUnit);

				//Go out of node
				m_xmlDocPtr->OutOfElem();

				//Add to array
				if (pShUnit != NULL) pObArray->Add(pShUnit);

				//Increment counter
				count++;
				if (count_max > 0 && count >= count_max) break;
			}
		}
	}//end if root node

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CImporter implementation

BOOL CImporter::DoProcessNode(CShapeUnit*& pShUnit)
{
	//Create CShapeUnit
	pShUnit = new CShapeUnit();

	//Symbol description
	if (m_xmlDocPtr->FindElem(m_strTCSymbolNodeDescLabel)){
		if (!m_xmlDocPtr->IntoElem()){
			return FALSE;
		}

		if (!m_xmlDocPtr->FindElem(m_strTCSymbolNodeDescDescriptionLabel)){
			return FALSE;
		}

		pShUnit->m_sUnitName = m_xmlDocPtr->GetData();

		m_xmlDocPtr->OutOfElem();
	}

	//Symbol geometry
	if (m_xmlDocPtr->FindElem(m_strTCSymbolNodeGeomLabel)){
		if (!m_xmlDocPtr->IntoElem()){
			return FALSE;
		}

		//Iterate polygons
		while (m_xmlDocPtr->FindElem(m_strTCSymbolNodeGeomPolygonLabel))
		{
			//Do process symbol
			CObArray* pobarrShapearr = &pShUnit->m_obarrShapearr;
			DoProcessPolygon(pobarrShapearr);

			//Need to normalize and rescale (so we have to store in some place all the points)
			//to reprocess after (maybe before saving, in upper layer)
		}

		//Iterate pins
		/*
		m_xmlDocPtr->ResetMainPos();
		while (m_xmlDocPtr->FindElem(m_strTCSymbolNodeGeomPinLabel))
		{
			//Do process symbol
			CObArray* pobarrShapearr = &pShUnit->m_obarrShapearr;
			DoProcessPin(pobarrShapearr);
		}
		*/

		//Go out of node
		m_xmlDocPtr->OutOfElem();
	}//end if root node

	return TRUE;
}

BOOL CImporter::DoProcessPolygon(CObArray* pobarrShapearr)
{
	TRY
	{
		//Get position of polygon
		CString pos = m_xmlDocPtr->GetAttrib("pos");
		CPoint point0 = GetPointFromStr(pos);
		point0 = CPoint(point0.x * m_scale, point0.y * m_scale);

		//Check if polygon is empty
		if (!m_xmlDocPtr->IntoElem()){
			return FALSE;
		}

		//Iterate points
		CArray<CPoint, CPoint> ptArray;
		while (m_xmlDocPtr->FindElem(m_strTCSymbolNodeGeomPolygonPointLabel))
		{
			CString pos = m_xmlDocPtr->GetAttrib("pos");
			int arc = atoi(m_xmlDocPtr->GetAttrib("arc"));
			if (arc > 0){
				//Create ellipse arc or end Polyline
				if (ptArray.GetCount() >= 2){
					CShape* pSh = NULL;
					DoCreatePolyline(&ptArray, pSh);
					if (pSh != NULL) pobarrShapearr->Add(pSh);
				}

				//Add point to array
				CPoint point = GetPointFromStr(pos);
				ptArray.Add(point0 + CPoint(point.x * m_scale, point.y * m_scale));

				//Now create the arc
				if (ptArray.GetCount()>=2){
					CShapeArc* pSh = new CShapeArc();
					//Create arc
					pSh->Create(&ptArray[0], &ptArray[1], TRUE);
					pSh->Unselect();
					pobarrShapearr->Add(pSh);
				}

				//Start new segment/spline
				ptArray.RemoveAll();
			}
			else{
				//Add point to array
				CPoint point = GetPointFromStr(pos);
				ptArray.Add(point0 + CPoint(point.x * m_scale, point.y * m_scale));
			}
		}
		//Create ellipse arc or end Polyline
		if (ptArray.GetCount() >= 2){
			CShape* pSh = NULL;
			DoCreatePolyline(&ptArray, pSh);
			if (pSh != NULL) pobarrShapearr->Add(pSh);
		}
	}
	CATCH_ALL(e)
	{

	}
	END_CATCH_ALL

	//Go out of node
	m_xmlDocPtr->OutOfElem();

	return TRUE;
}

BOOL CImporter::DoProcessPin(CObArray* pobarrShapearr)
{
	TRY
	{
		//<PIN pos='70.00000,37.00000' which='0' elec='4' direction='1' part='0' number='2' show='0' length='15' number_pos='0' centre_name='0'></PIN>
		//Get position of pin
		CString pos = m_xmlDocPtr->GetAttrib("pos");
		CString which = m_xmlDocPtr->GetAttrib("which");
		CString elec = m_xmlDocPtr->GetAttrib("elec");
		//1 = S; 0 = N; 3 = E; 2 = W
		CString direction = m_xmlDocPtr->GetAttrib("direction");
		CString part = m_xmlDocPtr->GetAttrib("part");
		CString number = m_xmlDocPtr->GetAttrib("number");
		CString show = m_xmlDocPtr->GetAttrib("show");
		CString length = m_xmlDocPtr->GetAttrib("length");
		CString number_pos = m_xmlDocPtr->GetAttrib("number_pos");
		CString centre_name = m_xmlDocPtr->GetAttrib("centre_name");

		CPoint point0 = GetPointFromStr(pos);
		point0 = CPoint(point0.x * m_scale, point0.y * m_scale);

	}
	CATCH_ALL(e)
	{

	}
	END_CATCH_ALL

	return TRUE;
}

void CImporter::DoCreatePolyline(CArray<CPoint, CPoint>* ptArray, CShape*& pSh){
	//Create ellipse arc or end Polyline
	if (ptArray->GetCount() > 0)
	{
		pSh = new CShapePolyline();
		LPPOINT pPoints = new POINT[ptArray->GetCount()];
		for (int i = 0; i < ptArray->GetCount(); i++)
		{
			pPoints[i] = (*ptArray)[i];
		}
		((CShapePolyline *)pSh)->Create(pPoints, ptArray->GetCount());
		ptArray->RemoveAll();
	}
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

POINT CImporter::GetPointFromStr(LPCTSTR pos, LPCTSTR delimiter /*= ","*/)
{
	int idata = CDraftDrawDoc::Split(pos, ",", NULL, 0);
	LPTSTR *sa = new LPTSTR[idata];
	for (int i = 0; i<idata; i++){
		sa[i] = new TCHAR[255];
	}
	CDraftDrawDoc::Split(pos, ",", sa, idata);
	if (idata >= 2){
		int x = atoi(sa[0]);
		int y = atoi(sa[1]);
		CPoint point(x, y);
		//ptArray.Add(point);
		return point;
	}
}



