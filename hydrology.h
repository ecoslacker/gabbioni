#ifndef HYDROLOGY_H
#define HYDROLOGY_H

class hydrology
{
public:
    hydrology();
    static double concentrationTime(double length, double rise, double area);
    static double runoffCoefficient(int landUse, int soilType, double rainfall);
    static double rationalFormula(double coefficient, double intensity, double area);
    static double kirpich(double length, double rise);
    static double giandotti(double length, double rise, double area);
};

#endif // HYDROLOGY_H
