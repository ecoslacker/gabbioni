#ifndef LAYER_H
#define LAYER_H

class Layer
{
public:
    Layer(double length = 0, double width = 0, double height = 0, double x = 0, double y = 0);

    double height() const;
    void setHeight(const double height);

    double length() const;
    void setLength(const double length);

    double width() const;
    void setWidth(const double width);

    double x() const;
    void setX(const double x);

    double y() const;
    void setY(const double y);

private:
    double _height;
    double _length;
    double _width;

    double _x;
    double _y;
};

#endif // LAYER_H
