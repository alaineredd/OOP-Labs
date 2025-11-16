#include <iostream>

#include "seven.h"

namespace {
void RemoveLeadZero(vector::vector& v) {
    while(v.Size() > 1 && v.Get(v.Size() - 1) == 0) {
        v.Pop_back();
    }
    if(v.Size() == 0) {
        v.Push_back(0);
    }
}
}

namespace Seven {
Seven::Seven() : data_(0) {}

Seven::Seven(size_t size, unsigned char number) : data_(size) {
    if(size == 0) {
        throw std::invalid_argument("zerolength");
    }
    if(number > '6' || number < '0') {
        throw std::invalid_argument("incorrect char");
    }
    for(size_t i = 0; i < size; ++i) {
        data_.Push_back(number - '0');
    }
    RemoveLeadZero(data_);
}

Seven::Seven(const std::string& s) : data_(s.size()) {
    if(s.size() == 0) {
        throw std::invalid_argument("zerolength");
    }
    size_t i = data_.Size() - 1;
    for(char c : s) {
        if (c < '0' || c > '6') {
            throw std::invalid_argument("incorrect char");
        }
    }
    for(char c : s) {
        data_.Push_back(c - '0');
    }
    RemoveLeadZero(data_);
}

Seven::Seven(const std::initializer_list<unsigned char>& list) : data_(list.size()) {
    if (list.size() == 0) {
        throw std::invalid_argument("zerolength");
    }
    for(unsigned char c : list) {
        if(c > '6' || c < '0') {
            throw std::invalid_argument("incorrect char");
        }
    }
    for(unsigned char c : list) {
        data_.Push_back(c - '0');
    }
    RemoveLeadZero(data_);
}

Seven::Seven(const Seven& other) = default;

Seven::Seven(Seven&& other) = default;

Seven::~Seven() = default;

bool Seven::EQ(const Seven& other) const {
    if(data_.Size() != other.data_.Size()) {
        return false;
    }
    for(size_t i = 0; i < data_.Size(); ++i) {
        if(data_.Get(i) != other.data_.Get(i)) {
            return false;
        }
    }
    return true;
}

bool Seven::GT(const Seven& other) const {
    if(data_.Size() > other.data_.Size()) {
        return true;
    }
    if(data_.Size() < other.data_.Size()) {
        return false;
    }
    for(size_t i = data_.Size() - 1; i >= 0; --i) {
        if(data_.Get(i) > other.data_.Get(i)) {
            return true;
        }
        if(data_.Get(i) < other.data_.Get(i)) {
            return false;
        }
    }
    return data_.Get(0) > other.data_.Get(0);
}

bool Seven::LT(const Seven& other) const {
    if(data_.Size() > other.data_.Size()) {
        return false;
    }
    if(data_.Size() < other.data_.Size()) {
        return true;
    }
    for(int i = data_.Size() - 1; i >= 0; --i) {
        if(data_.Get(i) > other.data_.Get(i)) {
            return false;
        }
        if(data_.Get(i) < other.data_.Get(i)) {
            return true;
        }
    }
    return false;
}

Seven Seven::ADD(const Seven& first, const Seven& second) {
    size_t max_size = std::max(first.data_.Size(), second.data_.Size());
    Seven result;
    result.data_.Reserve(max_size + 1);
    
    size_t added = 0;
    
    for(size_t i = 0; i < max_size || added != 0; ++i) {
        size_t sum = added;
        
        if(i < first.data_.Size()) {
            sum += first.data_.Get(i);
        }
        if(i < second.data_.Size()) {
            sum += second.data_.Get(i);
        }
        
        result.data_.Push_back(sum % 7);
        added = sum / 7;
    }
    
    RemoveLeadZero(result.data_);
    return result;
}

Seven Seven::SUB(const Seven& first, const Seven& second) {
    if(first.LT(second)) {
        throw std::invalid_argument("first must be greater than second");
    }
    
    Seven result;
    result.data_.Reserve(first.data_.Size());
    
    size_t taken = 0;
    
    for(size_t i = 0; i < first.data_.Size(); ++i) {
        int digit = first.data_.Get(i) - taken;
        
        if(i < second.data_.Size()) {
            digit -= second.data_.Get(i);
        }
        
        if(digit < 0) {
            digit += 7;
            taken = 1;
        } else {
            taken = 0;
        }
        
        result.data_.Push_back(digit);
    }
    
    RemoveLeadZero(result.data_);
    return result;
}
} // namespace seven