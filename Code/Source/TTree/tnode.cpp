#include "ttree.h"

/*!
\class TNode ttree.h
\brief Base node class.

All primitives and operators (indirectly) inherit from this node. This class doesn't have a box property to be efficient
when it comes to instancing (instance nodes don't store a box).
*/

static float Epsilon = 1e-4f;

/*!
\brief Empty.
*/
TNode::TNode() : box(Box(Vector3(0), 1.0))
{
}

/*!
\brief Empty.
*/
TNode::TNode(const Box& box) : box(box)
{
}

/*!
\brief
*/
TNode::~TNode()
{
}

/*!
\brief Compute intensity at a given point in space.
\param p
*/
float TNode::Intensity(const Vector3& p) const
{
	return 0.0;
}

/*!
\brief Compute the gradient at a given point.
\param p Point.
*/
Vector3 TNode::Gradient(const Vector3& p) const
{
	if (!GetBox().Contains(p))
		return Vector3(0.0);
	float x = Intensity(Vector3(p[0] + Epsilon, p[1], p[2])) - Intensity(Vector3(p[0] - Epsilon, p[1], p[2]));
	float y = Intensity(Vector3(p[0], p[1] + Epsilon, p[2])) - Intensity(Vector3(p[0], p[1] - Epsilon, p[2]));
	float z = Intensity(Vector3(p[0], p[1], p[2] + Epsilon)) - Intensity(Vector3(p[0], p[1], p[2] - Epsilon));
	return Vector3(x, y, z) / (2.0f * Epsilon);
}

/*!
\brief Returns the bounding box of the node.
*/
Box TNode::GetBox() const
{
	return box;
}
