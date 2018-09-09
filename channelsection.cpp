//#include <iomanip>
//#include "assert.h"
#include "channelsection.h"

const size_t MIN_DATA = 3;  // Minimum data points to make a channel cross-section
const int MIN_LEVELS = 1;  // Minimum depth levels that should have the channel cross-section

ChannelSection::ChannelSection(std::vector<double> coord_x, std::vector<double> coord_y, double layersHeight)
{
    _coordX = coord_x;
    _coordY = coord_y;
    _layersHeight = layersHeight;

    // Initialize report
    _strChannel = "";
}

int ChannelSection::levels() const
{
    return _levels;
}

double ChannelSection::channelDepth() const
{
    return _channelDepth;
}

double ChannelSection::layersHeight() const
{
    return _layersHeight;
}

void ChannelSection::setLayersHeight(double layersHeight)
{
    _layersHeight = layersHeight;
}

double ChannelSection::channelWidth() const
{
    return _channelWidth;
}

std::vector<double> ChannelSection::widths() const
{
    return _widths;
}

std::vector<double> ChannelSection::rightX() const
{
    return _rightX;
}

std::vector<double> ChannelSection::leftX() const
{
    return _leftX;
}

std::vector<double> ChannelSection::depthLevels() const
{
    return _depthLevels;
}

double ChannelSection::discharge() const
{
    return _discharge;
}

double ChannelSection::coefficient() const
{
    return _coefficient;
}

void ChannelSection::setCoefficient(double coefficient)
{
    _coefficient = coefficient;
}

double ChannelSection::slope() const
{
    return _slope;
}

void ChannelSection::setSlope(double slope)
{
    _slope = slope;
}

double ChannelSection::waterHeight() const
{
    return _waterHeight;
}

void ChannelSection::setWaterHeight(double waterHeight)
{
    _waterHeight = waterHeight;
}

std::vector<double> ChannelSection::coordX() const
{
    return _coordX;
}

void ChannelSection::setCoordX(const std::vector<double> &coordX)
{
    _coordX = coordX;
}

std::vector<double> ChannelSection::coordY() const
{
    return _coordY;
}

void ChannelSection::setCoordY(const std::vector<double> &coordY)
{
    _coordY = coordY;
}

double ChannelSection::hydraulicArea() const
{
    return _hydraulicArea;
}

void ChannelSection::setHydraulicArea(double hydraulicArea)
{
    _hydraulicArea = hydraulicArea;
}

double ChannelSection::wettedPerimeter() const
{
    return _wettedPerimeter;
}

void ChannelSection::setWettedPerimeter(double wettedPerimeter)
{
    _wettedPerimeter = wettedPerimeter;
}

double ChannelSection::hydraulicRadius() const
{
    return _hydraulicRadius;
}

void ChannelSection::setHydraulicRadius(double hydraulicRadius)
{
    _hydraulicRadius = hydraulicRadius;
}

double ChannelSection::flowVelocity() const
{
    return _flowVelocity;
}

void ChannelSection::setFlowVelocity(double flowVelocity)
{
    _flowVelocity = flowVelocity;
}

std::string ChannelSection::strChannel() const
{
    return _strChannel;
}

bool ChannelSection::isAValidSection()
{
    /* === === === === Checks whether or not the channel cross-section is valid === === === ===
     *
     * A valid section has:
     *  - At least three different coordinate points (x,y)
     *  - No negative values
     *  - Sorted x-axis values from left to right (is this valid to reverse order?)
     *  - The first and last y-values should be zero
     *  - NOTE: The above conditions are handled by the checkCoordinates method.
     *  -
     * === === === === === === === === === === === === === === === === === === === === === === */

    if (!checkCoordinates())
        return false;

    // Check if the 'shape' of the channel cross-section is valid
    return true;
}

double ChannelSection::streambed() const
{
    return _streambed;
}

