#include "ttree.h"

/*!
\class TTerrainNode ttree.h
\brief Base class for terrain primitives such as Heightfield and Noise primitives. This class is a generic elevation node.
*/

/*!
\brief Create a generic elevation node.
\param b The box.
\param r Radius of influence of the terrain.
*/
TTerrainNode::TTerrainNode(const Box& b, const float& X, const float& r) : TNode(b.Extended(Vector3(0.0, r, 0.0))), x(X), r(r), localbox(box.Extended(Vector3(-0.5f * r)))
{
}

/*!
\brief Compute the elevation.
\param p Point.
*/
float TTerrainNode::Height(const Vector2& p) const
{
	return 0.0;
}

/*!
\brief Compute the intensity.
We first compute the elevation of the terrain, and then evaluate the distance to the terrain.
The distance is then converted to a potential by using a compactly supported sigmoid-like function.
Finally, we compute the intersection with a box-primitive to generate a consistent compactly-supported field function.
\param p Point.
*/
float TTerrainNode::Intensity(const Vector3& p) const
{
	if (!box.Contains(p))
		return 0.0f;

	float z = Height(Vector2(p));

	// Distance to terrain
	float dt = z - p[1];

	// Elevation field
	float e = Math::CubicSigmoid(dt, r, TTree::T()) + TTree::T();

	// Box field
	float f = 2.0f * TTree::T() * Math::CubicSmoothCompact(localbox.Distance(p), r * r * 0.25f);
	return Math::Min(e, f);
}
