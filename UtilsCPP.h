
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UTILSCPP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UTILSCPP_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef UTILSCPP_EXPORTS
#define UTILSCPP_API __declspec(dllexport)
#else
#define UTILSCPP_API __declspec(dllimport)
#endif

// This class is exported from the UtilsCPP.dll
class UTILSCPP_API CUtilsCPP {
public:
	CUtilsCPP(void);
	// TODO: add your methods here.
};

extern UTILSCPP_API int nUtilsCPP;

UTILSCPP_API int fnUtilsCPP(void);

UTILSCPP_API void regex(std::wstring str);

UTILSCPP_API int split(std::wstring s,LPTSTR *sa,int nmax);

