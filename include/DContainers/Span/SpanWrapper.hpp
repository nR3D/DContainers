#ifndef DCONTAINERS_SPANWRAPPER_HPP
#define DCONTAINERS_SPANWRAPPER_HPP

#include "Span.hpp"
#include "DSpan.hpp"

/***
 * @brief Wrapper class to offer a common interface between Span and DSpan
 * @note Compile-time methods (i.e. returning DSpan objects) can be used in any runtime context,
 *       hence the lack of a runtime all() method
 */
class SpanWrapper {
public:
    /***
     * @brief Compile-time span across a single index, equals to interval<Value,Value>()
     * @tparam Value Index of the element spanned
     * @return DSpan of an index
     */
    template<std::size_t Value>
    static constexpr DSpan<SpanSize::Index<Value>> index() {
        return DSpan<SpanSize::Index<Value>>{};
    }

    /***
     * @brief Runtime span across a single index, equals to interval(value,value)
     * @param value Index of the element spanned
     * @return Span of an index
     */
    static constexpr Span index(std::size_t value) {
        return Span{value};
    }

    /***
     * @brief Compile-time span across an interval, where first and last indices are expressed at compile-time
     * @tparam From First index spanned (included)
     * @tparam To Last index spanned (included)
     * @return DSpan of an interval across two given indices
     */
    template<std::size_t From, std::size_t To>
    static constexpr DSpan<SpanSize::Interval<From, To>> interval() {
        return DSpan<SpanSize::Interval<From, To>>{};
    }

    /***
     * @brief Compile-time span across an interval,
     *        where only the size (i.e. length) of the span is expressed at compile-time,
     *        first and last positions are given at runtime
     * @tparam Size Number of elements spanned
     * @param from First index spanned (included)
     * @param to Last index spanned (included)
     * @return DSpan of an interval of a given size across two indices
     * @throws std::length_error If the number of elements between from and to are different than Size
     */
    template<std::size_t Size>
    static constexpr DSpan<SpanSize::Interval<Size>> interval(std::size_t from, std::size_t to) {
        return DSpan<SpanSize::Interval<Size>>{from, to};
    }

    /***
     * @brief Runtime span across an interval of two indices
     * @param from First index spanned (included)
     * @param to Last index spanned (included)
     * @return Span of an interval between two indices
     */
    static constexpr Span interval(std::size_t from, std::size_t to) {
        return Span{from, to};
    }

    /***
     * @brief Compile-time description of a span across the entire domain
     * @return DSpan of a full span
     */
    static constexpr DSpan<SpanSize::All> all() {
        return DSpan<SpanSize::All>{};
    }
};

#endif //DCONTAINERS_SPANWRAPPER_HPP
