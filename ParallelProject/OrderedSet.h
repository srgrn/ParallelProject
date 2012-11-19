/*
* a correct way of writing this code would be a bit more abstract using templates if possible to allow all kinds of objects in the set.
*/
#pragma once
class OrderedSet
{
public:
	OrderedSet(int a, int b);
	~OrderedSet(void);
	int first;
	int second;
	// nned to define the following operators 
	// >
	bool operator>(const OrderedSet& other) const;
	// == 
	bool operator==(const OrderedSet& other) const;
	// <
	bool operator<(const OrderedSet& other) const;
	// <=
	bool operator<=(const OrderedSet& other) const;
	// >=
	bool operator>=(const OrderedSet& other) const;

	// + becouse i don;t remeber how it is being done
	OrderedSet operator+( const OrderedSet& other ) const;
	OrderedSet operator+( const int& other ) const;

};

