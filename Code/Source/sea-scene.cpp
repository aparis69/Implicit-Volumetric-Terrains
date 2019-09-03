#include "ttree.h"
#include "geotree.h"
#include "bvh.h"

/*!
\brief Performs a poisson sphere check on a set of point and a candidate position. Returns true if the candidate
fits in the distribution, false otherwise.
\param p candidate position
\param pts set of point
\param radius poisson sphere radius.
*/
static bool PoissonSphereCheck(const Vector3& p, const std::vector<Vector3>& pts, float radius)
{
	float rr = radius * radius;
	for (int i = 0; i < pts.size(); i++)
	{
		if (SquaredMagnitude(p - pts[i]) < rr)
			return true;
	}
	return false;
}

/*!
\brief A simple erosion function working on the construction tree. This function will amplify the given terrain tree with skeletal primitives, placed
at the weakest geology points. Parameters are hardcoded in the function.
It also checks a poisson sphere criteria to ensure a minimum spacing between primitives.
\param tree terrain construction tree
\param geoTree geology tree
*/
static void ErodeWithPrimitives(TTree* tree, GeoTree* geoTree, float primitiveEnergy)
{
	// Hardcoded parameters
	Box subTreeBox = tree->GetBox().Extended(Vector3(-5.0f));
	const float hardnessMax = 0.05f;
	const int sampleCount = 10000;
	const float poissonRadius = 2.0f;
	const float primitiveRadius = 8.0f;
	const float slopeTolerance = 0.8f;

	std::vector<TNode*> nodes;
	std::vector<Vector3> poisson;
	for (int i = 0; i < sampleCount; i++)
	{
		// Find sample on the surface
		Vector3 p;
		if (!tree->GetSample(p, subTreeBox))
			continue;

		// Do not erode on plain surface
		Vector3 g = tree->Gradient(p);
		double d = Math::Abs(Dot(Normalize(g), Vector3(0, 1, 0)));
		if (d > slopeTolerance)
			continue;

		// Poisson criteria
		if (PoissonSphereCheck(p, poisson, poissonRadius))
			continue;

		// Accounting to hardness
		float hardness = Math::Clamp(geoTree->Intensity(p));
		if (hardness < hardnessMax)
			nodes.push_back(new TVertex(p, primitiveRadius, primitiveEnergy));
		poisson.push_back(p);
	}
	if (nodes.size() > 0)
		tree->Blend(TTreeBVH::OptimizeHierarchy(nodes, 0, int(nodes.size())));
	std::cout << "Eroded with " << nodes.size() << " primitives " << std::endl;
}

/*!
\brief Todo
*/
void SeaScene()
{
	// Terrain Tree
	const float sizeX = 1000;
	const float sizeY = 1000;
	const float minAlt = 0;
	const float maxAlt = 100;
	const Box2D bbox = Box2D(Vector2(-sizeY / 2.0, -sizeX / 2.0), Vector2(sizeY / 2.0, sizeX / 2.0));
	TTree* terrainTree = new TTree(new TAnalyticCliff(bbox.ToBox(minAlt, maxAlt), Vector2(minAlt, maxAlt)));

	// Geology Tree
	// A weak strata primitive is defined at the sea-level.
	GeoTree* geoTree = new GeoTree(
		new GeoNoise1D(0.5, 3, 2.0f)
	);
	geoTree->Blend(new GeoStrata(15, 10, -5.0));

	// Erosion (3 levels of depth for more interesting effects)
	std::cout << "Sea Erosion" << std::endl;
	{
		ErodeWithPrimitives(terrainTree, geoTree, -15.0);
		ErodeWithPrimitives(terrainTree, geoTree, -10.0);
		ErodeWithPrimitives(terrainTree, geoTree, -8.0);
	}

	// Export
	marching_cube("sea.obj", terrainTree, 350);
	std::cout << std::endl;
}
