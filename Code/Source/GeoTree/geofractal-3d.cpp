#include "geotree.h"

/*!
\class GeoFractal3D geotree.h
\brief
*/

/*!
\brief Todo
*/
GeoFractal3D::GeoFractal3D(float a, float f, int o) : a(a), f(f), o(o)
{
}

/*!
\brief Todo
*/
float GeoFractal3D::Intensity(const Vector3& p) const
{
	return PerlinNoise::fBm(p, a, f, o); // fBm01 ? @To check
}

/*!
\brief Todo
*/
int GeoFractal3D::Memory() const
{
	return sizeof(GeoFractal3D);
}
