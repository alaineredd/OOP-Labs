#pragma once

#include <iostream>

namespace figure{

constexpr const double EPSILON = 1e-9;

struct Point{
    double x;
    double y;
    
    Point();
    Point(double cx, double cy);
};

bool operator==(const Point& p1, const Point& p2);
bool operator!=(const Point& p1, const Point& p2);
std::ostream& operator<<(std::ostream& os, const Point& p);
std::istream& operator>>(std::istream& is, Point& p);

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
