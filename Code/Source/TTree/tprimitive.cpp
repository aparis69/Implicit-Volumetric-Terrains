#include "ttree.h"

/*!
\class TPrimitive ttree.h
\brief A generic primitive.
*/

/*!
\brief Empty.
*/
TPrimitive::TPrimitive() : TNode()
{
}

/*!
\brief Create a primitive given a box.
Simply initialize the bounding box.
\param box The box.
*/
TPrimitive::TPrimitive(const Box& box) : TNode(box)
{
}
