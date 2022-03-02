#ifndef TEMA_INITMATRIX_HPP
#define TEMA_INITMATRIX_HPP


#include <vector>

template<typename T, int D>
class InitMatrix : public std::vector<InitMatrix<T, D-1>> {
    using std::vector<InitMatrix<T, D-1>>::vector;
};

template<typename T>
class InitMatrix<T,1> : public std::vector<T> {
    using std::vector<T>::vector;
};


#endif //TEMA_INITMATRIX_HPP
