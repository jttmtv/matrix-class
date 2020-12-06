#include "Matrix.hpp"
#include <cblas.h>
#include <cstring>
#include <initializer_list>

#define MAXSIZE 100     //size of a Matrix bigger than 100 MB, shallow copy should be carry,in order to save the memory
#define CONVERT 1048576 //1024*1024

bool Matrix::check() const
{
    if (((__cols * __rows * sizeof(float)) / CONVERT) > 100)
        return true;
    else
        return false;
}

Matrix::Matrix()
{
    __rows = 1;
    __cols = 1;
    __flag = true;
    __buff = new char[sizeof(float) + sizeof(int)];
    __data = new (__buff) float;
    __refcount = new (__buff + sizeof(float)) int;
    memset(__buff, 0, sizeof(float) + sizeof(int));
}

Matrix::Matrix(const size_t rows, const size_t cols, const float data)
{
    if (!(rows && cols))
    {
        __rows = 1;
        __cols = 1;
    }
    else
    {
        __rows = rows;
        __cols = cols;
    }
    __flag = true;
    __buff = new char[sizeof(float) * __rows * __cols + sizeof(int)];
    __data = new (__buff) float[__rows * __cols];
    __refcount = new (__buff + sizeof(float) * __rows * __cols) int;
    if (rows && cols)
        *__refcount = 1;
    else
        *__refcount = 0;
    for (size_t i = 0; i < rows * cols; ++i)
        __data[i] = data;
}

Matrix::Matrix(const Matrix &m)
{
    __rows = m.__rows;
    __cols = m.__cols;
    if (m.check())
    {
        __flag = false;
        __buff = m.__buff;
        __data = m.__data;
        __refcount = m.__refcount;
        ++*__refcount;
    }
    else
    {
        __flag = true;
        __buff = new char[sizeof(float) * __rows * __cols + sizeof(int)];
        __data = new (__buff) float[__rows * __cols];
        __refcount = new (__buff + sizeof(float) * __rows * __cols) int;
        memcpy(__buff, m.__buff, sizeof(float) * __rows * __cols + sizeof(int));
    }
}

Matrix::Matrix(const Matrix &&m) noexcept
{
    __rows = m.__rows;
    __cols = m.__cols;
    __flag = false;
    __buff = m.__buff;
    __data = m.__data;
    __refcount = m.__refcount;
    ++*__refcount;
}

Matrix::Matrix(const std::initializer_list<std::initializer_list<float>> &list)
{
    size_t max = 0;
    for (auto iter = list.begin(); iter != list.end(); ++iter)
        if (iter->size() > max)
            max = iter->size();
    __rows = list.size();
    __cols = max;
    if (!(__rows && __cols))
    {
        __rows = 4;
        __cols = 4;
    }
    __flag = true;
    __buff = new char[sizeof(float) * __rows * __cols + sizeof(int)];
    __data = new (__buff) float[__rows * __cols];
    __refcount = new (__buff + sizeof(float) * __rows * __cols) int;
    if (__rows && __cols)
        *__refcount = 1;
    else
    {
        memset(__buff, 0, sizeof(float) * 16 + sizeof(int));
        *__refcount = 0;
    }
    size_t i = 0, j = 0;
    for (auto iter1 = list.begin(); iter1 != list.end(); ++iter1, ++i)
    {
        j = 0;
        for (auto iter2 = iter1->begin(); iter2 != iter1->end(); ++iter2, ++j)
            __data[i * __cols + j] = *iter2;
        for (size_t k = j; k < __cols; ++k)
            __data[i * __cols + k] = 0;
    }
}

size_t Matrix::rows() const
{
    if (*__refcount)
        return __rows;
    else
        return 0;
}

size_t Matrix::cols() const
{
    if (*__refcount)
        return __cols;
    else
        return 0;
}

void Matrix::set(const size_t rows, const size_t cols, float data)
{
    delete[] __buff;
    if (!(rows && cols))
    {
        __rows = 4;
        __cols = 4;
    }
    else
    {
        __rows = rows;
        __cols = cols;
    }
    __flag = true;
    __buff = new char[sizeof(float) * __rows * __cols + sizeof(int)];
    __data = new (__buff) float[__rows * __cols];
    __refcount = new (__buff + sizeof(float) * __rows * __cols) int;
    if (rows && cols)
        *__refcount = 1;
    else
        *__refcount = 0;
    for (size_t i = 0; i < rows * cols; ++i)
        __data[i] = data;
}

