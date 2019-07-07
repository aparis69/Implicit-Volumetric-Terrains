#include "geotree.h"

/*!
\class GeoWarp geotree.h
\brief A first warping operator depending on noise. Parameters are hardcoded for figures.
*/

/*!
\brief Constructor
\param e sub tree
*/
GeoWarp::GeoWarp(GeoNode* e) : GeoUnary(e)
{

}

/*!
\brief Compute the rock hardness at p.
\param p point
*/
float GeoWarp::Intensity(const Vector3& p) const
{
	float warp = 20.0f * PerlinNoise::GetValue(p * 0.01f) + 10.0f * PerlinNoise::GetValue(p * 0.005f);
	return e->Intensity(p + Vector3(warp));
}
