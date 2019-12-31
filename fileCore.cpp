
#include "stdafx.h"
#include "filecore.h"

#include "ShapeUnit.h"


#define MAX_COMBO_STRINGNUM		10
#define TEST_COMBO_STRINGNUM	4

FILE *fini;
CArchive *g_parTheArch;
CStringArray sini;
int sini_index=0;

//To load parts from db
//CArchiveDb ardb(NULL,CArchive::load   );
//CDatabase db;
std::stringstream g_sout;
BOOL g_bFlagDb=FALSE;//Set to true if part is stored in database
BOOL g_bFlagDbEnable=TRUE;//Set to true if database storage is enabled


void FindString(CComboBox *pcombo, CString *psText){

	CString sres;

	pcombo->GetLBText(0,sres);//default



	CString stext1,stext2;

	stext2=*psText;

	stext2=stext2.Left( stext2.Find('.') );

	int index_res=-1;
	int max_res=0;
	for(int i=0;i<pcombo->GetCount();i++){
	

		pcombo->GetLBText(i,stext1);

		stext1=stext1.Left( stext1.Find('.') );

		//stext1.MakeLower();
		

		if(stext1.GetLength()>stext2.GetLength()){

			stext1=stext1.Left(stext2.GetLength());
		}
		else{

			stext2=stext2.Left(stext1.GetLength());
		}

		if((stext1.CompareNoCase(stext2)==0)&&
			(stext1.GetLength()>max_res)){

			index_res=i;
			max_res=stext1.GetLength();
			pcombo->GetLBText(i,sres);

		}


	}

	pcombo->SelectString(0,sres);


}

void AddString(CComboBox *pcombo, CComboBox *pcombo1, CString *psText)
{
	

	CString stext;


	if(psText){

		stext=*psText;

	}
	else{

		pcombo->GetWindowText(stext);
	}

	int i;
	for(i=0;i<pcombo->GetCount();i++){

		CString stlabel;

		pcombo->GetLBText(i,stlabel);

		if(stlabel.Compare(stext)==0) break;

	}

	if(i==pcombo->GetCount()){
		if(pcombo->GetCount()>=MAX_COMBO_STRINGNUM){

			pcombo->DeleteString(pcombo->GetCount()-1-TEST_COMBO_STRINGNUM);
		}

		pcombo->InsertString(0,stext);
	}

	if(pcombo1!=NULL){

		AddString(pcombo1,NULL,&stext);
	}

}

void SPrintF(CStringArray *psArr,CString stlabel, CComboBox *pcombo)
{

	CString stlabel1;
	int index=stlabel.Find(']');
	stlabel=stlabel.Left(index);

	stlabel1=stlabel+" data]\n";
	//fwprintf(fini,"%s\n",stlabel1);
	psArr->Add(stlabel1);

	for(int i=0;i<pcombo->GetCount();i++){


		pcombo->GetLBText(i,stlabel1);
		//fwprintf(fini,"%s\n",stlabel1/*.GetBuffer(0)*/);
		stlabel1+="\n";
		psArr->Add(stlabel1);

	}

	stlabel1=stlabel+" select]\n";
	//fwprintf(fini,"%s\n",stlabel1);
	psArr->Add(stlabel1);

	pcombo->GetWindowText(stlabel1);
	//fwprintf(fini,"%s\n",stlabel1/*.GetBuffer(0)*/);
	stlabel1+="\n";
	psArr->Add(stlabel1);

}


void fwprintf(CString stlabel, CComboBox *pcombo)
{
	CString stlabel1;
	int index=stlabel.Find(']');
	stlabel=stlabel.Left(index);

	stlabel1=stlabel+" data]";
	fwprintf(fini,_T("%s\n"),stlabel1);

	for(int i=0;i<pcombo->GetCount();i++){

		pcombo->GetLBText(i,stlabel1);
		fwprintf(fini, _T("%s\n"), stlabel1/*.GetBuffer(0)*/);
	}

	stlabel1=stlabel+" select]";
	fwprintf(fini, _T("%s\n"), stlabel1);

	pcombo->GetWindowText(stlabel1);
	fwprintf(fini, _T("%s\n"), stlabel1/*.GetBuffer(0)*/);

}

