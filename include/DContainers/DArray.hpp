/*
 *   Copyright 2022 Alberto Guarnieri
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#ifndef DCONTAINERS_DARRAY_HPP
#define DCONTAINERS_DARRAY_HPP


#include <array>
#include <iostream>
#include "DContainers/Span/DSpanning.hpp"

namespace mdc {

/***
 * @brief Represent an array with a fixed size for each dimension
 * @tparam T Type of the elements stored
 * @tparam N Size of the outer-most dimension
 * @tparam O Parameter pack of the following sizes
 */
    template<typename T, std::size_t N, std::size_t ...O>
    class DArray : public std::array<DArray<T, O...>, N> {
    private:
        template<std::size_t ...Idx>
        constexpr static std::size_t get_idx(std::size_t num, std::size_t max) {
            auto displacement = sizeof...(Idx) - max - 1;
            return std::array<std::size_t, sizeof...(Idx)>{Idx...}.at(displacement + num);
        }

        template<typename Seq, std::size_t ...Idx>
        struct DArray_reducer {
        };

        template<std::size_t ...Seq, std::size_t ...Idx> requires (sizeof...(Seq) > 0)
        struct DArray_reducer<std::index_sequence<Seq...>, Idx...> {
            using type = DArray<T, get_idx<Idx...>(Seq, sizeof...(Seq) - 1)...>;
        };

        template<typename U, std::size_t ...I>
        constexpr std::array<U, N> array_initializer_extractor(const U *const data, std::index_sequence<I...>) {
            return {data[I]...};
        }

        template<std::size_t M, std::size_t ...P>
        static constexpr DArray<T, M, P...> fromArray(std::array<DArray<T, P...>, M> &&array) {
            return DArray<T, M, P...>(std::forward<decltype(array)>(array));
        }

    protected:
        // Total number of dimensions of DArray
        static constexpr std::size_t D = sizeof...(O) + 1;

        /***
         * @brief Type expressing a lower dimensional DArray.
         * @tparam max Dimension of the resulting DArray
         * @tparam Idx Parameter pack of sizes to be reduced to max
         * @return DArray where only the last "max" sizes have been left
         */
        template<std::size_t max, std::size_t ...Idx>
        using DArray_reduced_t = typename DArray_reducer<std::make_index_sequence<max>, Idx...>::type;

        /***
         * @brief Utility to initialize a DArray through an initializer_list, maintaining the same order.
         * @tparam U Type of the initialized objects
         * @param list initializer_list to be converted
         * @return Array containing all elements of list
         */
        template<typename U>
        constexpr std::array<U, N> array_initializer(std::initializer_list<U> list) {
            return array_initializer_extractor(std::data(list), std::make_index_sequence<N>());
        }

    public:
        /***
         * @brief Print function for n-dimentional DArrays.
         *        DArrays with an higher dimension than 2 will print their dimension and current allocation
         *        for each dimension, while lower dimensions will resemble an handwritten matrix.
         *        Format example:
         * @code
         * DArray<2,3,2>{
         * |0.5, 0.51|
         * |1.5, 1.51|
         * |2.5, 2.51|,
         *
         * |3.5, 3.51|
         * |3.55, 4.51|
         * |5.5, 5.51|
         * }
         * @endcode
         * @see operator<<(std::ostream &, const DArray<U,M1,M2> &)
         * @see operator<<(std::ostream &, const DArray<U,M> &)
         */
        template<typename U, std::size_t M, std::size_t ...P>
        friend std::ostream &operator<<(std::ostream &, const DArray<U, M, P...> &)requires (sizeof...(P) > 0);

        /***
         * @brief Print function for 2-dimentional DArrays.
         *        Format example:
         * @code
         * |42.0, 10.5|
         * |5.55, 11.2|
         * |-9.0, 0.01|
         * @endcode
         */
        template<typename U, std::size_t M1, std::size_t M2>
        friend std::ostream &operator<<(std::ostream &, const DArray<U, M1, M2> &);

        DArray() = default;

        /***
         * @brief Constructor of DArray with a nested initializer_list of DArrays of lower dimensions
         * @param values initializer_list of sub-arrays
         */
        DArray(std::initializer_list<DArray<T, O...>> values)
                : std::array<DArray<T, O...>, N>(array_initializer(values)) {}


        /***
         * @brief Construct DArray as copy of a std::array
         * @param array Array to be copied
         */
        DArray(const std::array<DArray<T, O...>, N> &array) : std::array<DArray<T, O...>, N>(array) {}

        /***
         * @brief Construct DArray moving r-value std::array
         * @param array Array to be moved
         */
        DArray(std::array<DArray<T, O...>, N> &&array) : std::array<DArray<T, O...>, N>(std::move(array)) {}

        /***
         * @brief Get a reference to a specific element held by DArray, specifying its position.
         * @param index Index of the higher (i.e. left-most) dimension
         * @param indices Parameter pack of the indices for the lower dimensions of the element
         * @return Reference to the requested element
         */
        template<std::integral Idx, std::integral... Indices>
        T &operator()(Idx index, Indices... indices)requires (sizeof...(Indices) == D - 1) {
            return this->at(index)(indices...);
        }

        /***
         * @see DArray<T,N,O...>::operator()(Idx index, Indices... indices)
         * @return Constant reference to the requested element
         */
        template<std::integral Idx, std::integral... Indices>
        const T &operator()(Idx index, Indices... indices) const requires (sizeof...(Indices) == D - 1) {
            return this->at(index)(indices...);
        }

        /***
         * @brief Get a reference to a specific sub-array given its position inside DArray
         * @param index Index of the higher (i.e. left-most) dimension
         * @param indices Parameter pack of the indices for the lower dimensions of the sub-array
         * @return Reference to the requested sub-array
         * @see DArray<T,N,O...>::DArray_reduced_t
         * @warning Getting a reference to a sub-array, and then assigning a new sub-array to the parent DArray,
         *          will result in a dangling reference, since the previous sub-array (to which the reference points to)
         *          will be deleted before assigning the new one
         */
        template<std::integral Idx, std::integral... Indices>
        DArray_reduced_t<D - sizeof...(Indices) - 1, N, O...> &
        operator()(Idx index, Indices... indices)requires (sizeof...(Indices) < D - 1) && (sizeof...(Indices) > 0) {
            return this->at(index)(indices...);
        }

        /***
         * @see DArray<T,N,O...>::operator()(Idx index, Indices... indices)
         * @see DArray<T,N,O...>::DArray_reduced_t
         * @return Constant reference to the requested sub-array
         */
        template<std::integral Idx, std::integral... Indices>
        const DArray_reduced_t<D - sizeof...(Indices) - 1, N, O...> &
        operator()(Idx index, Indices... indices) const requires (sizeof...(Indices) < D - 1) &&
                                                                 (sizeof...(Indices) > 0) {
            return this->at(index)(indices...);
        }

        /***
         * @brief Specialization of sub-array dereference for only one dimension lower
         * @see DArray<T,N,O...>::operator()(Idx index, Indices... indices)
         * @see DArray<T,N,O...>::DArray_reduced_t
         * @param index Index of the sub-array of lower dimension
         * @return Reference to a sub-array of one dimension lower
         */
        DArray_reduced_t<D - 1, N, O...> &operator()(std::size_t index) {
            return this->at(index);
        }

        /***
         * @see DArray<T,N,O...>::operator()(std::size_t index)
         * @see DArray<T,N,O...>::DArray_reduced_t
         * @return Constant reference to a sub-array of one dimension lower
         */
        const DArray_reduced_t<D - 1, N, O...> &operator()(std::size_t index) const {
            return this->at(index);
        }

        /***
         * @brief Empty dereference
         * @return Same DArray from which it's invoked
         */
        DArray<T, N, O...> &operator()() const {
            return *this;
        }

        /***
         * @brief View sub-array corresponding to intervals given by DSpan objects,
         *        specialization with the first span being a full span (i.e. DSpan<SpanSize::All>)
         * @param span First DSpan object which represents an interval for the higher (i.e. left-most) dimension
         * @param spans Parameter pack of subsequent DSpan objects for lower dimensions
         * @return DArray containing copies of the elements spanned,
         *         dimension of the DArray returned corresponds to the size (i.e. length) of each DSpan
         */
        template<typename... U>
        decltype(auto) operator()(mdc::DSpanning<mdc::SpanSize::All> span, U... spans) const requires (sizeof...(U) == sizeof...(O)) {
            return operator()(mdc::DSpanning<mdc::SpanSize::Interval<0, N - 1>>(), spans...);
        }

        /***
         * @brief View sub-array corresponding to intervals given by DSpan objects,
         *        specialization with the first span being across a single element (i.e. DSpan<SpanSize::Index<Value>>)
         * @tparam Value Index of the element spanned
         * @param span First DSpan object which represents an interval for the higher (i.e. left-most) dimension
         * @param spans Parameter pack of subsequent DSpan objects for lower dimensions
         * @return DArray containing copies of the elements spanned,
         *         dimension of the DArray returned corresponds to the size (i.e. length) of each DSpan
         */
        template<std::size_t Value, typename... U>
        decltype(auto)
        operator()(mdc::DSpanning<mdc::SpanSize::Index<Value>> span, U... spans) const requires (sizeof...(U) == sizeof...(O) &&
                                                                                   Value < N) {
            std::array<decltype(this->at(Value)(spans...)), 1> data = {this->at(Value)(spans...)};
            return fromArray(std::move(data));
        }

        /***
         * @brief View sub-array corresponding to intervals given by DSpan objects,
         *        specialization with the first span being an interval between two indices
         *        (i.e. DSpan<SpanSize::Interval<From, To>>)
         * @tparam From Index of the first element spanned
         * @tparam To Index of the last element spanned (included)
         * @param span First DSpan object which represents an interval for the higher (i.e. left-most) dimension
         * @param spans Parameter pack of subsequent DSpan objects for lower dimensions
         * @return DArray containing copies of the elements spanned,
         *         dimension of the DArray returned corresponds to the size (i.e. length) of each DSpan
         */
        template<std::size_t From, std::size_t To, typename... U>
        decltype(auto)
        operator()(mdc::DSpanning<mdc::SpanSize::Interval<From, To>> span, U... spans) const requires (sizeof...(U) == sizeof...(O) &&
                                                                                         From < N && To < N) {
            std::array<decltype(this->at(0)(spans...)), To - From + 1> data;
            auto j = 0;
            for (auto i = From; i <= To; ++i)
                data.at(j++) = this->at(i)(spans...);
            return fromArray(std::move(data));
        }

        /***
         * @brief View sub-array corresponding to intervals given by DSpan objects,
         *        specialization with the first span being an interval of fixed size (i.e. DSpan<SpanSize::Interval<Size>>)
         * @tparam Size Number of elements spanned
         * @param span First DSpan object which represents an interval for the higher (i.e. left-most) dimension
         * @param spans Parameter pack of subsequent DSpan objects for lower dimensions
         * @return DArray containing copies of the elements spanned,
         *         dimension of the DArray returned corresponds to the size (i.e. length) of each DSpan
         */
        template<std::size_t Size, typename... U>
        decltype(auto)
        operator()(mdc::DSpanning<mdc::SpanSize::Interval<Size>> span, U... spans) const requires (sizeof...(U) == sizeof...(O) &&
                                                                                     Size <= N) {
            std::array<decltype(this->at(0)(spans...)), Size> data;
            auto j = 0;
            for (auto i = span.from; i <= span.to; ++i)
                data.at(j++) = this->at(i)(spans...);
            return fromArray(std::move(data));
        }

        /***
         * @return Return total amount of elements stored
         */
        constexpr std::size_t total() const noexcept {
            return N * (O * ...);
        }
    };

}