bool ChannelSection::extractInformation()
{
    // Check the coordinates and extract useful information from the channel section
    if (isAValidSection()) {
        // Get the maximum value of depth
        _channelDepth = utilities::maxValueFrom(_coordY);
        _levels = static_cast<int>(_channelDepth / _layersHeight);
        _channelWidth = _coordX.at(_coordX.size() - 1) - _coordX.at(0);

        // Get the depth of the channel at each level
        if (!computeWidths())
            return false;

        // Get the center of the stream bed
        if (_widths.at(0) == 0)
            _streambed = _leftX.at(0);
        else
            _streambed = _leftX.at(0) + ((_rightX.at(0) - _leftX.at(0)) / 2);

    } else {
        std::cout << "Error checking coordinates." << std::endl;
        return false;
    }

    // Create report
    _strChannel = "Channel cross section info:\n";
    _strChannel += "x-coord\ty-coord\t\n";
    for (size_t i=0; i < _coordX.size(); i++) {
        _strChannel += std::to_string(_coordX.at(i)) + "\t" + std::to_string(_coordY.at(i)) + "\t\n";
    }
    _strChannel += "\nDepth:\t" + std::to_string(_channelDepth) + "\t\n";
    _strChannel += "Width:\t" + std::to_string(_channelWidth) + "\t\n";
    _strChannel += "Layers height:\t" + std::to_string(_layersHeight) + "\t\n";
    _strChannel += "Levels:\t" + std::to_string(_levels) + "\t\n";
    _strChannel += "Stream bed at:\t" + std::to_string(_streambed) + "\t\n";
    return true;
}

bool ChannelSection::computeDischarge()
{
    /* Compute the discharge
     *
     * Compute the channel's dicharge (flow rate) using its geometry and Manning's equation
     *
     */

    // Check the coefficient value
    if (_coefficient <= 0)
        return false;

    // Get the channel's flow geometry
    flowGeometry();

    // Get the discharge using Chezy-Manning's equation
    _discharge = chezyManning(_coefficient, _slope);
    return true;
}

bool ChannelSection::checkCoordinates()
{
    /* Check the coordinates data
     *
     * Both (x,y) coordinate lists should have the same length and have a minimum set
     * of coordinates required to design.
     *
     */

    // Both coordinate vectors should have the same length
    if (_coordX.size() != _coordY.size())
        return false;

    if (_coordX.size() < MIN_DATA) {
        std::cout << "No minimum data" << std::endl;
        return false;
    }

    // Negative values are not allowed in y-axis values
    // Should negative values be allowed in x-axis value?
    for (size_t i=0; i < _coordY.size(); i++) {
        if (_coordY.at(i) < 0 || _coordX.at(i) < 0)
            return false;
    }


    // Check if the x-axis coordinates are sorted from top to bottom
    // Which means the x-axis start from left to right (or right to left?)
    double prev = _coordX.at(0);
    for (size_t i=1; i < _coordX.size(); i++) {
        if (prev > _coordX.at(i)) {
            std::cout << "Error: data not sorted (" << i << ") < (" << i-1 << ")" << std::endl;
            return false;
        }
        prev = _coordX.at(i);
    }

    // First and last y-axis coordinates should be zero
    int ylast = int(_coordY.size()) - 1;
    int xlast = int(_coordX.size()) - 1;
    if (_coordY.at(0) != 0)
        return false;
    if (_coordY.at(ylast) != 0) {
        // Add a final coordinate (xlast, 0) to be sure
        _coordX.push_back(_coordX.at(xlast));
        _coordY.push_back(0);
    }
    return true;
}

