// UtilsCPP.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
//No se encuenta: instala el sdk?
//https://www.codeproject.com/Articles/13320/Using-Regular-Expressions-in-MFC
//#include "atlrx.h"
#include <regex>
//#include <boost/regex.hpp> 
#include <string> 
#include <list>
#include <iostream> 

#include "UtilsCPP.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
UTILSCPP_API int nUtilsCPP=0;

// This is an example of an exported function.
UTILSCPP_API int fnUtilsCPP(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see UtilsCPP.h for the class definition
CUtilsCPP::CUtilsCPP()
{ 
	return; 
}

//using namespace boost; 

namespace boost = std;

//boost::regex e("\\x7c\\x2a\\x2e[^\\x29]+\\x7c");
boost::wregex e(L"\\x7c\\x2a\\x2e[^\\x29]+\\x7c");

//unsigned tokenise(std::list<std::string>& l, std::string& s)
//{
//   return boost::regex_split(std::back_inserter(l), s, e);
//}

void regex(std::wstring str){
	e=boost::wregex(str);
}

int split(std::wstring s,LPTSTR *sa,int nmax)
{
	unsigned result;
	std::list<std::string> l;

   do{

	   auto words_begin =
			std::wsregex_iterator(s.begin(), s.end(), e);
	   auto words_end = std::wsregex_iterator();

	   std::cout << "Found "
		   << std::distance(words_begin, words_end)
		   << " words:\n";

   //   result = tokenise(l, s);
	  //std::cout << result << " tokens found" << std::endl;
   //   std::cout << "The remaining text is: \"" << s << "\"" << std::endl;
	  
	  int index = 0;
	  for (std::wsregex_iterator i = words_begin; i != words_end; ++i) {
		  std::wsmatch match = *i;
		  std::wstring match_str = match.str();
		  //std::wcout << match_str << '\n';

		  if (sa&&index<nmax){
			  wcscpy(sa[index], s.c_str());
			  i++;
		  }

		  index++;
	  }

	  //int i=0;
   //   while(l.size())
   //   {
   //      s = *(l.begin());
   //      l.pop_front();
   //      std::cout << s << std::endl;

		 //if(sa&&i<nmax){
			//strcpy(sa[i],s.c_str());
			//i++;
		 //}
   //   }
	  
   }while(0);


   return result;
}