#pragma once

#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
 
#include "figure.hpp"

namespace vector {
const int8_t DEFAULT_CAPACITY = 10;

template <typename T> 
class Vector {
public:
  Vector();

  Vector(std::initializer_list<T> init);

  T &operator[](size_t pos);

  const T &operator[](size_t pos) const;

  T &Front();

  T &Back();

  T *Data() const noexcept;

  bool IsEmpty() const noexcept;

  size_t Size() const noexcept;

  size_t Capacity() const noexcept;

  void Reserve(size_t new_cap);

  void Clear() noexcept;

  void Insert(size_t pos, T value);

  void Erase(size_t begin_pos, size_t end_pos);

  void Erase(size_t pos);

  void PushBack(T value);

  void PopBack();

  template <typename U> 
  double TotalArea() const;

  template <typename U> 
  void PrintAllCenters() const;

  void swap(Vector &other) noexcept {
    std::swap(other.arr_, arr_);
    std::swap(other.sz_, sz_);
    std::swap(other.cap_, cap_);
  }

  template <typename U> 
  void PrintAllAreas() const;

  ~Vector();

private:
  T* arr_;
  size_t sz_;
  size_t cap_;
};

template <typename T>
inline void swap(Vector<T> &left, Vector<T> &right) noexcept {
  left.swap(right);
}
} // namespace vector

#include "vector.ipp"