Matrix &Matrix::operator=(const Matrix &m)
{
    __rows = m.__rows;
    __cols = m.__cols;
    if (this != &m)
    {
        delete[] __buff;
        if (m.check())
        {
            if (*__refcount > 1)
                --*__refcount;
            __flag = false;
            __buff = m.__buff;
            __data = m.__data;
            __refcount = m.__refcount;
            ++*__refcount;
        }
        else
        {
            __flag = true;
            __buff = new char[sizeof(float) * __rows * __cols + sizeof(int)];
            __data = new (__buff) float[__rows * __cols];
            __refcount = new (__buff + sizeof(float) * __rows * __cols) int;
            memcpy(__buff, m.__buff, sizeof(float) * __rows * __cols + sizeof(int));
        }
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, Matrix &&m)
{
    if (*m.__refcount == 0)
        os << "[]" << std::endl;
    else if (m.__rows == 1)
    {
        os << "[";
        for (size_t i = 0; i < m.__cols - 1; ++i)
            os << m.__data[i] << ", ";
        os << m.__data[m.__cols - 1] << "]" << std::endl;
    }
    else
    {
        os << "[";
        for (size_t i = 0; i < m.__cols - 1; ++i)
            os << m.__data[i] << ", ";
        os << m.__data[m.__cols - 1] << ";" << std::endl;
        for (size_t i = 1; i < m.__rows - 1; ++i)
        {
            os << " ";
            for (size_t j = 0; j < m.__cols - 1; ++j)
                os << m.__data[i * m.__cols + j] << ", ";
            os << m.__data[i * m.__cols + m.__cols - 1] << ";" << std::endl;
        }
        os << " ";
        for (size_t i = 0; i < m.__cols - 1; ++i)
            os << m.__data[(m.__rows - 1) * m.__cols + i] << ", ";
        os << m.__data[(m.__rows - 1) * m.__cols + m.__cols - 1] << "]" << std::endl;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, Matrix &m)
{
    return operator<<(os, std::move(m));
}

std::istream &operator>>(std::istream &is, Matrix &m)
{
    if (*m.__refcount == 0)
    {
        delete[] m.__buff;
        std::cout << "Rows:";
        is >> m.__rows;
        std::cout << "Cols:";
        is >> m.__cols;
        if (m.__rows && m.__cols)
        {
            m.__flag = true;
            m.__buff = new char[sizeof(float) * m.__rows * m.__cols + sizeof(int)];
            m.__data = new (m.__buff) float[m.__rows * m.__cols];
            m.__refcount = new (m.__buff + sizeof(float) * m.__rows * m.__cols) int;
            *m.__refcount = 1;
        }
    }
    else
    {
        for (size_t i = 0; i < m.__rows; ++i)
            for (size_t j = 0; j < m.__cols; ++j)
                is >> m.__data[i * m.__cols + j];
    }
    return is;
}

float *Matrix::operator[](const size_t i)
{
    if (!*__refcount)
        std::cerr << "operator[][]:Uninitialized memory!\n";
    return __data + i * __cols;
}

const float *Matrix::operator[](const size_t i) const
{
    if (!*__refcount)
        std::cerr << "operator[][]:Uninitialized memory!\n";
    return __data + i * __cols;
}

bool operator==(const Matrix &m1, const Matrix &m2)
{
    if (m1.__rows == m2.__rows && m1.__cols == m2.__cols)
        for (size_t i = 0; i < m1.__rows * m1.__cols; ++i)
            if (m1.__data[i] != m2.__data[i])
                return false;
    return true;
}

bool operator!=(const Matrix &m1, const Matrix &m2)
{
    return !operator==(m1, m2);
}

Matrix::~Matrix()
{
    if (__flag && *__refcount <= 1)
        delete[] __buff;
    else
        --*__refcount;
}

Matrix operator*(const Matrix &m1, const Matrix &m2)
{
    if (*m1.__refcount && *m2.__refcount && m1.__cols == m2.__rows)
    {
        Matrix tmp(m1.__rows, m2.__cols);
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.__rows, m2.__cols, m1.__cols, 1.0f, m1.__data, m1.__cols, m2.__data, m2.__cols, 1.0f, tmp.__data, tmp.__cols);
        return tmp;
    }
    else
    {
        std::cerr << "operator *: Empty matrix or two matrices don't satisfy the condition of multiplication!\n";
        return Matrix();
    }
}

Matrix operator*(const float n, const Matrix &m)
{
    if (*m.__refcount)
        for (size_t i = 0; i < m.__rows * m.__cols; ++i)
            m.__data[i] *= n;
    else
        return m;
}
