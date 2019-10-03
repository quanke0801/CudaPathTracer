#pragma once

#include <algorithm>
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <math.h>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include <cuda_runtime.h>

#define HOST __host__
#define DEVICE __device__
#define GLOBAL __global__

#define CUDACHECK(cmd) do { \
    cudaError_t e = cmd;    \
    if( e != cudaSuccess ) { \
    printf("Failed: Cuda error %s:%d '%s'\n", \
        __FILE__,__LINE__,cudaGetErrorString(e)); \
    exit(EXIT_FAILURE);     \
  } \
} while(0)

typedef double Scalar;

#define PI 3.1415926535897932384626
#define SCALAR_DELTA 1.0E-10
#define SCALAR_LOOSE_DELTA 1.0E-5
#define SELF_INTERSECT_THRESHOLD 1.0E-5

class Managed {
public:
    HOST void *operator new (size_t _size) {
        void *pointer;
        cudaMallocManaged(&pointer, _size);
        CUDACHECK(cudaDeviceSynchronize());
        return pointer;
    }
    HOST void operator delete (void *_pointer) {
        CUDACHECK(cudaDeviceSynchronize());
        cudaFree(_pointer);
    }
};

template <typename T>
class Vector {
public:
    HOST Vector() {
        this->size_ = 0;
        this->capacity_ = 1;
        cudaMallocManaged(&this->data_, sizeof(T) * this->capacity_);
        CUDACHECK(cudaDeviceSynchronize());
    }
    HOST Vector(const int _size) {
        this->size_ = _size;
        this->capacity_ = 1;
        while (this->capacity_ < _size) {
            this->capacity_ *= 2;
        }
        cudaMallocManaged(&this->data_, sizeof(T) * this->capacity_);
        CUDACHECK(cudaDeviceSynchronize());
    }
    HOST Vector(const int _size, const T &_value) {
        this->size_ = _size;
        this->capacity_ = 1;
        while (this->capacity_ < _size) {
            this->capacity_ *= 2;
        }
        cudaMallocManaged(&this->data_, sizeof(T) * this->capacity_);
        CUDACHECK(cudaDeviceSynchronize());
        for (int i = 0; i < this->size_; ++ i) {
            this->data_[i] = _value;
        }
    }
    HOST Vector(const Vector &_other) {
        this->size_ = _other.size_;
        this->capacity_ = _other.capacity_;
        cudaMallocManaged(&this->data_, sizeof(T) * this->capacity_);
        CUDACHECK(cudaDeviceSynchronize());
        for (int i = 0; i < this->size_; ++ i) {
            this->data_[i] = _other[i];
        }
    }
    HOST ~Vector() {
        CUDACHECK(cudaDeviceSynchronize());
        cudaFree(this->data_);
    }
    HOST DEVICE int Size() const {
        return this->size_;
    }
    HOST DEVICE const T *Data() const {
        return this->data_;
    }
    HOST DEVICE T *Data() {
        return this->data_;
    }
    HOST DEVICE const T &operator [] (const int _index) const {
        return this->data_[_index];
    }
    HOST DEVICE T &operator [] (const int _index) {
        return this->data_[_index];
    }
    HOST void Reserve(const int _size) {
        if (_size <= this->capacity_) {
            return;
        }
        while (this->capacity_ < _size) {
            this->capacity_ *= 2;
        }
        T *new_data = nullptr;
        cudaMallocManaged(&new_data, sizeof(T) * this->capacity_);
        CUDACHECK(cudaDeviceSynchronize());
        for (int i = 0; i < this->size_; ++ i) {
            new_data[i] = this->data_[i];
        }
        CUDACHECK(cudaDeviceSynchronize());
        cudaFree(this->data_);
        this->data_ = new_data;
    }
    HOST void PushBack(const T &_value) {
        if (this->size_ == this->capacity_) {
            this->capacity_ *= 2;
            T *new_data = nullptr;
            cudaMallocManaged(&new_data, sizeof(T) * this->capacity_);
            CUDACHECK(cudaDeviceSynchronize());
            for (int i = 0; i < this->size_; ++ i) {
                new_data[i] = this->data_[i];
            }
            CUDACHECK(cudaDeviceSynchronize());
            cudaFree(this->data_);
            this->data_ = new_data;
        }
        this->data_[this->size_] = _value;
        ++ this->size_;
    }
    HOST void PopBack() {
        if (this->size_ == 0) {
            return;
        }
        this->data_[this->size_ - 1].~T();
        -- this->size_;
    }
    HOST void Clear() {
        for (int i = 0; i < this->size_; ++ i) {
            this->data_[i].~T();
        }
        this->size_ = 0;
    }
private:
    int size_;
    int capacity_;
    T *data_;
};

template <typename T1, typename T2>
class Pair {
public:
    HOST DEVICE Pair() {
    }
    HOST DEVICE Pair(const T1 &_first, const T2 &_second) {
        this->first = _first;
        this->second = _second;
    }
    T1 first;
    T2 second;
};

