#pragma once

#include "figure.h"

namespace figure{
    
constexpr const int PENTA_VERTICES = 5;

class Pentagon : public Figure {
    friend bool operator==(const Pentagon& first, const Pentagon& second);
    friend bool operator!=(const Pentagon& first, const Pentagon& second);
    friend std::istream &operator>>(std::istream &in, Pentagon& Pentagon);
    friend std::ostream &operator<<(std::ostream &out, const Pentagon& Pentagon);
private:
    Point vertices[PENTA_VERTICES];
public:
    Pentagon();

    Pentagon(const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Point& p5);

    Pentagon(const Pentagon& other) = default;

    Pentagon(Pentagon&& other) = default;

    ~Pentagon();

    Pentagon& operator=(const Pentagon& other);

    Pentagon& operator=(Pentagon&& other);

    operator double() override;

    void Print(std::ostream &out) const override;

    Point Center() const override;

    double Area() const override;

    Point GetVertex(int index) const;

    bool IsValid() const;

};

bool operator==(const Pentagon& first, const Pentagon& second);
bool operator!=(const Pentagon& first, const Pentagon& second);
std::istream &operator>>(std::istream &in, Pentagon& Pentagon);
std::ostream &operator<<(std::ostream &out, const Pentagon& Pentagon);
} // namespace figure