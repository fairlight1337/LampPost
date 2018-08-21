### Lamp Post

| Travis CI | Appveyor | Coveralls |
|:---:|:---:|:---:|
| [![Build Status](https://travis-ci.org/fairlight1337/LampPost.svg?branch=master)](https://travis-ci.org/fairlight1337/LampPost) | [![Build status](https://ci.appveyor.com/api/projects/status/hy51dcigae860x1d/branch/master?svg=true)](https://ci.appveyor.com/project/fairlight1337/lamppost/branch/master) | [![Coverage Status](https://coveralls.io/repos/github/fairlight1337/LampPost/badge.svg?branch=master&service=github)](https://coveralls.io/github/fairlight1337/LampPost?branch=master) |

#### Building from Source

##### Getting the required tools

Required build tools:
 * automake
 * autoconf
 * libtool

###### On Ubuntu

```bash
sudo apt-get install automake autoconf libtool
```

###### On Windows

You have to install MinGW in order to get `make` (required for building Intel TBB):
 * [https://code.google.com/archive/p/mingw-offline-install/downloads/](https://code.google.com/archive/p/mingw-offline-install/downloads/)

And install CMake:
 * [https://cmake.org/download/](https://cmake.org/download/)

Both have to be added to your `Path` environmental variables (see [here](https://stackoverflow.com/questions/9546324/adding-directory-to-path-environment-variable-in-windows#comment14126970_9546345) for a clue on how to do that).

##### Building in-tree

On Ubuntu, in the source directory do:
```bash
mkdir build
cd build
cmake ..
make
```
