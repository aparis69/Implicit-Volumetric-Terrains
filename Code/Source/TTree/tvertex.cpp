#include "ttree.h"

/*!
\class TPoint ttree.h
\brief A point skeleton primitive.
*/

/*!
\brief Constructor.
\param c Center.
\param r Radius
\param e Intensity.
*/
TVertex::TVertex(const Vector3& c, float r, float e) : TCubicFalloff(r, e), c(c)
{
	box = Box(c, r);
}

/*!
\brief Compute the intensity at a given point.
\param p Point.
*/
float TVertex::Intensity(const Vector3& p) const
{
	if (!box.Contains(p))
		return 0.0;
	return Falloff(SquaredMagnitude(p - c));
}
