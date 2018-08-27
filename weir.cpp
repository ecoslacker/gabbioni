#include "weir.h"

Weir::Weir(double width, double water_height, double wall_step, double coefficient)
{
    // NOTE: In the case of rectangular weir, the coefficient is not a discharge coefficient
    // it's a coefficient involving numerical values and gravity acceleration instead.

    // Initialize the variables
    _coefficient = coefficient; // Rectangular=1.45
    _width = width;
    _waterHeight = water_height;
    _wallStep = wall_step;

    // With those variables get the discharge of the weir
    dischargeBroadCrestedRectangular();
}

double Weir::coefficient() const
{
    return _coefficient;
}

void Weir::setCoefficient(double coefficient)
{
    _coefficient = coefficient;
}

double Weir::width() const
{
    return _width;
}

void Weir::setWidth(double width)
{
    _width = width;
}

double Weir::wallsHeight() const
{
    return _wallsHeight;
}

void Weir::setWallsHeight(double wallsHeight)
{
    _wallsHeight = wallsHeight;
}

double Weir::waterHeight() const
{
    return _waterHeight;
}

void Weir::setWaterHeight(double waterHeight)
{
    _waterHeight = waterHeight;
}

double Weir::discharge() const
{
    return _discharge;
}

void Weir::setDischarge(double discharge)
{
    _discharge = discharge;
}

double Weir::wallStep() const
{
    return _wallStep;
}

void Weir::setWallStep(double wallStep)
{
    _wallStep = wallStep;
}

void Weir::dischargeBroadCrestedRectangular()
{
    /*
     * Computes the discharge of a broad-crested rectangular weir
     *
     */

    _discharge = _coefficient * _width * pow(_waterHeight, 1.5);
}

void Weir::designBroadCrestedRectangular()
{
    /*
     * Design a broad-crested rectangular weir
     *
     * Using the length value, calculates the heigth of the lateral walls
     * required to handle a previously known discharge.
     *
     */

    _waterHeight = pow(_discharge / (_coefficient * _width), 2./3.);
    _wallsHeight = 0;
    _levels = 0;

    while (_wallsHeight < _waterHeight) {
        _wallsHeight = _wallsHeight + _wallStep;
        _levels++;
    }
}

int Weir::levels() const
{
    return _levels;
}

void Weir::setLevels(int levels)
{
    _levels = levels;
}
