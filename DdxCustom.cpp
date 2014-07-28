#include "stdafx.h"

void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, CDWordArray& avalue)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	CString value;

	if (pDX->m_bSaveAndValidate)
	{
		int nLen = ::GetWindowTextLength(hWndCtrl);
		::GetWindowText(hWndCtrl, value.GetBufferSetLength(nLen), nLen+1);
		value.ReleaseBuffer();

		//all this loop should be replaced by a regular expression search engine
		std::string svalue(value);

		int idata1,idata2=0;
		int index=0;
		WORD first,last;
		bool bFlag;

		do{

			bFlag=false;

			do{
				idata1=svalue.find_first_of("0123456789",idata2);
				first=svalue[idata1]-'0';

				if(idata1==std::string::npos){
					break;
				}

				idata1=svalue.find_first_of("-",idata1);

				if(idata1==std::string::npos){
					break;
				}

				idata1=svalue.find_first_of("0123456789",idata1);
				last=svalue[idata1]-'0';

				if(idata1==std::string::npos){
					break;
				}

				if(idata1-idata2>2){
					break;
				}

				avalue.Add(MAKELONG(first,last));

				idata2+=3;

				bFlag=true;

			}while(0);

			do{
				idata1=svalue.find_first_of("0123456789",idata2);
				first=svalue[idata1]-'0';

				if(idata1==std::string::npos){
					break;
				}

				idata1=svalue.find_first_of(",",idata1);

				if(idata1==std::string::npos){
					break;
				}

				if(idata1-idata2>1){
					break;
				}

				avalue.Add(MAKELONG(first,first));

				idata2+=2;

				bFlag=true;

			}while(0);

		}while(bFlag);
	}
	else
	{
		//AfxSetWindowText(hWndCtrl, value);
	}

}