#pragma once

#include "classes.h"

template<class T, class A = std::allocator<T>>
class CCircularBufferExt : public CCircularBuffer<T, A> {
public:
    void push_front(const T& value) override;
    void push_back(const T& elem) override;
    template<std::forward_iterator iter>
    CCircularBufferExt(iter it1, iter it2);
    CCircularBufferExt(const std::initializer_list<T>& il);
    CCircularBufferExt();
    CCircularBufferExt(const size_t size);
    CCircularBufferExt(const size_t size, const T value);


};

template<class T, class A>
void CCircularBufferExt<T, A>::push_front(const T &value) {
    if(this->capacity_ == 0) {
        CCircularBufferExt<T, A>::reserve(1);
    }
    if (this->size_ == this->capacity_) {
        CCircularBufferExt<T, A>::reserve(2 * this->capacity_);
    }
    this->begin_ = this->data_ + (this->begin_ - this->data_ + this->capacity_ - 1) % this->capacity_;
    std::construct_at(this->begin_, value);
    this->size_++;
    this->isFull = this->size_ == this->capacity_;
}

template<class T, class A>
void CCircularBufferExt<T, A>::push_back(const T &elem) {
    if(this->capacity_ == 0) {
        CCircularBufferExt<T, A>::reserve(1);
    }
    if (this->size_ == this->capacity_) {
        CCircularBufferExt<T, A>::reserve(2 * this->capacity_);
    }
    std::construct_at(this->end_, elem);
    this->end_++;
    if (this->end_ == this->data_ + this->capacity_) {
        this->end_ = this->data_;
    }
    this->size_++;
    this->isFull = this->size_ == this->capacity_;
}

template<class T, class A>
template<std::forward_iterator iter>
CCircularBufferExt<T, A>::CCircularBufferExt(iter it1, iter it2):CCircularBuffer<T, A>(it1, it2) {
}

template<class T, class A>
CCircularBufferExt<T, A>::CCircularBufferExt(){

}

template<class T, class A>
CCircularBufferExt<T, A>::CCircularBufferExt(const std::initializer_list<T> &il):CCircularBuffer<T, A>(il) {
}

template<class T, class A>
CCircularBufferExt<T, A>::CCircularBufferExt(const size_t size):CCircularBuffer<T, A>(size) {

}

template<class T, class A>
CCircularBufferExt<T, A>::CCircularBufferExt(const size_t size, const T value):CCircularBuffer<T, A>(size, value) {

}


