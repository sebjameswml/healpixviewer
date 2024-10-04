/*
 * View data from a FITS file
 *
 * I copied some code (the lines for reading a fits file) from
 * https://github.com/lpsinger/healpixviewer
 *
 * Author Seb James
 * Date 2024/10/04
 */

#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <morph/Visual.h>
#include <morph/HealpixVisual.h>
#include <morph/Config.h>
#include <chealpix.h>

int main (int argc, char** argv)
{
    // Pass a path to a fits file on the cmd line
    std::string fitsfilename("");
    if (argc > 1) { fitsfilename = std::string(argv[1]); }
    if (fitsfilename.empty()) {
        std::cout << "Usage: " << argv[0] << " path/to/fitsfile" << std::endl;
        return -1;
    }

    // Read data from the fits file
    int64_t nside = 0;
    char coordsys[80]; // Might be "C" or "GALACTIC". Not sure what to do with this though!
    char ordering[80]; // NESTED
    // read_healpix_map comes from chealpix. It allocates memory
    float* hpmap = read_healpix_map (fitsfilename.c_str(), &nside, coordsys, ordering);
    if (!hpmap) {
        std::cout << "Failed to read the healpix map at " << fitsfilename << std::endl;
        return -1;
    }
    std::cout << "coordsys: " << coordsys << "\nordering: " << ordering << std::endl;

    int32_t ord = 0;
    int32_t _n = nside;
    while ((_n >>= 1) != 0) { ++ord; } // Finds order as long as nside is a square (which it will be)

    // Create a visual scene/window object
    morph::Visual v(1024, 768, "Healpix FITS file viewer");

    // Create a HealpixVisual
    auto hpv = std::make_unique<morph::HealpixVisual<float>> (morph::vec<float>{0,0,0});
    v.bindmodel (hpv);
    hpv->set_order (ord);

    // Convert the data read by read_healpix_map to nest ordering if necessary and write into the
    // healpix visual's pixeldata
    hpv->pixeldata.resize (hpv->n_pixels());
    if (ordering[0] == 'R') { // R means ring ordering
        for (int64_t i_nest = 0; i_nest < hpv->n_pixels(); i_nest++) {
            int64_t i_ring = hp::nest2ring (nside, i_nest);
            hpv->pixeldata[i_nest] = hpmap[i_ring];
        }
    } else { // Assume NEST ordering, so simply copy
        for (int64_t i_nest = 0; i_nest < hpv->n_pixels(); i_nest++) {
            hpv->pixeldata[i_nest] = hpmap[i_nest];
        }
    }
    // Can now free the memory read by read_healpix_map
    free (hpmap);

    std::cout << "pixeldata range: " << hpv->pixeldata.range() << std::endl;

    // Use relief to visualize
    hpv->relief = true;

    hpv->colourScale.reset();
    hpv->reliefScale.reset();

    // Set a colour map
    hpv->cm.setType (morph::ColourMapType::Jet);

    // Manually compute scaling:
    hpv->colourScale.do_autoscale = false;
    hpv->colourScale.compute_autoscale (-0.0005, 0.0005);

    hpv->reliefScale.do_autoscale = false;
    hpv->reliefScale.output_range.set(-0.1f, 0.1f);
    hpv->reliefScale.compute_autoscale (-0.002, 0.002);

    std::stringstream ss;
    constexpr bool centre_horz = true;
    ss << ord << (ord == 1 ? "st" : (ord == 2 ? "nd" : (ord == 3 ? "rd" : "th")))
       << " order HEALPix data from " << fitsfilename << "\n";
    hpv->addLabel (ss.str(), {0.0f, -1.2f , 0.0f }, morph::TextFeatures{0.08f, centre_horz});

    // Finalize and add the model
    hpv->finalize();
    v.addVisualModel (hpv);

    v.keepOpen();
    return 0;
}
