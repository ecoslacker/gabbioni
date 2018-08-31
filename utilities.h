#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>

using namespace std;

class utilities
{
public:
    utilities();
    static double minValueFrom(vector<double> values);
    static double maxValueFrom(vector<double> values);
    static double interpolx(double x1, double y1, double x2, double y2, double y);
    static double interpoly(double x1, double y1, double x2, double y2, double x);
    static bool validatePoints(double x1, double y1, double x2, double y2);
};

#endif // UTILITIES_H
