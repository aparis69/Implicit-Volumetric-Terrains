#include "geotree.h"

/*!
\class GeoTree geotree.h
\brief Main class for the geology construction tree.

This tree defines rock hardness at every point in space.
Can be initialized with something like:
	GeoTree* newGeoTree = new GeoTree(
		new GeoNoiseKarst()
	);

Several operators are defined: blending, alpha blending on one direction, warping etc...
Note that the construction tree doesn't have a continuous intensity in space, since we don't blend it directly with the terrain construction tree.
Also note that intensity/rock hardness is not normalized: values may vary depending on nodes. Typically, noise node values are sometimes between [0, 2].
*/

/*!
\brief Constructor from a single node.
*/
GeoTree::GeoTree(GeoNode* n) : root(n)
{
}

/*!
\brief Destructor
*/
GeoTree::~GeoTree()
{
	delete root;
}

/*!
\brief Compute the rock hardness at point p.
\param p point
*/
float GeoTree::Intensity(const Vector3& p) const
{
	return root->Intensity(p);
}

/*!
\brief Blend the root node with the node in parameter.
\param n the node to blend the tree with.
*/
void GeoTree::Blend(GeoNode* n)
{
	root = new GeoBlend(root, n);
}

/*!
\brief Compute the memory.
*/
int GeoTree::Memory() const
{
	return sizeof(GeoTree) + root->Memory();
}
