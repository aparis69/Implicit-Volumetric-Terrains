#include "geotree.h"

/*!
\class GeoUnary geotree.h
\brief Generic Unary operator class.
*/

/*!
\brief Constructor
\param e sub tree
*/
GeoUnary::GeoUnary(GeoNode* E)
{
	e = E;
}

/*!
\brief Compute the memory.
*/
int GeoUnary::Memory() const
{
	return sizeof(GeoUnary) + e->Memory();
}
