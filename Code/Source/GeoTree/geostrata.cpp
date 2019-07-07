#include "geotree.h"

/*!
\class GeoStrata geotree.h
\brief A horizontal strata skeletal primitive. A strata is defined as an triplet (z, r, e) with
z the altitude, r the falloff radius and e the energy.
*/

/*!
\brief Constructor
\param z strata altitude
\param r strata radius
\param e strata energy
*/
GeoStrata::GeoStrata(float z, float r, float e) : z(z), GeoCubicFalloff(r, e)
{
}

/*!
\brief Compute the rock hardness at p.
\param p point
*/
float GeoStrata::Intensity(const Vector3& p) const
{
	float d = p[1] - z;
	return Falloff(d * d);
}

/*!
\brief Compute the memory
*/
int GeoStrata::Memory() const
{
	return sizeof(GeoStrata);
}
