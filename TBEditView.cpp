// TBEditView.cpp : implementation of the CTBEditView class
//

#include "stdafx.h"
#include "DraftDraw.h"

#include "DraftDrawDoc.h"
#include "TBEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTBEditView

IMPLEMENT_DYNCREATE(CTBEditView, CFormView)

BEGIN_MESSAGE_MAP(CTBEditView, CFormView)
	//{{AFX_MSG_MAP(CTBEditView)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTBEditView construction/destruction

CTBEditView::CTBEditView()
	: CFormView(CTBEditView::IDD)
{
	//{{AFX_DATA_INIT(CTBEditView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_bFlagIni=FALSE;
	m_bModifiedFlag=TRUE;
	m_nItemSel=1001;
	m_nID=-1;

	// Instantiate and initialize the background brush to black.
	m_pEditBkBrush = new CBrush(RGB(0, 128, 0));

}

CTBEditView::~CTBEditView()
{

	// Free the space allocated for the background brush
	delete m_pEditBkBrush;

}

void CTBEditView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTBEditView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CTBEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTBEditView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// Implement a GetDocSize( ) member function in 
	// your document class; it returns a CSize.
	//SetScrollSizes( MM_LOENGLISH, GetDocument( )->GetDocSize( ) );
	SetScrollSizes( MM_LOENGLISH, CSize(1000,1000) );


	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CTBEditView printing

BOOL CTBEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTBEditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTBEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTBEditView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CTBEditView diagnostics

#ifdef _DEBUG
void CTBEditView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTBEditView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG
CDraftDrawDoc* CTBEditView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDraftDrawDoc)));
	return (CDraftDrawDoc*)m_pDocument;
}
//#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTBEditView message handlers

