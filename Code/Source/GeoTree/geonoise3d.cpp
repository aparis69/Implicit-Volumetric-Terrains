#include "geotree.h"

/*!
\class GeoNoise3D geotree.h
\brief A simple parameterized 3D noise primitive, uses a simplex noise.
*/

/*!
\brief Constructor
\param A noise amplitude
\param F noise frequency
*/
GeoNoise3D::GeoNoise3D(float A, float F) : a(A), f(F)
{

}

/*!
\brief Compute the rock hardness at p.
\param p point
*/
float GeoNoise3D::Intensity(const Vector3& p) const
{
  return a * (PerlinNoise::GetValue(p * f) * 0.5f + 0.5f);
}

/*!
\brief Compute the memory.
*/
int GeoNoise3D::Memory() const
{
	return sizeof(GeoNoise3D);
}
