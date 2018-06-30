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

On Ubuntu, do:
```bash
sudo apt-get install automake autoconf libtool
```

##### Building in-tree

On Ubuntu, in the source directory do:
```bash
mkdir build
cd build
cmake ..
make
```
