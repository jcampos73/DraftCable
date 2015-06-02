#include "stdafx.h"
#include "Importer.h"
#include "Tipography.h"

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

BOOL CImporter::ImportLibrary(LPCTSTR lpszPathName, CObArray *pObArray, CSize szGrip)
{
	//Set size grip
	m_szGrid = szGrip;

	//Load raw xml data into object
	__DoLoadRawData(lpszPathName);

	if (m_xmlDocPtr->FindElem(m_strTCRootNodeLabel)){
		if (!m_xmlDocPtr->IntoElem()){
			return FALSE;
		}

		int count = 0;
		//Debug: set count_max to other value than 0, to limit number of processed shapes
		int count_max = 0;// 16;

		//Iterate symbols
		while (m_xmlDocPtr->FindElem(m_strTCSymbolNodeLabel))
		{
			if (m_xmlDocPtr->IntoElem()){

				//Unit to be created
				CShapeUnit* pShUnit = NULL;

				//Do process symbol
				__DoProcessNode(pShUnit);

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

BOOL CImporter::__DoProcessNode(CShapeUnit*& pShUnit)
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

		//Debug
		TRACE(_T("Importing part %s.\n"), (LPCTSTR)(pShUnit->m_sUnitName));
		if (pShUnit->m_sUnitName=="Inductor w/Core"){
			int stop = 1;
		}

		m_xmlDocPtr->OutOfElem();
	}

	//Symbol frame
	CShapeFrmRect *pShFrm = new CShapeFrmRect();
	pShFrm->m_Rect = CRect(0, 0, 0, 0);
	pShUnit->m_obarrShapearr.Add(pShFrm);

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
			__DoProcessPolygon(pobarrShapearr);

			//Need to normalize and rescale (so we have to store in some place all the points)
			//to reprocess after (maybe before saving, in upper layer)
		}

		//Iterate pins
		m_xmlDocPtr->ResetMainPos();
		while (m_xmlDocPtr->FindElem(m_strTCSymbolNodeGeomPinLabel))
		{
			//Do process symbol
			CObArray* pobarrShapearr = &pShUnit->m_obarrShapearr;
			__DoProcessPin(pobarrShapearr);
		}

		//Iterate ellipses
		m_xmlDocPtr->ResetMainPos();
		while (m_xmlDocPtr->FindElem(m_strTCSymbolNodeGeomEllipseLabel))
		{
			//Do process symbol
			CObArray* pobarrShapearr = &pShUnit->m_obarrShapearr;
			__DoProcessEllipse(pobarrShapearr);
		}

		//Go out of node
		m_xmlDocPtr->OutOfElem();
	}//end if root node

	return TRUE;
}

BOOL CImporter::__DoProcessPolygon(CObArray* pobarrShapearr)
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
			if (arc > STRAIGHT_SEGMENT){//Greater than 0 is an Arc: 1=CCW, 2=CW
				//Create ellipse arc or end Polyline
				if (ptArray.GetCount() >= 2){
					__DoCreatePolyline(&ptArray, pobarrShapearr);
				}

				//Add point to array
				CPoint point = GetPointFromStr(pos, ",", m_scale);
				ptArray.Add(point0 + point);

				//Now create the arc
				if (ptArray.GetCount()>=2)
				{
					//For security: check iif it's an arc (vertex should define a rectangle)
					if(ptArray[0].x != ptArray[1].x
						&& ptArray[0].y != ptArray[1].y
					){
						__DoCreateArc(&ptArray, arc, pobarrShapearr);
					}
					//If not, proceed with a line
					else
					{
						__DoCreatePolyline(&ptArray, pobarrShapearr);
					}
				}

				//Start new segment/spline
				ptArray.RemoveAt(0);
			}
			else{
				//Add point to array
				CPoint point = GetPointFromStr(pos, ",", m_scale);
				ptArray.Add(point0 + point);
			}
		}

		//Create ellipse arc or end Polyline
		if (ptArray.GetCount() >= 2){
			__DoCreatePolyline(&ptArray, pobarrShapearr);
		}
	}
	CATCH_ALL(e)
	{
		char msg[1024];
		e->GetErrorMessage(msg, 1024);
		AfxMessageBox(msg, MB_OK | MB_ICONEXCLAMATION, -1);
	}
	END_CATCH_ALL

	//Go out of node
	m_xmlDocPtr->OutOfElem();

	return TRUE;
}

