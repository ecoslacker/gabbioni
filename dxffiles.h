#ifndef DXFFILES_H
#define DXFFILES_H

#include <string>
#include <vector>
#include "layer.h"

using namespace std;

class DxfFiles
{
public:
    DxfFiles(string fileName = "", double xscale=1.0, double yscale=1.0);
    double _yscale;
    double _xscale;
    string _dxfFile;
    vector<Layer> _dimensions;
    vector<double> _x;
    vector<double>_y;
    vector<Layer> _nappe;

    vector<Layer> dimensions() const;
    void setDimensions(const vector<Layer> &dimensions);
    vector<double> x() const;
    void setX(const vector<double> &x);
    vector<double> y() const;
    void setY(const vector<double> &y);
    vector<Layer> nappe() const;
    void setNappe(const vector<Layer> &nappe);
    bool writeDxfFile();
};

#endif // DXFFILES_H
