#ifndef CHANNELSECTION_H
#define CHANNELSECTION_H

#include <iostream>
#include <cmath>
#include <string>
#include "utilities.h"

class ChannelSection
{
public:
    ChannelSection(std::vector<double> coord_x = {}, std::vector<double> coord_y = {}, double layersHeight=0.5);
    bool extractInformation();  // Extract the information (width, levels, etc.) once the channel cross-section is validated
    bool computeDischarge();    // Estimates the discharge of the channel given a water height
    bool isAValidSection();     // Validates the coordinates and the channel cross-section

    // Getters
    int levels() const;
    double channelDepth() const;
    double channelWidth() const;

    double layersHeight() const;
    void setLayersHeight(double layersHeight);

    double streambed() const;
    std::vector<double> widths() const;
    std::vector<double> rightX() const;
    std::vector<double> leftX() const;
    std::vector<double> depthLevels() const;
    double discharge() const;

    double coefficient() const;
    void setCoefficient(double coefficient);

    double slope() const;
    void setSlope(double slope);

    double waterHeight() const;
    void setWaterHeight(double waterHeight);

    std::vector<double> coordX() const;
    void setCoordX(const std::vector<double> &coordX);

    std::vector<double> coordY() const;
    void setCoordY(const std::vector<double> &coordY);

    double hydraulicArea() const;
    void setHydraulicArea(double hydraulicArea);

    double wettedPerimeter() const;
    void setWettedPerimeter(double wettedPerimeter);

    double hydraulicRadius() const;
    void setHydraulicRadius(double hydraulicRadius);

    double flowVelocity() const;
    void setFlowVelocity(double flowVelocity);

    std::string strChannel() const;

private:
    int _levels;
    double _layersHeight;
    double _channelDepth;
    double _channelWidth;
    double _streambed;
    double _waterHeight;
    double _discharge;
    double _coefficient;
    double _slope;
    double _hydraulicArea;
    double _wettedPerimeter;
    double _hydraulicRadius;
    double _flowVelocity;

    std::string _strChannel;

    std::vector<double> _coordX;
    std::vector<double> _coordY;
    std::vector<double> _widths;
    std::vector<double> _rightX;
    std::vector<double> _leftX;
    std::vector<double> _depthLevels;

    bool checkCoordinates();
    bool computeWidths();
    void flowGeometry();
    void flowGeometry(double& flowArea, double& wettedPerimeter);
    double trapeziumArea(double base1, double base2, double height);
    double chezyManning(double area, double perimeter, double n, double slope);
    double chezyManning(double n, double slope);
};

#endif // CHANNELSECTION_H
