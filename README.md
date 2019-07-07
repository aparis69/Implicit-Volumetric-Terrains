## Terrain Amplification with Implicit 3D Features
This repository showcases an implementation of the paper "Terrain Amplification with Implicit 3D Features" published in ACM-TOG in 2019. This is aimed at researchers, students or profesionnals who may want to reproduce and implement **some** of the methods and results described in the paper.

### Summary
Terrains in real life are composed of 2D elements as well as volumetric ones, such as arches, overhangs and cave networks. These features, despite being rare in nature, contribute to the scenic aspect of computer generated landscapes and are therefore important to model. Existing methods rely on Voxels (Becher 2018) or Hybrid representations (Peytavie 2009) and thus suffer from memory. This repository shows how to manipulate and generate volumetric terrains without using any voxel. It relies on Implicit Surfaces, a construction tree and generation algorithms to create large scale, realistic looking terrains such as coastal cliffs, caves or canyons with overhangs. 
The main advantage of this method is that it can be used to model very large terrains with a low memory impact. It also opens the door to volumetric feature generation on a much larger scale than the current state of the art.

### Disclaimers:
* This code is **not** the one which produced the scenes seen in the paper. Everything has been *recoded* on my side to make sure it is free to use and doesn't break copyrights. Hence, the results may slightly differ from the ones in the paper, but the proof of concept is here.
* This is **research** code developed mostly by a junior research engineer (me) during my master's degree internship. There are definitly bugs and things that could have been done in a much better way. This is not production-level code and never will be.

### Features
* Example scenes demonstrating some of the algorithms (Sea, Karsts, Islands)
* Geology tree and Terrain tree framework, with most of the primitives described in the paper.
* Shallow erosion method, karst invasion-percolation process.
* A public domain Marching-cubes (...)
* Documentation and a simple mix of C/C++.

### Testing
The makefile as well as the Visual Studio project files are embedded in the repository to facilitate the testing. Please report any issue you may have so that I can fix it. The code has been tested on these configurations:
* Windows 10, SDK 10.0.16299.0, Visual Studio 2017. Double click on the solution + Ctrl/F5 to run.
* Ubuntu 16.X, make && make run.

The code will output an obj file which can then be visualized in another application, such as Blender or MeshLab.

### Credit and License
This code has been developed entirely by me (aparis69 github) and is based on the work I did during my internship at the Geomod Team, leading to an ACM-TOG publication (XX.com/pdf). It is license-free and can be modified or used by everyone, even commercialy (Do what you want with it). However, we require a **citation** of the published paper: [Link](test.pdf)
You can find more information about the authors of the paper at:
[Axel Paris](https://aparis69.github.io/index.html), [Eric Galin](https://perso.liris.cnrs.fr/eric.galin/), [Eric Guérin](https://perso.liris.cnrs.fr/eguerin/), [Adrien Peytavie](https://perso.liris.cnrs.fr/apeytavi/publications-hal/), [James Gain](https://people.cs.uct.ac.za/~jgain/)

### Missing
There is still some things missing from the paper implementation, mostly due to time constraints from me. They might be added in the future, if someone is interested. What is not in the code:
* Optimized Marching-cubes
* Canyon scene
* Hoodoos
