//COM Core

#include "stdafx.h"
#include "COMItfCore.h"


//14/12/2004
//Functions obtained from
// Office Developer Documentation\...Office 97...\Microsoft Excel 97 Developer's Kit\Using the OLE 2 IDispatch Interface
//All this code should be in a file called "Dispargs.c"
//Other resources:
//Platform SDK: Atomation "Accessing Members Through IDispatch"

int            g_iArgCount;
int            g_iNamedArgCount;
VARIANTARG    g_aVargs[MAX_DISP_ARGS];
DISPID        g_aDispIds[MAX_DISP_ARGS + 1];    
LPOLESTR        g_alpszArgNames[MAX_DISP_ARGS + 1];
WORD            g_awFlags[MAX_DISP_ARGS];

HWND g_hwndApp;
TCHAR g_szAppTitle[1024];



void ClearVariant(VARIANTARG *pvarg)
{
    pvarg->vt = VT_EMPTY;
    pvarg->wReserved1 = 0;
    pvarg->wReserved2 = 0;
    pvarg->wReserved3 = 0;
    pvarg->lVal = 0;
}

void ClearAllArgs()
{
    int i;




    for (i = 0; i < g_iArgCount; i++) 
    {
        if (g_awFlags[i] & DISPARG_NOFREEVARIANT)
            // free the variant's contents based on type
            ClearVariant(&g_aVargs[i]);
        else
            ReleaseVariant(&g_aVargs[i]);
    }

    g_iArgCount = 0;
    g_iNamedArgCount = 0;
}

void ReleaseVariant(VARIANTARG *pvarg)
{
    VARTYPE vt;
    VARIANTARG HUGEP *pvargArray;
    long lLBound, lUBound, l;

    vt = pvarg->vt & 0xfff;        // mask off flags

    // check if an array.  If so, free its contents, 
    // then the array itself.
    if (V_ISARRAY(pvarg)) 
    {
        // variant arrays are all this routine currently knows about.
        // Since a variant can contain anything (even other arrays), 
        // call ourselves recursively.
        if (vt == VT_VARIANT) 
        {
            SafeArrayGetLBound(pvarg->parray, 1, &lLBound);
            SafeArrayGetUBound(pvarg->parray, 1, &lUBound);

            if (lUBound > lLBound) 
            {
                lUBound -= lLBound;

                SafeArrayAccessData(pvarg->parray,(void **) &pvargArray);




                for (l = 0; l < lUBound; l++) 
                {
                    ReleaseVariant(pvargArray);
                    pvargArray++;
                }

                SafeArrayUnaccessData(pvarg->parray);
            }
        }
        else 
        {
            MessageBox(g_hwndApp, _T("ReleaseVariant: Array contains non-variant type"), g_szAppTitle, MB_OK | MB_ICONSTOP);
        }

        // Free the array itself.
        SafeArrayDestroy(pvarg->parray);
    }
    else 
    {
        switch (vt) 
        {
            case VT_DISPATCH:
				//14/12/2004
				//!!!all comments had to be added
                (/***/(pvarg->pdispVal/*->lpVtbl*/->Release))(/*pvarg->pdispVal*/);
                break;

            case VT_BSTR:
                SysFreeString(pvarg->bstrVal);
                break;

            case VT_I2:
            case VT_BOOL:
            case VT_R8:
            case VT_ERROR:        // to avoid erroring on an error return 
                                // from Excel
                // no work for these types
                break;

            default:
                MessageBox(g_hwndApp, _T("ReleaseVariant: Unknown type"),g_szAppTitle, MB_OK | MB_ICONSTOP);
                break;
        }
    }

    ClearVariant(pvarg);
}

void AddArgumentCommon(LPOLESTR lpszArgName, WORD wFlags, VARTYPE vt)
{
    ClearVariant(&g_aVargs[g_iArgCount]);

    g_aVargs[g_iArgCount].vt = vt;
    g_awFlags[g_iArgCount] = wFlags;

    if (lpszArgName != NULL) 
    {
        g_alpszArgNames[g_iNamedArgCount + 1] = lpszArgName;
        g_iNamedArgCount++;
    }
}    

BOOL AddArgumentCString(LPOLESTR lpszArgName, WORD wFlags,LPOLESTR lpsz)
{
    BSTR b;

    b = SysAllocString(lpsz);
    if (!b)
        return FALSE;
    AddArgumentCommon(lpszArgName, wFlags, VT_BSTR);
    g_aVargs[g_iArgCount++].bstrVal = b;
    return TRUE;
}

void LaunchWord(CString sPathWord,CString sPathOutput){

	BSTR param2 = sPathWord.AllocSysString();//SysAllocString(OLESTR("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE"));
	BSTR param1 = sPathOutput.AllocSysString();//SysAllocString(OLESTR(""));
	BSTR name1 = SysAllocString(OLESTR("PathWord"));
	BSTR name2 = SysAllocString(OLESTR("PathOutput"));

	AddArgumentCString(name1,0,param1);
	AddArgumentCString(name2,0,param2);

	VARIANTARG FAR rgvarg[2];

	
	V_BSTRREF(&rgvarg[0]) = &param1;
	V_BSTRREF(&rgvarg[1]) = &param2;


	DISPPARAMS dpNoArgs = {g_aVargs, NULL, 2, 0};
	VARIANT vResult;
	OLECHAR FAR* szFunction;
	//BSTR bstrTemp;

	// IDispatch pointers for Word's objects
	//IDispatch* pDispDocs;      //Documents collection

	// DISPIDs
	DISPID dispid_Docs;        //Documents property of Application 
							   //object

	//Initialize the COM libraries
	::CoInitialize(NULL);

	// Create an instance of the Word application and obtain the 
	// pointer to the application's IDispatch interface.
	CLSID clsid;
	CLSIDFromString(L"UtilsVB.clsLauncher", &clsid);  

	IUnknown* pUnk;
	HRESULT hr = ::CoCreateInstance( clsid, NULL, CLSCTX_SERVER,
									 IID_IUnknown, (void**) &pUnk);
	IDispatch* pDispApp;
	hr = pUnk->QueryInterface(IID_IDispatch, (void**)&pDispApp);

	// Get IDispatch* for the Documents collection object
	szFunction = OLESTR("Launcher");


	hr = pDispApp->GetIDsOfNames (IID_NULL, &szFunction, 1, 
								  LOCALE_USER_DEFAULT, &dispid_Docs);

	unsigned int FAR uArgErr;
	hr = pDispApp->Invoke (dispid_Docs, IID_NULL, 
						   LOCALE_USER_DEFAULT, DISPATCH_METHOD, 
						   &dpNoArgs, &vResult, NULL, &uArgErr);
	




	//Execute Internet Explorer
	/*
	CString sPath;
	HINSTANCE hinstance=ShellExecute(
		AfxGetMainWnd( )->m_hWnd,	//HWND hwnd, 
		"open",						//LPCTSTR lpVerb,
		CString(vResult.bstrVal),//"C:\\Archivos de programa\\Internet Explorer\\IEXPLORE.EXE",//"C:\\Archivos de programa\\Microsoft Office\\Office\\WINWORD.EXE",//LPCTSTR lpFile, 
		sPath,//"\"file://C:\\2002\\DraftCable\\Src\\testfile.htm\"",				//LPCTSTR lpParameters, 
		NULL,						//LPCTSTR lpDirectory,
		SW_SHOWNORMAL				//INT nShowCmd
	);
	*/



	ClearAllArgs();
	pDispApp->Release();
	pUnk->Release();

	//...
	::CoUninitialize();

}