#include "geotree.h"

/*!
\class GeoNoise1D geotree.h
\brief A simple parameterized noise sum primitive depending on altitude, uses a fBm function defined
in MyFractal:: class.
*/

/*!
\brief Constructor
\param F noise frequency
\param O noise octave count
\param A noise amplitude
*/
GeoNoise1D::GeoNoise1D(float F, int O, float A) : a(A), f(F), o(O)
{
}

/*!
\brief Compute the rock hardness at p.
\param p Point.
*/
float GeoNoise1D::Intensity(const Vector3& p) const
{
  return PerlinNoise::fBm(Vector3(p[1], 0, 0), a, f, o);
}

/*!
\brief Compute the memory.
*/
int GeoNoise1D::Memory() const
{
	return sizeof(GeoNoise1D);
}