bool ChannelSection::computeWidths()
{
    /*================================ Compute widths ==================================*
     *
     * Calculates the width of the channel cross section at each depth level.
     * Fisrt, needs to find the x-axis coordinates of the extreme points of the channel
     * at each depth level. Use linear interpolation when necessary.
     *
     *==================================================================================*/

    if (_levels < MIN_LEVELS) {
        std::cout << "Unable to compute widgets for " << _levels << " levels. Exiting..." << std::endl;
        return false;
    }

    double currentDepth;
    double x1, x2;
    double y1, y2;
    double left_x;
    double right_x;

    _depthLevels.clear();
    _widths.clear();
    _rightX.clear();
    _leftX.clear();

    for (int i=0; i <= _levels; i++) {
        // Initialize start and end points
        left_x = _coordX.at(_coordX.size()-1);
        right_x = _coordX.at(0);

        currentDepth = _channelDepth - (_layersHeight * i);
//        std::cout << "  Current depth: " << currentDepth << std::endl;

        // Find extreme points of the channel for the current depth level
        for (size_t j=1; j < _coordY.size(); j++) {
            x1 = _coordX.at(j-1);
            y1 = _coordY.at(j-1);
            x2 = _coordX.at(j);
            y2 = _coordY.at(j);

            //std::cout << "  x1: " << x1 << ", y1: " << y1 << std::endl;
            //std::cout << "  x2: " << x2 << ", y2: " << y2 << std::endl;

            // Assuming x-axis direction from left to right
            if ((y2 >= currentDepth) && (y1 <= currentDepth)) {
                // Find initial point for the current depth level
                if (utilities::interpolx(x1, y1, x2, y2, currentDepth) < left_x) {
                    left_x = utilities::interpolx(x1, y1, x2, y2, currentDepth);
                    //std::cout << "  left x-coord: " << start_x << std::endl;
                }
            }
            if ((y2 <= currentDepth) && (y1 >= currentDepth)) {
                // Find end point for the current depth level
                if (utilities::interpolx(x1, y1, x2, y2, currentDepth) > right_x) {
                    right_x = utilities::interpolx(x1, y1, x2, y2, currentDepth);
                    //std::cout << "  right x-coord: " << end_x << std::endl;
                }
            }
        }

        // WARNING! This assumes right_x >= left_x, maybe a conditional could be used?
        if (left_x > right_x)
            return false;

        // Add the current level including its corresponding witdh and (x,y) coordinates
        _depthLevels.push_back(currentDepth);
        _widths.push_back(right_x - left_x);
        _rightX.push_back(right_x);
        _leftX.push_back(left_x);
    }

    return true;
}

void ChannelSection::flowGeometry()
{
    flowGeometry(_hydraulicArea, _wettedPerimeter);
}

