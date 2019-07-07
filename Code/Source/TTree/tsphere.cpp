#include "ttree.h"

/*!
\class TSphere ttree.h
\brief %Sphere skeletal primitive.
*/

/*!
\brief Constructor.
\param c Center.
\param sr Sphere radius.
\param r Falloff radius.
\param e Energy.
*/
TSphere::TSphere(const Vector3& c, float sr, float r, float e) : TCubicFalloff(r, e), Sphere(c, sr)
{
	box = Box(c, sr + r);
}

/*!
\brief Compute the intensity at a given point.
\param p Point.
*/
float TSphere::Intensity(const Vector3& p) const
{
	if (!box.Contains(p))
		return 0.0;

	// Distance to center
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