//void AFXAPI DDX_LBString( CDataExchange* pDX, int nIDC, CString& value );

BOOL DDXF_Pin(BOOL bStore,TCHAR *buffer,CShapePin **shPin){
	//locals
	TCHAR *pchar;
	BOOL result=FALSE;

	if(bStore==FALSE){
		if((buffer[0]=='[')&&(wcscmp(buffer,_T("[pin]")))){
			return result;
		}

		DD_GETS(buffer);

		//declarations
		CString str(buffer),s1,s2;
		//*shPin=new CShapePin;//CShapePin(UINT uiPinnumber,UINT uiPos,DWORD dwStyle)

		s1=CShapeUnit::CutString(str);
		s2=CShapeUnit::CutString(str);

		CRect rect=CRect(CPoint(_wtoi(s1),_wtoi(s2)),CSize(1,1));
		s1=CShapeUnit::CutString(str);
		UINT uiPinnumber=_wtoi(s1);
		s1=CShapeUnit::CutString(str);
		UINT uiPos=_wtoi(s1);
		s1=CShapeUnit::CutString(str);
		DWORD dwStyle=_wtoi(s1);

		*shPin=new CShapePin(uiPinnumber,uiPos,dwStyle);
		(*shPin)->m_Rect=rect;

		s1=CShapeUnit::CutString(str);
		(*shPin)->m_TypePin=_wtoi(s1);

		result=TRUE;
	}

	return result;
}

BOOL DDXF_Label(BOOL bStore,TCHAR *buffer,CShapeLabel **shLabel){
	//locals
	TCHAR *pchar;
	BOOL result=FALSE;

	if(bStore==FALSE){
		if((buffer[0]=='[')&&(wcscmp(buffer,_T("[label]")))){
			return result;
		}

		DD_GETS(buffer);

		//declarations
		CString str(buffer),s1,s2;
		POINT *points=new POINT[2];
		*shLabel=new CShapeLabel;

		//read label rectamgle
		for(int i=0;i<2;i++){

			s1=CShapeUnit::CutString(str);
			s2=CShapeUnit::CutString(str);
			CShapeUnit::CutString(str);
			CShapeUnit::CutString(str);
			/*CShapeUnit::CutString(str);
			CShapeUnit::CutString(str);*/

			points[i].x=_wtoi(s1);
			points[i].y=_wtoi(s2);
		}

		shLabel[0]->m_Label.rect=new CRect(points[0],points[0]);//points[1]);
		shLabel[0]->m_Rect=CRect(points[0],points[0]);

		//read label text
		DD_GETS(buffer);
		str=CString(buffer);
		shLabel[0]->m_Label.slabel=new CString(CShapeUnit::CutString(str));
		shLabel[0]->m_Label.sname=new CString(CShapeUnit::CutString(str));
		//read label flag vertical
		DD_GETS(buffer);
		str=CString(buffer);
		if(CShapeUnit::CutString(str)/*CString(buffer)*/==_T("0")){
			shLabel[0]->m_Label.bver=FALSE;
		}
		else{
			shLabel[0]->m_Label.bver=TRUE;
		}
		int idata=_wtoi(CShapeUnit::CutString(str).GetBuffer(255));
		shLabel[0]->m_Label.iSize=idata;


		LOGFONT lf;
		ZeroMemory(&lf, sizeof(LOGFONT));

		if(shLabel[0]->m_Label.iSize>0){
			lf.lfHeight=-shLabel[0]->m_Label.iSize-3;
		}
		else{
			lf.lfHeight=-13;
		}
		lf.lfWeight=400;
		lf.lfOutPrecision=3;
		lf.lfClipPrecision=2;
		lf.lfQuality=1;
		lf.lfPitchAndFamily=34;
		wcscpy(lf.lfFaceName,_T("Arial"));

		shLabel[0]->m_Label.font=new CFont();
		shLabel[0]->m_Label.font->CreateFontIndirect(&lf);


		result=TRUE;
	}
	return result;
}

