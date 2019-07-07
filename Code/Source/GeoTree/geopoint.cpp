#include "geotree.h"

/*!
\class GeoPoint geotree.h
\brief A Vertex skeletal primitive, similar to the one defined in the terrain construction tree.
A point is defined as a triplet (c, r, e) with c the center, r the radius and e the maxed energy at c.
*/

/*!
\brief Constructor
\param C point center
\param r point radius
\param e point energy at center
*/
GeoVertex::GeoVertex(const Vector3& C, float r, float e) : c(C), GeoCubicFalloff(r, e)
{
}

/*!
\brief Compute the rock hardness at p.
\param p point
*/
float GeoVertex::Intensity(const Vector3& p) const
{
	return Falloff(SquaredMagnitude(p - c));
}

/*!
\brief Compute the memory.
*/
int GeoVertex::Memory() const
{
	return sizeof(GeoVertex);
}
