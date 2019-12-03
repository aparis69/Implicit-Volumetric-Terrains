#include "ttree.h"
#include "geotree.h"
#include "bvh.h"
#include <algorithm>

/*!
\brief Performs a poisson sphere check on a set of points and a candidate position. Returns true if the candidate
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
\brief This function does a 2D analysis and sampling for the karst scene. We use the slope map to detect locations
on the cliff (infiltration/resurgence points). Parameters are hard-coded in the function. In the paper, we used a sampling of the stream
power field of the initial heightfield, which can give slightly more realistic seed points taking into account the drainage area
(strength of the river network).
\param hf the input 2D heightfield
\return a vector of locations for karst entry points. Used for the IP-simulation.
*/
static std::vector<Vector3> GetInitialSeeds(HeightField& hf)
{
	// Density map
	ScalarField2D slopeField = hf.Slope();
	slopeField.NormalizeField();

	// Sampling
	const float slopeThresholdMin = 0.3f;
	const float poissonRadius = 10.0f;
	std::vector<Vector3> ret;
	for (int i = 0; i < slopeField.SizeX(); i++)
	{
		for (int j = 0; j < slopeField.SizeY(); j++)
		{
			Vector3 p = hf.Vertex(i, j);
			if (slopeField.Get(i, j) > slopeThresholdMin
				&& !PoissonSphereCheck(p, ret, poissonRadius))
			{
				ret.push_back(p);
				slopeField.Set(i, j, 1.0);
			}
			else
				slopeField.Set(i, j, 0.0);
		}
	}
	std::cout << "Sample count : " << ret.size() << std::endl;
	return ret;
}

/*!
\brief Simple Invasion-Percolation used to create Karst-like landforms.
Starting from a set of initial seed, the algorithm progress in the 3D scene and place skeletal primitives
to erode the terrain surface. Seeds advance in the least resistant rock position, computed from the geology tree.

Many things could be optimized, improved or written better: but it works. This algorithm showcased the possibilities
of our model in modelling volumetric landforms automatically (and hopefully, fast).
\param tree terrain construction tree
\param geoTree geology construction tree
\param featurePositions resurgence point array computed by GetFeatureLocationKarsts().
\returns a vector of nodes to combine with the base construction tree.
*/
static std::vector<TNode*> KarstInvasionPercolation(TTree* tree, GeoTree* geoTree, std::vector<Vector3>& featurePositions)
{
	// Utility structure to sort the candidate position std::vector.
	// Compare rock hardness of the two candidates.
	struct IPSeedSortingPredicate
	{
	public:
		GeoTree * geoTree;
		bool operator()(Vector3 a, Vector3 b) const
		{
			float hardnessA = geoTree->Intensity(a);
			float hardnessB = geoTree->Intensity(b);
			return hardnessA < hardnessB;
		}
	};

	// Hardcoded parameters
	const Box bbox = tree->GetBox().Extended(Vector3(-5.0f));
	const float radius = 12.0f;
	const float energy = -8.0f;
	const float step = radius / 1.5f;
	const float breakingProbablity = 0.15f;
	const Vector2 altitudeRange = Vector2(-10.0, 40.0f);

	IPSeedSortingPredicate predicate;
	predicate.geoTree = geoTree;

	std::vector<TNode*> ret;
	std::vector<Vector3> poissonSampling;

	// You can also put a limited number of iteration, as this may take a while to converge if
	// You put too many sample in the neighbour computation step.
	while (featurePositions.empty() == false)
	{
		// Pop the softest seed and try to dig a karst from there
		Vector3 p = featurePositions[0];
		featurePositions.erase(featurePositions.begin());

		// Break rule before everything: random probability
		// Not having this condition means that the karstification
		// Will almost always advance everywhere in the scene.
		float rr = Random::Uniform();
		if (rr < breakingProbablity) // Break rule: random probability
			continue;

		// Check the altitude range (user mask) and the scene bounding box
		if (!bbox.Contains(p) || p[1] < altitudeRange[0] || p[1] > altitudeRange[1])
			continue;

		// Large poisson sphere criteria, to avoid infinite looping on the algorithm.
		if (PoissonSphereCheck(p, poissonSampling, radius / 1.52f))
			continue;
		poissonSampling.push_back(p);

		// Place multiple primitives whose radius is changed by a geology factor
		// Sample inside a sphere. We do this to create a more complex effect on the
		// Karst structure: having just one big primitive at each position doesn't lead
		// To good looking karsts.
		std::vector<Vector3> smallPoisson;
		Sphere smallSphere = Sphere(p, radius / 2.0);
		float smallRadius = radius / 2.0;
		for (int i = 0; i < 40; i++)
		{
			// Sample inside a small sphere
			Vector3 pp = smallSphere.RandomInside();

			// Poisson sphere criteria
			if (PoissonSphereCheck(pp, smallPoisson, smallRadius / 2.5f))
				continue;

			// Compute the geology factor (softness) at pp
			float geoFactor = 1.0f - Math::Clamp(geoTree->Intensity(pp));

			// If rock is too hard
			if (geoFactor < 0.3f)
			{
				float smallProba = Random::Uniform();
				if (smallProba < 0.05f) // 5% chance of karst in hard rock.
					continue;
			}

			// Modulate energy & radius with geology
			// Radius & Energy can't be too low, so we clamp the geoFactor to something acceptable.
			geoFactor = Math::Clamp(geoFactor, 0.85f, 1.0f);

			// Add the primitive
			float actualRadius = smallRadius * geoFactor;
			float actualEnergy = energy * geoFactor;
			ret.push_back(new TVertex(pp, actualRadius, actualEnergy));
			smallPoisson.push_back(pp);
		}

		// Break rule after primitives: random probability again
		rr = Random::Uniform();
		if (rr < breakingProbablity)
			continue;

		// Neighborhood is composed of 2 random point sampled in the unit circle, with y in [-0.2, 0.2]
		// You can put more of course, at the expense of computation time.
		featurePositions.push_back(p + Circle2(Vector2(0), 1).RandomOn().ToVector3(Random::Uniform(-0.2f, 0.2f)) * step);
		featurePositions.push_back(p + Circle2(Vector2(0), 1).RandomOn().ToVector3(Random::Uniform(-0.2f, 0.2f)) * step);

		// Re-sort all the seed by decreasing rock hardness
		std::sort(featurePositions.begin(), featurePositions.end(), predicate);
	}
	std::cout << "Total primitive count : " << ret.size() << std::endl;
	return ret;
}