BOOL DDXF_FrmRect(BOOL bStore,TCHAR *buffer,CShapeFrmRect **shFrmRect)
{
	//locals
	TCHAR *pchar;
	BOOL result=FALSE;

	if(bStore==FALSE){

		if((buffer[0]=='[')&&(wcscmp(buffer,_T("[rectangle]")))){

			return result;
		}

		/*while(*/DD_GETS(buffer);//fgetws(buffer,255,fini);/*!=NULL){*/
		pchar=wcschr(buffer,'\n');
		if(!pchar){

			pchar=&buffer[wcslen(buffer)];
		}
		*pchar=0;

		if(buffer[0]=='[') return FALSE;/*break;*/

		//declarations
		CString str(buffer),s1,s2;

		/*
		s1=CShapeUnit::CutString(str);
		int count=_wtoi(s1);
		*/

		POINT *points=new POINT[2/*count*/];

		/*
		if(shPolyline==NULL){
		*/

			*shFrmRect=new CShapeFrmRect;
		/*
		}
		*/

		for(int i=0;i<2/*count*/;i++){

			s1=CShapeUnit::CutString(str);
			s2=CShapeUnit::CutString(str);

			points[i].x=_wtoi(s1);
			points[i].y=_wtoi(s2);
		}

		shFrmRect[0]->m_Rect=CRect(points[0],points[1]);

		result=TRUE;

		/*}*/

	}

	return result;

}

BOOL DDXF_Ellipse(BOOL bStore,TCHAR *buffer,CShapeEllipse **shEllipse)
{
	//locals
	TCHAR *pchar;
	BOOL result=FALSE;

	if(bStore==FALSE){

		if((buffer[0]=='[')&&(wcscmp(buffer,_T("[ellipse]")))){

			return result;
		}

		/*while(*/DD_GETS(buffer);//fgetws(buffer,255,fini);/*!=NULL){*/
		pchar=wcschr(buffer,'\n');
		if(!pchar){

			pchar=&buffer[wcslen(buffer)];
		}
		*pchar=0;

		if(buffer[0]=='[') return FALSE;/*break;*/

		//declarations
		CString str(buffer),s1,s2;


		/*
		s1=CShapeUnit::CutString(str);
		int count=_wtoi(s1);
		*/

		POINT *points=new POINT[2/*count*/];

		/*
		if(shPolyline==NULL){
		*/

			*shEllipse=new CShapeEllipse;
		/*
		}
		*/

		for(int i=0;i<2/*count*/;i++){

			s1=CShapeUnit::CutString(str);
			s2=CShapeUnit::CutString(str);

			points[i].x=_wtoi(s1);
			points[i].y=_wtoi(s2);
		}

		shEllipse[0]->m_Rect=CRect(points[0],points[1]);

		s1=CShapeUnit::CutString(str);
		if(!s1.IsEmpty()){
			shEllipse[0]->m_bTransparent=FALSE;
			TCHAR   *stopstring;
			int idata=wcstol(s1,&stopstring,16);
			shEllipse[0]->m_crFill=idata;
		}

		result=TRUE;

		/*}*/


	}

	return result;

}

BOOL DDXF_Polyline(BOOL bStore,TCHAR *buffer,CShape **shShape)
{

	TCHAR *pchar;

	BOOL result=FALSE;

	if(bStore==FALSE){


		if((buffer[0]=='[')&&(wcscmp(buffer, _T("[polyline]")))){

			return result;
		}

		///*while(*/fgetws(buffer,255,fini);/*!=NULL){*/
		//pchar=wcschr(buffer,'\n');
		//if(!pchar){

		//	pchar=&buffer[wcslen(buffer)];
		//}
		//*pchar=0;

		//if(buffer[0]=='[') return FALSE;/*break;*/
DD_GETS(buffer);

		//declarations
		CString str(buffer),s1,s2;


		s1=CShapeUnit::CutString(str);
		int count=_wtoi(s1);

		if(count<=2){

			POINT *points=new POINT[count];
			*shShape=new CShapeLine;
			for(int i=0;i<count;i++){

				s1=CShapeUnit::CutString(str);
				s2=CShapeUnit::CutString(str);

				points[i].x=_wtoi(s1);
				points[i].y=_wtoi(s2);
			}

			*shShape[0]->m_Rect=CRect(points[0],points[1]);

			result=TRUE;
		}
		else{

			POINT *points=new POINT[count];

			/*
			if(shPolyline==NULL){
			*/

				CShapePolyline *shPolyline=new CShapePolyline;
			/*
			}
			*/

			for(int i=0;i<count;i++){

				s1=CShapeUnit::CutString(str);
				s2=CShapeUnit::CutString(str);

				points[i].x=_wtoi(s1);
				points[i].y=_wtoi(s2);
			}

			shPolyline->Create(points,count);

			*shShape=(CShape *)shPolyline;

			result=TRUE;

			/*}*/
		}

	}

	return result;

}

