#ifndef TEMA_LITERALDMATRIX_HPP
#define TEMA_LITERALDMATRIX_HPP


#include <vector>

template<std::size_t D, typename T>
class LiteralMatrix : public std::vector<LiteralMatrix<D - 1, T>> {
    using std::vector<LiteralMatrix<D - 1, T>>::vector;
};

template<typename T>
class LiteralMatrix<1, T> : public std::vector<T> {
    using std::vector<T>::vector;
};


#endif //TEMA_LITERALDMATRIX_HPP
