/*
	This is an example implementation of some of the results described in the paper.
	Three scenes (in three different file) can be run to export an .obj file.

	I didn't do a viewer to reduce dependencies. All base heightfields are also
	defined analytically to avoid loading/reading image files. I used a public domain
	implementation of the Marching-cube to output obj files.

	If you have any questions, you can contact me at:
	axel(dot)paris(at)liris(dot)cnrs(dot)fr
*/

void SeaScene();
void KarstScene();
void FloatingIsland();

/*!
\brief Running this program will export some
meshes similar to the ones seen in the paper. Each scene
is in its own file and contains all the algorithms necessary
to produce it.
*/
int main()
{
	//SeaScene();

	FloatingIsland();

	//KarstScene();

	return 0;
}
