#ifndef TEMA_DMATRIX_HPP
#define TEMA_DMATRIX_HPP


#include <vector>
#include <array>
#include <iostream>

template<std::size_t D, typename T>
class DMatrix {
    static_assert(D > 0);
private:
    std::vector<DMatrix<D - 1, T>> _container;

public:
    template<std::size_t M, typename U>
    friend std::ostream &operator<<(std::ostream &, const DMatrix<M,U> &);

    template<typename U>
    friend std::ostream &operator<<(std::ostream &, const DMatrix<2,U> &);

    template<std::integral Alloc, std::integral... Allocs>
    explicit DMatrix(Alloc alloc, Allocs... next_allocs) {
        for(Alloc i = 0; i < alloc; ++i)
            _container.push_back(DMatrix<D - 1, T>(next_allocs...));
    }

    template<std::integral Idx, std::integral... Indices>
    T& operator()(Idx index, Indices... indices) {
        return _container[index](indices...);
    }

    template<std::integral Idx, std::integral... Indices>
    T operator()(Idx index, Indices... indices) const {
        return _container[index](indices...);
    }

    void set(std::array<std::size_t, D> indices, const T& value) {
        std::array<std::size_t, D-1> next_indices;
        std::copy(indices.begin()+1, indices.end(), next_indices.begin());
        _container[indices[0]].set(next_indices, value);
    }

    std::size_t size() const {
        auto s = 0;
        for(auto c : _container)
            s += c.size();
        return s;
    }

    std::array<std::size_t,D> shape() const {
        std::array<std::size_t,D> dimensions;
        dimensions[0] = _container.size();
        auto rec = _container[0].shape();
        std::copy(rec.begin(), rec.end(), dimensions.begin() + 1);
        return dimensions;
    }
};

template<std::size_t D, typename T>
std::ostream &operator<<(std::ostream &os, const DMatrix<D,T> &matrix) {
    auto size = matrix._container.size();
    os << "DMatrix<" << D << ">[";
    auto shape = matrix.shape();
    for(std::size_t i = 0; i < D; ++i) {
        os << shape.at(i);
        if(i < D-1)
            os << ",";
    }
    os << "]{\n";
    for(std::size_t i = 0; i < size; ++i) {
        os << matrix._container[i];
        if(i < size-1)
            os << ",\n\n";
    }
    return os << "\n}";
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const DMatrix<2,T> &matrix) {
    auto size = matrix._container.size();
    for(std::size_t i = 0; i < size; ++i) {
        os << matrix._container[i];
        if(i < size-1)
            os << "\n";
    }
    return os;
}

template<typename T>
class DMatrix<1,T> {
private:
    std::vector<T> _container;
public:
    template<typename U>
    friend std::ostream &operator<<(std::ostream &, const DMatrix<1,U> &);

    explicit DMatrix(std::size_t alloc) : _container(alloc) {}

    T& operator()(std::size_t index) {
        return _container[index];
    }

    T operator()(std::size_t index) const {
        return _container[index];
    }

    void set(std::array<std::size_t,1> index, const T& value) {
        _container[index[0]] = value;
    }

    std::size_t size() const {
        return _container.size();
    }

    std::array<std::size_t, 1> shape() const {
        return {_container.size()};
    }
};

/*template<typename T>
std::ostream &operator<<(std::ostream &os, const DMatrix<1, T> &matrix) {
    auto size = matrix.size();
    os << "DMatrix<1>[" << size << "]{";
    for(std::size_t i = 0; i < size; ++i) {
        os << matrix._container[i];
        if(i < size-1)
            os << ", ";
    }
    return os << "}";
}
 */

template<typename T>
std::ostream &operator<<(std::ostream &os, const DMatrix<1, T> &matrix) {
    auto size = matrix.size();
    os << "|";
    for(std::size_t i = 0; i < size; ++i) {
        os << matrix._container[i];
        if(i < size-1)
            os << ", ";
    }
    return os << "|";
}


#endif //TEMA_DMATRIX_HPP
