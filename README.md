3D-Viewer
=========

3D View is an experimental project that extends Cocos2d-x to be 3D capable.

# Purpose
The project's main motivation behind 3D Viewer is to study and learn Cocos2d-x's rendering pipeline and scene management.
Therefore, the files in this repositary mainly demonstrates how the 3D rendering can be implemented.


# Requirements
* The project is based on Cocos2d-x 3.2.
* The current version runs on Mac OS X. 
However, the idea of cross-platform is beared in mind, so you can run it on Windows as long as you implment the corresponding SGLViewImp.cpp.
[This link would be helpful.](http://www.cocos2d-x.org/wiki/OpenGL_View)


# How to Run
1. Downlaod Cocos2d-x 3.2.
2. Create a new Cocos2d-x project.
3. Override Class folder with all the files in this folder.


# Features Implemented
* VAO, VBO
* Shader support
* Multiple cameras capable (the use of SMatrixProvider
* Commanly used primitives.


# Features to Be Implemented
* Dynamic batching, 
by grouping the positions, normals, colors, transform, and texture coordinates of models that share the same shader program into one attribute VBO.
* Texture support
* CPU side off-screen culling
