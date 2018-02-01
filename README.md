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

Make sure you have `python3` correctly installed
```
$ /usr/local/bin/python3 --version
Python 3.6.4
```
To run `./exec.py`
```
$ ./exec.py --help
usage: exec.py [-h] [-b] [-r]

optional arguments:
  -h, --help   show this help message and exit
  -b, --build  build only
  -r, --run    run only
```
