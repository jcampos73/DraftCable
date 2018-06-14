// PartEdit.cpp : implementation file
//

#include "stdafx.h"
#include "DraftDraw.h"
#include "PartEdit.h"

#include "ShapeUnit.h"

//dialogs
#include "CableEdit.h"
#include "DialogText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPartEdit dialog


CPartEdit::CPartEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CPartEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPartEdit)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_pShape=NULL;

	//Font initialization
	m_bVertical=FALSE;
	m_sFont=_T("Arial, 12");
	m_iSize=12;
}


void CPartEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPartEdit)
	DDX_Control(pDX, IDC_LIST1, m_lcRegister);
	DDX_Control(pDX, IDC_PROPERTY, m_cbProperty);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPartEdit, CDialog)
	//{{AFX_MSG_MAP(CPartEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON_EDITLABEL, OnButtonEditlabel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartEdit message handlers

BOOL CPartEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//Configures combo box control
	//-----------------------------------------------------------------

	m_cbProperty.AddString(_T("Connector"));
	m_cbProperty.AddString(_T("Card"));
	m_cbProperty.SelectString(-1,_T("Connector"));

	//-----------------------------------------------------------------

	//Configures list control
	//-----------------------------------------------------------------

#define _COLDEFWIDTH_PARTEDIT		100
#define _COLNUMBER_PARTEDIT			4

	//column width matrix
	int m_iColumn[_COLNUMBER_PARTEDIT];

	m_iColumn[0]=_COLDEFWIDTH_PARTEDIT;
	m_iColumn[1]=_COLDEFWIDTH_PARTEDIT;
	//m_iColumn[2]=_COLDEFWIDTH_PARTEDIT;
	//m_iColumn[3]=_COLDEFWIDTH_PARTEDIT;

	//Prepare columns
	m_lcRegister.InsertColumn(0,_T("ID"),LVCFMT_LEFT,m_iColumn[0],0);
	m_lcRegister.InsertColumn(1,_T("TIPO"),LVCFMT_LEFT,m_iColumn[1],1);
	//m_lcCable.InsertColumn(2,_T("FUNCTION"),LVCFMT_LEFT,m_iColumn[2],2);
	//m_lcCable.InsertColumn(3,_T("PIN"),LVCFMT_LEFT,m_iColumn[3],3);

	DWORD dwStyle = m_lcRegister.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	/*m_lcUser.ModifyStyle(LVS_ICON, LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);*/
	m_lcRegister.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);

	//-----------------------------------------------------------------

	//Load list control (register)
	//-----------------------------------------------------------------
	DoLoadListCtrl(_T(""), 0);
	//-----------------------------------------------------------------

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPartEdit::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int index=m_lcRegister.GetSelectionMark();

	if(index<0){
		return;
	}

	CString sIdent=m_lcRegister.GetItemText(index,0);
	CString sConnect=m_lcRegister.GetItemText(index,1);

	//Old code
	//sConnect+=_T(".txt");

	//Prepare cable/connector editor
	CCableEdit dialog(sConnect,TRUE);
	dialog.m_strName = sIdent;
	if(dialog.DoModal()==IDOK){
		int iType = dialog.m_iType;
		DoLoadListCtrl(sIdent, iType);
	}

	*pResult = 0;
}

void CPartEdit::OnButtonEditlabel() 
{
	UpdateData(TRUE);

	CDialogText dlgText;
	dlgText.m_bVertical=m_bVertical;
	dlgText.m_sFont=m_sFont;
	dlgText.m_iSize=m_iSize;
	dlgText.m_sAlias=m_strName;

	if(dlgText.DoModal()==IDOK){
		m_bVertical=dlgText.m_bVertical;
		m_sFont=dlgText.m_sFont;
		m_iSize=dlgText.m_iSize;
		m_strName=dlgText.m_sAlias;

		UpdateData(FALSE);
	}
}

void CPartEdit::SetItemText(int nIndex,CShapePin *pShPin = NULL, int nPin = -1)
{
	int nDefaultPin = 0;
	if(nPin >= 0)
	{
		if(pShPin != NULL){
			pShPin->m_TypePin = nPin;
		}
		m_lcRegister.SetItemText(nIndex,1,TABLE_PIN_LABEL[nPin]);
	}
	else if(pShPin != NULL &&
		pShPin->m_TypePin >= 0 && pShPin->m_TypePin < AfxGetConnectorCount()){
		TCHAR *pch=TABLE_PIN_LABEL[pShPin->m_TypePin];
		m_lcRegister.SetItemText(nIndex,1,TABLE_PIN_LABEL[pShPin->m_TypePin]);
	}
	else
	{
		m_lcRegister.SetItemText(nIndex,1,TABLE_PIN_LABEL[nDefaultPin]);
	}
}

void CPartEdit::DoLoadListCtrl(CString strIdent = _T(""), int nTypePin = 0)
{
	//Load list control (register)
	//-----------------------------------------------------------------
	//m_pShape contains shape under edition
	if((m_pShape)&&
		(m_pShape->IsKindOf(RUNTIME_CLASS(CShapeUnit)))	){
		CShapeUnit *pShUnit=(CShapeUnit *)m_pShape;
			int nIndex=0;
			for(int i=0;i<pShUnit->m_ShapeNumber;i++){
				CShape *pSh=(CShape *)pShUnit->m_obarrShapearr[i];
				//Process each pin
				if(pSh->IsKindOf(RUNTIME_CLASS(CShapePin))){
					CShapePin *pShPin=(CShapePin *)pSh;
					if(strIdent.GetLength() == 0){
						//Insert pin id
						m_lcRegister.InsertItem(nIndex,pShPin->m_strIdent);
						//Set item text (cable/connector type)
						SetItemText(nIndex, pShPin);
					}
					else if(strIdent == pShPin->m_strIdent){
						//Set item text (cable/connector type)
						SetItemText(nIndex, pShPin, nTypePin);
					}
					//Increment index
					nIndex++;
				}
			}
	}
	//-----------------------------------------------------------------
}
