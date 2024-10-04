# morphologica_template

This example project demonstrates how you
can write and build a program that uses [morphologica](https://github.com/ABRG-Models/morphologica) as a library

Really, this project is just one `CMakeLists.txt` file containing the
commands required to use morphologica and a single target (prog1),
which compiles the example program `prog1.cpp`.

To make your own program, you could either replace
prog1.cpp with your own code, or incorporate the relevant parts of the
CMakeLists.txt file into your own CMakeLists.txt in another project.

## RSECon24 Workshop

If you are attending the Research Software Engineering Conference [workshop on Sept 3 2024](https://virtual.oxfordabstracts.com/#/event/49081/submission/78) in Newcastle, please install the dependencies before the meeting! This is especially important if you wish to use your Mac or a non-Debian/Ubuntu Linux distro. Feel free to ask for help with this beforehand on this repository's [Issues](https://github.com/ABRG-Models/morphologica_template/issues).

## Dependencies

If you are using Debian or Ubuntu, the following `apt` command should
install the morphologica dependencies. Note that `libarmadillo-dev`
and `libhdf5-dev` are optional. They're not used by `prog1.cpp` but they
do allow all the morphologica headers to be used in this template.

```bash
sudo apt install build-essential cmake git wget  \
                 freeglut3-dev libglu1-mesa-dev libxmu-dev libxi-dev \
                 libglfw3-dev libfreetype-dev libarmadillo-dev libhdf5-dev
```

On Arch Linux the following command should install dependencies:
```bash
sudo pacman -S vtk lapack blas freeglut glfw-wayland
```

On Fedora Linux, the following command should install the required dependencies
```bash
sudo dnf install gcc cmake libglvnd-devel mesa-libGL-devel glfw-devel \
                 freetype-devel armadillo-devel hdf5-devel
```

I'd love to know the equivalents for other Linux distributions so I
can include them in the morphologica documentation, so if you know,
please pull-request them!

If you're building on a Mac, you can refer to the [Mac
README](https://github.com/ABRG-Models/morphologica/blob/main/README.build.mac.md#installation-dependencies-for-mac)
for help. You only need to obtain and build
[glfw3](https://github.com/ABRG-Models/morphologica/blob/main/README.build.mac.md#glfw3);
OpenGL and Freetype should already be installed by default.

## Building

To build and run the example:

```bash
# Clone this example
git clone git@github.com:ABRG-Models/morphologica_template # or your fork of it

# Clone, copy or symlink morphologica INSIDE your example:
cd morphologica_template # or whatever you named your fork/copy
git clone git@github.com:ABRG-Models/morphologica

# Build prog1 in a 'build' directory
mkdir build
cd build
cmake ..
make
./prog1 # You should see a window containing some graphs
```
