#include "geotree.h"
#include "vec.h"

/*!
\class GeoCubicFalloff geotree.h
\brief A Cubic fallof for skeletal primitive. Use Wyvill falloff function.
*/

/*!
\brief Constructor
\param r Radius
\param e Hardness.
*/
GeoCubicFalloff::GeoCubicFalloff(float r, float e) : r(r), e(e)
{
}

/*!
\brief Compute the falloff function.
\param d Squared distance to the skeleton.
*/
float GeoCubicFalloff::Falloff(float sd) const
{
  return e * Math::CubicSmoothCompact(sd, r * r);
}