void CTBEditView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	
	if((m_bFlagIni==FALSE)&&(m_nID>=0)){

		BOOL bFlagEnd=FALSE;


		m_nCountTotalMax1=0;
		m_nCountTotalMax2=0;


		int nIndex1=0;		//index to array of cables for left side
		int nIndex2=0;		//index to array of cables for right side

		int nIndexi=0;		//index of control rows
		int nIndexi1=0;
		int nIndexi2=0;

		int nCountLevel=1;	//level counter

		int y_coor=1;
		int x_coor='A';
		//int y_coor0;
		//int x_coor0;


		int y_offset=10;	//y offset for control position
		int	y_offset0=y_offset;

		int i;


		while(!bFlagEnd){

			CDraftDrawDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			CStringArray *pDocCables1=pDoc->m_obaCables1[m_nIndex];
			CStringArray *pDocCables2=pDoc->m_obaCables2[m_nIndex];

			char buffer[MAX_PATH];		//buffer to read text lines from file


			CString sPath;
			CString sName;

			int nCountWire;


			int x_offset;

			int x_width1=50;



			m_nCountMax1=-1;
			m_nCountMax2=-1;
			//Count number of wires in cable1
			if(nIndex1>=0){
				CString strCable1=pDocCables1[0][nIndex1];
			
				sPath=strCable1.Right(strCable1.GetLength()-strCable1.Find(";")-1);
				sName=strCable1.Left(strCable1.Find(";"));

				FILE *fin=fopen(g_sDCableBaseDir+"Cables\\"+sPath,"r");

				int i=0;
				while(fgets(buffer,MAX_PATH,fin)){

					i++;
				}
				m_nCountMax1=i-1;
m_nCountTotalMax1+=m_nCountMax1;

				fclose(fin);
			}

			//Count number of wires in cable2
			if(nIndex2>=0){
				CString strCable2=pDocCables2[0][nIndex2];
			
				sPath=strCable2.Right(strCable2.GetLength()-strCable2.Find(";")-1);
				sName=strCable2.Left(strCable2.Find(";"));

				FILE *fin=fopen(g_sDCableBaseDir+"Cables\\"+sPath,"r");

				int i=0;
				while(fgets(buffer,MAX_PATH,fin)){

					i++;
				}
				m_nCountMax2=i-1;
m_nCountTotalMax2+=m_nCountMax2;

				fclose(fin);
			}

			//Calcule max
			if(m_nCountMax1>m_nCountMax2){
				nCountWire=m_nCountMax1;
			}
			else{
				nCountWire=m_nCountMax2;
			}

			//Set scroll sizes
			//nCountTotal+=nCountWire;
			//SetScrollSizes( MM_LOENGLISH, CSize(1000,nCountWire*20) );


			int y_coor0=y_coor;
			int x_coor0=x_coor;

			if(nCountWire>=0){
			//******************************************************************

				CString strCable1=pDocCables1[0][nIndex1];
				

				sPath=strCable1.Right(strCable1.GetLength()-strCable1.Find(";")-1);
				sName=strCable1.Left(strCable1.Find(";"));

				FILE *fin=fopen(g_sDCableBaseDir+"Cables\\"+sPath,"r");

				//Creates controls



				
				int x_min=10;
				int x_width2=190,x_width3=50,x_width4=200;
				int x_gap=10;
				
				int i=0;
				for(i=0;i<nCountWire;i++){



					CComboBox *pCombo=new CComboBox();
					CComboBox *pCombo1=new CComboBox();
					CComboBox *pCombo2=new CComboBox();
					CEdit *pCombo3=new CEdit();
					CComboBox *pCombo4=new CComboBox();
					CComboBox *pCombo5=new CComboBox();
					CComboBox *pCombo6=new CComboBox();


					pCombo->Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, CRect(x_min,y_offset+i*30,x_min+x_width1,y_offset+190+i*30), this, nIndexi+1000+i );
					//int j=pCombo->AddString(sName);
					//pCombo->SetCurSel(j);

					x_offset=x_min+x_width1+x_gap;

					pCombo1->Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, CRect(x_offset,y_offset+i*30,x_offset+x_width2,y_offset+190+i*30), this, nIndexi+1100+i );
					//j=pCombo1->AddString(str.c_str());
					//pCombo1->SetCurSel(j);

					x_offset+=x_width2+x_gap;

					CString sStr;
					sStr.Format("%i%c",y_coor,x_coor);
					pCombo2->Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, CRect(x_offset,y_offset+i*30,x_offset+x_width3,y_offset+190+i*30), this, nIndexi+1200+i );
					//j=pCombo2->AddString(sStr);
					//pCombo2->SetCurSel(j);

					x_offset+=x_width3+x_gap;

					pCombo3->Create( WS_CHILD|WS_VISIBLE|WS_DLGFRAME, CRect(x_offset,y_offset+i*30,x_offset+x_width4,y_offset+25+i*30), this, nIndexi+1300+i );
					//j=pCombo3->AddString(str.c_str());
					//pCombo3->SetCurSel(j);


					x_offset+=x_width4+x_gap;

					pCombo4->Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, CRect(x_offset,y_offset+i*30,x_offset+x_width3,y_offset+190+i*30), this, nIndexi+1400+i );

					x_offset+=x_width3+x_gap;

					pCombo5->Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, CRect(x_offset,y_offset+i*30,x_offset+x_width2,y_offset+190+i*30), this, nIndexi+1500+i );

					x_offset+=x_width2+x_gap;

					pCombo6->Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, CRect(x_offset,y_offset+i*30,x_offset+x_width1,y_offset+190+i*30), this, nIndexi+1600+i );

					x_coor++;
					if(x_coor>'F'){

						CString str;
						str.Format("nivel%i",nCountLevel);
						nCountLevel++;

						CButton *p_btGroup=new CButton();
						CRect rect(0,y_offset+(i+1-6)*30-10,x_offset+x_width1,y_offset+(i+1-6)*30-10+30*6+10);
						p_btGroup->Create( str, WS_VISIBLE|BS_GROUPBOX , rect, this, 100+y_coor );

						y_offset0=y_offset;
						
						
						x_coor='A';
						y_coor++;



					}


					
				}
				//nCountWire=i;
				//m_nCountMax1=i;
				

				y_offset+=i*30;


				fclose(fin);


			}

			y_coor=y_coor0;
			x_coor=x_coor0;


			//Add first cable

			if(nIndex1>=0){

				FILE *fin=fopen(g_sDCableBaseDir+"Cables\\"+sPath,"r");


				
				int k=0;
				while(fgets(buffer,MAX_PATH,fin)){

					//pre-process string
					std::string str=buffer;

					while((str.find_first_of("\n\r"))!=std::string::npos){
						str.replace(str.find_first_of("\n\r"),1,"");
					}

					if(str.length()==0){
						continue;
					}


					//iterate controls
					for(int i=0;i<nCountWire;i++){

						//add wire description
						int j=GetDlgItem(nIndexi+1100+i)->SendMessage(CB_ADDSTRING,0,(LPARAM)str.c_str());

						//add patch panel cell
						CString sStr;
						sStr.Format("%i%c",y_coor,x_coor);

						j=GetDlgItem(nIndexi+1200+i)->SendMessage(CB_ADDSTRING,0,(LPARAM)std::string(sStr).c_str());

					}

					//add cable name
					int j=GetDlgItem(nIndexi+1000+k)->SendMessage(CB_ADDSTRING,0,(LPARAM)std::string(sName).c_str());

					//set cur sel in combos
					GetDlgItem(nIndexi+1000+k)->SendMessage(CB_SETCURSEL,j,0);
					GetDlgItem(nIndexi+1100+k)->SendMessage(CB_SETCURSEL,k,0);
					GetDlgItem(nIndexi+1200+k)->SendMessage(CB_SETCURSEL,k,0);




					x_coor++;
					if(x_coor>'F'){



						x_coor='A';
						y_coor++;


					}

					k++;
				}

		

				fclose(fin);

			//******************************************************************
			}//end if nIndex1>=0

			y_coor=y_coor0;
			x_coor=x_coor0;




			//Add second cable

			if(nIndex2>=0){
			//******************************************************************

				CString strCable2=pDocCables2[0][nIndex2];
			
				sPath=strCable2.Right(strCable2.GetLength()-strCable2.Find(";")-1);
				sName=strCable2.Left(strCable2.Find(";"));




				FILE *fin=fopen(g_sDCableBaseDir+"Cables\\"+sPath,"r");


				
				int k=0;
				while(fgets(buffer,MAX_PATH,fin)){

					std::string str=buffer;

					while((str.find_first_of("\n\r"))!=std::string::npos){
						str.replace(str.find_first_of("\n\r"),1,"");
					}

					if(str.length()==0){
						continue;
					}


					for(i=0;i<nCountWire;i++){

						

						int j=GetDlgItem(nIndexi+1500+i)->SendMessage(CB_ADDSTRING,0,(LPARAM)str.c_str());

						CString sStr;
						sStr.Format("%i%c",y_coor,x_coor);

						j=GetDlgItem(nIndexi+1400+i)->SendMessage(CB_ADDSTRING,0,(LPARAM)std::string(sStr).c_str());

					}
					int j=GetDlgItem(nIndexi+1600+k)->SendMessage(CB_ADDSTRING,0,(LPARAM)std::string(sName).c_str());
					GetDlgItem(nIndexi+1600+k)->SendMessage(CB_SETCURSEL,j,0);
					GetDlgItem(nIndexi+1500+k)->SendMessage(CB_SETCURSEL,k,0);

					

					GetDlgItem(nIndexi+1400+k)->SendMessage(CB_SETCURSEL,k,0);




					x_coor++;
					if(x_coor>'F'){


						x_coor='A';
						y_coor++;


					}

					k++;
				}

				nIndexi+=nCountWire;

				fclose(fin);

			//******************************************************************
			}//end if nIndex2>=0

			//Check end condition

			


			nIndex1++;
			if(nIndex1>=pDocCables1[0].GetSize()){

				nIndex1=-1;

			}

			nIndex2++;
			if(nIndex2>=pDocCables2[0].GetSize()){

				nIndex2=-1;

			}


			if((nIndex1<0)&&(nIndex2<0)){

				bFlagEnd=TRUE;
			}

			//bFlagEnd=TRUE;

		}//end while

		//Set scroll sizes
		SetScrollSizes( MM_LOENGLISH, CSize(1000,y_offset+190) );

		//Load initial values
		//**********************************************************************
		CDraftDrawDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		CObject *pObj=NULL;
		if(m_nID>0){
			pDoc->m_pmapShapeIdtoObj[0][m_nID];
		}
		if(pObj&&pObj->IsKindOf( RUNTIME_CLASS( CShapeContainer ) ) )
		{
			CShapeContainer *pShCont=(CShapeContainer *)pObj;

			int nCount=pShCont->m_iaParameter.GetSize();

			int i=0;

			int nRow=0;

			while(i<nCount){

				int j=1000;

				while((j<1300)&&(i<nCount)){
					int nIndex=pShCont->m_iaParameter[i];
					SendDlgItemMessage(j+nRow,CB_SETCURSEL,nIndex,0);
					j+=100;
					i++;
				}

				j=1400;

				while((j<1700)&&(i<nCount)){
					int nIndex=pShCont->m_iaParameter[i];
					SendDlgItemMessage(j+nRow,CB_SETCURSEL,nIndex,0);
					j+=100;
					i++;
				}


				nRow++;
			}


		}
		else if(!m_strID.IsEmpty()){

			void *ptr;
			CIntArray *piaParameter;

			if(pDoc->m_mapStrHWParameter.Lookup(m_strID,ptr)){

				piaParameter=(CIntArray *)ptr;

				int nCount=piaParameter->GetSize();

				int i=0;

				int nRow=0;

				while(i<nCount){

					int j=1000;

					while((j<1300)&&(i<nCount)){
						int nIndex=(*piaParameter)[i];
						SendDlgItemMessage(j+nRow,CB_SETCURSEL,nIndex,0);
						j+=100;
						i++;
					}

					j=1400;

					while((j<1700)&&(i<nCount)){
						int nIndex=(*piaParameter)[i];
						SendDlgItemMessage(j+nRow,CB_SETCURSEL,nIndex,0);
						j+=100;
						i++;
					}


					nRow++;
				}

			}
		}
		//**********************************************************************

		m_bFlagIni=TRUE;
	}
}

