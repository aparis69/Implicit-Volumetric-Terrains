#include "ttree.h"
#include "noise.h"

/*!
\class TPointNoise ttree.h
\brief A point skeleton primitive displaced with a simplex noise.
*/

/*!
\brief Constructor
\param c center
\param R radius
\param E Intensity
\param A noise amplitude
\param w Wavelength.
*/
TVertexNoise::TVertexNoise(const Vector3& c, float R, float E, float A, float w) : TVertex(c, R, E), a(A), w(w)
{
	box = box.Extended(Vector3(a));
}

/*!
\brief Compute intensity.
Radius of the primitive is displaced with noise.
\param p Point.
*/
float TVertexNoise::Intensity(const Vector3& p) const
{
	if (!box.Contains(p))
		return 0.0;

	// Radial Vector3
	Vector3 cp = p - c;
	float l = Magnitude(cp);

	// Point on the sphere
	Vector3 q = c + cp * (r / l);

	// Simplex Noise
	float n = a * PerlinNoise::GetValue(q / w);

	// Modified radius
	float rn = r + n;
	return e * Math::CubicSmoothCompact(l * l, rn * rn);
}
