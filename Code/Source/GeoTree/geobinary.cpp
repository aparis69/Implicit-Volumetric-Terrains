#include "geotree.h"

/*!
\class GeoBinary geotree.h
\brief Binary operator generic class.
*/

/*!
\brief Constructor
\param e1 first sub tree
\param e2 second sub tree
*/
GeoBinary::GeoBinary(GeoNode* e1, GeoNode* e2) 
{
	e[0] = e1;
	e[1] = e2;
}

/*!
\brief Compute the memory.
*/
int GeoBinary::Memory() const
{
	return sizeof(GeoBinary) + e[0]->Memory() + e[1]->Memory();
}