BOOL CImporter::__DoProcessPin(CObArray* pobarrShapearr)
{
	TRY
	{
		//<PIN pos='70.00000,37.00000' which='0' elec='4' direction='1' part='0' number='2' show='0' length='15' number_pos='0' centre_name='0'></PIN>
		//Get position of pin
		CString pos = m_xmlDocPtr->GetAttrib("pos");
		//If 1 means NOT output, for example
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

		//Start to process
		CPoint point0 = GetPointFromStr(pos);
		CPoint point_copy = GetPointFromStr(pos);
		point0 = CPoint(point0.x * m_scale, point0.y * m_scale);

		//We can not snap here becouse we are going to snap in post processing imported shapes
		//for normalization to bounding rectangle
		//SnapToGrid(&point0, m_szGrid);
		CShapePin* pSh = new CShapePin(atoi(number), _DRAFTDRAW_SEL_RESIZING_RECT_S, SHAPEUNIT_PINTYPE_WIRE);
		//Just becouse we don't want empty rectangles
		pSh->m_Rect = CRect(point0, point0 + CPoint(1, 1));
		pSh->Unselect();
		pobarrShapearr->Add(pSh);

		//Get pin length normalized to DraftCable
		int l = atoi(length) * m_scale / COOR_TO_PIN_LENGTH;
		CSize sz(DCABLE_PIN_WIDTH, DCABLE_PIN_HEIGHT);
		CPoint ptOffset = CPoint(-l, 0);
		CPoint ptOffset0 = ptOffset;

		//Process orientation
		__DoProcessPinOrientation(atoi(direction), l, sz, ptOffset, ptOffset0);
	
		//Create pin line
		CArray<CPoint, CPoint> ptArray;
		ptArray.Add(point0);
		ptArray.Add(point0 + ptOffset0);
		__DoCreatePolyline(&ptArray, pobarrShapearr);

		//Check if it is a not pin
		if (which == m_strWhichNotPin){
			//Do create pin
			ptArray.RemoveAll();
			ptArray.Add(point0 + ptOffset);
			ptArray.Add(point0 + ptOffset + sz);
			__DoCreateNotPin(&ptArray, pobarrShapearr);
		}

		//show!=0 means visible pin numbers
		if (show == m_strShowNumberPin)
		{
			CShape* pSh = new CShapeLabel();
			((CShapeLabel *)pSh)->Create(CRect(point0, point0), number, AFX_FONT_NORMAL_DEFAULT_HEIGHT, FALSE);
			((CShapeLabel *)pSh)->m_bResizeOnFirstDraw = TRUE;
			//Add shape to array
			if (pobarrShapearr != NULL) pobarrShapearr->Add(pSh);
		}
	}
	CATCH_ALL(e)
	{
		char msg[1024];
		e->GetErrorMessage(msg, 1024);
		AfxMessageBox(msg, MB_OK | MB_ICONEXCLAMATION, -1);
	}
	END_CATCH_ALL

	return TRUE;
}

BOOL CImporter::__DoProcessEllipse(CObArray* pobarrShapearr)
{
	TRY
	{
		//Get position of polygon
		CString pos = m_xmlDocPtr->GetAttrib("pos");
		CPoint point0 = GetPointFromStr(pos);
		point0 = CPoint(point0.x * m_scale, point0.y * m_scale);

		//Add point to array
		CArray<CPoint, CPoint> ptArray;
		CString a = m_xmlDocPtr->GetAttrib("a");
		CPoint point = GetPointFromStr(a, ",", m_scale);
		ptArray.Add(point0 + point);

		CString b = m_xmlDocPtr->GetAttrib("b");
		point = GetPointFromStr(b, ",", m_scale);
		ptArray.Add(point0 + point);

		CShape* pSh = NULL;
		__DoCreateEllipse(&ptArray, pSh);
		if (pSh != NULL) pobarrShapearr->Add(pSh);

	}
	CATCH_ALL(e)
	{
		char msg[1024];
		e->GetErrorMessage(msg, 1024);
		AfxMessageBox(msg, MB_OK | MB_ICONEXCLAMATION, -1);
	}
	END_CATCH_ALL

	return TRUE;
}

