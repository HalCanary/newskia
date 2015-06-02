/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef Memory_DEFINED
#define Memory_DEFINED

#include <cassert>
#include <cstdlib>
#include <atomic>

namespace drawing {

template <typename T>
class Memory {
public:
    Memory() : body_(nullptr) {}
    Memory(size_t);
    Memory(T*, size_t, void (*)(T*, size_t, void*), void*);
    size_t size() { return body_ ? body_->size_ : 0; }
    const T* get() const { return body_ ? body_->ptr_ : nullptr; }
    T* get() { return body_ ? body_->ptr_ : nullptr; }
    const T& operator[](int i) const;
    T& operator[](int i);
    ~Memory();
    Memory(Memory<T>& other) { this->copyFrom(other); }
    Memory(const Memory<T>& other) { this->copyFrom(other); }
    Memory<T>& operator=(const Memory<T>& other);
    Memory(Memory<T>&& other) { this->moveFrom((Memory<T> && )other); }
    Memory<T>& operator=(Memory<T>&& other);
    explicit operator bool() const { return body_ != nullptr; }

private:
    void copyFrom(const Memory<T>& other);
    void moveFrom(Memory<T>&& other);
    struct Body {
        std::atomic<int> ref_count_;
        const size_t size_;
        T* ptr_;
        void (*release_)(T* ptr, size_t length, void* context);
        void* context_;
        T data_;
    } * body_;
};

Memory<const char> OpenFile(const char* path);

////////////////////////////////////////////////////////////////////////////////
template <typename T>
Memory<T>::Memory(T* ptr,
                  size_t size,
                  void (*release)(T*, size_t, void*),
                  void* context)
    : body_(nullptr) {
    body_ = (Body*)calloc(1, sizeof(Body) - sizeof(T));
    assert(body_);
    new (&(body_->ref_count_)) std::atomic<int>(1);
    const_cast<size_t&>(body_->size_) = size;
    body_->ptr_ = ptr;
    body_->release_ = release;
    body_->context_ = context;
}

template <typename T>
Memory<T>::Memory(size_t size)
    : body_(nullptr) {
    if (size) {
        body_ = (Body*)calloc(1, sizeof(Body) + sizeof(T) * (size - 1));
        assert(body_);
        new (&(body_->ref_count_)) std::atomic<int>(1);
        const_cast<size_t&>(body_->size_) = size;
        body_->ptr_ = &body_->data_;
        body_->release_ = nullptr;
        body_->context_ = nullptr;
    }
}
template <typename T>
const T& Memory<T>::operator[](int i) const {
    assert(body_);
    assert(i >= 0);
    assert(i < body_->size_);
    return this->get()[i];
}
template <typename T>
T& Memory<T>::operator[](int i) {
    assert(body_);
    assert(i >= 0);
    assert(i < body_->size_);
    return this->get()[i];
}
template <typename T>
Memory<T>::~Memory() {
    if (body_ && 1 == (body_->ref_count_)--) {
        if (body_->release_) {
            body_->release_(body_->ptr_, body_->size_, body_->context_);
        }
        free(body_);
    }
}
template <typename T>
Memory<T>& Memory<T>::operator=(const Memory<T>& other) {
    if (this != &other) {
        this->~Memory<T>();
        this->copyFrom(other);
    }
    return *this;
}
template <typename T>
Memory<T>& Memory<T>::operator=(Memory<T>&& other) {
    if (this != &other) {
        this->~Memory<T>();
        this->moveFrom((Memory<T> && )other);
    }
    return *this;
}
template <typename T>
void Memory<T>::copyFrom(const Memory<T>& other) {
    assert(this != &other);
    body_ = other.body_;
    if (body_) {
        (body_->ref_count_)++;
    }
}
template <typename T>
void Memory<T>::moveFrom(Memory<T>&& other) {
    assert(this != &other);
    body_ = other.body_;
    other.body_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class ConstMemory {
public:
    ConstMemory() : body_(nullptr) {}
    ConstMemory(size_t);
    ConstMemory(const T*, size_t, void (*)(const T*, size_t, void*), void*);
    size_t size() { return body_ ? body_->size_ : 0; }
    const T* get() const { return body_ ? body_->ptr_ : nullptr; }
    const T& operator[](int i) const;
    ~ConstMemory();
    ConstMemory(ConstMemory<T>& other) { this->copyFrom(other); }
    ConstMemory(const ConstMemory<T>& other) { this->copyFrom(other); }
    ConstMemory<T>& operator=(const ConstMemory<T>& other);
    ConstMemory(ConstMemory<T>&& other) {
        this->moveFrom((ConstMemory<T> && )other);
    }
    ConstMemory<T>& operator=(ConstMemory<T>&& other);
    explicit operator bool() const { return body_ != nullptr; }

private:
    void copyFrom(const ConstMemory<T>& other);
    void moveFrom(ConstMemory<T>&& other);
    struct Body {
        std::atomic<int> ref_count_;
        const size_t size_;
        const T* ptr_;
        void (*release_)(const T* ptr, size_t length, void* context);
        void* context_;
        const T data_;
    } * body_;
};

template <typename T>
ConstMemory<T>::ConstMemory(const T* ptr,
                            size_t size,
                            void (*release)(const T*, size_t, void*),
                            void* context)
    : body_(nullptr) {
    body_ = (Body*)calloc(1, sizeof(Body) - sizeof(T));
    assert(body_);
    new (&(body_->ref_count_)) std::atomic<int>(1);
    const_cast<size_t&>(body_->size_) = size;
    body_->ptr_ = ptr;
    body_->release_ = release;
    body_->context_ = context;
}

template <typename T>
ConstMemory<T>::ConstMemory(size_t size)
    : body_(nullptr) {
    if (size) {
        body_ = (Body*)calloc(1, sizeof(Body) + sizeof(T) * (size - 1));
        assert(body_);
        new (&(body_->ref_count_)) std::atomic<int>(1);
        const_cast<size_t&>(body_->size_) = size;
        body_->ptr_ = &body_->data_;
        body_->release_ = nullptr;
        body_->context_ = nullptr;
    }
}
template <typename T>
const T& ConstMemory<T>::operator[](int i) const {
    assert(body_);
    assert(i >= 0);
    assert(i < body_->size_);
    return this->get()[i];
}

template <typename T>
ConstMemory<T>::~ConstMemory() {
    if (body_ && 1 == (body_->ref_count_)--) {
        if (body_->release_) {
            body_->release_(body_->ptr_, body_->size_, body_->context_);
        }
        free(body_);
    }
}

template <typename T>
ConstMemory<T>& ConstMemory<T>::operator=(const ConstMemory<T>& other) {
    if (this != &other) {
        this->~ConstMemory<T>();
        this->copyFrom(other);
    }
    return *this;
}

template <typename T>
ConstMemory<T>& ConstMemory<T>::operator=(ConstMemory<T>&& other) {
    if (this != &other) {
        this->~ConstMemory<T>();
        this->moveFrom((ConstMemory<T> && )other);
    }
    return *this;
}
template <typename T>
void ConstMemory<T>::copyFrom(const ConstMemory<T>& other) {
    assert(this != &other);
    body_ = other.body_;
    if (body_) {
        (body_->ref_count_)++;
    }
}
template <typename T>
void ConstMemory<T>::moveFrom(ConstMemory<T>&& other) {
    assert(this != &other);
    body_ = other.body_;
    other.body_ = nullptr;
}
}

#endif  // Memory_DEFINED
