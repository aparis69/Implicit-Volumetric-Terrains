#include "ttree.h"
#include "noise.h"

/*!
\class TAnalyticCliff ttree.h
\brief Example of a node with a specific elevation function.
*/

/*!
\brief Todo
*/
TAnalyticCliff::TAnalyticCliff(const Box& B, const Vector2& minMax) : TTerrainNode(B, 15.0, 15.0), c((B.Vertex(0) + B.Vertex(1)) / 2.0f), minMaxElevation(minMax)
{
}

/*!
\copydoc TTerrainNode::Height
\param p Point.
*/
float TAnalyticCliff::Height(const Vector2& p) const
{
	Vector2 q = p - c;

	// Cliffs
	float zc = minMaxElevation[0] + 15.0f * PerlinNoise::GetValue(q / 500.0f) + 7.0f * PerlinNoise::GetValue(q / 300.0f) + 2.0f * PerlinNoise::GetValue(q / 150.0f);
	zc += minMaxElevation[1] * (0.5f + 0.5f * PerlinNoise::GetValue(q / 1050.0f));

	// Sea shore
	float zs = minMaxElevation[0] + 10.0f;

	// Interpolant
	Vector2 qq = q + 135.0f * PerlinNoise::GetValue(q.ToVector3(0.24f) / 150.0f) + 75.0f * PerlinNoise::GetValue(q.ToVector3(0.24f) / 70.0f);

	float u = Math::CubicSmoothStep(qq[0], -35.0f, 25.0f);
	float z = Math::Lerp(zs, zc, u);

	// Global smooth slope towards the sea
	z += minMaxElevation[0] * Math::Step(-qq[0], -500.0f, 500.0f) + 2.0f * PerlinNoise::GetValue(q / 50.0f) + 1.0f * PerlinNoise::GetValue(q / 25.0f);

	// Check bounds, just to be sure
	return Math::Clamp(z, minMaxElevation[0], minMaxElevation[1]);
}
