#include <stdexcept>

#include "../include/vector.h"

namespace vector {

Vector::Vector() : arr_(new figure::Figure*[DEFAULT_CAPACITY]), sz_(0), cap_(DEFAULT_CAPACITY) {}

Vector::Vector(std::initializer_list<figure::Figure*> init) 
    : arr_(new figure::Figure*[init.size()]), sz_(init.size()), cap_(init.size()) {
    size_t i = 0;
    for (auto item : init) {
        arr_[i++] = item;
    }
}

figure::Figure& Vector::operator[](size_t pos) {
    if (pos >= sz_) throw std::out_of_range("Index out of range");
    return *arr_[pos];
}

const figure::Figure& Vector::operator[](size_t pos) const {
    if (pos >= sz_) throw std::out_of_range("Index out of range");
    return *arr_[pos];
}

figure::Figure& Vector::Front() const noexcept {
    return *arr_[0];
}

figure::Figure& Vector::Back() const noexcept {
    return *arr_[sz_ - 1];
}

figure::Figure** Vector::Data() const noexcept {
    return arr_;
}

bool Vector::IsEmpty() const noexcept {
    return sz_ == 0;
}

size_t Vector::Size() const noexcept {
    return sz_;
}

size_t Vector::Capacity() const noexcept {
    return cap_;
}

void Vector::Reserve(size_t new_cap) {
    if (new_cap <= cap_) return;
    
    figure::Figure** new_arr = new figure::Figure*[new_cap];
    for (size_t i = 0; i < sz_; ++i) {
        new_arr[i] = arr_[i];
    }
    
    delete[] arr_;
    arr_ = new_arr;
    cap_ = new_cap;
}

void Vector::Clear() noexcept {
    for (size_t i = 0; i < sz_; ++i) {
        delete arr_[i];
    }
    sz_ = 0;
}

void Vector::Insert(size_t pos, figure::Figure* value) {
    if (pos > sz_) throw std::out_of_range("insert out of range");
    
    if (sz_ == cap_) {
        Reserve(cap_ * 2);
    }
    
    for (size_t i = sz_; i > pos; --i) {
        arr_[i] = arr_[i - 1];
    }
    
    arr_[pos] = value;
    ++sz_;
}

void Vector::Erase(size_t begin_pos, size_t end_pos) {
    if (begin_pos > end_pos || end_pos > sz_) throw std::out_of_range("erase out of range");
    
    size_t erase_count = end_pos - begin_pos;
    for (size_t i = begin_pos; i < end_pos; ++i) {
        delete arr_[i];
    }
    
    for (size_t i = begin_pos; i < sz_ - erase_count; ++i) {
        arr_[i] = arr_[i + erase_count];
    }
    
    sz_ -= erase_count;
}

void Vector::Erase(size_t pos) {
    Erase(pos, pos + 1);
}

void Vector::PushBack(figure::Figure* value) {
    if (sz_ == cap_) {
        Reserve(cap_ * 2);
    }
    
    arr_[sz_++] = value;
}

void Vector::PopBack() {
    if (sz_ > 0) {
        delete arr_[sz_ - 1];
        --sz_;
    }
}

double Vector::TotalArea() const {
    double total = 0.0;
    for (size_t i = 0; i < sz_; ++i) {
        total += arr_[i]->Area();
    }
    return total;
}

void Vector::PrintAllCenters() const {
    for (size_t i = 0; i < sz_; ++i) {
        figure::Point center = arr_[i]->Center();
        std::cout << "Center " << i << ": (" << center.x << ", " << center.y << ")" << "\n";
    }
}

void Vector::PrintAllAreas() const {
    for (size_t i = 0; i < sz_; ++i) {
        std::cout << "Area " << i << ": " << arr_[i]->Area() << "\n";
    }
}

Vector::~Vector() {
    Clear();
    delete[] arr_;
}

} // namespace vector