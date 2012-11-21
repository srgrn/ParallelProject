#include "OrderedSet.h"

OrderedSet::OrderedSet(plane* a, plane* b)
{
	if((*a)>=(*b))
	{
		first = b;
		second = a;
	}
	else
	{
		first = a;
		second = b;
	}
}
OrderedSet::~OrderedSet(void)
{
}
/*OrderedSet OrderedSet::operator+( const OrderedSet& other ) const
{
	return OrderedSet(first+other.first,second+other.second);
}
*/
/*OrderedSet OrderedSet::operator+( const int& other ) const
{
	return OrderedSet(first+other,second+other);
}*/
bool OrderedSet::operator>(const OrderedSet& other) const
{
	if ((*first)>(*other.first))
		return true;
	else
		if((*first)== (*other.first) && (*second)>(*other.second))
			return true;
	return false;
}
bool OrderedSet::operator==(const OrderedSet& other) const
{
	if((*first) == (*other.first) && (*second) == (*other.second))
		return true;
	if((*first) == (*other.second) && (*second) == (*other.first)) // this is becouse my set while called an ordered set is actually unordered and i am possibly wrong in my implmantation
		return true;
	return false;
}
bool OrderedSet::operator<(const OrderedSet& other) const
{
	if((*first)<(*other.first))
		return true;
	else
		if((*first) == (*other.first) && (*second) < (*other.second))
			return true;
	return false;

}
bool OrderedSet::operator<=(const OrderedSet& other) const
{
	if(this->operator<(other) || this->operator==(other))
		return true;
	return false;
}
	// >=
bool OrderedSet::operator>=(const OrderedSet& other) const
{
	if(this->operator>(other) || this->operator==(other))
		return true;
	return false;
}