template<typename U, std::size_t M, size_t... P>
std::ostream &operator<<(std::ostream &os, const mdc::DArray<U, M, P...> &dArray)
requires (sizeof...(P) > 0) {
    auto size = dArray.size();
    os << "DArray<" << M;
    ((os << ',' << P), ...);
    os << ">{\n";
    for(std::size_t i = 0; i < size; ++i) {
        os << dArray.at(i);
        if(i < size-1)
            os << ",\n\n";
    }
    return os << "\n}";
}

template<typename U, std::size_t M1, std::size_t M2>
std::ostream &operator<<(std::ostream &os, const mdc::DArray<U, M1, M2> &dArray) {
    for(std::size_t i = 0; i < M1; ++i) {
        os << dArray.at(i);
        if(i < M1-1)
            os << '\n';
    }
    return os;
}


namespace mdc {

/***
 * @brief Template specialization of DArray with a single dimension
 * @tparam T Type of elements stored
 * @see DArray
 */
    template<typename T, std::size_t N>
    class DArray<T, N> : public std::array<T, N> {
    public:
        using std::array<T, N>::array;

        /***
         * @brief Print function for 1-dimentional DArrays.
         *        Format example:
         * @code
         * |0.0, 3.0, 4.3|
         * @endcode
         */
        template<typename U, std::size_t M>
        friend std::ostream &operator<<(std::ostream &, const DArray<U, M> &);