void ChannelSection::flowGeometry(double &flowArea, double &wettedPerimeter)
{
    /* Flow geometry
     *
     * Get the geometry from a channel's cross-section and a value of water height from the stream bed.
     * The geometry consists of:
     *
     * - Area of the water flow in the channel cross-section
     * - Wetted perimeter of the water flow
     *
     */

    double sum_area = 0;
    double sum_perimeter = 0;
    double x1, x2;
    double y1, y2;
    double base1, base2;

    // Water height can't be greather than channel depth
    if (_waterHeight >= _channelDepth)
        return;

    // Get the real water height
    _waterHeight = _channelDepth - _waterHeight;

    // Iterate over the channel's vertical divisions and get each area and perimeter
    for (size_t i = 1; i < _coordX.size(); i++) {
        double area = 0;       // Flow area of the section i
        double perimeter = 0;  // Wetted perimeter of the section i
        double h;
        double b;

        x1 = _coordX.at(i-1);
        y1 = _coordY.at(i-1);
        x2 = _coordX.at(i);
        y2 = _coordY.at(i);

//        std::cout << "  x1:"                   << setw(8) << x1 << ", y1:" << setw(8) << y1 << std::endl;
//        std::cout << "  x2:"                   << setw(8) << x2 << ", y2:" << setw(8) << y2 << std::endl;
//        std::cout << "  * Water at depth:\t\t" << setw(8) << _waterHeight  << std::endl;

        // When water height is between y1 and y2, find themissing  point by interpolation
        // in this case the area is computed from a triangle instead from a trapezium
        if ((y2 > _waterHeight) && (y1 < _waterHeight)) {
            // In this case, we need to find the first point
            x1 = utilities::interpolx(x1, y1, x2, y2, _waterHeight);
            y1 = _waterHeight;
            std::cout << "  -Point found:" << "\t(" << x1 << ", " << y1 << ")" << std::endl;
        }
        if ((y2 < _waterHeight) && (y1 > _waterHeight)) {
            // In this case, we need to find the second point
            x2 = utilities::interpolx(x1, y1, x2, y2, _waterHeight);
            y2 = _waterHeight;
            std::cout << "  -Point found: " << "\t(" << x2 << ", " << y2 << ")" << std::endl;
        }

        /* *** *** *** *** *** *** *** *** *** IMPORTANT! *** *** *** *** *** *** *** *** ***
         *
         * Now get trapezoid (a.k.a. trapezium) area, the followings asumptions are made:
         * a) At this point y1 >= water height and y2 >= water height.
         * b) The assumption when y1 == y2 > water height is haldled.
         * c) And when y1 == y2 == water height is also haldled, besides it's a particular condition in clause a).
         *
         * *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***/

        // Get the flow area of the section
        base1 = y1 - _waterHeight;
        base2 = y2 - _waterHeight;
        h = x2 - x1;
        area = trapeziumArea(base1, base2, h);

        // Get the wetted perimter by using the Pythagoras's theorem to get
        // the hypotenuse of the triangle formed by b and h, as follows
        if (y1 == y2) {
            perimeter = x2 - x1;
        } else {
            b = abs(base2 - base1);
            perimeter = sqrt(pow(b, 2) + pow(h, 2));
        }

        // If area is zero then, wetted perimeter should be zero too
        if (area == 0)
            perimeter = 0;

//        std::cout << "  * Area (" << i << "):\t\t"  << setw(8) << area      << std::endl;
//        std::cout << "  * Perim (" << i << "):\t\t" << setw(8) << perimeter << "\n" << std::endl;

        sum_area = sum_area + area;
        sum_perimeter = sum_perimeter + perimeter;
    }
//    std::cout << "  Total area:\t\t" << setw(8) << sum_area << "\n" << std::endl;

    // Assign the values to the reference arguments
    flowArea = sum_area;
    wettedPerimeter = sum_perimeter;

    // Assing the values to private variables
    _hydraulicArea = flowArea;
    _wettedPerimeter = wettedPerimeter;
    _hydraulicRadius = _hydraulicArea / _wettedPerimeter;

    return;
}

double ChannelSection::trapeziumArea(double base1, double base2, double height)
{
    if ((base1 == 0) && (base2 == 0))
        return 0;

    if ((base1 < 0) || (base2 < 0) || height <= 0)
        return 0;

    return ((base1 + base2) * height) / 2;
}

double ChannelSection::chezyManning(double area, double perimeter, double n, double slope)
{
    /********************************* Chezy-Manning equation **************************************
     *                                                                                             *
     * Computes a channel's discharge (flow rate) using its geometry and Chezy-Manning's equation. *
     *                                                                                             *
     * area:      hydraulic area of the channel's cross section [m^2].                             *
     * perimeter: wetted perimeter of the channel's cross section [m].                             *
     * n:         Manning's roughness coefficient [dimensionless]                                  *
     * slope:     channels longitudinal slope [m/m]                                                *
     *                                                                                             *
     * NOTE: This equation does not handle US units properly by itself, the user should input the  *
     * values in SI units properly.                                                                *
     *                                                                                             *
     ***********************************************************************************************/

    // Numerator according to units
    // double units = 1.49; // For US units
    double units = 1.0; // For SI units

    // Compute the hydraulic radius (R)
    double R = area / perimeter;

    // Compute the flow velocity using Manning's equation
    _flowVelocity = (units / n) * pow(R, 2./3.) * sqrt(slope);

    return (units / n) * area * pow(R, 2./3.) * sqrt(slope);
}

double ChannelSection::chezyManning(double n, double slope)
{
    return chezyManning(_hydraulicArea, _wettedPerimeter, n, slope);
}
