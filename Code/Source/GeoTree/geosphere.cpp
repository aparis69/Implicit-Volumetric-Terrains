#include "geotree.h"

/*!
\class GeoSphere geotree.h
\brief A Sphere skeletal primitive.
*/

/*!
\brief Create a spherical region with a prescribed hardness.
\param c Center.
\param r falloff radius
\param sr sphere radius
\param e sphere energy
*/
GeoSphere::GeoSphere(const Vector3& c, float r, float sr, float e) : Sphere(c, sr), GeoCubicFalloff(r, e)
{

}

/*!
\brief Compute the rock hardness at a given point.
\param p point
*/
float GeoSphere::Intensity(const Vector3& p) const
{
	Vector3 n = p - center;
	float t = Dot(n, n);
	if (t < Sphere::radius * Sphere::radius)
	{
		return e;
	}

	t = sqrt(t);
	t -= Sphere::radius;
	t *= t;

	return Falloff(t);
}

/*!
\brief Compute the memory.
*/
int GeoSphere::Memory() const
{
	return sizeof(GeoSphere);
}
