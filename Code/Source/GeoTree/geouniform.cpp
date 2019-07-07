#include "geotree.h"

/*!
\class GeoUniform geotree.h
\brief An uniform in the geology tree.
*/

/*!
\param An uniform amplitude
*/
GeoUniform::GeoUniform(float A) : a(A)
{
}

/*!
\brief Compute the rock hardness at p.
\param p point
*/
float GeoUniform::Intensity(const Vector3& p) const
{
	return a;
}

/*!
\brief Compute memory.
*/
int GeoUniform::Memory() const
{
	return sizeof(GeoUniform);
}
