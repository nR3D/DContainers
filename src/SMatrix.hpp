#ifndef TEMA_SMATRIX_HPP
#define TEMA_SMATRIX_HPP


#include <array>

template<typename T, std::size_t N, std::size_t ...O>
class SMatrix {
private:
    static constexpr std::size_t D = sizeof...(O) + 1;
    std::array<SMatrix<T, O...>,N> _container;

    template<std::size_t ...Idx>
    constexpr static std::size_t get_idx(std::size_t num, std::size_t max) {
        auto displacement = sizeof...(Idx) - max - 1;
        return std::array<std::size_t, sizeof...(Idx)>{Idx...}.at(displacement + num);
    }

    template<typename Seq, std::size_t ...Idx>
    struct SMatrix_reducer {};

    template<std::size_t ...Seq, std::size_t ...Idx>
    struct SMatrix_reducer<std::index_sequence<Seq...>, Idx...> {
        using type = SMatrix<T, get_idx<Idx...>(Seq, sizeof...(Seq)-1)...>;
    };

    template<std::size_t max, std::size_t ...Idx>
    using SMatrix_reduced_t = typename SMatrix_reducer<std::make_index_sequence<max>, Idx...>::type;

public:
    template<typename U, std::size_t M, std::size_t ...P>
    requires (sizeof...(P) > 0)
    friend std::ostream &operator<<(std::ostream &, const SMatrix<U, M, P...> &);

    template<typename U, std::size_t M1, std::size_t M2>
    friend std::ostream &operator<<(std::ostream &, const SMatrix<U, M1, M2> &);

    template<std::integral Idx, std::integral... Indices>
    requires (sizeof...(Indices) == D-1)
    T& operator()(Idx index, Indices... indices) {
        return _container[index](indices...);
    }

    template<std::integral Idx, std::integral... Indices>
    requires (sizeof...(Indices) == D-1)
    const T& operator()(Idx index, Indices... indices) const {
        return _container[index](indices...);
    }

    template<std::integral Idx, std::integral... Indices>
    requires (sizeof...(Indices) < D-1) && (sizeof...(Indices) > 0)
    SMatrix_reduced_t<D - sizeof...(Indices) - 1, N, O...>& operator()(Idx index, Indices... indices) {
        return _container[index](indices...);
    }

    template<std::integral Idx, std::integral... Indices>
    requires (sizeof...(Indices) < D-1) && (sizeof...(Indices) > 0)
    const SMatrix_reduced_t<D - sizeof...(Indices) - 1, N, O...>& operator()(Idx index, Indices... indices) const {
        return _container[index](indices...);
    }

    SMatrix_reduced_t<D - 1, N, O...>& operator()(std::size_t index) {
        return _container[index];
    }

    const SMatrix_reduced_t<D - 1, N, O...>& operator()(std::size_t index) const {
        return _container[index];
    }

    void set(std::array<std::size_t, D> indices, const T& value) {
        std::array<std::size_t, D-1> next_indices;
        std::copy(indices.begin() + 1, indices.end(), next_indices.begin());
        _container[indices[0]].set(next_indices, value);
    }

    constexpr std::size_t size() const {
        return N * (O * ...);
    }

    constexpr std::array<std::size_t,D> shape() const {
        return { N, O... };
    }
};

template<typename U, std::size_t M, size_t... P>
requires (sizeof...(P) > 0)
std::ostream &operator<<(std::ostream &os, const SMatrix<U, M, P...> &matrix) {
    auto size = matrix._container.size();
    os << "SMatrix[" << matrix.D << "]<";
    auto shape = matrix.shape();
    for(std::size_t i = 0; i < matrix.D; ++i) {
        os << shape.at(i);
        if(i < matrix.D-1)
            os << ",";
    }
    os << ">{\n";
    for(std::size_t i = 0; i < size; ++i) {
        os << matrix._container[i];
        if(i < size-1)
            os << ",\n\n";
    }
    return os << "\n}";
}

template<typename U, std::size_t M1, std::size_t M2>
std::ostream &operator<<(std::ostream &os, const SMatrix<U, M1, M2> &matrix) {
    for(std::size_t i = 0; i < M1; ++i) {
        os << matrix._container[i];
        if(i < M1-1)
            os << "\n";
    }
    return os;
}

template<typename T, std::size_t N>
class SMatrix<T,N> {
private:
    std::array<T,N> _container;
public:
    template<typename U, std::size_t M>
    friend std::ostream &operator<<(std::ostream &, const SMatrix<U, M> &);

    T& operator()(std::size_t index) {
        return _container[index];
    }

    T operator()(std::size_t index) const {
        return _container[index];
    }

    void set(std::array<std::size_t,1> index, const T& value) {
        _container[index[0]] = value;
    }

    constexpr std::size_t size() const {
        return N;
    }

    constexpr std::array<std::size_t, 1> shape() const {
        return { N };
    }
};

template<typename U, std::size_t M>
std::ostream &operator<<(std::ostream & os, const SMatrix<U, M> &matrix) {
    os << "|";
    for(std::size_t i = 0; i < M; ++i) {
        os << matrix._container[i];
        if(i < M-1)
            os << ", ";
    }
    return os << "|";
}


#endif //TEMA_SMATRIX_HPP
