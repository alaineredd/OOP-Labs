#pragma once

#include <iostream>
#include <memory>

#include "point.hpp"

namespace figure {

template<Scalar T>
class Figure {
public:
    virtual ~Figure() = default;

    virtual Point<T> Center() const = 0;
    virtual double Area() const = 0;
    virtual void Print(std::ostream& os) const = 0;
    virtual void Read(std::istream& is) = 0;

    virtual std::shared_ptr<Figure<T>> clone() const = 0;
    virtual bool operator==(const Figure& other) const = 0;

    operator double() const;
};

template<Scalar T>
std::ostream& operator<<(std::ostream& os, const Figure<T>& figure);

template<Scalar T>
std::istream& operator>>(std::istream& is, Figure<T>& figure);

}

#include "figure.ipp"