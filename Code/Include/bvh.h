#pragma once
#include <vector>

class TNode;

// A Bounding volume hierarchy interface for the terrain construction tree.
class TTreeBVH
{
public:
	static TNode* BVHRecursive(std::vector<TNode*>& pts, unsigned int begin, unsigned int end);
	static TNode* OptimizeHierarchy(std::vector<TNode*>& pts, unsigned int begin, unsigned int end);
};
