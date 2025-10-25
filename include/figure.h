#pragma once

#include <iostream>

namespace figure{

constexpr const int TETRAVERTICES = 4;
constexpr const int PENTAVERTICES = 5;
constexpr const double EPSILON = 1e-9;

struct Point{
    double x;
    double y;

    Point();
    Point(double cx, double cy);
};

class Figure{
    friend std::ostream &operator<<(std::ostream &out, const Figure &figure);

public:

    virtual Point Center() const = 0;

    virtual double Area() const = 0;
    
    virtual void Print(std::ostream& out) const = 0;

    virtual operator double() = 0;

    virtual ~Figure() = default;
};

std::ostream &operator<<(std::ostream &out, const Figure &figure);
} // namespace figure
