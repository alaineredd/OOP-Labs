#pragma once

#include "figure.h"

namespace figure{

class Rhombus : public Figure {
    friend bool operator==(const Rhombus& first, const Rhombus& second);
    friend bool operator!=(const Rhombus& first, const Rhombus& second);
    friend std::istream &operator>>(std::istream &in, Rhombus& Rhombus);
    friend std::ostream &operator<<(std::ostream &out, const Rhombus& Rhombus);
private:
    Point vertices[TETRAVERTICES];
public:
    Rhombus();

    Rhombus(const Point& p1, const Point& p2, const Point& p3, const Point& p4);

    Rhombus(const Rhombus& other);

    Rhombus(Rhombus&& other);

    ~Rhombus();

    Rhombus& operator=(const Rhombus& other);

    Rhombus& operator=(Rhombus&& other);

    operator double() override;

    void Print(std::ostream &out) const override;

    Point Center() const override;

    double Area() const override;

    Point GetVertex(int index) const;

    bool IsValid() const;

};

bool operator==(const Rhombus& first, const Rhombus& second);
bool operator!=(const Rhombus& first, const Rhombus& second);
std::istream &operator>>(std::istream &in, Rhombus& Rhombus);
std::ostream &operator<<(std::ostream &out, const Rhombus& Rhombus);
} // namespace figure