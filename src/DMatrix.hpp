#ifndef TEMA_DMATRIX_HPP
#define TEMA_DMATRIX_HPP


#include <vector>
#include <array>
#include <iostream>

template<std::size_t D, typename T>
class DMatrix : public std::vector<DMatrix<D - 1, T>> {
public:
    using std::vector<DMatrix<D - 1, T>>::vector;

    explicit DMatrix(std::size_t alloc)
                    : std::vector<DMatrix<D - 1, T>>(alloc, DMatrix<D-1,T>(alloc)) {}

    template<std::integral Alloc, std::integral... Allocs>
    requires (sizeof...(Allocs) == D-1)
    explicit DMatrix(Alloc alloc, Allocs... next_allocs)
                    : std::vector<DMatrix<D - 1, T>>(alloc, DMatrix<D-1,T>(next_allocs...)) {}

    template<std::size_t M, typename U>
    friend std::ostream &operator<<(std::ostream &, const DMatrix<M,U> &);

    template<typename U>
    friend std::ostream &operator<<(std::ostream &, const DMatrix<2,U> &);

    template<std::integral Idx, std::integral... Indices>
    requires (sizeof...(Indices) == D-1)
    T& operator()(Idx index, Indices... indices) {
        return this->at(index)(indices...);
    }

    template<std::integral Idx, std::integral... Indices>
    requires (sizeof...(Indices) == D-1)
    const T& operator()(Idx index, Indices... indices) const {
        return this->at(index)(indices...);
    }

    template<std::integral Idx, std::integral... Indices>
    requires (sizeof...(Indices) < D-1) && (sizeof...(Indices) > 0)
    DMatrix<D - sizeof...(Indices) - 1, T>& operator()(Idx index, Indices... indices) {
        return this->at(index)(indices...);
    }

    template<std::integral Idx, std::integral... Indices>
    requires (sizeof...(Indices) < D-1) && (sizeof...(Indices) > 0)
    const DMatrix<D - sizeof...(Indices) - 1, T>& operator()(Idx index, Indices... indices) const {
        return this->at(index)(indices...);
    }

    DMatrix<D - 1, T>& operator()(std::size_t index) {
        return this->at(index);
    }

    const DMatrix<D - 1, T>& operator()(std::size_t index) const {
        return this->at(index);
    }

    std::size_t total() const {
        auto s = 0;
        for(auto it = this->begin(); it != this->end(); ++it)
            s += it->size();
        return s;
    }

    std::array<std::size_t,D> shape() const {
        std::array<std::size_t,D> dimensions;
        dimensions[0] = this->size();
        auto rec = this->at(0).shape();
        std::copy(rec.begin(), rec.end(), dimensions.begin() + 1);
        return dimensions;
    }
};

template<std::size_t D, typename T>
std::ostream &operator<<(std::ostream &os, const DMatrix<D,T> &matrix) {
    auto size = matrix.size();
    os << "DMatrix<" << D << ">[";
    auto shape = matrix.shape();
    for(std::size_t i = 0; i < D; ++i) {
        os << shape.at(i);
        if(i < D-1)
            os << ',';
    }
    os << "]{\n";
    for(std::size_t i = 0; i < size; ++i) {
        os << matrix.at(i);
        if(i < size-1)
            os << ",\n\n";
    }
    return os << "\n}";
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const DMatrix<2,T> &matrix) {
    auto size = matrix.size();
    for(std::size_t i = 0; i < size; ++i) {
        os << matrix.at(i);
        if(i < size-1)
            os << '\n';
    }
    return os;
}

template<typename T>
class DMatrix<1, T> : public std::vector<T> {
public:
    using std::vector<T>::vector;

    template<typename U>
    friend std::ostream &operator<<(std::ostream &, const DMatrix<1,U> &);

    T& operator()(std::size_t index) {
        return this->at(index);
    }

    const T& operator()(std::size_t index) const {
        return this->at(index);
    }

    std::size_t total() const {
        return this->size();
    }

    std::array<std::size_t, 1> shape() const {
        return { this->size() };
    }
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const DMatrix<1, T> &matrix) {
    auto size = matrix.size();
    os << '|';
    for(std::size_t i = 0; i < size; ++i) {
        os << matrix.at(i);
        if(i < size-1)
            os << ", ";
    }
    return os << '|';
}


#endif //TEMA_DMATRIX_HPP
