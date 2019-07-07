#include "ttree.h"

/*!
\class TCubicFalloff ttree.h
\brief Cubic falloff primitive.
*/

/*!
\brief Cubic Falloff constructor.
\param r Radius.
\param e Energy.
*/
TCubicFalloff::TCubicFalloff(float r, float e)
{
	TCubicFalloff::r = r;
	TCubicFalloff::e = e;
}

/*!
\brief Compute the cubic falloff function.
\param d Squared distance.
*/
float TCubicFalloff::Falloff(float d) const
{
	return e * Math::CubicSmoothCompact(d, r * r);
}

/*!
\brief Compute the cubic falloff function.
\param d Squared distance.
\param r Squared radius
*/
float TCubicFalloff::Falloff(float d, float rr) const
{
	return e * Math::CubicSmoothCompact(d, rr);
}
