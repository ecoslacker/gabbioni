#ifndef WEIR_H
#define WEIR_H

#include <cmath>

class Weir
{
public:
    Weir(double width=0, double water_height=0, double wall_step = 0.5, double coefficient = 1.45);

    double coefficient() const;
    void setCoefficient(double coefficient);

    double width() const;
    void setWidth(double width);

    double wallsHeight() const;
    void setWallsHeight(double wallsHeight);

    double waterHeight() const;
    void setWaterHeight(double waterHeight);

    double discharge() const;
    void setDischarge(double discharge);

    double wallStep() const;
    void setWallStep(double wallStep);

    void dischargeBroadCrestedRectangular();
    void designBroadCrestedRectangular();

    int levels() const;
    void setLevels(int levels);

private:
    int _levels;
    double _coefficient;
    double _width;
    double _wallsHeight;
    double _waterHeight;
    double _discharge;
    double _wallStep;
};

#endif // WEIR_H
