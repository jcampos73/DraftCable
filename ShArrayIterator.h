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

class CShArrayIterator : public std::iterator<std::input_iterator_tag, CShArray>
{
private:
	CShArray* m_pShArray;
public:
	CShArrayIterator(CShArray* shArray) { m_pShArray = shArray; }
	CShArrayIterator();
	~CShArrayIterator();
};