/*!
\brief Entry point of the Karst scene.
*/
void KarstScene()
{
	// Terrain Tree
	const float sizeX = 400.0f;
	const float sizeY = 400.0f;
	const float minAlt = -20.0f;
	const float maxAlt = 70.0f;
	const Box2D bbox = Box2D(Vector2(-sizeY / 2.0, -sizeX / 2.0), Vector2(sizeY / 2.0, sizeX / 2.0));
	TAnalyticCliff* root = new TAnalyticCliff(bbox.ToBox(minAlt, maxAlt), Vector2(minAlt, maxAlt));
	TTree* terrainTree = new TTree(root);
	HeightField hf = HeightField(root, 256, 256, bbox);

	// Geology Tree
	// Strata are defined on top of the noise so that Invasion-Percolation 
	// Can achieve some "geological floor" effects, as it can be observed in references pictures.
	GeoTree* geoTree = new GeoTree(
		new GeoBlend(
			new GeoBlend(
				new GeoStrata(14, 3, 1),
				new GeoStrata(4, 3, 1)
			),
			new GeoNoiseKarst()
		)
	);

	// Invasion-Percolation: first compute initial seed points and then perform the simulation.
	std::cout << "Karst Invasion-Percolation" << std::endl;
	{
		std::vector<Vector3> featurePositions = GetInitialSeeds(hf);
		std::vector<TNode*> nodes = KarstInvasionPercolation(terrainTree, geoTree, featurePositions);
		if (nodes.size() > 0)
			terrainTree->Blend(TTreeBVH::OptimizeHierarchy(nodes, 0, int(nodes.size())));
	}

	// Export
	marching_cube("karst.obj", terrainTree, 200);
	std::cout << std::endl;
}
