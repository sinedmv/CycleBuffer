#pragma once

#include <iostream>
#include <memory>
#include <limits>

template<class T, class A = std::allocator<T>>
class CCircularBuffer {
public:
    typedef typename A::difference_type difference_type;
    typedef typename A::size_type size_type;

    class Iterator: public std::iterator<std::random_access_iterator_tag, T> {
    public:
        friend class CCircularBuffer<T, A>;
        Iterator(T* p, CCircularBuffer<T, A>* cont, bool isbeg = false, bool isend = false);
        Iterator(const Iterator& it);

        difference_type operator-(const typename CCircularBuffer<T, A>::Iterator other);
        Iterator operator+(const difference_type n);
        Iterator operator-(const difference_type n);

        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);
        Iterator& operator+=(const difference_type);
        Iterator& operator-=(const difference_type);

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator & other) const;
        bool operator<(const Iterator& it) const;
        bool operator>(const Iterator& it) const;
        bool operator>=(const Iterator& it) const;
        bool operator<=(const Iterator& it) const;

        typename Iterator::pointer operator->() const;
        typename Iterator::reference operator*() const;


    protected:
        T* add(T* p, difference_type n) const;
        T* sub(T* p, difference_type n) const;

        T* linearize() const;

        T* point;
        CCircularBuffer<T, A>* cont_;

        bool isBegin = false;
        bool isEnd = false;
    };

    class const_Iterator: public std::iterator<std::random_access_iterator_tag, T> {
    public:
        friend class CCircularBuffer<T, A>;
        const_Iterator(T* p, const CCircularBuffer<T, A>* cont, bool isbeg = false, bool isend = false);
        const_Iterator(const const_Iterator& it);
        const_Iterator(const Iterator& it);

        const_Iterator& operator++();
        const_Iterator operator++(int);
        const_Iterator& operator--();
        const_Iterator operator--(int);
        const_Iterator& operator+=(const difference_type);
        const_Iterator& operator-=(const difference_type);

        difference_type operator-(const typename CCircularBuffer<T, A>::const_Iterator& other);
        const_Iterator operator+(const difference_type n);
        const_Iterator operator-(const difference_type n);


        bool operator==(const const_Iterator& other) const;
        bool operator!=(const const_Iterator& other) const;
        bool operator<(const const_Iterator& it) const;
        bool operator>(const const_Iterator& it) const;
        bool operator>=(const const_Iterator& it) const;
        bool operator<=(const const_Iterator& it) const;


        const typename const_Iterator::pointer operator->() const;

        const typename const_Iterator::reference operator*();


    protected:
        typename const_Iterator::reference operator*() const;
        T* add(T* p, difference_type n) const;
        T* sub(T* p, difference_type n) const;

        T* linearize() const;

        T* point;
        const CCircularBuffer<T, A>* cont_;

        bool isBegin = false;
        bool isEnd = false;
    };

    template<std::forward_iterator iter>
    CCircularBuffer(iter it1, iter it2);
    CCircularBuffer(const std::initializer_list<T> &);
    CCircularBuffer(const CCircularBuffer &);
    CCircularBuffer();
    CCircularBuffer(const size_t size);
    CCircularBuffer(const size_t size, const T value);

    ~CCircularBuffer();


    Iterator begin();
    Iterator begin() const;
    const_Iterator cbegin() const;
    T& front();
    const T& front() const;
    Iterator end();
    Iterator end() const;
    const_Iterator cend() const;
    T& back();
    const T& back() const;

    T& operator[](size_type index);
    const T& operator[](size_type index) const;



    Iterator insert(Iterator it, const T data);
    Iterator insert(Iterator it, size_t n, const T data);

    template<std::forward_iterator iter>
    Iterator insert(Iterator it, const iter& it1, const iter& it2);
    Iterator insert(Iterator it, std::initializer_list<T> list);

    Iterator erase(Iterator it);
    const_Iterator erase(const_Iterator it);
    Iterator erase(Iterator it1, Iterator it2);
    const_Iterator erase(const_Iterator it1, const_Iterator it2);


    virtual void push_front(const T& value);
    void pop_front();

    virtual void push_back(const T& elem);
    void pop_back();

    void reserve(size_t newCapacity);
    void resize(size_t newSize);

    template<std::forward_iterator iter>
    void assign(iter it1, iter it2);
    void assign(std::initializer_list<T> il);
    void assign(size_t n, T t);
    void clear();
    bool empty() const;
    size_type size() const;
    size_type capacity() const;

    void swap(CCircularBuffer &);
    size_type max_size() const;

