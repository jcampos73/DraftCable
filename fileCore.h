#ifndef _FILECORE
#define _FILECORE

#include <io.h>
#include <sstream>

#include "Shape.h"
#include "ShapeUnit.h"

extern FILE *fini;
extern CArchive *g_parTheArch;
extern CStringArray sini;
extern int sini_index;

//To load parts from db
//CArchiveDb ardb(NULL,CArchive::load   );
//CDatabase db;
extern std::stringstream g_sout;
extern BOOL g_bFlagDb;//Set to true if part is stored in database
extern BOOL g_bFlagDbEnable;//Set to true if database storage is enabled
/*
#define DD_GETS(buffer){\
		if(!fgets(buffer,255,fini)){\
			buffer[0]=0;\
			return FALSE;\
		}\
		pchar=strchr(buffer,'\n');\
		if(!pchar){\
			pchar=&buffer[strlen(buffer)];\
		}\
		*pchar=0;\
		if(buffer[0]=='[') return FALSE;\
}
*/

//20/12/2004
//Macro to be able to read either from a database or file

#define DD_GETS(buffer){\
		if(g_bFlagDb&&g_bFlagDbEnable){\
			CString stdline;\
			g_parTheArch->ReadString(stdline);\
			strncpy(buffer,stdline,255);\
			buffer[254]=0;\
			if(stdline.GetLength()==0) return FALSE;\
		}\
		else if(!fgets(buffer,255,fini)){\
			buffer[0]=0;\
			return FALSE;\
		}\
		pchar=strchr(buffer,'\n');\
		if(!pchar){\
			pchar=&buffer[strlen(buffer)];\
		}\
		*pchar=0;\
		if(buffer[0]=='[') return FALSE;\
}

/*
//Backup
#define DD_GETS(buffer){\
		if(!fgets(buffer,255,fini)){\
			buffer[0]=0;\
			return FALSE;\
		}\
		pchar=strchr(buffer,'\n');\
		if(!pchar){\
			pchar=&buffer[strlen(buffer)];\
		}\
		*pchar=0;\
		if(buffer[0]=='[') return FALSE;\
}
*/
//20/10/2004
//This macro it is ony used in DDXF_Unit.
//As it causes not loading of repeated unis from project files,
//last if was commented.
#define DD_GETS1(buffer,retval){\
		if(!fgets(buffer,255,fini)){\
			buffer[0]=0;\
			return retval;\
		}\
		pchar=strchr(buffer,'\n');\
		if(!pchar){\
			pchar=&buffer[strlen(buffer)];\
		}\
		*pchar=0;\
		/*if(buffer[0]=='[') */return retval;\
}

void FindString(CComboBox *pcombo, CString *psText);

void AddString(CComboBox *pcombo, CComboBox *pcombo1, CString *psText=NULL);


void FPrintF(CString stlabel, CComboBox *pcombo);
void SPrintF(CStringArray *psArr,CString stlabel, CComboBox *pcombo);

BOOL DDXF_Spare(BOOL bStore,char *buffer);
BOOL DDXS_Spare(BOOL bStore,char *buffer);

BOOL DDXF_ComboBox(BOOL bStore,char *buffer,CString sLabel, CComboBox *pComboBox);
BOOL DDXS_ComboBox(BOOL bStore,char *buffer,CString sLabel, CComboBox *pComboBox);

BOOL DDXF_FrmRect(BOOL bStore,char *buffer,CShapeFrmRect **shFrmRect);
BOOL DDXF_Ellipse(BOOL bStore,char *buffer,CShapeEllipse **shEllipse);
BOOL DDXF_Polyline(BOOL bStore,char *buffer,CShape **shPolyline);
BOOL DDXF_Label(BOOL bStore,char *buffer,CShapeLabel **shLabel);
BOOL DDXF_Pin(BOOL bStore,char *buffer,CShapePin **shPin);
BOOL DDXF_Unit(BOOL bStore,char *buffer,LPCTSTR lpszNamePrev,CShapeUnit **shUnit);
BOOL DDXF_Wire(BOOL bStore,char *buffer,CShapeWire **shWire);
BOOL DDXF_Bus(BOOL bStore,char *buffer,CShapeBus **shWire);
BOOL DDXF_Connect(BOOL bStore,char *buffer,mapShapeIdtoObj_t *pMapShapeIdtoObj);
BOOL DDXF_Sheet(BOOL bStore,char *buffer);

//GDI PLUS FUNCTIONS
//Save a bitmap using a instaled code
int savebmp(HBITMAP hbm,HPALETTE hpal,const wchar_t *lpszCodec,LPCTSTR lpszPath);
//Auxiliar function
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

#endif