void CImporter::__DoCreatePolyline(CArray<CPoint, CPoint>* ptArray, CObArray* pobarrShapearr /*= NULL*/){
	//Create Polyline
	if (ptArray->GetCount() > 0)
	{
		CShape* pSh = new CShapePolyline();
		LPPOINT pPoints = new POINT[ptArray->GetCount()];
		for (int i = 0; i < ptArray->GetCount(); i++)
		{
			pPoints[i] = (*ptArray)[i];
		}
		((CShapePolyline *)pSh)->Create(pPoints, ptArray->GetCount());

		//Add shape to array
		if (pobarrShapearr != NULL) pobarrShapearr->Add(pSh);
	}
}

void CImporter::__DoCreateArc(CArray<CPoint, CPoint>* ptArray, int arc /*= 1*/, CObArray* pobarrShapearr /*= NULL*/){
	//Create ellipse arc
	if (ptArray->GetCount() >= 2){
		CShape* pSh = new CShapeArc();
		//Create arc
		((CShapeArc*)pSh)->Create(&(*ptArray)[0], &(*ptArray)[1], TRUE, arc);
		pSh->Unselect();

		//Add shape to array
		if (pobarrShapearr != NULL) pobarrShapearr->Add(pSh);
	}
}

void CImporter::__DoCreateNotPin(CArray<CPoint, CPoint>* ptArray, CObArray* pobarrShapearr /*= NULL*/){
	//Create not pin
	if (ptArray->GetCount() >= 2){
		CRect rect = new CRect((*ptArray)[0], (*ptArray)[1]);
		CShape *pSh = new CShapeEllipse(rect);
		pSh->Unselect();

		//Add shape to array
		if (pobarrShapearr != NULL) pobarrShapearr->Add(pSh);
	}
}

void CImporter::__DoCreateNotPin(CPoint point1, CPoint point2, CObArray* pobarrShapearr /*= NULL*/){
	//Create not pin
	CRect rect = new CRect(point1,point2);
	CShape *pSh = new CShapeEllipse(rect);
	pSh->Unselect();

	//Add shape to array
	if (pobarrShapearr != NULL) pobarrShapearr->Add(pSh);
}

void CImporter::__DoCreateEllipse(CArray<CPoint, CPoint>* ptArray, CShape*& pSh, CObArray* pobarrShapearr /*= NULL*/){
	//Create not pin
	if (ptArray->GetCount() >= 2){
		CRect rect = new CRect((*ptArray)[0], (*ptArray)[1]);
		pSh = new CShapeEllipse(rect);
		pSh->Unselect();

		//Add shape to array
		if (pobarrShapearr != NULL) pobarrShapearr->Add(pSh);
	}
}

void CImporter::__DoProcessPinOrientation(int direction, int l, CSize sz, CPoint& offset, CPoint& offset0)
{
	switch (direction)
	{
	case importerPinW:
		offset = CPoint(-l, 0);
		offset0 = offset;
		offset += CPoint(0, -sz.cy *.5);
		break;
	case importerPinE:
		offset = CPoint(l, 0);
		offset0 = offset;
		offset += CPoint(0, -sz.cy *.5);
		break;
	case importerPinN:
		offset = CPoint(0, -l);
		offset0 = offset;
		offset += CPoint(-sz.cx *.5, 0);
		break;
	case importerPinS:
		offset = CPoint(0, l);
		offset0 = offset;
		offset += CPoint(-sz.cx *.5, 0);
		break;
	}
}

void CImporter::__DoLoadRawData(LPCTSTR lpszPathName)
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

POINT CImporter::GetPointFromStr(LPCTSTR pos, LPCTSTR delimiter /*= ","*/, float scale /*= 0.0f*/)
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
		if (scale != 0.0f){
			x = round(atof(sa[0]) * scale);
			y = round(atof(sa[1]) * scale);
		}
		CPoint point(x, y);
		return point;
	}
	return CPoint(0, 0);
}



