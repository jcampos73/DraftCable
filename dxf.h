//This file will hold tables from dxf expecification
//Can be machine generated

class dxfgroup{
public:
	int group_code;

	std::string sdata;
	union{
		
		float fdata;
		int idata;
	};
};

int TableGroup_code_ranges[][3]={/*
+================================+
| Group code  | Following value  |
| range       |                  |
|-------------|------------------|*/
  {0 , 9 , 2}       // String

, {10 , 59 , 1}     // Floating-point

, {60 , 79 , 0}     // Integer

, {140 , 147 , 1}   // Floating-point

, {170 , 175 , 0}   // Integer

, {210 , 239 , 1}   // Floating-point

, {999 , 999 , 2}   // Comment (string)

, {1010 , 1059 , 1} // Floating-point

, {1060 , 1079 , 0} // Integer

, {1000 , 1009 , 2} // String

, {-1 , -1 , -1}	// End
};