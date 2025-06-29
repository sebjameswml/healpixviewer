# HEALPix Viewer program

![A view of Earth's topograpy and the cosmic microwave background radiation](https://github.com/sebjameswml/healpixviewer/blob/main/images/Earth_and_CMBR.png?raw=true)

This is a simple HEALPix viewer, which uses the [`morph::HealpixVisual`](https://github.com/ABRG-Models/morphologica/blob/main/morph/HealpixVisual.h) class from [morphologica](https://github.com/ABRG-Models/morphologica).

It plots your scalar-valued HEALPix dataset on the sphere using a colour map *and* optionally relief to indicate value. There is a wide choice of colour maps (including those from [CET](https://colorcet.com), matplotlib and [Fabio Crameri](https://www.fabiocrameri.ch/colourmaps/)) which, along with the relief scaling, can be configured via a simple JSON file.

## Dependencies

If you are using Debian or Ubuntu, the following `apt` command should
install the morphologica dependencies as well as the official Healpix
C library.

```bash
sudo apt install build-essential cmake git wget  \
                 nlohmann-json3-dev librapidxml-dev \
                 freeglut3-dev libglu1-mesa-dev libxmu-dev libxi-dev \
                 libglfw3-dev libfreetype-dev libchealpix-dev
```

On Arch Linux, this should be the `pacman` command:
```bash
sudo pacman -S vtk lapack blas freeglut glfw-wayland nlohmann-json chealpix
```

On Fedora Linux, the following `dnf` command should install the dependencies:
```bash
sudo dnf install gcc cmake libglvnd-devel mesa-libGL-devel glfw-devel \
                 json-devel rapidxml-devel \
                 freetype-devel cfitsio-dev chealpix-dev
```

If you're building on a Mac, you can refer to the [Mac
README](https://github.com/ABRG-Models/morphologica/blob/main/README.build.mac.md#installation-dependencies-for-mac)
for help. You only need to obtain and build
[glfw3](https://github.com/ABRG-Models/morphologica/blob/main/README.build.mac.md#glfw3);
OpenGL and Freetype should already be installed by default.

On Windows, you can use vcpkg to install morphologica and its
dependencies and you should be able to compile with Visual Studio. If
the program runs slowly, try rebuilding in Release mode rather than
Debug mode.

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

I also managed to open cosmic microwave background data from the [Planck Legacy Archive](http://pla.esac.esa.int/pla/#home).
Again, this uses a JSON file to set up the input data scaling.

```bash
wget http://pla.esac.esa.int/pla-sl/data-action?MAP.MAP_OID=13486 -O cmb.fits
./build/viewer cmb.fits
```

These skymaps from the Planck Legacy Archive also appear to work
```bash
wget http://pla.esac.esa.int/pla-sl/data-action?MAP.MAP_OID=13749 -O skymap.fits
./build/viewer skymap.fits

wget http://pla.esac.esa.int/pla-sl/data-action?MAP.MAP_OID=13612 -O skymap2.fits
./build/viewer skymap2.fits
```

## The Config file

This small application has no user interface. As in all my scientific visualization programs, I use a simple configuration file to set options, using the JSON format.
When you run the program for **file.fits** it will attempt to open **file.fits.json** and read parameters from that file.

The **earth-2048.fits.json** example looks like this (without the comments in the file):

```json
{
    "colourmap_input_range" : [ -400, 8000 ],
    "colourmap_type" : "Batlow",
    "use_relief" : true,
    "reliefmap_input_range" : [ -400, 8000 ],
    "reliefmap_output_range" : [-0.00065, 0.0013 ],
    "order_reduce" : 1
}
```

Briefly, you use `colour/reliefmap_input_range` and `reliefmap_output_range` to control the colour and relief data scaling.
Here, a range of values from -400 to 8000 are scaled to [0, 1] for the colourmap (this is never changed) and to [-0.00065, 0.0013] for relief (assuming `use_relief` is true).
The units for relief are in arbitrary length units in the 3D scene.
These should relate to the base radius of the HEALPix sphere, which is 1.

`order_reduce` allows you to reduce the order of the visualization with respect to your data.
Here, I reduce from an 11th order/nside=2048 HEALPix dataset to a 10th order/nside=1024 visualization.
This averages the values in groups of 4 pixels down to 1.
If your GPU and RAM can do it, you can change this to 0, or you can order_reduce by more.

`colourmap_type` allows you to choose from about 80 colour maps in morphologica, which includes maps from [Crameri](https://www.fabiocrameri.ch/colourmaps/), [CET](https://colorcet.com/), [matplotlib](https://matplotlib.org/stable/users/explain/colors/colormaps.html) and [W Lenthe](https://github.com/wlenthe/UniformBicone).
To find out the names you can use, see [ColourMap documentation](https://abrg-models.github.io/morphologica/ref/visual/colourmap) and `enum class ColourMapType` in [the code](https://github.com/ABRG-Models/morphologica/blob/main/morph/ColourMap.h#L17).

You can override any of the fields on the command line. Try:

```bash
./build/viewer earth-2048.fits -co:colourmap_type=inferno
```

Take a look at the example files **cmb.fits.json**, **bayestar.fits.gz.json** and **earth-2048.fits.json**. It should be easy to see how to adapt these to your chosen data.
