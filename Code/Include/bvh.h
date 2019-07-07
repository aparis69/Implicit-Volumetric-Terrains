#pragma once
#include <vector>

class TNode;

// A Bounding volume hierarchy interface for the terrain construction tree.
// Essential in order to keep acceptable meshing time.
class TTreeBVH
{
public:
	static TNode* BVHRecursive(std::vector<TNode*>& pts, unsigned int begin, unsigned int end);
	static TNode* OptimizeHierarchy(std::vector<TNode*>& pts, unsigned int begin, unsigned int end);
};