        DArray() = default;

        /***
         * @brief Constructor of DArray<T,N> with values passed without initializer_lists
         * @tparam U Type of parameters initialized, must be the same as typename T of DArray
         */
        template<typename ...U>
        DArray(const U &...values) requires(std::is_convertible_v<U, T> &&...) : std::array<T, N>({values...}) {}

        /***
         * @brief Construct DArray as copy of a std::array
         * @param array Array to be copied
         */
        DArray(const std::array<T, N> &array) : std::array<T, N>(array) {}

        /***
         * @brief Construct DArray moving r-value std::array
         * @param array Array to be moved
         */
        DArray(std::array<T, N> &&array) : std::array<T, N>(std::move(array)) {}

        /***
         * @brief Get reference of element at given position
         * @param index Position of the element
         * @return Reference to the requested element
         */
        T &operator()(std::size_t index) {
            return this->at(index);
        }

        /***
         * @see DArray<T,N>::operator()(std::size_t index)
         * @return Constant reference to the requested element
         */
        T operator()(std::size_t index) const {
            return this->at(index);
        }

        /***
         * @brief Empty dereference
         * @return Same DArray from which it's invoked
         */
        DArray<T, N> &operator()() const {
            return *this;
        }

        /***
         * @brief View sub-array corresponding to a span interval,
         *        specialization with the interval being a full span (i.e. DSpan<SpanSize::All>)
         * @param span Span object describing an interval of elements
         * @return DArray containing copies of the elements spanned,
         *         dimension of the DArray returned corresponds to the size (i.e. length) of the span
         */
        DArray<T, N> operator()(const mdc::DSpanning<mdc::SpanSize::All> span) const {
            return *this;
        }

