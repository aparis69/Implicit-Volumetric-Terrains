#include "ttree.h"
#include "heightfield.h"
#include "noise.h"

class SmoothDisc2D
{
protected:
	Vector2 c;	//!< Center.
	float r;	//!< Radius.
	float fr;	//!< Falloff radius.
public:
	SmoothDisc2D(const Vector2&, float, float);
	float Intensity(const Vector2&) const;
};

/*!
\brief Constructor
\param p World point in 2D
\param r radius
\param fr falloff radius
*/
SmoothDisc2D::SmoothDisc2D(const Vector2& p, float r, float f) : c(p), r(r), fr(f)
{
}

/*!
\brief Compute a smooth intensity inside the disc.
*/
float SmoothDisc2D::Intensity(const Vector2& p) const
{
	Vector2 cp = p - c;
	float d = Dot(cp, cp);
	float rre = r + fr;

	// Clamping is performed here
	if (d > rre * rre)
		return 0.0f;

	// Inside disc
	if (d < r * r)
		return 1.0f;

	d = sqrt(d);
	d -= r;
	d *= d;
	return Math::CubicSmoothCompact(d, fr * fr); // Warning: this should be Smooth() and not SmoothCompact() TodoTodoTodo
}


/*!
\class TFloatingIsland ttree.h
\brief Floating islands.
*/

/*!
\brief Create a floating island.
*/
TFloatingIsland::TFloatingIsland(const Vector3& c, float r, float depth, float height) : r(r), c(c), depth(depth), height(height), localbox(Box2D(Vector2(c), 2.5f*r).ToBox(-2.5f*r, 2.5f*r)), TPrimitive(localbox.Extended(Vector3(r)))
{
	box = localbox.Extended(Vector3(r));
}

/*!
\brief Compute the intensity.

\param p Point.
*/
float TFloatingIsland::Intensity(const Vector3& q) const
{
	// @Todo: optimize all this.
	Vector3 p = q - c;

	if (!box.Contains(p))
		return 0.0f;

	// Main smoothing function
	float t = 1.0f - SmoothDisc2D(Vector2(0.0f), r / 2.0f, r).Intensity(Vector2(p));

	float za = -depth + depth / 2.0f*(1.0f - t)*PerlinNoise::GetValue(p / 30.0f + 0.54f) + depth / 4.0f*(1.0f - t)*PerlinNoise::GetValue(p / 14.0f + .63f) + depth / 8.0f*PerlinNoise::GetValue(p / 7.0f + .13f) + depth / 16.0f*PerlinNoise::GetValue(p / 4.0f + .79f) + depth / 32.0f*PerlinNoise::GetValue(p / 2.0f + .79f);
	float zb = height / 2.0f + height / 4.0f*(1.0f - t)*PerlinNoise::GetValue(p / 89.0f) + height / 8.0f*PerlinNoise::GetValue(p / 46.0f) + 3 * PerlinNoise::GetValue(p / 14.0f);

	// big pikes inside
	za -= 10.0f*SmoothDisc2D(Vector2(-r / 4.0f, r / 8.0f), 0.0f, r / 2.0f).Intensity(Vector2(p));
	za -= 8.0f*SmoothDisc2D(Vector2(r / 2.0f, r / 4.0f), 0.0f, r / 2.0f).Intensity(Vector2(p));
	za -= 12.0f*SmoothDisc2D(Vector2(r / 8.0f, -r / 8.0f), 0.0f, r).Intensity(Vector2(p));

	za = 10.0f*t + (1 - t)*za;
	zb = -20.0f*t + (1 - t)*zb;

	// Distance to terrain
	float dta = za - p[1];
	float dtb = zb - p[1];

	// Radius blend
	const float rb = 20.0f;

	// Elevation field
	float ea = Math::CubicSigmoid(-dta, rb, TTree::T()) + TTree::T();
	float eb = Math::CubicSigmoid(dtb, rb, TTree::T()) + TTree::T();
	float e = Math::Min(ea, eb);

	// Box field
	float f = 2.0f*TTree::T() * Math::CubicSmoothCompact(localbox.Distance(p), 0.25f*r*r);
	return Math::Min(e, f);
}


/*!
\brief Create a floating island.
*/
TFloatingIsland2::TFloatingIsland2(const Vector3& c, const float& r, const float& depth, const float& height) : r(r), c(c), depth(depth), height(height), localbox(Box2D(Vector2(c), 2.5f*r).ToBox(-2.5f*r, 2.5f*r)), TPrimitive(localbox.Extended(Vector3(r)))
{

}

/*!
\brief Compute the intensity.

\param p Point.
*/
float TFloatingIsland2::Intensity(const Vector3& q) const
{
	// Todo: optimize all this.
	Vector3 p = q - c;

	if (!box.Contains(p))
		return 0.0f;

	// Main smoothing function
	float t = 1.0f - SmoothDisc2D(Vector2(0.0f), r / 2.0f, r).Intensity(Vector2(p));

	float za = depth + depth / 2.0f*(1.0f - t)*PerlinNoise::GetValue(p / 30.0f + 0.54f) + depth / 4.0f*(1.0f - t)*PerlinNoise::GetValue(p / 14.0f + .63f) + depth / 8.0f*PerlinNoise::GetValue(p / 7.0f + .13f) + depth / 16.0f*PerlinNoise::GetValue(p / 4.0f + .79f) + depth / 32.0f*PerlinNoise::GetValue(p / 2.0f + .79f);
	za = -za;
	float zb = height / 2.0f + height / 4.0f*(1.0f - t)*PerlinNoise::GetValue(p / 89.0f) + height / 8.0f*PerlinNoise::GetValue(p / 46.0f) + 3 * PerlinNoise::GetValue(p / 14.0f);

	za = 0.0f;
	zb = 0.0f;

	// big pikes inside
	za -= 10.0f*SmoothDisc2D(Vector2(-r / 4.0f, r / 8.0f), 0.0f, r / 2.0f).Intensity(Vector2(p));
	za -= 8.0f*SmoothDisc2D(Vector2(r / 2.0f, r / 4.0f), 0.0f, r / 2.0f).Intensity(Vector2(p));
	za -= 12.0f*SmoothDisc2D(Vector2(r / 8.0f, -r / 8.0f), 0.0f, r).Intensity(Vector2(p));

	// Crossing
	za += 10.0f*t;
	zb -= 15.0f*t;

	// Distance to terrain
	float dta = za - p[1];
	float dtb = zb - p[1];

	// Radius blend
	const float rb = 20.0f;

	// Elevation field
	float ea = Math::CubicSigmoid(-dta, rb, TTree::T()) + TTree::T();
	float eb = Math::CubicSigmoid(dtb, rb, TTree::T()) + TTree::T();
	float e = Math::Min(ea, eb);

	// Box field
	float f = 2.0f*TTree::T() * Math::CubicSmoothCompact(localbox.Distance(p), rb);
	return Math::Min(e, f);
}
