#pragma once
class CShArrayIterator : public std::iterator<std::input_iterator_tag, CShArray>
{
public:
	CShArrayIterator();
	~CShArrayIterator();
};

