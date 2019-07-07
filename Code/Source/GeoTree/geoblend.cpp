#include "geotree.h"

/*!
\class GeoBlend geotree.h
\brief A Simple blend defined as the sum of the intensity of the two sub trees.
*/


/*!
\brief Constructor
\param e1 first sub tree
\param e2 second sub tree
*/
GeoBlend::GeoBlend(GeoNode* e1, GeoNode* e2) : GeoBinary(e1, e2)
{
}

/*!
\brief Compute the rock hardness at p.
\param p point
*/
float GeoBlend::Intensity(const Vector3& p) const
{
	return e[0]->Intensity(p) + e[1]->Intensity(p);
}
