#include "layer.h"

Layer::Layer(double length, double width, double height, double x, double y)
{
    _length = length;
    _width = width;
    _height = height;
    _x = x;
    _y = y;
}

double Layer::height() const
{
    return _height;
}

void Layer::setHeight(const double height)
{
    _height = height;
}

double Layer::length() const
{
    return _length;
}

void Layer::setLength(const double length)
{
    _length = length;
}

double Layer::width() const
{
    return _width;
}

void Layer::setWidth(const double width)
{
    _width = width;
}

double Layer::x() const
{
    return _x;
}

void Layer::setX(const double x)
{
    _x = x;
}

double Layer::y() const
{
    return _y;
}

void Layer::setY(const double y)
{
    _y = y;
}
