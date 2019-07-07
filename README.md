## Terrain Amplification with Implicit 3D Features
Source code for some results shown in the paper "Terrain Amplification with Implicit 3D Features" published in ACM-TOG in 2019. 
This is aimed at researchers, students or profesionnals who may want to reproduce **some** of the results described in the paper.
For more information about the project: [Project page](https://aparis69.github.io/projects/paris2019_3D.html)

### Important note
* This code is **not** the one which produced the scenes seen in the paper. Everything has been *recoded* on my side to make sure it is free to use and doesn't break copyrights. 
Hence, the results may slightly differ from the ones in the paper.
* This is **research** code provided without any warranty. However, if you have any problem you can still send me an email or create an issue.

### Testing
* Visual Studio 2017: double click on the solution in VS2017 folder + Ctrl/F5 to run.
* Ubuntu 16.0: make && make run in the G++ folder.

The code will output 3 obj files which can then be visualized in another application (Blender, MeshLab...)

### Citation
You can use this code in any way you want, however please credit the original article:
```
@article {paris2019,
    author = {Guérin, Eric and Digne, Julie and Galin,
              Eric and Peytavie, Adrien and Wolf, Christian
              and Benes, Bedrich and Martinez, Benoit},
    title = {Interactive Example-Based Terrain Authoring
             with Conditional Generative Adversarial Networks},
    journal = {ACM Transactions on Graphics
               (proceedings of Siggraph Asia 2017)},
    volume = {36},
    number = {6},
    year = {2017},
  }
```	

### Missing
There is still some things missing from the paper implementation, mostly due to time constraints from me. They might be added in the future if someone is interested. What is not in the code:
* Optimized Marching-cubes
* Canyon erosion
* Hoodoos shape-grammar methods
