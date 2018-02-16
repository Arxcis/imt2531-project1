## 1. Setup development environment - macOS and Ubuntu

### Running the python tool 

Make sure you have `python3` correctly installed
```
$ /usr/local/bin/python3 --version
Python 3.6.4
```
To run `./exec.py`
```
$ ./exec.py --help
usage: exec.py [-h] [-r] [-i] [--ssh]

optional arguments:
  -h, --help     show this help message and exit
  -r, --run      run only
  -i, --install  install required external libraries
  --ssh          use ssh when git cloning during installation

```

### Manually install required external libraries  

1. Go to [LAB03](http://prod3.imt.hig.no/imt2531/imt2531_lectures/tree/master/lab03).
2. Copy the `GLFW/`, `GLEW/`, and `GLM/` to the root directory of project.
3. Download `stb_image.h` form [here](https://github.com/nothings/stb/blob/master/stb_image.h) and place it in `stb/stb_image.h`.


## 2. References
opengl error handling - https://www.khronos.org/opengl/wiki/OpenGL_Error#Testing_for_errors - 16.02.18 <br>
modern opengl youtube tutorial - https://youtu.be/csKrVBWCItc - 15.02.18 <br> 
loadShader files - https://badvertex.com/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c.html  - 12.02.18  <br>
glPoints - https://solarianprogrammer.com/2013/05/13/opengl-101-drawing-primitives/ - 12.02.18  <br>
glBufferSubData - https://www.khronos.org/opengl/wiki/Buffer_Object#Data_Specification - 04.02.18 <br>
Texture/texture-blending tutorial - https://learnopengl.com/Getting-started/Textures 04.02.18 <br>
Texturing a cube - http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/ - 04.02.18 <br>
Texture kittens OpenGL tutorial - https://open.gl/textures - 04.02.18 <br>
Texture sampler GLSL - https://www.khronos.org/opengl/wiki/Sampler_(GLSL)#Binding_textures_to_samplers - 03.02.18 <br>
VBO's and VAO's  - http://headerphile.com/sdl2/opengl-part-2-vertexes-vbos-and-vaos/ - 02.02.18  <br>