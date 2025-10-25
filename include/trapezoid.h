#pragma once

#include "figure.h"

namespace figure{

class Trapezoid : public Figure {
    friend bool operator==(const Trapezoid& first, const Trapezoid& second);
    friend bool operator!=(const Trapezoid& first, const Trapezoid& second);
    friend std::istream &operator>>(std::istream &in, Trapezoid& trapezoid);
    friend std::ostream &operator<<(std::ostream &out, const Trapezoid& trapezoid);
private:
    Point vertices[TETRAVERTICES];
public:
    Trapezoid();

    Trapezoid(const Point& p1, const Point& p2, const Point& p3, const Point& p4);

    Trapezoid(const Trapezoid& other);

    Trapezoid(Trapezoid&& other);

    ~Trapezoid();

    Trapezoid& operator=(const Trapezoid& other);

    Trapezoid& operator=(Trapezoid&& other);

    operator double() override;

    void Print(std::ostream &out) const override;

    Point Center() const override;

    double Area() const override;

    Point GetVertex(int index) const;

    bool IsValid() const;

};

bool operator==(const Trapezoid& first, const Trapezoid& second);
bool operator!=(const Trapezoid& first, const Trapezoid& second);
std::istream &operator>>(std::istream &in, Trapezoid& trapezoid);
std::ostream &operator<<(std::ostream &out, const Trapezoid& trapezoid);
} // namespace figure