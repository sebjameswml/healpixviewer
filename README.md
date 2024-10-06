# HEALPix Viewer program

![A view of the cosmic microwave background radiation](https://github.com/sebjameswml/healpixviewer/blob/main/images/Healpix_FITS_file_viewer.png?raw=true)

This is a simple HEALPix viewer, which uses the [`morph::HealpixVisual`](https://github.com/ABRG-Models/morphologica/blob/main/morph/HealpixVisual.h) class from [morphologica](https://github.com/ABRG-Models/morphologica).

## Dependencies

If you are using Debian or Ubuntu, the following `apt` command should
install the morphologica dependencies as well as the official Healpix
C library.

```bash
sudo apt install build-essential cmake git wget  \
                 freeglut3-dev libglu1-mesa-dev libxmu-dev libxi-dev \
                 libglfw3-dev libfreetype-dev libchealpix-dev
```

On Arch Linux, this should be the `pacman` command:
```bash
sudo pacman -S vtk lapack blas freeglut glfw-wayland chealpix
```

On Fedora Linux, the following `dnf` command should install the dependencies:
```bash
sudo dnf install gcc cmake libglvnd-devel mesa-libGL-devel glfw-devel \
                 freetype-devel cfitsio-dev chealpix-dev
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

Topographic data of the Earth makes lovely example data:

```bash
wget https://www.sfu.ca/physics/cosmology/healpix/data/earth-2048.fits
./build/viewer earth-2048.fits
```
Note that there is a JSON config file that matches this filename.

You can open the Bayestar data that @lpsinger uses in his viewer (there's a .json file for this one too):

```bash
wget --no-check-certificate http://ligo.org/science/first2years/2015/compare/18951/bayestar.fits.gz
./build/viewer bayestar.fits.gz
```

I also managed to open cosmic microwave background data from the [Planck Legacy Archive](http://pla.esac.esa.int/pla/#home). Again, this uses a JSON file to set up the input data scaling.

```bash
wget http://pla.esac.esa.int/pla-sl/data-action?MAP.MAP_OID=13486 -O cmb.fits
./build/viewer cmb.fits
```

These skymaps from the Planck Legacy Archive also appear to work, though I've not got configs for them yet.
```bash
wget http://pla.esac.esa.int/pla-sl/data-action?MAP.MAP_OID=13749 -O skymap1.fits
./build/viewer skymap1.fits

wget http://pla.esac.esa.int/pla-sl/data-action?MAP.MAP_OID=13612 -O skymap2.fits
./build/viewer skymap2.fits
```

## The Config file

This sets a few parameters for the visualization in JSON, which is read at runtime. Take a look at the examples cmb.fits.json, bayestar.fits.gz.json and earth-2048.fits.json. It should be easy to see how to adapt these to your chosen data.