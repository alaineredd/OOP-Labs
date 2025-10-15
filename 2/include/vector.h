#pragma once

#include <initializer_list>
#include <cstdlib>
#include <cstddef>
#include <utility>
#include <new>
#include <string>
#include <algorithm>

namespace vector {
    constexpr std::size_t DEFAULTCAPACITY = 10;

    class vector{
        public:

            vector();

            vector(size_t size);

            vector(size_t size, unsigned char element);

            vector(const std::string& s);
    
            vector(vector&& other);

            vector(const vector& other);

            vector(const std::initializer_list<unsigned char> list);

            unsigned char& Get(size_t position);

            unsigned char Get(size_t position) const;

            size_t Size() const;

            size_t Capacity() const;

            bool IsEmpty() const;

            void Push_back(unsigned char value);

            void Pop_back();

            void Reserve(size_t cap);

            ~vector();


        private:
            size_t sz_;
            size_t cap_;
            unsigned char* data_;
            void Swap(vector& other) {
                std::swap(other.data_, data_);
                std::swap(other.sz_, sz_);
                std::swap(other.cap_, cap_);
            }
    };
}