#include "ttree.h"

/*!
\class TBinary ttree.h
\brief Base class for binary operators. Stores two sub-trees.
*/

/*!
\brief Create a binary node.
\param a, b Left and right sub-trees.
*/
TBinary::TBinary(TNode* a, TNode* b)
{
	e[0] = a;
	e[1] = b;
	box = Box(e[0]->GetBox(), e[1]->GetBox());
}

/*!
\brief Recursively delete the sub-trees.
*/
TBinary::~TBinary()
{
	delete e[0];
	delete e[1];
}
