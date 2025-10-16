#include "vector.h"

#include <memory>
#include <algorithm>

namespace vector {
    vector::vector() : sz_(0), cap_(DEFAULT_CAPACITY), data_(new unsigned char[cap_]) {}

    vector::vector(size_t size, unsigned char value) : sz_(size), cap_(size), data_(new unsigned char[sz_]) {
        for(size_t i = 0; i < sz_; ++i) {
            data_[i] = value;
        }
    }

    vector::vector(size_t size) : sz_(size), cap_(std::max(size, DEFAULT_CAPACITY)), data_(new unsigned char[size]) {}

    vector::vector(const std::string& s) : sz_(s.size()), cap_(s.size()), data_(new unsigned char[s.size()]) {
        for(size_t i = 0; i < sz_; ++i) {
            data_[i] = s[i];
        }
    }

    vector::vector(const vector& other) : sz_(other.sz_), cap_(other.cap_), data_(new unsigned char[cap_]) {
        std::uninitialized_copy(other.data_, other.data_ + other.sz_, data_);
    }

    vector::vector(vector&& other) : sz_(other.sz_), cap_(other.cap_), data_(other.data_) {
        other.sz_ = 0;
        other.cap_= 0;
        other.data_ = nullptr;
    }

    vector::vector(const std::initializer_list<unsigned char> list) : sz_(list.size()), cap_(list.size()), data_(new unsigned char[cap_]) {
        int i = 0;
        for (unsigned char c: list) {
            data_[i++] = c;
        }
    }

    unsigned char& vector::Get(std::size_t position) {
        return data_[position];
    }

    unsigned char vector::Get(std::size_t position) const {
        return data_[position];
    }

    size_t vector::Size() const {
        return sz_;    
    }

    size_t vector::Capacity() const {
        return cap_;
    }

    bool vector::IsEmpty() const {
        return sz_ == 0;
    }

    void vector::Push_back(unsigned char value) {
        if(cap_ < sz_ + 1) {
            Reserve(2 * cap_);
        }
        data_[sz_] = value;
        sz_++;
    }

    void vector::Pop_back() {
        if(sz_ > 0) {
            --sz_;
        }
    }

    void vector::Reserve(size_t capacity) {
        if (cap_ >= capacity) {
            return;
        }

        unsigned char* new_data = new unsigned char[capacity];
        cap_ = capacity;
        if (data_) {
            std::uninitialized_move(data_, data_ + sz_, new_data);
        }
        delete[] data_;
        data_ = new_data;
    }

    vector::~vector() {
        delete[] data_;
    }

    void vector::Swap(vector& other) {
        std::swap(other.data_, data_);
        std::swap(other.sz_, sz_);
        std::swap(other.cap_, cap_);
    }
}
