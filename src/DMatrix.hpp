#ifndef TEMA_DMATRIX_HPP
#define TEMA_DMATRIX_HPP


#include <vector>
#include <array>
#include <iostream>


/***
 * @brief Represent a matrix with a fixed dimension, e.g. a 3-dimensional matrix
 * @tparam D Dimension of the matrix
 * @tparam T Type of the elements stored in the matrix
 */
template<std::size_t D, typename T>
class DMatrix : public std::vector<DMatrix<D - 1, T>> {
public:
    using std::vector<DMatrix<D - 1, T>>::vector;

    /***
     * @brief Constructor with a single allocation size for all dimensions
     * @param alloc Number of elements allocated for each dimension
     */
    explicit DMatrix(std::size_t alloc)
                    : std::vector<DMatrix<D - 1, T>>(alloc, DMatrix<D-1,T>(alloc)) {}

    /***
     * @brief Constructor to specify a different allocation for each dimension.
     * @param alloc Number of elements to allocate to the first (i.e. left most) dimension
     * @param next_allocs  Parameter pack for allocation of subsequent dimensions
     */
    template<std::integral Alloc, std::integral... Allocs>
    explicit DMatrix(Alloc alloc, Allocs... next_allocs)
    requires (sizeof...(Allocs) == D-1) : std::vector<DMatrix<D - 1, T>>(alloc, DMatrix<D-1,T>(next_allocs...)) {}

    /***
     * @brief Print function for n-dimentional matrices.
     *        Matrices with an higher dimension than 2 will print their dimension and current allocation
     *        for each dimension, while lower dimensions will resemble an handwritten matrix/vector.
     *        Format example:
     * @code
     * DMatrix<3>[2,3,2]{
     * |0.5, 0.51|
     * |1.5, 1.51|
     * |2.5, 2.51|,
     *
     * |3.5, 3.51|
     * |3.55, 4.51|
     * |5.5, 5.51|
     * }
     * @endcode
     * @see operator<<(std::ostream &, const DMatrix<2,U> &)
     * @see operator<<(std::ostream &, const DMatrix<1,U> &)
     */
    template<std::size_t M, typename U>
    friend std::ostream &operator<<(std::ostream &, const DMatrix<M,U> &);

    /***
     * @brief Print function for 2-dimentional matrices.
     *        Format example:
     * @code
     * |42.0, 10.5|
     * |5.55, 11.2|
     * |-9.0, 0.01|
     * @endcode
     */
    template<typename U>
    friend std::ostream &operator<<(std::ostream &, const DMatrix<2,U> &);

    /***
     * @brief Get a reference to a specific element held by the matrix, specifying its position.
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
     * @see DMatrix<D,T>::operator()(Idx index, Indices... indices)
     * @return Constant reference to the requested element
     */
    template<std::integral Idx, std::integral... Indices>
    const T& operator()(Idx index, Indices... indices) const
    requires (sizeof...(Indices) == D-1)
    {
        return this->at(index)(indices...);
    }

    /***
     * @brief Get a reference to a specific submatrix given its position inside the matrix
     * @param index Index of the higher (i.e. left-most) dimension
     * @param indices Parameter pack of the indices for the lower dimensions of the submatrix
     * @return Reference to the requested submatrix
     * @warning Getting a reference to a submatrix, and then assigning a new submatrix to the parent matrix,
     *          will result in a dangling reference, since the previous submatrix (to which the reference points to)
     *          will be deleted before assigning the new one
     */
    template<std::integral Idx, std::integral... Indices>
    DMatrix<D - sizeof...(Indices) - 1, T>& operator()(Idx index, Indices... indices)
    requires (sizeof...(Indices) < D-1) && (sizeof...(Indices) > 0)
    {
        return this->at(index)(indices...);
    }

    /***
     * @see DMatrix<D,T>::operator()(Idx index, Indices... indices)
     * @return Constant reference to the requested submatrix
     */
    template<std::integral Idx, std::integral... Indices>
    const DMatrix<D - sizeof...(Indices) - 1, T>& operator()(Idx index, Indices... indices) const
    requires (sizeof...(Indices) < D-1) && (sizeof...(Indices) > 0)
    {
        return this->at(index)(indices...);
    }

    /***
     * @brief Specialization of submatrix dereference for only one dimension lower
     * @see DMatrix<D,T>::operator()(Idx index, Indices... indices)
     * @param index Index of the submatrix of lower dimension
     * @return Reference to a submatrix of one dimension lower
     */
    DMatrix<D - 1, T>& operator()(std::size_t index) {
        return this->at(index);
    }

    /***
     * @see DMatrix<D, T>::operator()(std::size_t index)
     * @return Constant reference to a submatrix of one dimension lower
     */
    const DMatrix<D - 1, T>& operator()(std::size_t index) const {
        return this->at(index);
    }

    /***
     * @return Return total amount of element stored in the matrix
     */
    std::size_t total() const {
        auto s = 0;
        for(auto it = this->begin(); it != this->end(); ++it)
            s += it->size();
        return s;
    }

    /***
     * @brief Number of elements for each dimensions.
     * @see DMatrix<D,T>::total()
     * @return Array containing higher dimensions first and lower ones last
     */
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

/***
 * @brief Template specialization of DMatrix with a single dimension, i.e. a vector
 * @tparam T Type of the matrix
 * @see DMatrix
 */
template<typename T>
class DMatrix<1, T> : public std::vector<T> {
public:
    using std::vector<T>::vector;

    /***
     * @brief Print function for 1-dimentional matrices.
     *        Format example:
     * @code
     * |0.0, 3.0, 4.3|
     * @endcode
     */
    template<typename U>
    friend std::ostream &operator<<(std::ostream &, const DMatrix<1,U> &);

    /***
     * @brief Get reference of element at given position
     * @param index Position of the element
     * @return Reference to the requested element
     */
    T& operator()(std::size_t index) {
        return this->at(index);
    }

    /***
     * @see DMatrix<1,T>::operator()(std::size_t index)
     * @return Constant reference to the requested element
     */
    const T& operator()(std::size_t index) const {
        return this->at(index);
    }

    /***
     * @return Number of elements held by the matrix
     */
    std::size_t total() const {
        return this->size();
    }

    /***
     * @see DMatrix<D,T>::shape()
     * @return Array containing the number of elements contained in the single dimension of the matrix
     */
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
