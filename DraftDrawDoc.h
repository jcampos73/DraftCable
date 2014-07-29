// DraftDrawDoc.h : interface of the CDraftDrawDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAFTDRAWDOC_H__CBF459FA_3972_40C5_9957_AEA294A73AE8__INCLUDED_)
#define AFX_DRAFTDRAWDOC_H__CBF459FA_3972_40C5_9957_AEA294A73AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"
#include "PLexdoc.h"
#include "OMParser.h"		//Object model parser
#include "ShArray.h"			//Screen array

//04/12/2004
//View types
//Standard notation for DraftCable constants
//DC<name>_<value>
#define DCVIEWTYPE_SCHEMATIC					0	//schematic view type
#define DCVIEWTYPE_PART							1	//part view type

//Selected tool
#define _TOOLSELECT_DRAFTCABLE					0
#define _TOOLSELECTMUL_DRAFTCABLE				10//multiple selection tool
#define _TOOLROTATE_DRAFTCABLE					11//rotate tool
#define _TOOLPLACE_DRAFTCABLE					1
#define _TOOLSELECT_ZOOMPLUS					2
#define _TOOLSELECT_ZOOMMINUS					3
#define _TOOLSELECT_PAN							4

//Selectel tool type
#define _TOOLTYPENORMAL_DRAFTCABLE				0
#define _TOOLTYPECHAININI_DRAFTCABLE			1
#define _TOOLTYPECHAIN_DRAFTCABLE				2
#define _TOOLTYPEPOLY_DRAFTCABLE				20

//Constants
#define _DRAFTCABLE_DOC_ARCHIVED_COUNT			5
//'all files' is omitted from filter string, as is added in run time.
#define _DRAFTCABLE_DOC_FILTER_STR				"Draft Draw Files (*.ddw)|*.ddw|Gerber Files (*.gbr)|*.gbr|Cut Files (*.cut)|*.cut|ACAD Interchange (*.dxf)|*.dxf|Draft Draw Files 1 (*.dd1)|*.dd1|"
//Filters
#define _DRAFTCABLE_DOC_FILTER_DDW				1
#define _DRAFTCABLE_DOC_FILTER_GERBER			2
#define _DRAFTCABLE_DOC_FILTER_DXF				4
#define _DRAFTCABLE_DOC_FILTER_DD1				5
#define _DRAFTCABLE_DOC_FILTER_SVG				6

enum{CURSOR_NW=0,CURSOR_NE,CURSOR_NS=4,CURSOR_EW,
CURSOR_4W=8,CURSOR_ARROW,
CURSOR_DRAW,CURSOR_PENCIL,
CURSOR_ZOOM_PLUS,CURSOR_ZOOM_MINUS,CURSOR_ROTATE
};

//Default parts
#define _DRAFTCABLE_DOC_DEFAULT_BUSJUNC			"Standard.busjunc.ddw"
#define _DRAFTCABLE_DOC_DEFAULT_JUNCTION		"Standard.junction.ddw"

//Macro
#define GET_SHAPE(pDoc,Shape){\
		if(pDoc->m_pSh) delete(pDoc->m_pSh);\
		pDoc->m_pSh=new(Shape);\
		pDoc->m_pSh->m_pCursorArray=pDoc->m_CursorArray;\
}

extern CString afxWinwordPath;

class CDraftDrawDoc : public CDocument
{
protected: // create from serialization only
	CDraftDrawDoc();
	DECLARE_DYNCREATE(CDraftDrawDoc)

// Attributes
public:
	UINT GetActiveViewType();					//DCVIEWTYPE_SCHEMATIC or DCVIEWTYPE_PART
	UINT SetActiveViewType(UINT nViewType);		//Set active view type
	//Object array functions
	CObject * LastObject(int &index);
	void DeleteObject(int index=-1);
	void DeleteShape(DWORD dwId, BOOL bForce =TRUE);
	int AddObject( CObject* newElement,UINT nId =0 );
	int AddShape(DWORD dwType,int left,int top,int right,int bottom,DWORD dwId,std::deque<CDBVariant*> *lpParameters =NULL);
	CObList m_ObList;
	CObject * EndObject();
	CObject * FirstObject(int& index);
	CObject * PrevObject(int& index );
	CObject * NextObject(int& index );
	int FindObject(CObject *pObFind);
	int FindShape(UINT nId);
	int GetSizeObject();						//Get size of object stack

