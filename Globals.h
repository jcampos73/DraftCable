#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

#include <map>
#include <afxtempl.h>

//14/12/2004
//Global flags that affect application behaviour
//Enables CDocument::OnFileSave() to send part archiving (m_bFlagPartEdit==TRUE) throw CArchiveDb.
//To this date CArchiveDb only support part archiving in binary format.
#define DCABLE_PART_DB_BIN
//27/12/2004
//Enables hardkey security
//#define DCABLE_SECURITY_HARDKEY
//26/12/2004
//Enables ShArray that maps screen coordinates to shapes.
#define DCABLE_SHARRAY
//10/11/2014
//Loads polylines into array for unit loading,
//instead of loading them embedded
#define DCABLE_SHUNIT_LOAD_POLYLINES_INTO_ARRAY
//28/11/2014
//For developping not modal place part dialog
#define DCABLE_PLACEPART_DIALOG_NOT_MODAL
//24/12/2014
//Enable shell console in tools menu
#define DCABLE_SHELL_DIALOG_ENABLE


//This parameter is used:
//-To calcule update region when rotating pins = DCABLE_GRID_DEFAULT * 2
#define DCABLE_GRIDX_DEFAULT		10
#define DCABLE_GRIDY_DEFAULT		10

#define DCABLE_PIN_WIDTH			5
#define DCABLE_PIN_HEIGHT			5

//Default padding for shapes to frame rectangle
#define DCABLE_PADDINGX_DEFAULT		DCABLE_GRIDX_DEFAULT * 4
#define DCABLE_PADDINGY_DEFAULT		DCABLE_GRIDX_DEFAULT * 4

//Equivalence mm to program units 1mm=5 program unis
//200 program units = 40 mm
#define DCABLE_BORDERX_MARK			200
#define DCABLE_BORDERY_MARK			200

//Euivalence program units-U's. This relationship will be soon replace by a more convenient scale.
#define DCABLE_GRID_US				20	//20 program units = 1U

//Dots per mm
#define DCABLE_DPMM					5
#define INCH_TO_MM					25.4

//Paper sizes
#define A4_WIDTH_MM					297.0
#define A4_HEIGHT_MM				210.0
#define A3_WIDTH_MM					420.0
#define A3_HEIGHT_MM				297.0

//07/02/2005
//Enables OLE structured storage in application
#define DCABLE_COMPOUNDFILE	

//2014/09/20
//Enables shape filling code
#define DCABLE_SHAPE_FILL	
//Allways comment this for releasing (can produce unexpected outputs)
#define DCABLE_SHAPE_FILL_PROTOTYPE

extern CString g_sDCableBaseDir;
extern CString g_sConnectString;
//Global db conection to load units for example
extern CDatabase g_db;

//Global gdiplus Token: used for startup and shut down
extern ULONG_PTR g_gdiplusToken;

//Goblal current part library
extern CString g_sCurrentLibrary;

#define DCABLE_CABLEDIR		"Cables\\"
#define DCABLE_CONNECTORDIR	"Connector\\"

#define DCABLE_DATABASEDIR	_T("db\\")

#define DCABLE_RACKDIR	".\\Rack\\"

#define DCABLE_TB_TABLE_CONNECTOR		"tbConnector"
#define DCABLE_TB_TABLE_CONNECTOR_NAME	"nNameCon"
#define DCABLE_TB_TABLE_CABLE			"tbCable"
#define DCABLE_TB_TABLE_CABLE_NAME		"nNameCable"

//TYPEDEFS
//******************************************************************************
typedef std::map<int,CObject*> mapShapeIdtoObj_t;


typedef CArray<int,int> CIntArray;


//shape callback 1 function
//used for TB display Patch Panel Edit Dialog
typedef void (__cdecl *callback_shp1)(UINT nID); 

 
//******************************************************************************


extern int g_iX;
extern int g_iY;


extern CImageList m_ImageList;
extern int iHardDrv;
extern int iFolderClose;
extern int iMyComp;
extern int iFolderOpen;
extern int iSheet;
extern int iDB;
extern int iLibrary;
extern int iPart;
extern int iPartRack;

extern CImageList g_ImageListPatterns;

//public
BOOL LoadProfileSettings();
void AfxInitGlobal();
int AfxGetCables(LPTSTR	*lpCableList,int nCount,int nMaxLen);
int AfxGetCableCount();
CString AfxGetCablePath();
int AfxGetConnector(LPTSTR	*lpConnectorList,int nCount,int nMaxLen);
int AfxGetConnectorCount();
CString AfxGetConnectorPath();

int AfxGetConnectorPintCount(int iType);
int AfxGetCableWireCount(CString strTypeIdent);

BOOL AfxIsConnectorLogical(int iType);

int g_GetConnectString(LPTSTR lpConnStr,int nMaxLen);

void SnapToGrid(LPPOINT lpPoint, CSize szGrid);

//private
int CreateImagelist();
int CreateImagelistPatterns();

#endif