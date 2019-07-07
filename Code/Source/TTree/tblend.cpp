#include "ttree.h"

/*!
\class TBlend ttree.h
\brief Blending node.
*/

/*!
\brief Constructor.
\param a,b Left and right sub-trees.
*/
TBlend::TBlend(TNode* a, TNode* b) : TBinary(a, b)
{
}

/*!
\brief Constructor.
\param a,b,c,d Four sub-trees.
*/
TBlend::TBlend(TNode* a, TNode* b, TNode* c, TNode *d) : TBlend(new TBlend(a, b), new TBlend(c,d))
{

}

/*!
\brief Constructor.

Shortcut provided for convenience, same as:
\code
TNode* n=new TBlend(new TBlend(a,b),c);
\endcode
\param a,b,c Sub-trees.
*/
TBlend::TBlend(TNode* a, TNode* b, TNode* c) : TBlend(new TBlend(a, b), c)
{
}

/*!
\brief Compute the intensity.
\param p Point.
*/
float TBlend::Intensity(const Vector3& p) const
{
	if (!box.Contains(p))
		return 0.0;
	return e[0]->Intensity(p) + e[1]->Intensity(p);
}

/*!
\brief Compute the gradient for the blend at a given point, defined as G0 + G1.
\param p Point.
*/
Vector3 TBlend::Gradient(const Vector3& p) const
{
	if (!box.Contains(p))
		return Vector3(0.0);
	return e[0]->Gradient(p) + e[1]->Gradient(p);
}
