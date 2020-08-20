#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <ostream>
#include <type_traits>
#include <array>
#include <ostream>

template <typename T, size_t x, size_t y>
class Matrix
{
public:
    constexpr Matrix()
    :_matrix()
    {
        static_assert (x > 0 && y > 0, "size of Matrix have to be positive");
    }
    constexpr Matrix(const T& defaultValue)
        :_matrix()
    {
        static_assert (x > 0 && y > 0, "size of Matrix have to be positive");
        for (size_t i = 0; i < y; i++)
            for (size_t j = 0; j < x; j++)
                _matrix[i][j] = defaultValue;

    }
    constexpr Matrix(const Matrix<T, x, y>& matrix)
        :_matrix(matrix._matrix)
    {
        static_assert (x > 0 && y > 0, "size of Matrix have to be positive");
    }
    constexpr Matrix(Matrix<T, x, y>&& matrix)
        :_matrix(std::move(matrix._matrix))
    {
        static_assert (x > 0 && y > 0, "size of Matrix have to be positive");
    }
    constexpr Matrix(const std::array<std::array<T, x>, y>& list)
        :_matrix(list)
    {
        static_assert (x > 0 && y > 0, "size of Matrix have to be positive");
    }
    constexpr Matrix(std::array<std::array<T, x>, y>&& list)
        :_matrix(std::move(list))
    {
        static_assert (x > 0 && y > 0, "size of Matrix have to be positive");
    }
    void operator =(const Matrix<T, x, y>& matrix)
    {
        _matrix = matrix._matrix;
    }
    void operator =(Matrix<T, x, y>&& matrix)
    {
        _matrix = std::move(matrix._matrix);
    }
    void operator =(const std::array<std::array<T, x>, y>& list)
    {
        _matrix = list;
    }
    void operator =(std::array<std::array<T, x>, y>&& list)
    {
        _matrix = std::move(list);
    }
    inline std::array<T, x>& operator[](int index)
    {
        return _matrix[index];
    }
    constexpr inline const std::array<T, x>& operator[](int index) const
    {
        return _matrix[index];
    }
    constexpr inline size_t width() const
    {
        return x;
    }
    constexpr inline size_t height() const
    {
        return y;
    }

private:
    std::array<std::array<T, x>, y> _matrix;
};

template <typename T, typename T2, size_t x, size_t y1, size_t x2>
constexpr decltype(auto) operator *(const Matrix<T, x, y1>& m1, const Matrix<T2, x2, x>& m2)
{
    using return_type = decltype(m1[0][0] * m2[0][0]);
    Matrix<return_type, x2, y1> ans;

    for (size_t k = 0; k < x; k++)
        for (size_t i = 0; i < y1; i++)
            for (size_t j = 0; j < x2; j++)
                ans[i][j] += m1[i][k] * m2[k][j];
    return ans;
}

template <typename T, typename T2, size_t x, size_t y>
constexpr decltype(auto) operator +(const Matrix<T, x, y>& m1, const Matrix<T2, x, y>& m2)
{
    using return_type = decltype(m1[0][0] + m2[0][0]);
    Matrix<return_type, x, y> ans;

    for (size_t i = 0; i < y; i++)
        for (size_t j = 0; j < x; j++)
            ans[i][j] = m1[i][j] + m2[i][j];
    return ans;
}

template <typename T, typename T2, size_t x, size_t y>
void operator +=(Matrix<T, x, y>& m1, const Matrix<T2, x, y>& m2)
{
    using return_type = decltype(m1[0][0] + m2[0][0]);
    Matrix<return_type, x, y> ans;

    for (size_t i = 0; i < y; i++)
        for (size_t j = 0; j < x; j++)
            m1[i][j] += m2[i][j];
}

template <typename T, size_t x, size_t y>
std::ostream& operator << (std::ostream& os, const Matrix<T, x, y>& m)
{
    for (size_t i = 0; i < m.height(); i++)
    {
        for (size_t j = 0; j < m.width(); j++)
        {
            os<<m[i][j]<<' ';
        }
        os<<'\n';
    }
    return os;
}

template <typename T, size_t x, size_t y>
std::istream& operator >> (std::istream& is, Matrix<T, x, y>& m)
{
    for (size_t i = 0; i < m.height(); i++)
    {
        for (size_t j = 0; j < m.width(); j++)
        {
            is>>m[i][j];
        }
    }
    return is;
}

#endif // MATRIX_HPP
