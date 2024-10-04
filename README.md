# HEALPix Viewer program

This is a simple HEALPix viewer, which uses the [`morph::HealpixVisual`](https://github.com/ABRG-Models/morphologica/blob/main/morph/HealpixVisual.h) class from [morphologica](https://github.com/ABRG-Models/morphologica).

## Dependencies

If you are using Debian or Ubuntu, the following `apt` command should
install the morphologica dependencies.

```bash
sudo apt install build-essential cmake git wget  \
                 freeglut3-dev libglu1-mesa-dev libxmu-dev libxi-dev \
                 libglfw3-dev libfreetype-dev
```

On Arch Linux the following command should install dependencies:
```bash
sudo pacman -S vtk lapack blas freeglut glfw-wayland
```

On Fedora Linux, the following command should install the required dependencies
```bash
sudo dnf install gcc cmake libglvnd-devel mesa-libGL-devel glfw-devel freetype-devel
```

If you're building on a Mac, you can refer to the [Mac
README](https://github.com/ABRG-Models/morphologica/blob/main/README.build.mac.md#installation-dependencies-for-mac)
for help. You only need to obtain and build
[glfw3](https://github.com/ABRG-Models/morphologica/blob/main/README.build.mac.md#glfw3);
OpenGL and Freetype should already be installed by default.

## Building

To build and run the viewer:

```bash
git clone --recurse-submodules git@github.com:sebjameswml/healpixviewer

cd healpixviewer
mkdir build
cd build
cmake ..
make
./viewer path/to/fitfile.fit
```

## Finding some example data

You can open the Bayestar data that @lpsinger uses in his viewer:

```bash
wget http://ligo.org/science/first2years/2015/compare/18951/bayestar.fits.gz
./build/viewer bayestar.fits.gz
```

I also managed to open cosmic microwave background data from the [Planck Legacy Archive](http://pla.esac.esa.int/pla/#home). I'm not sure these make very good examples yet.

```bash
wget http://pla.esac.esa.int/pla-sl/data-action?MAP.MAP_OID=13749 -O skymap1.fits
./build/viewer skymap1.fits

wget http://pla.esac.esa.int/pla-sl/data-action?MAP.MAP_OID=13612 -O skymap2.fits
./build/viewer skymap2.fits
```
