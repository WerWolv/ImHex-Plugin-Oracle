# ImHex C++ Plugin Template

This repository is a template repository for building plugins for ImHex in C++.

## Building

Building a plugin works similarly to building ImHex. Make sure you have all dependencies installed that are necessary to build ImHex itself. Afterwards simply use cmake in the top level of this repository to build libimhex and the plugin. Consult the ImHex README.md for a more in-depth guide on how to compile ImHex using cmake.

## Notice

Plugins can depend on the ImHex version you're using. To find out what commit the plugin was built for, check the commit hash next to the ImHex submodule.

![image](https://user-images.githubusercontent.com/10835354/144566767-31c39e72-2d08-42b1-a38b-dcba44939ac9.png)
