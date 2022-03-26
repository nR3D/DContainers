#ifndef DCONTAINERS_DVECTOR_HPP
#define DCONTAINERS_DVECTOR_HPP


#include <vector>
#include <array>
#include <numeric>
#include <iostream>

#include "Span.hpp"


/***
 * @brief Represent a vector with a fixed dimension
 * @tparam D Vector dimension
 * @tparam T Type of the elements stored
 */
template<std::size_t D, typename T>
class DVector : public std::vector<DVector<D - 1, T>> {
public:
    using std::vector<DVector<D - 1, T>>::vector;

    /***
     * @brief Constructor with a single allocation size for all dimensions
     * @param alloc Number of elements allocated for each dimension
     */
    explicit DVector(std::size_t alloc)
                    : std::vector<DVector<D - 1, T>>(alloc, DVector<D - 1,T>(alloc)) {}

    /***
     * @brief Constructor to specify a different allocation for each dimension.
     * @param alloc Number of elements to allocate to the first (i.e. left most) dimension
     * @param next_allocs  Parameter pack for allocation of subsequent dimensions
     */
    template<std::integral Alloc, std::integral... Allocs>
    explicit DVector(Alloc alloc, Allocs... next_allocs)
    requires (sizeof...(Allocs) == D-1) : std::vector<DVector<D - 1, T>>(alloc, DVector<D - 1,T>(next_allocs...)) {}

    /***
     * @brief Print function for n-dimentional DVectors.
     *        DVectors with an higher dimension than 2 will print their dimension and current allocation
     *        for each dimension, while lower dimensions will resemble an handwritten matrix.
     *        Format example:
     * @code
     * DVector<3>{
     * |0.5, 0.51|
     * |1.5, 1.51|
     * |2.5, 2.51|,
     *
     * |3.5, 3.51|
     * |3.55, 4.51|
     * |5.5, 5.51|
     * }
     * @endcode
     * @see operator<<(std::ostream &, const DVector<2,U> &)
     * @see operator<<(std::ostream &, const DVector<1,U> &)
     */
    template<std::size_t M, typename U>
    friend std::ostream &operator<<(std::ostream &, const DVector<M,U> &);

    /***
     * @brief Print function for 2-dimentional DVectors.
     *        Format example:
     * @code
     * |42.0, 10.5|
     * |5.55, 11.2|
     * |-9.0, 0.01|
     * @endcode
     */
    template<typename U>
    friend std::ostream &operator<<(std::ostream &, const DVector<2,U> &);

    /***
     * @brief Get a reference to a specific element held by DVector, specifying its position.
     * @param index Index of the higher (i.e. left-most) dimension
     * @param indices Parameter pack of the indices for the lower dimensions of the element
     * @return Reference to the requested element
     */
    template<std::integral Idx, std::integral... Indices>
    T& operator()(Idx index, Indices... indices)
    requires (sizeof...(Indices) == D-1)
    {
        return this->at(index)(indices...);
    }

    /***
     * @see DVector<D,T>::operator()(Idx index, Indices... indices)
     * @return Constant reference to the requested element
     */
    template<std::integral Idx, std::integral... Indices>
    const T& operator()(Idx index, Indices... indices) const
    requires (sizeof...(Indices) == D-1)
    {
        return this->at(index)(indices...);
    }

    /***
     * @brief Get a reference to a specific sub-vector given its position inside DVector
     * @param index Index of the higher (i.e. left-most) dimension
     * @param indices Parameter pack of the indices for the lower dimensions of the sub-vector
     * @return Reference to the requested sub-vector
     * @warning Getting a reference to a sub-vector, and then assigning a new sub-vector to the parent DVector,
     *          will result in a dangling reference, since the previous sub-vector (to which the reference points to)
     *          will be deleted before assigning the new one
     */
    template<std::integral Idx, std::integral... Indices>
    DVector<D - sizeof...(Indices) - 1, T>& operator()(Idx index, Indices... indices)
    requires (sizeof...(Indices) < D-1) && (sizeof...(Indices) > 0)
    {
        return this->at(index)(indices...);
    }