	CPLexdoc *m_plexProcessor;
	//Added temporary to insert TBEditView in project
	//--------------------------------------------------------------------------

	CTypedPtrArray<CObArray,CStringArray*> m_obaCables1;
	CTypedPtrArray<CObArray,CStringArray*> m_obaCables2;
	std::map<int,HWND> m_mapIdHWView;
	CMap<int,int,int,int> m_mapIdHWIndex;//std::map<int,int>
	//Map of cable identifier to HWND of Patch Panel view
	CMapStringToPtr m_mapStrHWView;
	//Map of cable identifier to index for m_obaCables1 and m_obaCables2
	CMapStringToPtr m_mapStrHWIndex;//not seriallized yet
	//Each member holds all PatchPanel asignations in form of a CIntArray
	CMapStringToPtr m_mapStrHWParameter;//not seriallized yet

	//--------------------------------------------------------------------------
	float m_fVer;//version of document

// Operations
public:
	void Order(int idata);									//Changes shape order
	void CDraftDrawDoc::LoadRack(CDraftDrawDoc *pSchDoc);	//Loads a rack document
	int AddSheet();											//Add a new sheet
	int ChangeSheet(int nIndex);							//Change sheet
	int GetSheetCount();									//Get sheet count
	int GetSheetSerialCount();								//Return number of sheet that will be serialized
	int GetSheet();											//Get index beginning with 0 of selected sheet
	CObject *GetCurSelOb();									//Get selected object
	CString GetRackOb(CString strName);						//Get rack counter part
	void SelectShapes(BOOL bSelect=TRUE,UINT nId=0);		//Select/Deselect shapes
	void MoveShapes(int x,int y,BOOL bAbsolute=FALSE);		//Move shapes
	void ScaleShapes(int x,int y,int nScale,BOOL bAbsolute=FALSE);//Scale shapes
	const CString& GetPathName1( ) const;
	virtual void SetPathName1( LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE );
	void SetModifiedFlag( BOOL bModified = TRUE );
	BOOL IsModified( );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDraftDrawDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL
	virtual void OnDatabaseSave(CString strFile ="");
	virtual void OnDatabaseLoad();
	BOOL OnOpenDocument2(LPCTSTR lpszPathName);
	BOOL OnInsertDocument(LPCTSTR lpszPathName);
	//Part document parallel framework
	//--------------------------------------------------------------------------
	virtual void OnNewDocumentPart();
	virtual BOOL OnOpenDocumentPart(LPCTSTR lpszPathName);
	//--------------------------------------------------------------------------

// Implementation
public:
	//Display patch panel properties
	void DisplayPatchPProp(UINT nID);
	void DisplayPatchPProp(CString strID);
	//Serialize patch panel
	void SerializePatchP(CArchive& ar,UINT nID);
	//Reset all drawing operations
	CRect ResetAllDrwOpe();

	CSize GetSize();
	CSize GetDefaultSize();
	CShape * FindShape(CShape *pShSearch,CShape **pShSearchChild = NULL);
	BOOL SetCursor();
	int GetCursor();

	//This two members determine program status
	int m_iToolSel;		//Selected tool: place, selection...
	int m_iToolType;	//Selected tool type: normal, chain...

	int m_iTypeDocument;//Document type 0->schematic;1->rack;2->patch panel
	CSize m_szGrid;

	int nFilterIndex;	//Filter index for input/output operations
	//CString sFilter;
	//CStringArray saCodecs;
	CArray<std::wstring,std::wstring> saCodecs;
	BOOL m_bFlagPartEdit;			//Flag to indicate part edition document
	int m_iIdPart;					//Hold part identifier for part editing document
	CDraftDrawDoc *m_pDocRack;		//Rack view initiated.

