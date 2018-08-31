/*
 * DXF Files
 *
 * A class to manage the creation of DXF files.
 *
 * This class takes the channel cross section and the dimensions of
 * a gabion dam, and creates a DXF file with it. This file is a representation
 * of the dam layers, nappe, and channel in the front, lateral and top views.
 * At the moment this class only creates ASCII DXF and does not retrieve any
 * information from DXF files.
 */
#include <iostream>
#include <fstream>
#include "dxffiles.h"
#include "utilities.h"

DxfFiles::DxfFiles(string fileName, double xscale, double yscale)
{
    _dxfFile = fileName;
    _xscale = xscale;
    _yscale = yscale;
}

vector<Layer> DxfFiles::nappe() const
{
    return _nappe;
}

void DxfFiles::setNappe(const vector<Layer> &nappe)
{
    _nappe = nappe;
}

bool DxfFiles::writeDxfFile()
{
    // Create the DXF file for writing
    fstream dxfWriter(_dxfFile, ios::out);

    // Try to open the file, exits if failure
    if (dxfWriter.fail()) {
        cerr << "Unable to open file " << _dxfFile << " for writing!";
        return false;
    }

    // Initialize the DXF file
    dxfWriter << 0 << endl
              << "SECTION" << endl
              << 2 << endl
              << "ENTITIES" << endl;

    // Set the data to the file, first write the channel cross section

    // ****** Draw the channel cross section ******

    if (_x.size() != _y.size()) {
        cerr << "Channel x-axis and y-axis list of coordinates have different length!";
        return false;
    }

    // Set a name for the layers of DXF file
    string lyrChannel = "Channel";
    string lyrDamName = "Dam";

    for (size_t i = 0; i < _x.size() - 1; i++) {
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrChannel << endl
                  << 10 << endl
                  << _x.at(i) * _xscale << endl
                  << 20 << endl
                  << _y.at(i) * _yscale << endl
                  << 11 << endl
                  << _x.at(i+1) * _xscale << endl
                  << 21 << endl
                  << _y.at(i+1) * _yscale << endl;
    }

    // ****** Draw the front view ******

    vector<double> x_values;
    vector<double> y_values;

    // Write the dimensions
    for (size_t i=0; i < _dimensions.size(); i++) {

        x_values.push_back(_dimensions.at(i).x() + _dimensions.at(i).width());
        y_values.push_back(_dimensions.at(i).y() + _dimensions.at(i).height());

        // Create a rectangle, first draw the top line
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << _dimensions.at(i).x() * _xscale << endl
                  << 20 << endl
                  << _dimensions.at(i).y() * _yscale << endl
                  << 11 << endl
                  << (_dimensions.at(i).x() + _dimensions.at(i).width()) * _xscale << endl
                  << 21 << endl
                  << _dimensions.at(i).y() * _yscale << endl;

        // Next, draw the left side
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << _dimensions.at(i).x() * _xscale << endl
                  << 20 << endl
                  << _dimensions.at(i).y() * _yscale << endl
                  << 11 << endl
                  << _dimensions.at(i).x() * _xscale << endl
                  << 21 << endl
                  << (_dimensions.at(i).y() + _dimensions.at(i).height()) * _yscale << endl;

        // Then, draw the top line
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << _dimensions.at(i).x() * _xscale << endl
                  << 20 << endl
                  << (_dimensions.at(i).y() + _dimensions.at(i).height()) * _yscale << endl
                  << 11 << endl
                  << (_dimensions.at(i).x() + _dimensions.at(i).width()) * _xscale << endl
                  << 21 << endl
                  << (_dimensions.at(i).y() + _dimensions.at(i).height()) * _yscale << endl;

        // Finally, draw the right line
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << (_dimensions.at(i).x() + _dimensions.at(i).width()) * _xscale  << endl
                  << 20 << endl
                  << _dimensions.at(i).y() * _yscale << endl
                  << 11 << endl
                  << (_dimensions.at(i).x() + _dimensions.at(i).width()) * _xscale << endl
                  << 21 << endl
                  << (_dimensions.at(i).y() + _dimensions.at(i).height()) * _yscale << endl;
    }

    // ****** Draw the lateral and top view ******

    // Determine the (x, y) origin that the lateral and top views should have
    double offset = 5;
    double x_orig = (utilities::maxValueFrom(x_values) + offset);
    double y_orig = (utilities::maxValueFrom(y_values) + offset);

    for (size_t i=0; i < _dimensions.size(); i++) {

        // ****** Plot the layers in lateral view ******

        // Create a rectangle, first draw the bottom line
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << x_orig * _xscale << endl
                  << 20 << endl
                  << _dimensions.at(i).y() * _yscale << endl
                  << 11 << endl
                  << (x_orig + _dimensions.at(i).length()) * _xscale << endl
                  << 21 << endl
                  << _dimensions.at(i).y() * _yscale << endl;
        // Next, draw the left side line
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << x_orig * _xscale << endl
                  << 20 << endl
                  << _dimensions.at(i).y() * _yscale << endl
                  << 11 << endl
                  << x_orig * _xscale << endl
                  << 21 << endl
                  << (_dimensions.at(i).y() + _dimensions.at(i).height()) * _yscale << endl;
        // Then, draw the right line
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << (x_orig + _dimensions.at(i).length()) * _xscale  << endl
                  << 20 << endl
                  << _dimensions.at(i).y() * _yscale << endl
                  << 11 << endl
                  << (x_orig + _dimensions.at(i).length()) * _xscale << endl
                  << 21 << endl
                  << (_dimensions.at(i).y() + _dimensions.at(i).height()) * _yscale << endl;
        // Finally, draw the top line
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << x_orig * _xscale << endl
                  << 20 << endl
                  << (_dimensions.at(i).y() + _dimensions.at(i).height()) * _yscale << endl
                  << 11 << endl
                  << (x_orig + _dimensions.at(i).length()) * _xscale << endl
                  << 21 << endl
                  << (_dimensions.at(i).y() + _dimensions.at(i).height()) * _yscale << endl;

        // ****** Plot the layers in top view ******

        // Create a rectangle, first draw the top line
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << _dimensions.at(i).x() * _xscale << endl
                  << 20 << endl
                  << y_orig * _yscale << endl
                  << 11 << endl
                  << (_dimensions.at(i).x() + _dimensions.at(i).width()) * _xscale << endl
                  << 21 << endl
                  << y_orig * _yscale << endl;
        // Next, draw the left line
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << _dimensions.at(i).x() * _xscale << endl
                  << 20 << endl
                  << y_orig * _yscale << endl
                  << 11 << endl
                  << _dimensions.at(i).x() * _xscale << endl
                  << 21 << endl
                  << (y_orig + _dimensions.at(i).length()) * _yscale << endl;
        // Then, draw the right line
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << (_dimensions.at(i).x() + _dimensions.at(i).width()) * _xscale << endl
                  << 20 << endl
                  << y_orig * _yscale << endl
                  << 11 << endl
                  << (_dimensions.at(i).x() + _dimensions.at(i).width()) * _xscale << endl
                  << 21 << endl
                  << (y_orig + _dimensions.at(i).length()) * _yscale << endl;
        // Finally, draw the top line
        dxfWriter << 0 << endl
                  << "LINE" << endl
                  << 8 << endl
                  << lyrDamName << endl
                  << 10 << endl
                  << _dimensions.at(i).x() * _xscale << endl
                  << 20 << endl
                  << (y_orig + _dimensions.at(i).length()) * _yscale << endl
                  << 11 << endl
                  << (_dimensions.at(i).x() + _dimensions.at(i).width()) * _xscale << endl
                  << 21 << endl
                  << (y_orig + _dimensions.at(i).length()) * _yscale << endl;
    }

    // Close the DXF file
    dxfWriter << 0 << endl
              << "ENDSEC" << endl
              << 0 << endl
              << "EOF" << endl;
    dxfWriter.close();
    return true;
}

vector<double> DxfFiles::y() const
{
    return _y;
}

void DxfFiles::setY(const vector<double> &y)
{
    _y = y;
}

vector<double> DxfFiles::x() const
{
    return _x;
}

void DxfFiles::setX(const vector<double> &x)
{
    _x = x;
}

vector<Layer> DxfFiles::dimensions() const
{
    return _dimensions;
}

void DxfFiles::setDimensions(const vector<Layer> &dimensions)
{
    _dimensions = dimensions;
}