HBRUSH CTBEditView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  switch (nCtlColor) {
  case CTLCOLOR_EDIT:{
      // Set color to green on black and return the background brush.
	  POSITION pos=m_listItemSel.GetHeadPosition();
	  if(pos){
		  //do something
		  do{
			  WORD id=m_listItemSel.GetNext(pos);
			  //do something
			  if (pWnd->GetDlgCtrlID() == id){
				pDC->SetTextColor(RGB(0, 0, 0));
				pDC->SetBkColor(RGB(255, 0, 0));
				return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
			  }
		  }while(pos);
	  }
	  return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
/*
	  if (pWnd->GetDlgCtrlID() == m_nItemSel){
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(255, 0, 0));
		return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
	  }
	  else if((pWnd->GetDlgCtrlID() >= 1300)&&(pWnd->GetDlgCtrlID() < 1400)){
		pDC->SetBkColor(RGB(255, 0, 0));
		return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
	  }
	  else{
		return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	  }
*/
		}
  default:
      return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

  }
}


BOOL CTBEditView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	int i;

	switch(HIWORD(wParam)){
	case CBN_SELCHANGE:

		int j=SendDlgItemMessage(LOWORD(wParam),CB_GETCURSEL);

		for(i=0;i<m_nCountMax2;i++){

			if(((LOWORD(wParam)-1400)!=i)&&(j==SendDlgItemMessage(1400+i,CB_GETCURSEL)) ){

				m_nItemSel=LOWORD(wParam);

				m_listItemSel.AddTail(LOWORD(wParam));
			}
			else{
				
				POSITION pos=m_listItemSel.Find(LOWORD(wParam));
				if(pos)m_listItemSel.RemoveAt(pos);
			}
		}

		//1400
		break;


	}
	
	return CFormView::OnCommand(wParam, lParam);
}