        /***
         * @brief View sub-array corresponding to a span interval,
         *        specialization with the interval being across a single element (i.e. DSpan<SpanSize::Index<Value>>)
         * @tparam Value Index of the element spanned
         * @param span Span object describing an interval of elements
         * @return DArray containing copies of the elements spanned,
         *         dimension of the DArray returned corresponds to the size (i.e. length) of the span
         */
        template<std::size_t Value>
        DArray<T, 1> operator()(const mdc::DSpanning<mdc::SpanSize::Index<Value>> span) const {
            return {this->at(Value)};
        }

        /***
         * @brief View sub-array corresponding to a span interval,
         *        specialization with the interval being an interval between two indices
         *        (i.e. DSpan<SpanSize::Interval<From, To>>)
         * @tparam From Index of the first element spanned
         * @tparam To Index of the last element spanned (included)
         * @param span Span object describing an interval of elements
         * @return DArray containing copies of the elements spanned,
         *         dimension of the DArray returned corresponds to the size (i.e. length) of the span
         */
        template<std::size_t From, std::size_t To>
        DArray<T, To - From + 1>
        operator()(const mdc::DSpanning<mdc::SpanSize::Interval<From, To>> span) const requires (From < N && To < N) {
            std::array<T, To - From + 1> data;
            auto j = 0;
            for (auto i = From; i <= To; ++i)
                data.at(j++) = this->at(i);
            return data;
        }

        /***
         * @brief View sub-array corresponding to a span interval,
         *        specialization with the interval being an interval of fixed size (i.e. DSpan<SpanSize::Interval<Size>>)
         * @tparam Size Number of elements spanned
         * @param span Span object describing an interval of elements
         * @return DArray containing copies of the elements spanned,
         *         dimension of the DArray returned corresponds to the size (i.e. length) of the span
         */
        template<std::size_t Size>
        DArray<T, Size> operator()(const mdc::DSpanning<mdc::SpanSize::Interval<Size>> span) const requires (Size <= N) {
            std::array<T, Size> data;
            auto j = 0;
            for (auto i = span.from; i <= span.to; ++i)
                data.at(j++) = this->at(i);
            return data;
        }

        /***
         * @return Number of elements stored
         */
        constexpr std::size_t total() const noexcept {
            return N;
        }
    };

}

template<typename U, std::size_t M>
std::ostream &operator<<(std::ostream & os, const mdc::DArray<U, M> &dArray) {
    os << '|';
    for(std::size_t i = 0; i < M; ++i) {
        os << dArray.at(i);
        if(i < M-1)
            os << ", ";
    }
    return os << '|';
}


#endif //DCONTAINERS_DARRAY_HPP
