#pragma once

namespace vector {
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

    void Swap(vector& other);

private:
    size_t sz_;
    size_t cap_;
    unsigned char* data_;
    static constexpr std::size_t DEFAULT_CAPACITY = 10;
};
} // namespace vector