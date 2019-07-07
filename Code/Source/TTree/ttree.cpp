#include "ttree.h"
#include "geotree.h"

/*!
\class TTree ttree.h
\brief Base Terrain tree class, store all the hierarchy and provide query methods like Intensity(), Gradient() etc...
Example :
  TTree* newTree = new TTree(
	  new TBlend(
		new TBlend(
			new TVertex(Vector3(0.0, 0.0, 0.0), 50, 1.0),
			new TVertex(Vector3(0.0, 0.0, 40.0), 50, -1.0)
		),
		new TVertex(Vector3(0.0, 0.0, -40.0), 50, -1.0)
	  )
  );
Static variable 't' is used to defined where the surface is during visualisation.
*/

float TTree::t = 7.5f;
static float epsilon = 1e-4f;

/*!
\brief Base tree constructor.
\param n Root.
*/
TTree::TTree(TNode* n)
{
	root = n;
}

/*!
\brief Destructor.
*/
TTree::~TTree()
{
	delete root;
}

/*!
\brief Compute the intensity at a given point.
The function prunes the whole tree data structure, starting from the root node.
*/
float TTree::Intensity(const Vector3& p) const
{
	return root->Intensity(p) - t;
}

/*!
\brief Compute the gradient at a given point.
The function prunes the whole tree data structure, starting from the root node.
*/
Vector3 TTree::Gradient(const Vector3& p) const
{
	return root->Gradient(p);
}

/*!
\brief Update the root node with a blend
\param n node to blend with root
*/
void TTree::Blend(TNode* n)
{
	root = new TBlend(root, n);
}

/*!
\brief Compute the domain bounding box, starting from the root node.
The function prunes the whole tree data structure, starting from the root node.
*/
Box TTree::GetBox() const
{
	return root->GetBox();
}

/*!
\brief Return the threshold value.
*/
float TTree::T()
{
	return t;
}

/*!
\brief Find a random sample point inside or outside the surface.
\param p Returned point.
\param s Prescribed position with respect to the surface, true if inside, false if outside.
\param box Box domain where the point is to be found.
\param n Maximum number of random points being evaluated.
\return Return true if a sample has been found, false otherwise.
*/
bool TTree::Find(Vector3& p, bool s, const Box& box, int n) const
{
	for (int i = 0; i < n; i++)
	{
		p = box.RandomInside();
		double v = Intensity(p);
		if (s == (v > 0.0))
			return true;
	}
	return false;
}

/*!
\brief Compute the intersection between a segment and an implicit surface.

This method iteratively converges to the solution by performing a bisection on
the segment. It avoids the internal computation if the length of the segment
if it happens to be known already.

The segment should straddle the implicit surface only once, otherwise
should several intersections exist, there is no guarantee that the bissection will
converge to an intersection.

\param a,b End vertices of the segment straddling the surface.
\param va,vb Field function value at those end vertices.
\param length Distance between vertices.
\param epsilon Precision.
\return Point on the implicit surface.
*/
Vector3 TTree::Dichotomy(Vector3 a, Vector3 b, float va, float vb, float length, float epsilon) const
{
	int ia = va > 0 ? 1 : -1;

	// Get an accurate first guess
	Vector3 c = (a * vb - b * va) / (vb - va);;
	while (length > epsilon)
	{
		double vc = Intensity(c);
		int ic = vc > 0 ? 1 : -1;
		if (ia + ic == 0)
		{
			b = c;
		}
		else
		{
			ia = ic;
			a = c;
		}
		length *= 0.5f;
		c = (a + b) * 0.5f;
	}
	return c;
}

/*!
\brief Sample the implicit construction tree inside a given box. Used
by most of the algorithm presented here.
\param p point (by reference)
\param box constrained domain
*/
bool TTree::GetSample(Vector3& p, const Box& box) const
{
	Vector3 a, b;
	if (!Find(a, true, box, 10000))
		return false;
	if (!Find(b, false, box, 10000))
		return false;
	p = Dichotomy(a, b, Intensity(a), Intensity(b), Magnitude(b - a), 1e-2f);
	return true;
}
