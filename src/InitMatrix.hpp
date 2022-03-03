#ifndef TEMA_INITMATRIX_HPP
#define TEMA_INITMATRIX_HPP


#include <vector>

template<typename T, std::size_t D>
class InitMatrix : public std::vector<InitMatrix<T, D-1>> {
    static_assert(D > 0);
    using std::vector<InitMatrix<T, D-1>>::vector;
};

template<typename T>
class InitMatrix<T,1> : public std::vector<T> {
    using std::vector<T>::vector;
};


#endif //TEMA_INITMATRIX_HPP
