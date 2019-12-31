#if !defined(_DRAFTCABLE_SHAPE_ARRAY)
#define _DRAFTCABLE_SHAPE_ARRAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShArray.h : header file
//

//#define DCABLE_SHARRAY_RECONNECT

//05/12/2004
//This class will implement a 2D array to handle shapes in screen.
//1.When ever a shape is added/deleted to main document array it should be added to screen array.
typedef CArray<CObList,CObList> ShArrayRow;

class CShArray : public CObject{

public:
	//Construction
	CShArray();
	CShArray(SIZE szTotal,SIZE szCell,LPVOID lpddDoc =NULL);
	BOOL Create(SIZE szTotal,SIZE szCell,LPVOID lpddDoc =NULL);
	~CShArray();
	//Attributes
	SIZE GetSize( ) const;
	//Operations
	void AddShape(CShape *pSh);				//Add a shape to array
	void MoveShape(CShape *pSh,LPRECT lpRecLast,BOOL bReconnect =TRUE);			//Move shape in array
	void DeleteShape(CShape *pSh);			//Delete shape
	void RemoveAll();						//Removes all the pointers from this array but does not actually delete the objects
	void RemoveAt( SIZE szIndex, SIZE szCount = CSize(1,1) );								//Removes one or more elements starting at a specified index
	void SetSize( SIZE szTotal,SIZE szCell, SIZE szGrowBy = CSize(-1,-1) );				//Establishes the size of an empty or existing array
	CObList& GetCell(int x,int y){return m_vvRows[x][y];};

	//Datamembers
	LPVOID m_pddDoc;

protected:
	CArray<ShArrayRow,ShArrayRow> m_vvRows;
	SIZE m_szArray;							//Size of array
	SIZE m_szCell;							//Size of cell
};

/////////////////////////////////////////////////////////////////////////////

#endif // !define _DRAFTCABLE_SHAPE_ARRAY)