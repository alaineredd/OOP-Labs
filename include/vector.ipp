#pragma once

#include <memory>

#include "vector.hpp"

namespace vector {

template <typename T> 
Vector<T>::Vector() : arr_(nullptr), sz_(0), cap_(0) {}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> init)
    : sz_(init.size()), cap_(init.size()) {
  arr_ = new T[init.size()];
  size_t i = 0;
  for (const T &fig : init) {
    arr_[i] = fig;
    ++i;
  }
}

template <typename T> 
T &Vector<T>::operator[](size_t pos) {
  if (pos >= sz_) {
    throw std::out_of_range("Invalid index");
  }
  return arr_[pos];
}

template <typename T> 
const T &Vector<T>::operator[](size_t pos) const {
  if (pos >= sz_) {
    throw std::out_of_range("Invalid index");
  }
  return arr_[pos];
}

template <typename T> 
T &Vector<T>::Front() {
  if (sz_ == 0) {
    throw std::out_of_range("Vector is empty!");
  }
  return arr_[0];
}

template <typename T> 
bool Vector<T>::IsEmpty() const noexcept {
  return sz_ == 0;
}

template <typename T> 
T &Vector<T>::Back() {
  if (sz_ == 0) {
    throw std::out_of_range("Vector is empty!");
  }
  return arr_[sz_ - 1];
}

template <typename T> 
T* Vector<T>::Data() const noexcept { return arr_.get(); }

template <typename T> 
size_t Vector<T>::Size() const noexcept { return sz_; }

template <typename T> 
size_t Vector<T>::Capacity() const noexcept {
  return cap_;
}

template <typename T> 
void Vector<T>::Reserve(size_t new_cap) {
  if (new_cap <= cap_) {
    return;
  }
  auto new_arr = new T[new_cap];
  if (arr_ != nullptr) {
    for (size_t i = 0; i < sz_; i++) {
      new_arr[i] = std::move(arr_[i]);
    }
    delete[] arr_;
  }
  arr_ = std::move(new_arr);
  cap_ = new_cap;
}

template <typename T> 
void Vector<T>::Clear() noexcept { sz_ = 0; }

template <typename T> 
void Vector<T>::Insert(size_t pos, T value) {
  if (pos > sz_) {
    throw std::out_of_range("Insert position out of range");
  }
  if (cap_ == sz_) {
    Reserve(cap_ == 0 ? DEFAULT_CAPACITY : cap_ * 2);
  }
  for (size_t i = sz_; i > pos; --i) {
    arr_[i] = std::move(arr_[i - 1]);
  }
  arr_[pos] = std::move(value);
  ++sz_;
}

template <typename T> 
void Vector<T>::Erase(size_t begin_pos, size_t end_pos) {
  if (end_pos > sz_ || begin_pos > end_pos) {
    throw std::out_of_range("Erase range out of bounds");
  }
  for (size_t i = end_pos; i < sz_; ++i) {
    arr_[i - (end_pos - begin_pos)] = std::move(arr_[i]);
  }
  sz_ -= (end_pos - begin_pos);
}

template <typename T> 
void Vector<T>::Erase(size_t pos) { Erase(pos, pos + 1); }

template <typename T> 
void Vector<T>::PushBack(T value) {
  if (cap_ == 0) {
    Reserve(DEFAULT_CAPACITY);
  }
  if (cap_ == sz_) {
    Reserve(2 * cap_);
  }
  arr_[sz_] = std::move(value);
  ++sz_;
}

template <typename T> 
void Vector<T>::PopBack() {
  if (sz_ == 0)
    return;
  --sz_;
}

template <typename T>
template <typename U>
double Vector<T>::TotalArea() const {
  double total = 0.0;
  for (size_t i = 0; i < sz_; ++i) {
    total += arr_[i]->Area();
  }
  return total;
}

template <typename T>
template <typename U>
void Vector<T>::PrintAllCenters() const {
  for (size_t i = 0; i < sz_; ++i) {
    std::cout << "Center of figure " << (i + 1) << ": " << arr_[i]->Center()
              << std::endl;
  }
}

template <typename T>
template <typename U>
void Vector<T>::PrintAllAreas() const {
  for (size_t i = 0; i < sz_; ++i) {
    std::cout << "Area of figure " << (i + 1) << ": " << arr_[i]->Area()
              << std::endl;
  }
}

template <typename T> Vector<T>::~Vector() {
  delete[] arr_;
};
} // namespace vector