BOOL DDXF_Spare(BOOL bStore,TCHAR *buffer)
{

	TCHAR *pchar;

	BOOL result=FALSE;


	if(bStore==FALSE){


		if(buffer[0]=='['){

			TCHAR *cresult;
			cresult=fgetws(buffer,255,fini);

			while(cresult!=NULL){
				pchar=wcschr(buffer,'\n');
				*pchar=0;

				if(buffer[0]=='[') break;

				cresult=fgetws(buffer,255,fini);
			}

			if(!cresult){

				buffer[0]=0;
			}

			result=TRUE;

		}


	}

	return result;

}

BOOL DDXS_Spare(BOOL bStore,TCHAR *buffer)
{
	TCHAR *pchar;

	BOOL result=FALSE;

	if(bStore==FALSE){

		if(buffer[0]=='['){

			if(sini_index<sini.GetSize()){

				wcscpy(buffer,sini.GetAt(sini_index));
			}
			else{

				buffer[0]=0;
			}
			sini_index++;

			while(wcslen(buffer)!=0){
				pchar=wcschr(buffer,'\n');
				*pchar=0;

				if(buffer[0]=='[') break;

				if(sini_index<sini.GetSize()){

					wcscpy(buffer,sini.GetAt(sini_index));
				}
				else{

					buffer[0]=0;
				}
				sini_index++;

			}

			result=TRUE;

		}


	}

	return result;

}

BOOL DDXF_ComboBox(BOOL bStore,TCHAR *buffer,CString sLabel, CComboBox *pComboBox)
{
	TCHAR *pchar;

	BOOL result=FALSE;

	if(bStore==FALSE){

		CString slabel=sLabel.Left(sLabel.Find(']'));
		CString slabel1=slabel+" data]";
		CString slabel2=slabel+" select]";

		if(wcscmp(buffer,slabel1)==0){

			while (fgetws(buffer, 255, fini) != NULL){
				pchar=wcschr(buffer,'\n');
				*pchar=0;

				if(buffer[0]=='[') break;

				pComboBox->InsertString(0,buffer);

			}

			result=TRUE;

		}

		if(wcscmp(buffer,slabel2)==0){

			while (fgetws(buffer, 255, fini) != NULL){
				pchar=wcschr(buffer,'\n');
				*pchar=0;

				if(buffer[0]=='[') break;

				pComboBox->SelectString(0,buffer);

			}

			result=TRUE;

		}

	}

	return result;
}

BOOL DDXS_ComboBox(BOOL bStore,TCHAR *buffer,CString sLabel, CComboBox *pComboBox)
{
	TCHAR *pchar;

	BOOL result=FALSE;

	if(bStore==FALSE){

		CString slabel=sLabel.Left(sLabel.Find(']'));
		CString slabel1=slabel+" data]";
		CString slabel2=slabel+" select]";

		if(wcscmp(buffer,slabel1)==0){

			if(sini_index<sini.GetSize()){

				wcscpy(buffer,sini.GetAt(sini_index));
			}
			else{

				buffer[0]=0;
			}
			sini_index++;

			while(wcslen(buffer)!=0){
				pchar=wcschr(buffer,'\n');
				*pchar=0;

				if(buffer[0]=='[') break;

				pComboBox->InsertString(0,buffer);

				if(sini_index<sini.GetSize()){

					wcscpy(buffer,sini.GetAt(sini_index));
				}
				else{

					buffer[0]=0;
				}
				sini_index++;


			}

			result=TRUE;

		}

		if(wcscmp(buffer,slabel2)==0){

			if(sini_index<sini.GetSize()){

				wcscpy(buffer,sini.GetAt(sini_index));
			}
			else{

				buffer[0]=0;
			}
			sini_index++;

			while(wcslen(buffer)!=0){
				pchar=wcschr(buffer,'\n');
				*pchar=0;

				if(buffer[0]=='[') break;

				pComboBox->SelectString(0,buffer);

				if(sini_index<sini.GetSize()){

					wcscpy(buffer,sini.GetAt(sini_index));
				}
				else{

					buffer[0]=0;
				}
				sini_index++;


			}

			result=TRUE;

		}

	}

	return result;
}

