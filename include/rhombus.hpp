#pragma once

#include <memory> 

#include "figure.hpp"

namespace figure {

template<Scalar T>
class Rhombus : public Figure<T> {
private:
    static const int FOUR_VERTICES = 4;
    std::unique_ptr<Point<T>> vertices[FOUR_VERTICES];

public:
    Rhombus();
    Rhombus(const Point<T>& p1, const Point<T>& p2, const Point<T>& p3, const Point<T>& p4);
    Rhombus(const Rhombus& other);
    Rhombus(Rhombus&& other) noexcept;

    Rhombus& operator=(const Rhombus& other);
    Rhombus& operator=(Rhombus&& other) noexcept;

    bool operator==(const Figure<T>& other) const override;

    Point<T> Center() const override;
    double Area() const override;

    void Print(std::ostream& os) const override;
    void Read(std::istream& is) override;

    std::shared_ptr<Figure<T>> clone() const override;
};

} // namespace figure

#include "rhombus.ipp"
