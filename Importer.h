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
	BOOL ImportLibrary(LPCTSTR lpszPathName, CObArray *pObArray);
	CMarkup* m_xmlDocPtr;
	//Operations
	static void SnapToGrid(LPPOINT lpPoint, CSize szGrid);
protected:
	float m_scale;
	const float m_scaleDefault = 4.0;

	const CString m_strTCRootNodeLabel = "Library";
	const CString m_strTCSymbolNodeLabel = "SYMBOL";
	const CString m_strTCSymbolNodeDescLabel = "DETAILS";
	const CString m_strTCSymbolNodeDescDescriptionLabel = "DESCRIPTION";
	const CString m_strTCSymbolNodeGeomLabel = "TinyCAD";
	const CString m_strTCSymbolNodeGeomPolygonLabel = "POLYGON";
	const CString m_strTCSymbolNodeGeomPinLabel = "PIN";
	const CString m_strTCSymbolNodeGeomPolygonPointLabel = "POINT";

	//! true if already opened
	bool m_bOpened;
	void DoLoadRawData(LPCTSTR lpszPathName);
	BOOL DoProcessNode(CShapeUnit*& pSh);
	BOOL DoProcessPolygon(CObArray* pobarrShapearr);
	BOOL DoProcessPin(CObArray* pobarrShapearr);
	static void DoCreatePolyline(CArray<CPoint, CPoint>* ptArray, CShape*& pSh);
	static POINT GetPointFromStr(LPCTSTR input, LPCTSTR delimiter = ",");
};

