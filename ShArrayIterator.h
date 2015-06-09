#pragma once

class MyIterator : public std::iterator<std::input_iterator_tag, int>
{
	int* p;
public:
	MyIterator(int* x) :p(x) {}
	MyIterator(const MyIterator& mit) : p(mit.p) {}
	MyIterator& operator++() { ++p; return *this; }
	MyIterator operator++(int) { MyIterator tmp(*this); operator++(); return tmp; }
	bool operator==(const MyIterator& rhs) { return p == rhs.p; }
	bool operator!=(const MyIterator& rhs) { return p != rhs.p; }
	int& operator*() { return *p; }
};

/*
int main () {
int numbers[]={10,20,30,40,50};
MyIterator from(numbers);
MyIterator until(numbers+5);
for (MyIterator it=from; it!=until; it++)
std::cout << *it << ' ';
std::cout << '\n';

return 0;
}
*/

// Define BaseMFCIter as a standard input iterator.
//
// The template arguments are: 
//	Item: the contained element type
//	Cont: the container type
//	Key:  the access key (defaults to POSITION)

template < typename Item, class Cont, class Key = POSITION >
class BaseMFCIter : public std::iterator < std::input_iterator_tag, Item >
{
public:
	// Define types for the 2 member functions to be used:
	typedef Key(Cont::*GetFirstFunctionPtr) ()     const;
	typedef Item(Cont::*GetNextFunctionPtr)  (Key&) const;


	// Default constructor, makes a null iterator, equal to BaseMFCIter::end()
	BaseMFCIter() : m_pCont(0), m_Pos(0), m_GetFirstFunc(0), m_GetNextFunc(0), m_End(true) {}

	// Constructor taking pointer to container and the iteration functions
	BaseMFCIter(Cont* pCont, GetFirstFunctionPtr pFF, GetNextFunctionPtr pNF)
		: m_pCont(pCont), m_Pos(0), m_GetFirstFunc(pFF), m_GetNextFunc(pNF)
	{
		init();
	}

	// Copy constructor, initialises iterator to first element
	BaseMFCIter(const BaseMFCIter& vi) : m_pCont(vi.m_pCont), m_Pos(0),
		m_GetFirstFunc(vi.m_GetFirstFunc), m_GetNextFunc(vi.m_GetNextFunc)
	{
		init();
	}

	// Assignment operator, initialises iterator to first element
	BaseMFCIter& operator=(const BaseMFCIter& vi)
	{
		m_pCont = vi.m_pCont;
		m_GetFirstFunc = vi.m_GetFirstFunc;
		m_GetNextFunc = vi.m_GetNextFunc;
		init();
		return *this;
	}

	bool operator == (const BaseMFCIter& rhs) const
	{
		return (m_Pos == rhs.m_Pos && m_End == rhs.m_End);
	}

	bool operator != (const BaseMFCIter& rhs) const
	{
		return !operator==(rhs);
	}

	BaseMFCIter&	operator ++ ()    { advance(); return *this; }
	BaseMFCIter&	operator ++ (int) { BaseMFCIter ret(*this); advance(); return ret; }
	Item	        operator *  ()    { return m_Item; }
	Item	        operator -> ()    { return m_Item; }

	static BaseMFCIter end()    { return BaseMFCIter(); }	// end() returns default null iterator

private:


	Item	m_Item;			// Current item from container
	Cont*	m_pCont;		// Pointer to container
	Key		m_Pos;			// Key to item in container
	bool	m_End;			// Flag to indicate end of container reached

	// Pointers to container iteration functions
	GetFirstFunctionPtr m_GetFirstFunc;
	GetNextFunctionPtr  m_GetNextFunc;

	// Use container GetFirst & GetNext functions to set to first element, or end() if not found
	void init()
	{
		m_Pos = 0;
		m_End = true;

		if (m_pCont && m_GetFirstFunc != 0)
		{
			m_Pos = (m_pCont->*m_GetFirstFunc)();
			advance();
		}
	}

	// Use container GetNext function to find next element in container
	void advance()
	{
		m_End = m_Pos ? false : true;
		m_Item = (m_Pos && m_pCont && m_GetNextFunc != 0) ?
			(m_pCont->*m_GetNextFunc)(m_Pos) : Item();
	}

};

class myCObject : public CObject
{
public:
	myCObject(int val)
	{
		x = val;
	}
	int x;
};

struct Container : public CObList
{
	//Container() : CObList(){};
	//Container(CObList &lst) : CObList(lst){};
	myCObject*       GetNext(POSITION& rPosition)       { return dynamic_cast<myCObject*>(CObList::GetNext(rPosition)); }
	myCObject const* GetNext(POSITION& rPosition) const { return dynamic_cast<const myCObject*>(CObList::GetNext(rPosition)); }
};

class ListIter : public BaseMFCIter < const CObject*, CObList, POSITION  >
{
public:
	ListIter(CObList* pObj = 0)
		: BaseMFCIter< const CObject*, CObList, POSITION >
		(pObj, &CObList::GetHeadPosition, &CObList::GetNext)
	{
	}
};



