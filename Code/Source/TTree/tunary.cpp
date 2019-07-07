#include "ttree.h"

/*!
\class TUnary ttree.h
\brief Unary nodes.
*/

/*!
\brief Create a unary sub-tree.
\param n Sub-tree.
*/
TUnary::TUnary(TNode* n) : e(n)
{
	box = e->GetBox();
}

/*!
\brief Recursively deletes the sub-tree.
*/
TUnary::~TUnary()
{
	delete e;
}
