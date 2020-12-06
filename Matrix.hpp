#ifndef _MATRIX_
#define _MATRIX_
#include <cstddef>
#include <initializer_list>
#include <iostream>

class Matrix
{

private:
    bool __flag;     //mark whether the memory is belong to the object itself
    char *__buff;    //buffer
    float *__data;   //data
    size_t __rows;   //row
    size_t __cols;   //col
    int *__refcount; //reference times
    bool check() const;

public:
    Matrix();
    Matrix(const Matrix &m);           //copy constructor
    Matrix(const Matrix &&m) noexcept; //move constructor
    Matrix(const size_t rows, const size_t cols, const float data = 0);
    Matrix(const std::initializer_list<std::initializer_list<float>> &list);
    ~Matrix();

    Matrix &operator=(const Matrix &m);
    size_t cols() const;
    size_t rows() const;
    void set(const size_t rows, const size_t cols, const float data = 0);

    float *operator[](const size_t i);
    const float *operator[](const size_t i) const; //provide access to the const vriabal

    friend std::ostream &operator<<(std::ostream &os, Matrix &&m);
    friend std::ostream &operator<<(std::ostream &os, Matrix &m);
    friend std::istream &operator>>(std::istream &is, Matrix &m);

    friend bool operator==(const Matrix &m1, const Matrix &m2);
    friend bool operator!=(const Matrix &m1, const Matrix &m2);

    friend Matrix operator*(const Matrix &m1, const Matrix &m2);
    friend Matrix operator*(const float n, const Matrix &m);
    friend Matrix operator*(const Matrix &m, const float n) { return n * m; }
};

#endif
