#include "ttree.h"
#include "noise.h"
/*!
\class TSphereNoise ttree.h
\brief %Warped sphere skeletal primitive with some noise.
\sa TSphere, TPointNoise
*/

/*!
\brief Constructor.
\param c Center.
\param sr Sphere radius.
\param fr Falloff radius.
\param e Energy.
\param A Noise amplitude.
\param W Noise wavelength.
\param O Noise octave count.
*/
TSphereNoise::TSphereNoise(const Vector3& c, float sr, float fr, float e, float A, float W, int O) : TCubicFalloff(fr, e), Sphere(c, sr)
{
	box = Box(c, sr + fr);
	a = A;
	f = 1.0f / W;
	o = O;
}

/*!
\brief Compute the intensity at a given point.
\param p Point.
*/
float TSphereNoise::Intensity(const Vector3& p) const
{
	if (!box.Contains(p))
		return 0.0;

	// Distance to center
	Vector3 n = p - center;
	float t = Dot(n, n);
	if (t < Sphere::radius * Sphere::radius)
		return e;
	t = sqrt(t);
	t -= Sphere::radius;
	t *= t;

	// Point on sphere
	Vector3 q = center + n * (Sphere::radius / Magnitude(n));

	// Displace falloff radius with fractal noise
	float no = PerlinNoise::GetValue(q);
	float rn = TCubicFalloff::r + no;

	return Falloff(t, rn * rn);
}