BOOL DDXF_Unit(BOOL bStore,TCHAR *buffer,LPCTSTR lpszNamePrev,CShapeUnit **shUnit)
{
	//locals
	TCHAR *pchar;
	BOOL result=FALSE;

	if(bStore==FALSE){

		if((buffer[0]=='[')&&(buffer[1]=='*')){

			return result;
		}
		if(wcscmp(buffer,_T("[label]"))==0){

			return result;
		}

		CString sName;

		if(buffer[0]=='['){
			sName=buffer;
			sName.Remove('[');
			sName.Remove(']');
			DD_GETS(buffer);
		}
		else{
			sName=lpszNamePrev;
		}

		//declarations
		CString str(buffer),s1,s2;

		/*
		s1=CShapeUnit::CutString(str);
		int count=_wtoi(s1);
		*/

		POINT *points=new POINT[2/*count*/];

		/*
		if(shPolyline==NULL){
		*/

			*shUnit=new CShapeUnit;
		/*
		}
		*/

		for(int i=0;i<2/*count*/;i++){

			s1=CShapeUnit::CutString(str);
			s2=CShapeUnit::CutString(str);

			points[i].x=_wtoi(s1);
			points[i].y=_wtoi(s2);
		}

		CObject *pobj=shUnit[0]->LoadUnit(sName);

		shUnit[0]->m_Rect=CRect(points[0],points[1]);

		while(1){

			DD_GETS1(buffer,TRUE);

		}

		result=TRUE;
	}

	return result;

}

BOOL DDXF_Wire(BOOL bStore,TCHAR *buffer,CShapeWire **shWire)
{
	//locals
	TCHAR *pchar;
	BOOL result=FALSE;

	if(bStore==FALSE){


		if((buffer[0]=='[')&&(wcscmp(buffer,_T("[*wire]")))){

			return result;
		}

		DD_GETS(buffer);

		//declarations
		CString str(buffer),s1,s2;

		POINT *points=new POINT[2/*count*/];

		/*
		if(shPolyline==NULL){
		*/

			*shWire=new CShapeWire;
		/*
		}
		*/

		for(int i=0;i<2/*count*/;i++){

			s1=CShapeUnit::CutString(str);
			s2=CShapeUnit::CutString(str);

			points[i].x=_wtoi(s1);
			points[i].y=_wtoi(s2);
		}

		shWire[0]->m_Rect=CRect(points[0],points[1]);

		if(str.GetLength()>0){
			s1=CShapeUnit::CutString(str);
			shWire[0]->m_strIdent=s1;
		}

		if(str.GetLength()>0){
			s1=CShapeUnit::CutString(str);
			shWire[0]->m_strTypeIdent=s1;
		}


		result=TRUE;
	}

	return result;

}

BOOL DDXF_Bus(BOOL bStore,TCHAR *buffer,CShapeBus **shWire)
{
	//locals
	TCHAR *pchar;
	BOOL result=FALSE;

	if(bStore==FALSE){

		if((buffer[0]=='[')&&(wcscmp(buffer,_T("[*bus]")))){

			return result;
		}

		DD_GETS(buffer);

		//declarations
		CString str(buffer),s1,s2;

		/*
		s1=CShapeUnit::CutString(str);
		int count=_wtoi(s1);
		*/

		s1=CShapeUnit::CutString(str);
		int count=_wtoi(s1);

		POINT *points=new POINT[count];

		/*
		if(shPolyline==NULL){
		*/

			CShapeBus *shPolyline=new CShapeBus;
		/*
		}
		*/

		for(int i=0;i<count;i++){

			s1=CShapeUnit::CutString(str);
			s2=CShapeUnit::CutString(str);

			points[i].x=_wtoi(s1);
			points[i].y=_wtoi(s2);
		}

		shPolyline->Create(points,count);

		*shWire=shPolyline;

		result=TRUE;

		/*}*/

		if(str.GetLength()>0){
			s1=CShapeUnit::CutString(str);
			shWire[0]->m_strIdent=s1;
		}

		if(str.GetLength()>0){
			s1=CShapeUnit::CutString(str);
			shWire[0]->m_strTypeIdent=s1;
		}


		result=TRUE;

		/*}*/


	}

	return result;

}

