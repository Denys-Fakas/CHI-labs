#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <type_traits>

template <typename T = int>
class Matrix {
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    Matrix(size_t rows = 0, size_t cols = 0) : m_rows(rows), m_cols(cols), m_data(rows* cols) {}
    Matrix(const Matrix& other) : m_rows(other.m_rows), m_cols(other.m_cols), m_data(other.m_data) {}

    Matrix& operator=(const Matrix& other) {
        if (&other == this) {
            return *this;
        }
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        m_data = other.m_data;
        return *this;
    }

    void resize(size_t rows, size_t cols) {
        m_rows = rows;
        m_cols = cols;
        m_data.resize(rows * cols);
    }

    reference operator()(size_t i, size_t j) {
        return m_data[i * m_cols + j];
    }

    const_reference operator()(size_t i, size_t j) const {
        return m_data[i * m_cols + j];
    }

    reference operator[](size_t i) {
        return m_data[i];
    }

    const_reference operator[](size_t i) const {
        return m_data[i];
    }

    size_t rows() const {
        return m_rows;
    }

    size_t cols() const {
        return m_cols;
    }

    void sort_rows() {
        for (size_t i = 0; i < m_rows; i++) {
            auto begin = m_data.begin() + i * m_cols;
            auto end = begin + m_cols;
            std::sort(begin, end);
        }
    }

    void sort_rows(std::function<bool(const_iterator, const_iterator)> comp) {
        for (size_t i = 0; i < m_rows; i++) {
            auto begin = m_data.begin() + i * m_cols;
            auto end = begin + m_cols;
            std::sort(begin, end, comp);
        }
    }

    template<typename U = T>
    typename std::enable_if<!std::is_void<U>::value, void>::type sort_cols()
    {
        for (size_t j = 0; j < m_cols; j++) {
            std::vector<T> col(m_rows, T{});
            for (size_t i = 0; i < m_rows; i++) {
                col[i] = m_data[i * m_cols + j];
            }
            std::sort(col.begin(), col.end());
            for (size_t i = 0; i < m_rows; i++) {
                m_data[i * m_cols + j] = col[i];
            }
        }
    }

    template<typename U = T>
    typename std::enable_if<!std::is_void<U>::value>::type sort_cols(std::function<bool(const_iterator, const_iterator)> comp) {
        for (size_t j = 0; j < m_cols; j++) {
            std::vector<T> col(m_rows);
            for (size_t i = 0; i < m_rows; i++) {
                col[i] = m_data[i * m_cols + j];
            }
            std::sort(col.begin(), col.end(), comp);
            for (size_t i = 0; i < m_rows; i++) {
                m_data[i * m_cols + j] = col[i];
            }
        }
    }

    /*SFINAE*/
    template <typename U = T, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
    Matrix operator+(const Matrix& other) const {
        static_assert(std::is_same_v<T, typename Matrix::value_type>, "Matrices must have the same value_type");

        Matrix result(m_rows, m_cols);
        std::transform(m_data.begin(), m_data.end(), other.m_data.begin(), result.m_data.begin(), std::plus<T>());
        return result;
    }

    template <typename U = T, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
    Matrix operator-(const Matrix& other) const {
        static_assert(std::is_same_v<T, typename Matrix::value_type>, "Matrices must have the same value_type");

        Matrix result(m_rows, m_cols);
        std::transform(m_data.begin(), m_data.end(), other.m_data.begin(), result.m_data.begin(), std::minus<T>());
        return result;
    }

    template <typename U>
    Matrix operator*(U scalar) const {
        Matrix result(m_rows, m_cols);
        for (size_t i = 0; i < m_rows; i++) {
            for (size_t j = 0; j < m_cols; j++) {
                result(i, j) = (*this)(i, j) * scalar;
            }
        }
        return result;
    }

    template <typename U>
    friend Matrix operator*(U scalar, const Matrix& matrix) {
        return matrix * scalar;
    }

    Matrix operator*(const Matrix& other) const {
        Matrix result(m_rows, other.m_cols);
        for (size_t i = 0; i < m_rows; i++) {
            for (size_t j = 0; j < other.m_cols; j++) {
                T sum = 0;
                for (size_t k = 0; k < m_cols; k++) {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    /*SFINAE*/
    template <typename U = T, typename = std::enable_if_t<std::is_floating_point_v<U>>>
    Matrix operator/(const Matrix& other) const {
        Matrix result(m_rows, m_cols);
        for (size_t i = 0; i < m_rows; i++) {
            for (size_t j = 0; j < m_cols; j++) {
                result(i, j) = (*this)(i, j) / other(i, j);
            }
        }
        return result;
    }

    Matrix& operator+=(const Matrix& other) {
        std::transform(m_data.begin(), m_data.end(), other.m_data.begin(), m_data.begin(), std::plus<T>());
        return *this;
    }

    Matrix& operator-=(const Matrix& other) {
        std::transform(m_data.begin(), m_data.end(), other.m_data.begin(), m_data.begin(), std::minus<T>());
        return *this;
    }

    bool operator==(const Matrix& other) const {
        return (m_rows == other.m_rows && m_cols == other.m_cols && m_data == other.m_data);
    }

    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }

    iterator begin() {
        return m_data.begin();
    }

    const_iterator begin() const {
        return m_data.begin();
    }

    iterator end() {
        return m_data.end();
    }

    const_iterator end() const {
        return m_data.end();
    }

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& matrix);

    /*SFINAE*/
    template <typename U = T>
    struct rank : std::conditional_t<std::is_integral_v<U>, std::integral_constant<int, 0>, std::integral_constant<int, -1>> {};

private:
    size_t m_rows;
    size_t m_cols;
    std::vector<T> m_data;
};

template<typename U>
std::ostream& operator<<(std::ostream& os, const Matrix<U>& matrix)
{
    for (size_t i = 0; i < matrix.rows(); i++) {
        for (size_t j = 0; j < matrix.cols(); j++) {
            if constexpr (std::is_integral_v<U>) {
                os << matrix(i, j);
                if (j != matrix.cols() - 1) {
                    os << ";";
                }
            }
            else if constexpr (std::is_same_v<U, std::string>) {
                os << matrix(i, j);
                if (j != matrix.cols() - 1) {
                    os << "|";
                }
            }
            else {
                os << matrix(i, j);
                if (j != matrix.cols() - 1) {
                    os << "|";
                }
            }
        }
        os << "\n";
    }
    return os;
}