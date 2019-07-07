#include "bvh.h"
#include "ttree.h"
#include <algorithm>

/*!
\brief Create a bounding box hierarchy.
*/
TNode* TTreeBVH::BVHRecursive(std::vector<TNode*>& pts, unsigned int begin, unsigned int end)
{
	/*
	\brief BVH space partition predicate. Could also use a lambda function to avoid the structure.
	*/
	struct BVHPartitionPredicate
	{
		int axis;
		float cut;

		BVHPartitionPredicate(int a, float c) : axis(a), cut(c)
		{
		}

		bool operator()(TNode* p) const
		{
			Vector3 c = (p->GetBox().Vertex(0) + p->GetBox().Vertex(1)) / 2.0f;
			return c[axis] < cut;
		}
	};

	// If leaf, returns primitive
	if (end - begin <= 1)
		return pts[begin];

	// Bounding box of primitive in [begin, end] range
	Box bbox = pts[begin]->GetBox();
	for (unsigned int i = begin + 1; i < end; i++)
		bbox = Box(bbox, pts[i]->GetBox());

	// Find the most stretched axis of the bounding box
	// Cut the box in the middle of this stretched axis
	Vector3 diag = bbox[2] - bbox[1];
	int stretchedAxis = diag.MaxIndex();
	float axisMiddleCut = (bbox[0][stretchedAxis] + bbox[1][stretchedAxis]) / 2.0f;

	// Partition our primitives in relation to the axisMiddleCut
	auto pmid = std::partition(pts.begin() + begin, pts.begin() + end, BVHPartitionPredicate(stretchedAxis, axisMiddleCut));

	// Ensure the partition is not degenerate : all primitives on the same side
	unsigned int midIndex = (unsigned int)std::distance(pts.begin(), pmid);
	if (midIndex == begin || midIndex == end)
		midIndex = (begin + end) / 2;

	// Recursive construction of sub trees
	TNode* left = BVHRecursive(pts, begin, midIndex);
	TNode* right = BVHRecursive(pts, midIndex, end);

	// Blend of the two child nodes
	return new TBlend(left, right);
}

/*!
\brief Recursive BVH Tree construction from a vector<TNode*>.
\param begin start index
\param end end index
*/
TNode* TTreeBVH::OptimizeHierarchy(std::vector<TNode*>& pts, unsigned int begin, unsigned int end)
{
	if (pts.empty())
		return nullptr;
	return BVHRecursive(pts, begin, end);
}