BOOL DDXF_Connect(BOOL bStore,TCHAR *buffer,mapShapeIdtoObj_t *pMapShapeIdtoObj)
{
	//locals
	TCHAR *pchar;
	BOOL result=FALSE;

	if(bStore==FALSE){

		if((buffer[0]=='[')&&(wcscmp(buffer,_T("[*connection]")))){

			return result;
		}

		DD_GETS(buffer);

		//declarations
		CString str(buffer),s1,s2;

		int ish1,ishchild1,ish2,ishchild2;

		s1=CShapeUnit::CutString(str);
		ish1=_wtoi(s1);
		s1=CShapeUnit::CutString(str);
		ishchild1=_wtoi(s1);
		s1=CShapeUnit::CutString(str);
		ish2=_wtoi(s1);
		s1=CShapeUnit::CutString(str);
		ishchild2=_wtoi(s1);

		mapShapeIdtoObj_t::iterator it1=pMapShapeIdtoObj->find(ish1);

		CObject *pSh1=it1->second;

		//Check if first shape is of type container
		if(pSh1->IsKindOf(RUNTIME_CLASS(CShapeContainer))){


			mapShapeIdtoObj_t::iterator it2=pMapShapeIdtoObj->find(ish2);

			CObject *pSh2=it2->second;

			//Check if second shape is of type container
			if(pSh2->IsKindOf(RUNTIME_CLASS(CShapeContainer))){


				CShape *pShChild1=(CShape *)(((CShapeContainer *)pSh1)->m_obarrShapearr[ishchild1-1]);
				CShape *pShChild2=(CShape *)(((CShapeContainer *)pSh2)->m_obarrShapearr[ishchild2-1]);

				//Check if first pin is of type container
				if(pShChild1->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

					((CShapeContainer *)pSh1)->m_pshConn=(CShape *)pSh2;
					((CShapeContainer *)pSh1)->m_pshChildConn=pShChild2;
					((CShapeContainer *)pSh1)->SetConnection(NULL,ishchild1-1);

				}

			}
		}
		


		//shWire[0]->m_Rect=CRect(points[0],points[1]);

		result=TRUE;

		/*}*/


	}

	return result;

}

BOOL DDXF_Sheet(BOOL bStore,TCHAR *buffer)
{
	//locals
	TCHAR *pchar;
	BOOL result=FALSE;

	if(bStore==FALSE){

		if((buffer[0]=='[')&&(wcscmp(buffer,_T("[*SHEET]")))){

			return result;
		}

		DD_GETS1(buffer,TRUE);

	}

	return result;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

int savebmp(HBITMAP hbm,HPALETTE hpal,const wchar_t *lpszCodec,LPCTSTR lpszPath)
{


   // Initialize GDI+.
   //GdiplusStartupInput gdiplusStartupInput;
   //ULONG_PTR gdiplusToken;
   //GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

   CLSID   encoderClsid;
   Status  stat;
   //Image*   image = new Image(L"sample.png");
   Bitmap*   image = new Bitmap(hbm,hpal);

   // Get the CLSID of the PNG encoder.
   //GetEncoderClsid(L"image/gif", &encoderClsid);
   GetEncoderClsid(lpszCodec, &encoderClsid);

   #ifndef UNICODE 

		wchar_t wszPath[MAX_PATH+1];
		char szPath[MAX_PATH+1];

		mbstowcs( wszPath, lpszPath, sizeof(wszPath) );

		wcstombs(szPath, wszPath, sizeof(szPath));

   #endif

	stat = image->Save(lpszPath, &encoderClsid, NULL);

   if(stat == Ok)
      printf("Bird.png was saved successfully\n");
   else
      printf("Failure: stat = %d\n", stat); 

   delete image;
   //GdiplusShutdown(gdiplusToken);
   return 0;
}
