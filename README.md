# Assignment Repository Skeleton

Please modify the Readme file to your project specifics, including project name, involved group members (including mail contacts), as well as a version history.

In addition, use this file to describe the project functionality, provide setup instructions, and a brief overview on how to use it.


# Setup development environment - macOS and Ubuntu

### How to Install libraries inside here
```
/usr/local/include   # header files
/usr/local/lib       # binary files
```

**macOS** - `brew install glfw glm glew`
 
**Ubuntu**

1. Download the source code of `glfw`, `glm`, `glew` directly from github.
    * [https://github.com/glfw/glfw/releases](https://github.com/glfw/glfw/releases)
    * [https://github.com/nigels-com/glew/releases](https://github.com/nigels-com/glew/releases)
    * [https://github.com/g-truc/glm/tags](https://github.com/g-truc/glm/tags)

2. Unzip source code.
3. Go to source code root folder.
3. Run `cmake .`,  to generate makefiles for your OS.
4. ..then `make`, to build libraries.
5. ..then `sudo make install`, to install libraries to `/usr/local/include` and `/usr/local/lib`.


### How to run build-scripts

Make sure you have `python3`.

**Python3**
```
$ /usr/local/bin/python3 --version
Python 3.6.4
```
To run `./exec.py`

