#include "ttree.h"

/*!
\class TUnion ttree.h
\brief Boolean union node.
*/

/*!
\brief Create a union of two sub-trees.
\param a, b Sub-trees.
*/
TUnion::TUnion(TNode* a, TNode* b) : TBinary(a, b)
{
}

/*!
\brief Compute the field function at a given point in space.
\param p Point.
*/
float TUnion::Intensity(const Vector3& p) const
{
	if (!box.Contains(p))
		return 0.0;
	return Math::Max(e[0]->Intensity(p), e[1]->Intensity(p));
}

/*!
\brief Compute the gradient at a given point in space.
\param p Point.
*/
Vector3 TUnion::Gradient(const Vector3& p) const
{
	if (!box.Contains(p))
		return Vector3(0.0);
	if (e[0]->Intensity(p) > e[1]->Intensity(p))
		return e[0]->Gradient(p);
	return e[1]->Gradient(p);
}