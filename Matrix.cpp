#include "Matrix.hpp"
#include <cblas.h>
#include <cstring>

#define MAXSIZE 100     //size of a Matrix bigger than 100 MB, shallow copy should be carry,in order to save the memory
#define CONVERT 1048576 //1024*1024

bool Matrix::check() const
{
    if (((__cols * __rows * sizeof(float)) / CONVERT) > 100)
        return true;
    else
        return false;
}

void memset(Matrix &m, const size_t rows, const size_t cols, const float data)
{
    if (!(rows && cols))
    {
        m.__rows = 1;
        m.__cols = 1;
    }
    else
    {
        m.__rows = rows;
        m.__cols = cols;
    }
    m.__flag = true;
    m.__buff = new char[sizeof(float) * m.__rows * m.__cols + sizeof(int)];
    m.__data = new (m.__buff) float[m.__rows * m.__cols];
    m.__refcount = new (m.__buff + sizeof(float) * m.__rows * m.__cols) int;
    for ( size_t i = 0; i < m.__rows * m.__cols; ++i)
        m.__data[i] = data;
    if (rows && cols)
        *m.__refcount = 1;
    else
        *m.__refcount = 0;
}

void memcpy(Matrix &m1, const Matrix &m2)
{
    m1.__rows = m2.__rows;
    m1.__cols = m2.__cols;
    m1.__flag = false;
    m1.__buff = m2.__buff;
    m1.__data = m2.__data;
    m1.__refcount = m2.__refcount;
    ++*m1.__refcount;
}

Matrix::Matrix()
{
    memset(*this, 1, 1, 0);
    *__refcount = 0;
}

Matrix::Matrix(const size_t rows, const size_t cols, const float data)
{
    memset(*this, rows, cols, data);
}

Matrix::Matrix(const Matrix &m)
{
    if (m.check())
        memcpy(*this, m);
    else
    {
        memset(*this, __rows, __cols, 0);
        memcpy(__data, m.__data, sizeof(float) * __rows * __cols);
    }
}

Matrix::Matrix(const Matrix &&m) noexcept
{
    memcpy(*this, m);
}

Matrix::Matrix(const std::initializer_list<std::initializer_list<float>> &list)
{
    size_t max = 0;
    for (auto iter = list.begin(); iter != list.end(); ++iter)
        if (iter->size() > max)
            max = iter->size();
    memset(*this, list.size(), max, 0);
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

const size_t Matrix::rows() const
{
    if (*__refcount)
        return __rows;
    else
        return 0;
}

const size_t Matrix::cols() const
{
    if (*__refcount)
        return __cols;
    else
        return 0;
}

void Matrix::set(const size_t rows, const size_t cols, const float data)
{
    if (*__refcount > 1)
        --*__refcount;
    delete[] __buff;
    memset(*this, rows, cols, data);
}

Matrix &Matrix::operator=(const Matrix &m)
{
    if (this != &m)
    {
        if (m.check())
        {
            if (*__refcount > 1)
                --*__refcount;
            delete[] __buff;
            memcpy(*this, m);
        }
        else
        {
            delete[] __buff;
            memset(*this, m.__rows, m.__cols, 0);
            memcpy(__data, m.__data, m.__rows * m.__cols);
        }
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, Matrix &&m)
{
    if (*m.__refcount == 0)
        os << "[empty!]" << std::endl;
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
        memset(m, m.__rows, m.__cols, 0);
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
        for ( size_t i = 0; i < m1.__rows * m1.__cols; ++i)
            if (m1.__data[i] != m2.__data[i])
                return false;
    return true;
}

Matrix::~Matrix()
{
    if (__flag && *__refcount <= 1)
        delete[] __buff;
    else if (!__flag && *__refcount == 1)
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
        std::cerr << "operator *: Empty matrix or two matrices don't "
                  << "satisfy the condition of multiplication!\n";
        return Matrix();
    }
}

Matrix operator*(const float n, const Matrix &m)
{
    Matrix tmp(m.__rows, m.__cols);
    memcpy(tmp.__data, m.__data, sizeof(float) * m.__cols * m.__rows);
    if (*m.__refcount)
        for ( size_t i = 0; i < tmp.__rows * tmp.__cols; ++i)
            tmp.__data[i] *= n;
    else
        std::cerr << "The multiplier is an empty matrix!" << std::endl;
    return tmp;
}
