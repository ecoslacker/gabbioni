#include <cmath>
#include <vector>
#include "hydrology.h"

const int LAND_USES = 14;
const int SOIL_TYPES = 3;

hydrology::hydrology()
{

}

double hydrology::runoffCoefficient(int landUse, int soilType, double rainfall)
{
    /************************************************************************************************
     * Computes the runoff coefficient
     *
     * Get the runoff coefficient of the watershed with land use, soil type, and precipitation data
     * This method is from Official Mexican Norm:  NOM-011-CONAGUA-2015.
     *
     * IMPORTANT: Only valid for annual precipitation values between 350 and 2150 mm.
     *
     * Valid land uses are:
     *  0 = "Bare ground"
     *  1 = "Crops"
     *  2 = "Vegetables or temporal meadow"
     *  3 = "Grain crops"
     *  4 = "Grassland (75% soil covered)"
     *  5 = "Grassland (50-75%)"
     *  6 = "Grassland (50%)"
     *  7 = "Forest (75% soil covered)"
     *  8 = "Forest (50-75%)"
     *  9 = "Forest (25-50%)"
     * 10 = "Forest (25%)"
     * 11 = "Urban"
     * 12 = "Pathways"
     * 13 = "Meadow"
     *
     * Valid soil types are:
     * 0 = "Pervious soil"
     * 1 = "Semi-pervious soil"
     * 2 = "Impervious soil"
     ************************************************************************************************/

    double k = -1;
    double runoffCoefficient = -1;
    std::vector< std::vector<double> > k_list;

    // Check the land use and soil type values
    if (landUse < 0 || landUse >= LAND_USES)
        return -1;
    if (soilType < 0 || soilType >= SOIL_TYPES)
        return -1;
    if (rainfall < 0)
        return -1;

    // Initialize the list of the parameter "k"
    k_list = { {0.26, 0.28, 0.30},
               {0.24, 0.27, 0.30},
               {0.24, 0.27, 0.30},
               {0.24, 0.27, 0.30},
               {0.14, 0.20, 0.28},
               {0.20, 0.24, 0.30},
               {0.24, 0.28, 0.30},
               {0.07, 0.16, 0.24},
               {0.12, 0.22, 0.26},
               {0.17, 0.26, 0.28},
               {0.22, 0.28, 0.30},
               {0.26, 0.29, 0.32},
               {0.27, 0.30, 0.33},
               {0.18, 0.24, 0.30}
             };

    // Get the parameter "k" from the table, according to land use and soil type
    k = k_list[size_t(landUse)][size_t(soilType)];

    // Finally get the runoff coefficient
    if (k <= 0.15) {
        runoffCoefficient = k * (rainfall - 250) / 2000;
    } else {
        runoffCoefficient = k * (rainfall - 250) / 2000 + (k - 0.15) / 1.5;
    }

    return runoffCoefficient;
}

double hydrology::rationalFormula(double coefficient, double intensity, double area)
{
    /* Rational formula
     *
     * Computes the peak discharge of an drainage watershed (or basin)
     *
     * About the units used:
     * - Watershed area should be in hectares [ha],
     * - Annual rainfall (precipitation) in milimeters [mm],
     * - Rainfall intensity in milimeters per hour [mm/h], and
     * - Peak discharge will be returned in cubic meters per second [m^3/s].
     *
     */

    double discharge = (coefficient * intensity * area) / 360;

    return discharge;

}

double hydrology::kirpich(double length, double rise)
{
    /* The Kirpich equation */

    // Concentration time in minutes
    double tc = ((0.000325 * std::pow(length, 0.77) / std::pow((rise / length), 0.385)) * 60);
    return tc;
}

double hydrology::giandotti(double length, double rise, double area)
{
    /* Use Giandotti equation. ¿for watersheds greater than 200 hectares? ¿is this true? */

    double t1 = 4 * std::sqrt(area / 100.0); // 100 ha = 1 km^2
    double t2 = 1.5 * (length / 1000.0);
    double t3 = std::sqrt((rise / length) * (length / 1000.0));

    // Concentration time in minutes
    double tc = (((t1 + t2) / (25.3 * t3)) * 60.0);
    return tc;
}