protected:
    A alloc;
    T* data_;
    T* begin_;
    T* end_;
    size_t size_;
    size_t capacity_;

    bool isFull;
};

template<class T, class A>
typename CCircularBuffer<T, A>::size_type CCircularBuffer<T, A>::max_size() const {
    return std::numeric_limits<size_type>::max() / sizeof(typename A::value_type);
}

template<class T, class A>
T* CCircularBuffer<T, A>::Iterator::linearize() const {
    return isEnd ? cont_->data_ + cont_->size_ :
           (point < cont_->begin_ ? point + (cont_->data_ + cont_->capacity_ - cont_->begin_)
                                  : cont_->data_ + (point - cont_->begin_));
}

template<class T, class A>
T* CCircularBuffer<T, A>::const_Iterator::linearize() const {
    return isEnd ? cont_->data_ + cont_->size_ :
           (point < cont_->begin_ ? point + (cont_->data_ + cont_->capacity_ - cont_->begin_)
                                  : cont_->data_ + (point - cont_->begin_));
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator &CCircularBuffer<T, A>::Iterator::operator++() {
    point++;
    if (point == cont_->data_ + cont_->capacity_) {
        point = cont_->data_;
    }
    if (this->isEnd) {
        point = cont_->begin_;
        this->isBegin = true;
        this->isEnd = false;
    } else if (point == cont_->end_) {
        this->isEnd = true;
        this->isBegin = false;
    } else {
        this->isEnd = false;
        this->isBegin = false;
    }
    return *this;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::Iterator::operator++(int) {
    point++;
    if (point == cont_->data_ + cont_->capacity_) {
        point = cont_->data_;
    }
    if (this->isEnd) {
        point = cont_->begin_;
        this->isBegin = true;
        this->isEnd = false;
    } else if (point == cont_->end_) {
        this->isEnd = true;
        this->isBegin = false;
    } else {
        this->isEnd = false;
        this->isBegin = false;
    }
    return *this;
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator CCircularBuffer<T, A>::const_Iterator::operator++(int) {
    point++;
    if (point == cont_->data_ + cont_->capacity_) {
        point = cont_->data_;
    }
    if (this->isEnd) {
        point = cont_->begin_;
        this->isBegin = true;
        this->isEnd = false;
    } else if (point == cont_->end_) {
        this->isEnd = true;
        this->isBegin = false;
    } else {
        this->isEnd = false;
        this->isBegin = false;
    }
    return *this;
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator& CCircularBuffer<T, A>::const_Iterator::operator++() {
    point++;
    if (point == cont_->data_ + cont_->capacity_) {
        point = cont_->data_;
    }
    if (this->isEnd) {
        point = cont_->begin_;
        this->isBegin = true;
        this->isEnd = false;
    } else if (point == cont_->end_) {
        this->isEnd = true;
        this->isBegin = false;
    } else {
        this->isEnd = false;
        this->isBegin = false;
    }
    return *this;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator &CCircularBuffer<T, A>::Iterator::operator--() {
    point--;
    if (point == cont_->data_ - 1) {
        point = cont_->data_ + cont_->capacity_ - 1;
    }
    if (this->isBegin) {
        point = cont_->end_;
        this->isBegin = false;
        this->isEnd = true;
    } else if (point == cont_->begin_) {
        this->isBegin = true;
        this->isEnd = false;
    } else {
        this->isBegin = false;
        this->isEnd = false;
    }
    return *this;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::Iterator::operator--(int) {
    point--;
    if (point == cont_->data_ - 1) {
        point = cont_->data_ + cont_->capacity_ - 1;
    }
    if (this->isBegin) {
        point = cont_->end_;
        this->isBegin = false;
        this->isEnd = true;
    } else if (point == cont_->begin_) {
        this->isBegin = true;
        this->isEnd = false;
    } else {
        this->isBegin = false;
        this->isEnd = false;
    }
    return *this;
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator CCircularBuffer<T, A>::const_Iterator::operator--(int) {
    point--;
    if (point == cont_->data_ - 1) {
        point = cont_->data_ + cont_->capacity_ - 1;
    }
    if (this->isBegin) {
        point = cont_->end_;
        this->isBegin = false;
        this->isEnd = true;
    } else if (point == cont_->begin_) {
        this->isBegin = true;
        this->isEnd = false;
    } else {
        this->isBegin = false;
        this->isEnd = false;
    }
    return *this;
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator &CCircularBuffer<T, A>::const_Iterator::operator--() {
    point--;
    if (point == cont_->data_ - 1) {
        point = cont_->data_ + cont_->capacity_ - 1;
    }
    if (this->isBegin) {
        point = cont_->end_;
        this->isBegin = false;
        this->isEnd = true;
    } else if (point == cont_->begin_) {
        this->isBegin = true;
        this->isEnd = false;
    } else {
        this->isBegin = false;
        this->isEnd = false;
    }
    return *this;
}


template<class T, class A>
typename CCircularBuffer<T, A>::Iterator &CCircularBuffer<T, A>::Iterator::operator+=(const difference_type n) {
    if (n > 0) {
        if (n == cont_->end() - *this) {
            isEnd = true;
            isBegin = false;
            point = cont_->end_;
            return *this;
        }
        if (n - 1 == cont_->end() - *this) {
            isEnd = false;
            isBegin = true;
            point = cont_->begin_;
            return *this;
        }
        point = add(point, n);
        isEnd = false;
        isBegin = false;
    } else if (n < 0) {
        *this -= -n;
    }
    return *this;
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator &CCircularBuffer<T, A>::const_Iterator::operator+=(const difference_type n) {
    if (n > 0) {
        if (n == cont_->cend() - *this) {
            isEnd = true;
            isBegin = false;
            point = cont_->end_;
            return *this;
        }
        if (n - 1 == cont_->cend() - *this) {
            isEnd = false;
            isBegin = true;
            point = cont_->begin_;
            return *this;
        }
        point = sub(point, n);

        isEnd = false;
        isBegin = false;
    } else if (n < 0) {
        *this -= -n;
    }
    return *this;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator &CCircularBuffer<T, A>::Iterator::operator-=(const difference_type n) {
    if (n > 0) {
        if (n == *this - cont_->begin()) {
            isBegin = true;
            isEnd = false;
            point = cont_->begin_;
            return *this;
        }
        if (n - 1 == *this - cont_->begin()) {
            isBegin = false;
            isEnd = true;
            point = cont_->end_;
            return *this;
        }
        point = sub(point, n);
        isEnd = false;
        isBegin = false;
    } else if (n < 0) {
        *this += -n;
    }
    return *this;
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator &CCircularBuffer<T, A>::const_Iterator::operator-=(const difference_type n) {
    if (n > 0) {
        if (n == *this - cont_->cbegin()) {
            isBegin = true;
            isEnd = false;
            point = cont_->begin_;
            return *this;
        }
        if (n - 1 == *this - cont_->cbegin()) {
            isBegin = false;
            isEnd = true;
            point = cont_->end_;
            return *this;
        }
        point = sub(point, n);
        isEnd = false;
        isBegin = false;
    } else if (n < 0) {
        *this += -n;
    }
    return *this;
}

template<class T, class A>
typename CCircularBuffer<T, A>::difference_type CCircularBuffer<T, A>::Iterator::operator-(const typename CCircularBuffer<T, A>::Iterator other) {
    Iterator temp = other;
    return this->linearize() - temp.linearize();
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::Iterator::operator+(const difference_type n) {
    Iterator temp(*this);
    temp += n;
    return temp;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::Iterator::operator-(const difference_type n) {
    Iterator temp(*this);
    temp -= n;
    return temp;
}

template<class T, class A>
typename CCircularBuffer<T, A>::difference_type CCircularBuffer<T, A>::const_Iterator::operator-(const typename CCircularBuffer<T, A>::const_Iterator& other) {
    const_Iterator temp = other;
    return this->linearize() - temp.linearize();
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator CCircularBuffer<T, A>::const_Iterator::operator+(const difference_type n) {
    const_Iterator temp(*this);
    temp += n;
    return temp;
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator CCircularBuffer<T, A>::const_Iterator::operator-(const difference_type n) {
    const_Iterator temp(*this);
    temp -= n;
    return temp;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::insert(Iterator it, const T data) {
    size_t dist = it - begin();
    if (size_ == capacity_) {
        reserve(capacity_ * 2 + 1);
    } else {
        reserve(capacity_);
    }
    it = Iterator(data_ + dist, it.cont_, it.isBegin, it.isEnd);
    if (size_ == 0) {
        std::construct_at(data_, data);
        size_++;
        begin_ = data_;
        end_ = data_;
        it.point = data_;
        isFull = size_ == capacity_;
        return it;
    }
    end_++;
    size_++;
    if (end_ == data_ + capacity_) {
        end_ = begin_;
    }
    if (!it.isEnd) {
        Iterator end = this->end();
        Iterator it1 = end--;
        Iterator it2 = end--;
        for (; it2 != it; it1--, it2--) {
            std::destroy_at(it1.point);
            std::construct_at(it1.point, *it2.point);
        }
        std::destroy_at(it1.point);
        std::construct_at(it1.point, *it2.point);
    }
    std::destroy_at(it.point);
    std::construct_at(it.point, data);
    return it;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::insert(Iterator it, size_t n, const T data) {
    for (size_t i = 0; i < n; i++) {
        it = insert(it, data);
    }
    return it;
}

template<class T, class A>
template<std::forward_iterator iter>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::insert(Iterator it, const iter& it1, const iter& it2) {
    for (auto temp = it1; temp != it2; temp++){
        it = insert(it, *temp);
    }
    return it;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::insert(CCircularBuffer<T, A>::Iterator it, std::initializer_list<T> list) {
    for (auto temp = list.begin(); temp != list.end(); temp++){
        it = insert(it, *temp);
    }
    return it;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::erase(Iterator it) {
    if (it.isEnd || size_ == 0) {
        return it;
    }
    Iterator next = it;
    next++;
    for(Iterator p = it; next != it.cont_->end(); p = next, next++){
        std::destroy_at(p.point);
        std::construct_at(p.point, *next.point);
    }
    it.cont_->end_--;
    if (it.cont_->end_ == it.cont_->data_ - 1) {
        it.cont_->end_ = it.cont_->data_ + it.cont_->capacity_ - 1;
    }
    it.cont_->size_--;
    isFull = false;
    return it;
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator CCircularBuffer<T, A>::erase(const_Iterator it) {
    if (it.isEnd || size_ == 0) {
        return it;
    }
    const_Iterator next = it;
    next++;
    for(const_Iterator p = it; next != it.cont_->cend(); p = next, next++){
        std::destroy_at(p.point);
        std::construct_at(p.point, *next.point);
    }
    it.cont_->end_--;
    if (it.cont_->end_ == it.cont_->data_ - 1) {
        it.cont_->end_ = it.cont_->data_ + it.cont_->capacity_ - 1;
    }
    it.cont_->size_--;
    isFull = false;
    return it;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::erase(Iterator it1, Iterator it2) {
    Iterator it = it1;
    size_t n = it2 - it1;
    for (size_t i = 0; i < n; i++) {
        erase(it);
    }
    return it;
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator CCircularBuffer<T, A>::erase(const_Iterator it1, const_Iterator it2) {
    const_Iterator it = it1;
    size_t n = it2 - it1;
    for (size_t i = 0; i < n; i++) {
        erase(it);
    }
    return it;
}

template<class T, class A>
void CCircularBuffer<T, A>::push_front(const T& value){
    if(capacity_ == 0) {
        return;
    }
    if (size_ == capacity_) {
        std::destroy_at(data_ + (begin_ - data_ + capacity_ - 1) % capacity_);
        end_ = data_ + (end_ - data_ + capacity_ - 1) % capacity_;
        begin_ = data_ + (begin_ - data_ + capacity_ - 1) % capacity_;
        std::construct_at(begin_, value);
    } else {
        begin_ = data_ + (begin_ - data_ + capacity_ - 1) % capacity_;
        std::construct_at(begin_, value);
        size_++;
    }
    isFull = size_ == capacity_;
}

template<class T, class A>
void CCircularBuffer<T, A>::pop_front(){
    if (size_ == 0) {
        return;
    }
    if (size_ == 1) {
        clear();
        return;
    }
    std::destroy_at(begin_);
    size_--;
    begin_++;
    if (begin_ == data_ + capacity_) {
        begin_ = data_;
    }
    isFull = false;
}

template<class T, class A>
void CCircularBuffer<T, A>::push_back(const T& elem) {
    if(capacity_ == 0) {
        return;
    }
    std::construct_at(end_, elem);
    if (size_ == capacity_) {
        begin_++;
        if (begin_ == data_ + capacity_) {
            begin_ = data_;
        }
        end_ = begin_;
    } else {
        end_++;
        if (end_ == data_ + capacity_) {
            end_ = data_;
        }
        size_++;
        isFull = size_ == capacity_;
    }
}

template<class T, class A>
void CCircularBuffer<T, A>::pop_back(){
    if (size_ == 0) {
        return;
    }
    if (size_ == 1) {
        clear();
        return;
    }
    end_--;
    if (end_ == data_ - 1) {
        if (size_ == capacity_) {
            end_ = data_ + capacity_ - 1;
        } else {
            end_ = data_ + capacity_;
        }
    }
    size_--;
    isFull = false;
}

template<class T, class A>
void CCircularBuffer<T, A>::reserve(size_t newCapacity){
    if (size_ == 0) {
        alloc.deallocate(data_, capacity_);
        capacity_ = newCapacity;
        data_ = alloc.allocate(capacity_);
        begin_ = data_;
        end_ = data_;
        return;
    }
    T* data_temp = alloc.allocate(newCapacity);
    size_t i = 0;
    for(Iterator it = begin(); it != end(); it++, i++) {
        std::construct_at(data_temp + i, *it);
        std::destroy_at(it.point);
    }
    alloc.deallocate(data_, capacity_);
    capacity_ = newCapacity;
    data_ = data_temp;
    begin_ = data_;
    end_ = size_ < capacity_ ? data_ + size_ : data_;
    isFull = size_ == capacity_;
}

template<class T, class A>
void CCircularBuffer<T, A>::resize(size_t newSize) {
    if (newSize > size_) {
        if (newSize > capacity_) {
            reserve(newSize);
        }
        Iterator it = end();
        insert(it, newSize - size_, T());
    } else {
        Iterator it = end();
        it -= (size_ - newSize);
        Iterator e = end();
        erase(it, e);
    }
    size_ = newSize;
}

template<class T, class A>
template<std::forward_iterator iter>
void CCircularBuffer<T, A>::assign(iter it1, iter it2) {
    CCircularBuffer<T, A> temp(it1, it2);
    this->swap(temp);
}

template<class T, class A>
void CCircularBuffer<T, A>::assign(std::initializer_list<T> il) {
    CCircularBuffer<T, A> temp(il);
    this->swap(temp);
}

template<class T, class A>
void CCircularBuffer<T, A>::assign(size_t n, T t) {
    CCircularBuffer<T, A> temp(n, t);
    this->swap(temp);
}

template<class T, class A>
void CCircularBuffer<T, A>::clear() {
    for (size_t i = 0; i < size_; i++) {
        std::destroy_at(data_ + i);
    }
    size_ = 0;
    begin_ = data_;
    end_ = data_;
    isFull = false;
}

template<class T, class A>
CCircularBuffer<T, A>::CCircularBuffer(const CCircularBuffer& cont): size_(cont.size_), capacity_(cont.capacity_), isFull(cont.isFull) {
    data_ = alloc.allocate(capacity_);
    size_t i = 0;
    if (capacity_ > 0) {
        for (const_Iterator it = cont.cbegin(); it != cont.cend(); it++, i++) {
            std::construct_at(data_ + i, *it);
        }
    }
    begin_ = data_;
    end_ = isFull ? data_ : data_ + size_;
}

template<class T, class A>
CCircularBuffer<T, A>::CCircularBuffer(const std::initializer_list<T> &il) :
        data_(alloc.allocate(il.size())),
        size_(il.size()), capacity_(il.size()),
        begin_(data_), end_(data_), isFull(true){
            size_t i = 0;
            for (auto it = il.begin(); it != il.end(); i++, it++) {
                std::construct_at(data_ + i, *it);
            }
}

template<class T, class A>
template<std::forward_iterator iter>
CCircularBuffer<T, A>::CCircularBuffer(iter it1, iter it2): data_(alloc.allocate(it2 - it1)),
                                                            size_(it2 - it1), capacity_(it2 - it1),
                                                            begin_(data_), end_(data_), isFull(true){
    size_t i = 0;
    for (auto it = it1; it != it2; i++, it++) {
        std::construct_at(data_ + i, *it);
    }
}

template<class T, class A>
CCircularBuffer<T, A>::CCircularBuffer(): data_ (0), size_(0), capacity_(0), begin_(0), end_(0), isFull(true){}

template<class T, class A>
CCircularBuffer<T, A>::CCircularBuffer(const size_t size): size_(size), capacity_(size), data_(alloc.allocate(size)),
                                                           begin_(data_), end_(data_), isFull(false){
    for (size_t i = 0; i < capacity_; i++) {
        std::construct_at(data_ + i, T());
    }
}

template<class T, class A>
CCircularBuffer<T, A>::CCircularBuffer(const size_t size, const T value): size_(size), capacity_(size), data_(alloc.allocate(capacity_)),
                                                                          begin_(data_), end_(data_), isFull(true){
    for (size_t i = 0; i < size; i++) {
        std::construct_at(data_ + i, value);
    }
};

template<class T, class A>
CCircularBuffer<T, A>::~CCircularBuffer(){
    if (capacity_ != 0) {
        for (size_t i = 0; i < capacity_; i++) {
            std::destroy_at(data_ + i);
        }
        alloc.deallocate(data_, capacity_);
    }
}

template<class T, class A>
void CCircularBuffer<T, A>::swap(CCircularBuffer& b) {
    T* temp1 = data_;
    data_ = b.data_;
    b.data_ = temp1;

    size_t temp = size_;
    size_ = b.size_;
    b.size_ = temp;

    temp = capacity_;
    capacity_ = b.capacity_;
    b.capacity_ = temp;

    T* temp2 = begin_;
    begin_ = b.begin_;
    b.begin_ = temp2;

    T* temp3 = end_;
    end_ = b.end_;
    b.end_ = temp3;

    isFull = size_ == capacity_;
    b.isFull = b.size_ == b.capacity_;
}

template<class T, class A>
void swap(CCircularBuffer<T, A>& a, CCircularBuffer<T, A>& b) {
    a.swap(b);
}

template<class T, class A>
bool CCircularBuffer<T, A>::empty() const {
    return size_ == 0;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::begin() {
    return Iterator(begin_, this, true);
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::begin() const {
    return Iterator(begin_, this, true);
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator CCircularBuffer<T, A>::cbegin() const {
    return const_Iterator(begin_, this, true);
}

template<class T, class A>
T& CCircularBuffer<T, A>::front()  {
    return *begin_;
}

template<class T, class A>
const T& CCircularBuffer<T, A>::front() const {
    return *begin_;
}

template<class T, class A>
T& CCircularBuffer<T, A>::back()  {
    return *(end() - 1);
}

template<class T, class A>
const T& CCircularBuffer<T, A>::back() const {
    return *(end() - 1);
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::end()  {
    return Iterator(end_, this, false, true);
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator CCircularBuffer<T, A>::end() const {
    return Iterator(end_, this, false, true);
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator CCircularBuffer<T, A>::cend() const {
    return const_Iterator(end_, this, false, true);
}


template<class T, class A>
typename CCircularBuffer<T, A>::size_type CCircularBuffer<T, A>::size() const {
    return size_;
}

template<class T, class A>
typename CCircularBuffer<T, A>::size_type CCircularBuffer<T, A>::capacity() const {
    return capacity_;
}

template<class T, class A>
T& CCircularBuffer<T, A>::operator[](size_type index) {
    return *(begin() + index);
}

template<class T, class A>
const T& CCircularBuffer<T, A>::operator[](size_type index) const {
    return *(begin() + index);
}


template<class T, class A>
bool operator==(const CCircularBuffer<T, A> &cont1, const CCircularBuffer<T, A> &cont2) {
    return (cont1.size() == cont2.size() && std::equal(cont1.begin(), cont1.end(), cont2.begin()));
}

template<class T, class A>
bool CCircularBuffer<T, A>::Iterator::operator==(Iterator const& other) const {
    if (!(isBegin && other.isEnd || isEnd && other.isBegin)) {
        return point == other.point;
    }
    return false;
}

template<class T, class A>
bool CCircularBuffer<T, A>::Iterator::operator!=(Iterator const& other) const {
    if (!(isBegin && other.isEnd || isEnd && other.isBegin)) {
        return point != other.point;
    }
    return true;
}

template<class T, class A>
bool CCircularBuffer<T, A>::Iterator::operator<(const Iterator& it) const {
    return linearize() - it.linearize() < 0;
}

template<class T, class A>
bool CCircularBuffer<T, A>::Iterator::operator>(const Iterator& it) const {
    return linearize() - it.linearize() > 0;
}

template<class T, class A>
bool CCircularBuffer<T, A>::Iterator::operator>=(const Iterator& it) const {
    return !(*this < it);
}

template<class T, class A>
bool CCircularBuffer<T, A>::Iterator::operator<=(const Iterator& it) const {
    return !(*this > it);
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator::pointer CCircularBuffer<T, A>::Iterator::operator->() const {
    return point;
}

template<class T, class A>
typename CCircularBuffer<T, A>::Iterator::reference CCircularBuffer<T, A>::Iterator::operator*() const {
    return *point;
}

template<class T, class A>
CCircularBuffer<T, A>::Iterator::Iterator(T* p, CCircularBuffer<T, A>* cont, bool isbeg, bool isend): point(p), cont_(cont), isBegin(isbeg), isEnd(isend){}
template<class T, class A>
CCircularBuffer<T, A>::Iterator::Iterator(const Iterator& it): point(it.point), cont_(it.cont_), isBegin(it.isBegin), isEnd(it.isEnd){}

template<class T, class A>
CCircularBuffer<T, A>::const_Iterator::const_Iterator(T* p, const CCircularBuffer<T, A>* cont, bool isbeg, bool isend): point(p), cont_(cont), isBegin(isbeg), isEnd(isend){}
template<class T, class A>
CCircularBuffer<T, A>::const_Iterator::const_Iterator(const const_Iterator& it): point(it.point), cont_(it.cont_), isBegin(it.isBegin), isEnd(it.isEnd){}
template<class T, class A>
CCircularBuffer<T, A>::const_Iterator::const_Iterator(const Iterator& it): point(it.point), cont_(it.cont_), isBegin(it.isBegin), isEnd(it.isEnd){}

template<class T, class A>
bool CCircularBuffer<T, A>::const_Iterator::operator==(const const_Iterator& other) const {
    if (!(isBegin && other.isEnd || isEnd && other.isBegin)) {
        return point == other.point;
    }
    return false;
}

template<class T, class A>
bool CCircularBuffer<T, A>::const_Iterator::operator!=(const const_Iterator& other) const {
    if (!(isBegin && other.isEnd || isEnd && other.isBegin)) {
        return point != other.point;
    }
    return true;
}

template<class T, class A>
bool CCircularBuffer<T, A>::const_Iterator::operator<(const const_Iterator& it) const {
    return linearize() - it.linearize() < 0;
}

template<class T, class A>
bool CCircularBuffer<T, A>::const_Iterator::operator>(const const_Iterator& it) const {
    return linearize() - it.linearize() > 0;
}

template<class T, class A>
bool CCircularBuffer<T, A>::const_Iterator::operator>=(const const_Iterator& it) const {
    return !(*this < it);
}

template<class T, class A>
bool CCircularBuffer<T, A>::const_Iterator::operator<=(const const_Iterator& it) const {
    return !(*this > it);
}

template<class T, class A>
const typename CCircularBuffer<T, A>::const_Iterator::pointer CCircularBuffer<T, A>::const_Iterator::operator->() const {
    return point;
}

template<class T, class A>
const typename CCircularBuffer<T, A>::const_Iterator::reference CCircularBuffer<T, A>::const_Iterator::operator*(){
    return *point;
}

template<class T, class A>
typename CCircularBuffer<T, A>::const_Iterator::reference CCircularBuffer<T, A>::const_Iterator::operator*() const { //TODO
    return *point;
}

template<class T, class A>
T* CCircularBuffer<T, A>::const_Iterator::add(T* p, difference_type n) const {
    return p + (n < (cont_->data_ + cont_->capacity_ - p) ? n : n - cont_->capacity_);
}

template<class T, class A>
T* CCircularBuffer<T, A>::const_Iterator::sub(T* p, difference_type n) const {
    return p - (n > (p - cont_->data_) ? n - cont_->capacity_ : n);
}

template<class T, class A>
T* CCircularBuffer<T, A>::Iterator::add(T* p, difference_type n) const {
    return p + (n < (cont_->data_ + cont_->capacity_ - p) ? n : n - cont_->capacity_);
}

template<class T, class A>
T* CCircularBuffer<T, A>::Iterator::sub(T* p, difference_type n) const {
    return p - (n > (p - cont_->data_) ? n - cont_->capacity_ : n);
}
