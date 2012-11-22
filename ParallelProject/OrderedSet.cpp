#include "OrderedSet.h"

OrderedSet::OrderedSet()
{
}
OrderedSet::OrderedSet(plane* a, plane* b)
{
	if((*a)>=(*b))
	{
		m_first = b;
		m_second = a;
	}
	else
	{
		m_first = a;
		m_second = b;
	}
}
OrderedSet::~OrderedSet(void)
{
}
/*OrderedSet OrderedSet::operator+( const OrderedSet& other ) const
{
	return OrderedSet(m_first+other.m_first,m_second+other.m_second);
}
*/
/*OrderedSet OrderedSet::operator+( const int& other ) const
{
	return OrderedSet(m_first+other,m_second+other);
}*/
bool OrderedSet::operator>(const OrderedSet& other) const
{
	if ((*m_first)>(*other.m_first))
		return true;
	else
		if((*m_first)== (*other.m_first) && (*m_second)>(*other.m_second))
			return true;
	return false;
}
bool OrderedSet::operator==(const OrderedSet& other) const
{
	if((*m_first) == (*other.m_first) && (*m_second) == (*other.m_second))
		return true;
	if((*m_first) == (*other.m_second) && (*m_second) == (*other.m_first)) // this is becouse my set while called an ordered set is actually unordered and i am possibly wrong in my implmantation
		return true;
	return false;
}
bool OrderedSet::operator<(const OrderedSet& other) const
{
	if((*m_first)<(*other.m_first))
		return true;
	else
		if((*m_first) == (*other.m_first) && (*m_second) < (*other.m_second))
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
void OrderedSet::first(plane *ptr)
{
	m_first = ptr;
}
void OrderedSet::second(plane *ptr)
{
	m_second = ptr;
}
plane* OrderedSet::first()
{
	return m_first;
}
plane* OrderedSet::second()
{
	return m_second;
}