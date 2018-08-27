#include <limits>
#include "utilities.h"

using namespace std;

utilities::utilities()
{

}

double utilities::minValueFrom(vector<double> values)
{
    // Get the minimum value from a vector of doubles

    double minVal;

    minVal = values.at(0);
    for (size_t i=0; i < values.size(); i++) {
        if (values.at(i) < minVal)
            minVal = values.at(i);
    }
    return minVal;
}

double utilities::maxValueFrom(vector<double> values)
{
    // Get the maximum value from a vector of doubles

    double maxVal;

    maxVal = values.at(0);
    for (size_t i=0; i < values.size(); i++) {
        if (values.at(i) > maxVal)
            maxVal = values.at(i);
    }
    return maxVal;
}

double utilities::interpolx(double x1, double y1, double x2, double y2, double y)
{
    /* Linear interpolation fo to find x-axis coordinate
     *
     * Finds an unknown x-axis coordinate between two points (x1, y1) and (x2, y2) for a
     * given known y-axis coordinate using similar triangles
     *
     */

    // Cannot perform an interpolation when x1 == x2, just return the same value
    if (x1 == x2)
        return x1;

    // WARNING! Don't return NaN, instead return valid numbers to keep the program running
    if (!validatePoints(x1, y1, x2, y2)) {
        // Return Not a Number
        //return numeric_limits<double>::quiet_NaN();

        // If both points are the same, return any x-axis value
        return x1;
    }

    // Cannot perform an interpolation when y1 = y2 != y
    if ((y1 == y2) && (y1 != y)) {
        // Return Not a Number
        //return numeric_limits<double>::quiet_NaN();

        // If the points have the same y-axis value, return the average of the x-axis values
        return (x1 + x2) / 2.;
    }

    // Perform the linear interpolation and return the correct x-axis value
    return ((x2 - x1)/(y2 - y1)) * (y - y1) + x1;
}

double utilities::interpoly(double x1, double y1, double x2, double y2, double x)
{
    /* Linear interpolation fo to find y-axis coordinate
     *
     * Finds an unknown y-axis coordinate between two points (x1, y1) and (x2, y2) for a
     * given known x-axis coordinate using similar triangles
     *
     */

    // Cannot perform an interpolation when x1 == x2, just return the average of the y-axis coordinates
    if (x1 == x2)
        return (y1 + y2) / 2.;

    // WARNING! Don't return NaN, instead return valid numbers to keep the program running
    if (!validatePoints(x1, y1, x2, y2)) {
        // Return Not a Number
        //return numeric_limits<double>::quiet_NaN();

        // If both points are the same, return any y-axis value
        return y1;
    }

    // Perform the linear interpolation and return the correct y-axis value
    return ((y2 - y1)/(x2 - x1)) * (x - x1) + y1;
}

bool utilities::validatePoints(double x1, double y1, double x2, double y2)
{
    /* Validate two points (x1,y1) and (x2,y2) to perform an interpolation
     *
     * So far the next conditions are checked:
     *
     * - Whether or not the points are the same (included in the condition below)
     * - Whether or not the x-axis coordinates are the same
     *
     */

    // Exact equal points share same x-axis and y-axis coordinates and are invalid
    // cannot perform an interpolation with these points.
    if ((x1 == x2) && (y1 == y2))
        return false;

    return true;
}
