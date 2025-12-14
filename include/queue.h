#pragma once

#include <memory_resource>
#include <iterator>
#include <stdexcept>
#include <utility>


template<typename T>
struct QueueNode {
    T value;
    QueueNode* next;
    
    template<typename... Args>
    QueueNode(Args&&... args);
};


template<typename T>
class Queue {
private:
    QueueNode<T>* head_;
    QueueNode<T>* tail_;
    std::size_t size_;
    std::pmr::polymorphic_allocator<QueueNode<T>> allocator_;

public:
    using value_type = T;
    using allocator_type = std::pmr::polymorphic_allocator<QueueNode<T>>;

    class Iterator {
    private:
        QueueNode<T>* current_;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit Iterator(QueueNode<T>* node = nullptr);

        reference operator*() const;
        pointer operator->() const;

        Iterator& operator++();
        Iterator operator++(int);

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };

   
    explicit Queue(std::pmr::memory_resource* resource = std::pmr::get_default_resource());
    Queue(const Queue& other);
    Queue(Queue&& other) noexcept;
    ~Queue();

    Queue& operator=(const Queue& other);
    Queue& operator=(Queue&& other) noexcept;

 
    template<typename U>
    void push(U&& value);

    void pop();
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    bool empty() const;
    std::size_t size() const;
    void clear();


    Iterator begin();
    Iterator end();
    Iterator begin() const;
    Iterator end() const;
};


#include "queue.ipp"