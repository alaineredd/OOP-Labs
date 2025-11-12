#pragma once

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <string>

#include "figure.h"

namespace vector {
constexpr const int DEFAULT_CAPACITY = 10;

class Vector {
public:
    Vector();

    Vector(std::initializer_list<figure::Figure*> init);

    figure::Figure& operator[](size_t pos);

    const figure::Figure& operator[](size_t pos) const;

    figure::Figure& Front() const noexcept;

    figure::Figure& Back() const noexcept;

    figure::Figure** Data() const noexcept;

    bool IsEmpty() const noexcept;

    size_t Size() const noexcept;

    size_t Capacity() const noexcept;

    void Reserve(size_t new_cap);

    void Clear() noexcept;

    void Insert(size_t pos, figure::Figure* value);

    void Erase(size_t begin_pos, size_t end_pos);

    void Erase(size_t pos);

    void PushBack(figure::Figure* value);

    void PopBack();

    double TotalArea() const;

    void PrintAllCenters() const;

    void PrintAllAreas() const;

    ~Vector();

private:
    figure::Figure** arr_;
    size_t sz_;
    size_t cap_;

    void Swap(Vector& v) {
        std::swap(v.arr_, arr_);
        std::swap(v.sz_, sz_);
        std::swap(v.cap_, cap_);
    }
};
}