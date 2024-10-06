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
#include <morph/unicode.h>
#include <chealpix.h>

// I extend morph::Visual simply to modify the coordinate arrows object in the
// constructor so their labels are not simply x, y and z.
class MyVisual : public morph::Visual<>
{
public:
    MyVisual (int width, int height, const std::string& title) : morph::Visual<> (width, height, title)
    {
        using morph::unicode;
        this->coordArrows->clear();
        this->coordArrows->x_label = unicode::toUtf8 (unicode::lambda)+std::string("=0");
        this->coordArrows->y_label = unicode::toUtf8 (unicode::lambda) + std::string("=") + unicode::toUtf8 (unicode::pi) + ("/2");
        this->coordArrows->z_label = "N";
        this->coordArrows->initAxisLabels();
        this->coordArrows->reinit();
    }
};

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

    // Visualization parameters that might be set from JSON config
    int32_t order_reduce = 0;
    bool use_relief = false;
    std::string colourmap_type = "plasma";
    morph::range<float> colourmap_input_range; // use value to indicate autoscaling
    colourmap_input_range.search_init(); // sets min to numeric_limits<>::max and max to numeric_limits<>::lowest
    morph::range<float> reliefmap_input_range;
    reliefmap_input_range.search_init();
    morph::range<float> reliefmap_output_range (0, 0.1f);

    // Access config if it exists
    std::string conf_file = fitsfilename + ".json";
    std::cout << "Attempt to read JSON config at " << conf_file << "...\n";
    morph::Config conf (conf_file);
    if (conf.ready) {
        // Allow command line overrides. e.g. viewer file.fits -co:colourmap_type=viridis
        conf.process_args (argc, argv);
        order_reduce = conf.get<int32_t>("order_reduce", 0);
        use_relief = conf.get<bool>("use_relief", false);
        colourmap_type = conf.getString("colourmap_type", "plasma");
        morph::vvec<float> tmp_vvec = conf.getvvec<float> ("colourmap_input_range");
        if (tmp_vvec.size() == 2) {
            colourmap_input_range.set (tmp_vvec[0], tmp_vvec[1]);
            reliefmap_input_range.set (tmp_vvec[0], tmp_vvec[1]);
        }
        tmp_vvec = conf.getvvec<float> ("reliefmap_input_range");
        if (tmp_vvec.size() == 2) { reliefmap_input_range.set (tmp_vvec[0], tmp_vvec[1]); }
        tmp_vvec = conf.getvvec<float> ("reliefmap_output_range");
        if (tmp_vvec.size() == 2) { reliefmap_output_range.set (tmp_vvec[0], tmp_vvec[1]); }
    }

    // Now convert nside to order and check order_reduce
    int32_t ord = 0;
    int32_t _n = nside;
    while ((_n >>= 1) != 0) { ++ord; } // Finds order as long as nside is a square (which it will be)
    if (ord - order_reduce < 1) { throw std::runtime_error ("Can't drop order that much"); }

    // Create a visual scene/window object
    MyVisual v(1024, 768, "Healpix FITS file viewer");
    v.setSceneRotation (morph::Quaternion<float>{ float{0.5}, float{-0.5}, float{-0.5}, float{-0.5} });

    // Create a HealpixVisual
    auto hpv = std::make_unique<morph::HealpixVisual<float>> (morph::vec<float>{0,0,0});
    v.bindmodel (hpv);
    hpv->set_order (ord - order_reduce);
    // Could set radius in hpv if required, but seems simplest to leave it always as 1

    // Convert the data read by read_healpix_map to nest ordering if necessary and write into the
    // healpix visual's pixeldata
    float downdivisor = std::pow(4, order_reduce);
    float downmult = 1.0f / downdivisor;
    hpv->pixeldata.resize (hpv->n_pixels());
    if (ordering[0] == 'R') { // R means ring ordering
        for (int64_t i_nest = 0; i_nest < 12 * nside * nside; i_nest++) {
            int64_t i_ring = hp::nest2ring (nside, i_nest);
            int64_t i_nest_down = i_nest >> (2 * order_reduce);
            hpv->pixeldata[i_nest_down] += hpmap[i_ring] * downmult;
        }
    } else { // Assume NEST ordering, so simply copy
        for (int64_t i_nest = 0; i_nest < 12 * nside * nside; i_nest++) {
            int64_t i_nest_down = i_nest >> (2 * order_reduce);
            hpv->pixeldata[i_nest_down] += hpmap[i_nest] * downmult;
        }
    }
    // Can now free the memory read by read_healpix_map
    free (hpmap);

    std::cout << "pixeldata range: " << hpv->pixeldata.range() << std::endl;

    // Use relief to visualize?
    hpv->relief = use_relief;

    hpv->colourScale.reset();
    hpv->reliefScale.reset();

    // Set a colour map
    hpv->cm.setType (colourmap_type);

    // Automatically or manually compute colour scaling:
    if (colourmap_input_range.min == std::numeric_limits<float>::max()
        && colourmap_input_range.max == std::numeric_limits<float>::lowest()) {
        // Auto scale colur
        hpv->colourScale.do_autoscale = true;
    } else {
        hpv->colourScale.do_autoscale = false;
        hpv->colourScale.compute_autoscale (colourmap_input_range.min, colourmap_input_range.max);
    }

    // Determine whether to autoscale or use users config for relief map scaling output range
    if (reliefmap_output_range.min == std::numeric_limits<float>::max()
        && reliefmap_output_range.max == std::numeric_limits<float>::lowest()) {
        hpv->reliefScale.output_range.set (0, 0.1f);
    } else {
        hpv->reliefScale.output_range.set (reliefmap_output_range.min, reliefmap_output_range.max);
    }

    // Determine whether to autoscale or use users scaling for relief map scaling input range
    if (reliefmap_input_range.min == std::numeric_limits<float>::max()
        && reliefmap_input_range.max == std::numeric_limits<float>::lowest()) {
        hpv->reliefScale.do_autoscale = true;
    } else {
        hpv->reliefScale.do_autoscale = false;
        hpv->reliefScale.compute_autoscale (reliefmap_input_range.min, reliefmap_input_range.max);
    }

    std::stringstream ss;
    constexpr bool centre_horz = false;
    auto pord = ord - order_reduce;
    ss << ord << (ord == 1 ? "st" : (ord == 2 ? "nd" : (ord == 3 ? "rd" : "th")))
       << " order HEALPix data from " << fitsfilename << " plotted at "
       << pord << (pord == 1 ? "st" : (pord == 2 ? "nd" : (pord == 3 ? "rd" : "th"))) << " order\n";
    v.addLabel (ss.str(), {0.0f, 0.0f, 0.0f}, morph::TextFeatures{0.005f, centre_horz});

    // Finalize and add the model to the morph::Visual scene
    hpv->finalize();
    v.addVisualModel (hpv);

    v.keepOpen(); // Until user quits with Ctrl-q
    return 0;
}
