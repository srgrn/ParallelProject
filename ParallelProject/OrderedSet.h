/*
* a correct way of writing this code would be a bit more abstract using templates if possible to allow all kinds of objects in the set.
* since i'm not using the correct way i will change it from using int into using plane pointer
*/
#pragma once
#include "Plane.h"

class OrderedSet
{
public:
	//OrderedSet(int a, int b);
	OrderedSet();
	OrderedSet(plane* a, plane* b);
	~OrderedSet(void);
	plane* first();
	plane* second();
	void first(plane *ptr);
	void second(plane *ptr);
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
	//OrderedSet operator+( const OrderedSet& other ) const;
	//OrderedSet operator+( const int& other ) const;
private:
	//int first;
	//int second;
	plane* m_first;
	plane* m_second;
};

