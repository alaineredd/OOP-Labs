#include "queue.h"


template<typename T>
template<typename... Args>
QueueNode<T>::QueueNode(Args&&... args) 
    : value(std::forward<Args>(args)...), next(nullptr) {}


template<typename T>
Queue<T>::Iterator::Iterator(QueueNode<T>* node) : current_(node) {}

template<typename T>
typename Queue<T>::Iterator::reference Queue<T>::Iterator::operator*() const { 
    return current_->value; 
}

template<typename T>
typename Queue<T>::Iterator::pointer Queue<T>::Iterator::operator->() const { 
    return &current_->value; 
}

template<typename T>
typename Queue<T>::Iterator& Queue<T>::Iterator::operator++() {
    if (current_) {
        current_ = current_->next;
    }
    return *this;
}

template<typename T>
typename Queue<T>::Iterator Queue<T>::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename T>
bool Queue<T>::Iterator::operator==(const Iterator& other) const { 
    return current_ == other.current_; 
}

template<typename T>
bool Queue<T>::Iterator::operator!=(const Iterator& other) const { 
    return current_ != other.current_; 
}


template<typename T>
Queue<T>::Queue(std::pmr::memory_resource* resource)
    : head_(nullptr), tail_(nullptr), size_(0), allocator_(resource) {}

template<typename T>
Queue<T>::Queue(const Queue& other) 
    : head_(nullptr), tail_(nullptr), size_(0), allocator_(other.allocator_) {
    for (auto it = other.begin(); it != other.end(); ++it) {
        push(*it);
    }
}

template<typename T>
Queue<T>::Queue(Queue&& other) noexcept 
    : head_(other.head_), tail_(other.tail_), size_(other.size_), allocator_(std::move(other.allocator_)) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

template<typename T>
Queue<T>::~Queue() {
    clear();
}

template<typename T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
    if (this != &other) {
        clear();
        allocator_ = other.allocator_;
        for (auto it = other.begin(); it != other.end(); ++it) {
            push(*it);
        }
    }
    return *this;
}

template<typename T>
Queue<T>& Queue<T>::operator=(Queue&& other) noexcept {
    if (this != &other) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        allocator_ = std::move(other.allocator_);
        
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template<typename T>
template<typename U>
void Queue<T>::push(U&& value) {
    QueueNode<T>* new_node = allocator_.allocate(1);
    try {
        allocator_.construct(new_node, std::forward<U>(value));
    } catch (...) {
        allocator_.deallocate(new_node, 1);
        throw;
    }

    if (tail_) {
        tail_->next = new_node;
    } else {
        head_ = new_node;
    }
    tail_ = new_node;
    ++size_;
}

template<typename T>
void Queue<T>::pop() {
    if (empty()) {
        throw std::runtime_error("Queue is empty");
    }
    
    QueueNode<T>* old_head = head_;
    head_ = head_->next;
    if (!head_) {
        tail_ = nullptr;
    }
    
    allocator_.destroy(old_head);
    allocator_.deallocate(old_head, 1);
    --size_;
}

template<typename T>
T& Queue<T>::front() {
    if (empty()) {
        throw std::runtime_error("Queue is empty");
    }
    return head_->value;
}

template<typename T>
const T& Queue<T>::front() const {
    if (empty()) {
        throw std::runtime_error("Queue is empty");
    }
    return head_->value;
}

template<typename T>
T& Queue<T>::back() {
    if (empty()) {
        throw std::runtime_error("Queue is empty");
    }
    return tail_->value;
}

template<typename T>
const T& Queue<T>::back() const {
    if (empty()) {
        throw std::runtime_error("Queue is empty");
    }
    return tail_->value;
}

template<typename T>
bool Queue<T>::empty() const { 
    return size_ == 0; 
}

template<typename T>
std::size_t Queue<T>::size() const { 
    return size_; 
}

template<typename T>
void Queue<T>::clear() {
    while (!empty()) {
        pop();
    }
}

template<typename T>
typename Queue<T>::Iterator Queue<T>::begin() { 
    return Iterator(head_); 
}

template<typename T>
typename Queue<T>::Iterator Queue<T>::end() { 
    return Iterator(nullptr); 
}

template<typename T>
typename Queue<T>::Iterator Queue<T>::begin() const { 
    return Iterator(head_); 
}

template<typename T>
typename Queue<T>::Iterator Queue<T>::end() const { 
    return Iterator(nullptr); 
}