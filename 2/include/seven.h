#pragma once

#include <string>
#include <initializer_list>
#include <stdexcept>

#include "vector.h"

namespace Seven {
class Seven {
private:
    vector::vector data_;
public:
    Seven();

    Seven(size_t size, unsigned char number);

    Seven(const std::string& s);

    Seven(const std::initializer_list<unsigned char>& list);

    Seven(const Seven& other);

    Seven(Seven&& other);

    ~Seven();

    bool EQ(const Seven& other) const;

    bool GT(const Seven& other) const;

    bool LT(const Seven& other) const;

    static Seven ADD(const Seven& first, const Seven& second);

    static Seven SUB(const Seven& first, const Seven& second);
};
} // namespace seven