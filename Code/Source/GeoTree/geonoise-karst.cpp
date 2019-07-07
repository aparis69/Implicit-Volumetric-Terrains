#include "geotree.h"

/*!
\class GeoNoiseKarst geotree.h
\brief The noise function used for the karst scene. Parameters are hardcoded.
*/

/*!
\brief Constructor
*/
GeoNoiseKarst::GeoNoiseKarst()
{
}

/*!
\brief Compute the rock hardness at p.
\param p point
*/
float GeoNoiseKarst::Intensity(const Vector3& p) const
{
	return PerlinNoise::fBm(Vector3(p[1], 0, 0), 1.0, 0.25, 3); // fBm01 ? Todo
}