    /***
     * @see DVector<D,T>::operator()(Idx index, Indices... indices)
     * @return Constant reference to the requested sub-vector
     */
    template<std::integral Idx, std::integral... Indices>
    const DVector<D - sizeof...(Indices) - 1, T>& operator()(Idx index, Indices... indices) const
    requires (sizeof...(Indices) < D-1) && (sizeof...(Indices) > 0)
    {
        return this->at(index)(indices...);
    }

    /***
     * @brief Specialization of sub-vector dereference for only one dimension lower
     * @see DVector<D,T>::operator()(Idx index, Indices... indices)
     * @param index Index of the sub-vector of lower dimension
     * @return Reference to a sub-vector of one dimension lower
     */
    DVector<D - 1, T>& operator()(std::size_t index) {
        return this->at(index);
    }

    /***
     * @see DVector<D, T>::operator()(std::size_t index)
     * @return Constant reference to a sub-vector of one dimension lower
     */
    const DVector<D - 1, T>& operator()(std::size_t index) const {
        return this->at(index);
    }

    template<typename J, typename... K>
    DVector<D,T> operator()(J span, K... spans)
    requires (sizeof...(K) == D-1) && std::is_convertible_v<J,Span> && (std::is_convertible_v<K,Span> && ...)
    {
        std::size_t from = span.isAll() ? 0 : span.from(),
                to   = span.isAll() ? this->size()-1 : span.to();
        DVector<D,T> dVector (to - from + 1);
        for(std::size_t i = from, j = 0; i <= to; ++i)
            dVector.at(j++) = std::move(this->at(i)(spans...));
        return dVector;
    }

    /***
     * @return Return total amount of elements stored
     */
    constexpr std::size_t total() const noexcept {
        return std::accumulate(this->begin(), this->end(), 0,
                               [](auto sum, const auto& dVector) { return sum + dVector.total(); });
    }
};

template<std::size_t D, typename T>
std::ostream &operator<<(std::ostream &os, const DVector<D,T> &dVector) {
    auto size = dVector.size();
    os << "DVector<" << D << ">{\n";
    for(std::size_t i = 0; i < size; ++i) {
        os << dVector.at(i);
        if(i < size-1)
            os << ",\n\n";
    }
    return os << "\n}";
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const DVector<2,T> &dVector) {
    auto size = dVector.size();
    for(std::size_t i = 0; i < size; ++i) {
        os << dVector.at(i);
        if(i < size-1)
            os << '\n';
    }
    return os;
}

/***
 * @brief Template specialization of DVector with a single dimension
 * @tparam T Type of elements stored
 * @see DVector
 */
template<typename T>
class DVector<1, T> : public std::vector<T> {
public:
    using std::vector<T>::vector;

    /***
     * @brief Print function for 1-dimentional vectors.
     *        Format example:
     * @code
     * |0.0, 3.0, 4.3|
     * @endcode
     */
    template<typename U>
    friend std::ostream &operator<<(std::ostream &, const DVector<1,U> &);

    /***
     * @brief Get reference of element at given position
     * @param index Position of the element
     * @return Reference to the requested element
     */
    T& operator()(std::size_t index) {
        return this->at(index);
    }

    /***
     * @see DVector<1,T>::operator()(std::size_t index)
     * @return Constant reference to the requested element
     */
    const T& operator()(std::size_t index) const {
        return this->at(index);
    }

    DVector<1,T> operator()(Span index) {
        if(index.isAll())
            return *this;
        return { this->begin() + index.from(), this->end() - this->size() + index.to() + 1 };
    }

    /***
     * @return Number of elements held by DVector
     */
    constexpr std::size_t total() const noexcept {
        return this->size();
    }
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const DVector<1, T> &dVector) {
    auto size = dVector.size();
    os << '|';
    for(std::size_t i = 0; i < size; ++i) {
        os << dVector.at(i);
        if(i < size-1)
            os << ", ";
    }
    return os << '|';
}


#endif //DCONTAINERS_DVECTOR_HPP
