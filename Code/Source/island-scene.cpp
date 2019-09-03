#include "ttree.h"

/*!
\brief This scene is an example of the "Floating Islands" figure shown in the paper.
It is not the exact same scene. Everything island is defined analytically by combining
multiple noise function with our volumetric system.
*/
void FloatingIsland()
{
	std::cout << "Floating Islands" << std::endl;

	// Terrain
	TNode* major = new TBlend(
		new TFloatingIsland(Vector3(5.0, 0.0, 0.0), 50.0, 15.0, 10),
		new TFloatingIsland(Vector3(-25.0, 30.0, 15.0), 35.0, 5.0, 5.0),
		new TFloatingIsland(Vector3(45.0, 0.0, 35.0), 35.0, 10.0, 5.0)
	);
	TTree* terrainTree = new TTree(major);

	// Export
	marching_cube("islands.obj", terrainTree, 150);
	std::cout << std::endl;
}