void CTBEditView::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnClose();
}

BOOL CTBEditView::DestroyWindow() 
{
	CDraftDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//Check if TB has been modiefied
	if(m_bModifiedFlag){

		//Ask user if save
		int iResult=AfxMessageBox(IDS_MSG_PATCHPCHANGE,MB_YESNOCANCEL|MB_ICONQUESTION,-1);

		if(iResult==IDCANCEL){
			return FALSE;
		}
		else if(iResult==IDYES){

			CObject *pObj=NULL;
				
			if(m_nID>0){
				pObj=pDoc->m_pmapShapeIdtoObj[0][m_nID];
			}

			if(pObj&&pObj->IsKindOf( RUNTIME_CLASS( CShapeContainer ) ) )
			{
				CShapeContainer *pShCont=(CShapeContainer *)pObj;

				int nCount=(m_nCountTotalMax1>m_nCountTotalMax2 ? m_nCountTotalMax1:m_nCountTotalMax2);

				for(int i=0;i<nCount;i++){

					int nIndex=SendDlgItemMessage(1000+i,CB_GETCURSEL,0,0);
					pShCont->m_iaParameter.Add(nIndex);
					nIndex=SendDlgItemMessage(1100+i,CB_GETCURSEL,0,0);
					pShCont->m_iaParameter.Add(nIndex);
					nIndex=SendDlgItemMessage(1200+i,CB_GETCURSEL,0,0);
					pShCont->m_iaParameter.Add(nIndex);
					nIndex=SendDlgItemMessage(1400+i,CB_GETCURSEL,0,0);
					pShCont->m_iaParameter.Add(nIndex);
					nIndex=SendDlgItemMessage(1500+i,CB_GETCURSEL,0,0);
					pShCont->m_iaParameter.Add(nIndex);
					nIndex=SendDlgItemMessage(1600+i,CB_GETCURSEL,0,0);
					pShCont->m_iaParameter.Add(nIndex);

				}
			}
			else if(!m_strID.IsEmpty()){

				void *ptr;
				CIntArray *piaParameter;

				if(pDoc->m_mapStrHWParameter.Lookup(m_strID,ptr)){

					piaParameter=(CIntArray *)ptr;

					int nCount=(m_nCountTotalMax1>m_nCountTotalMax2 ? m_nCountTotalMax1:m_nCountTotalMax2);

					piaParameter->SetSize(nCount*6);
					for(int i=0;i<nCount;i++){

						int nIndex=SendDlgItemMessage(1000+i,CB_GETCURSEL,0,0);
						(*piaParameter)[6*i]=nIndex;
						nIndex=SendDlgItemMessage(1100+i,CB_GETCURSEL,0,0);
						(*piaParameter)[6*i+1]=nIndex;
						nIndex=SendDlgItemMessage(1200+i,CB_GETCURSEL,0,0);
						(*piaParameter)[6*i+2]=nIndex;
						nIndex=SendDlgItemMessage(1400+i,CB_GETCURSEL,0,0);
						(*piaParameter)[6*i+3]=nIndex;
						nIndex=SendDlgItemMessage(1500+i,CB_GETCURSEL,0,0);
						(*piaParameter)[6*i+4]=nIndex;
						nIndex=SendDlgItemMessage(1600+i,CB_GETCURSEL,0,0);
						(*piaParameter)[6*i+5]=nIndex;

					}
				}
			}

		}
	}


	//Delete hwnd of this editor from map

	std::map<int,HWND>::iterator it1=pDoc->m_mapIdHWView.find(m_nID);

	//asserts!
	//pDoc->m_mapIdHWView.erase(it1);
	//it1=pDoc->m_mapIdHWView.find(m_nID);

	if(it1!=pDoc->m_mapIdHWView.end()){
		it1->second=0;
	}

	if(!m_strID.IsEmpty()){
		pDoc->m_mapStrHWView[m_strID]=0;
	}

	return CFormView::DestroyWindow();
}