	CPoint m_ptPasteOffset;
	int m_iCursor;
	HCURSOR m_CursorArray[16];
	CShape * m_pSh;					//Shape to be placed
	int InsertObject(int index,CObject *newElement,UINT nId =0);

	//Containers
	CObArray m_ObArray;			//Object array for schematic
	CObArray m_ObArrayPart;		//Object array for parts
	CObArray *m_pObArray;		//Main stack
	CObArray *m_pObArrayTemp;	//Clipboard stack
	//Adding sheets to application
	//Instead of m_ObArray
	CObArray m_ObObArray;		//Object array of Object arrays for schematic

	UINT *m_pnCountId;
	UINT m_nCountId;			//Counter for id's is incremented by AddObject, InsertObject and never decremented
	UINT m_nCountIdPart;		//Counter for id's is incremented by AddObject, InsertObject and never decremented

	mapShapeIdtoObj_t m_MapShapeIdtoObj;
	//04/12/2004
	mapShapeIdtoObj_t *m_pmapShapeIdtoObj;					//Pointer to map
	mapShapeIdtoObj_t m_mapShapeIdtoObjPart;				//Map for parts
	std::deque<mapShapeIdtoObj_t*> m_mapShapeIdtoObjArray;	//Object array of Maps for schematic
	//26/01/2004
	CShArray *m_pShArray;		//Pointer to shape array
	CShArray m_ShArrayPart;		//Screen array for part edition
	std::deque<CShArray*> m_ShArrayArray;	//Object array of arrays for schematic

/*
	//Two deque's are necessary:
	//1.When user hits redo button m_dqCommands is advanced (back to front).
	//2.When user hits undo button m_dqCommandsUndo is retroceded (front to back).
	//Deque to store visual basic commands
	std::deque<std::string> m_dqCommands;
	//Deque to store visual basic undo commands
	std::deque<std::string> m_dqCommandsUndo;
	//index to both queues
	int m_nIndexCommQue;
*/
	cmddeque *cmdDeque;
	cmddeque m_cmdDeque;		//Queue for schematic
	cmddeque m_cmdDequePart;	//Queue for part

	//static std::string m_sFilter;//filter string for saving

	float m_fUTemp;				//Temp U height calculed while saving  rack chasis parts

	virtual ~CDraftDrawDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	//Prepare list of sheet to be saved
	void DoListSheetSave(CList<int,int> *pshLst);
	//Prepare file extensions for OnFileSaveAs dialog.
	void CDraftDrawDoc::DoFileExtensions(std::string& sFilter,CStringArray& saExtensions);
	//Writes in supplied storage objects that implements persistenece
	void WriteStorage(LPSTORAGE pStg);
	//Reads from storage persistent objects
	void ReadStorage(LPSTORAGE pStg);
public:
	static int Split(CString str,LPTSTR *sa,int max);

// Generated message map functions
protected:
	//List of sheet to be serialized
	//List is cleared in delete contents
	CList<int,int> m_shList;//!!!MAGIC NUMBER
	CSize m_szDesign;
	int m_nIndexSheet;//index of selected sheet

	CString m_sPathName1;//Path name for part under edition: library.part
	BOOL m_bModified1;//Modified flag for part under edition

	//Added temporary to insert TBEditView in project
	CStringArray m_straCable1;
	CStringArray m_straCable2;

	//There are two views supported:
	//0 ->  schematic
	//1 ->  part edition
	void SetupCollections(UINT nTypeView);

	//{{AFX_MSG(CDraftDrawDoc)
	afx_msg void OnButtonRect();
	afx_msg void OnButtonEllipse();
	afx_msg void OnButtonLine();
	afx_msg void OnButtonArc();
	afx_msg void OnButtonBoard();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnPartSave();
	afx_msg void OnPartSaveAs();
	afx_msg void OnSelectNet();
	afx_msg void OnArchivoGencable();
	afx_msg void OnToolOptionPage();
	afx_msg void OnToolConfig();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnToolSic();
	afx_msg void OnFileSaveSheetCurrent();
	afx_msg void OnPlaceSheets();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAFTDRAWDOC_H__CBF459FA_3972_40C5_9957_AEA294A73AE8__INCLUDED_)
