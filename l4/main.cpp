#include "matrix.h"

using namespace std;

int main() {

    Matrix<int> m(4, 5);
    
    // change matrix size
    m.resize(3, 3);

    m(0, 0) = 1;
    m(0, 1) = 2;
    m(0, 2) = 3;
    m(1, 0) = 4;
    m(1, 1) = 5;
    m(1, 2) = 6;
    m(2, 0) = 7;
    m(2, 1) = 8;
    m(2, 2) = 9;

    std::cout << "Matrix m:\n" << m << "\n";

    Matrix<int> n(3, 3);
    n(0, 0) = 9;
    n(0, 1) = 8;
    n(0, 2) = 7;
    n(1, 0) = 6;
    n(1, 1) = 5;
    n(1, 2) = 4;
    n(2, 0) = 3;
    n(2, 1) = 2;
    n(2, 2) = 1;

    std::cout << "Matrix n:\n" << n << "\n";

    Matrix<int> sum = m + n;
    std::cout << "Matrix sum:\n" << sum << "\n";

    Matrix<int> diff = m - n;
    std::cout << "Matrix diff:\n" << diff << "\n";

    Matrix<int> prod = m * n;
    std::cout << "Matrix prod:\n" << prod << "\n";

    //If the matrices are of floating-point type
    //Matrix<double> sub = m / n;

    // Create string matrix
    Matrix<std::string> stringMatrix(2, 2);

    stringMatrix(0, 0) = "abc";
    stringMatrix(0, 1) = "defg";
    stringMatrix(1, 0) = "hijkl";
    stringMatrix(1, 1) = "mno";

    std::cout << "String matrix:\n";
    std::cout << stringMatrix << "\n";

    n.sort_rows();
    std::cout << "Sorted rows:" << '\n';
    std::cout << n << '\n';

    n.sort_cols();

    //m.sort_cols([](typename Matrix<int>::const_iterator it1, typename Matrix<int>::const_iterator it2) { return *it1 < *it2; });
    std::cout << "Sorted columns:" << '\n';
    std::cout << n << '\n';

    if (m == n)
        std::cout << "m == n\n";
    else
        std::cout << "m != n\n";

    return 0;
}

