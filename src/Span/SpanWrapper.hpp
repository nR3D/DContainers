#ifndef DCONTAINERS_SPANWRAPPER_HPP
#define DCONTAINERS_SPANWRAPPER_HPP

#include "Span.hpp"
#include "DSpan.hpp"

class SpanWrapper {
public:
    template<std::size_t Value>
    static constexpr DSpan<SpanSize::Index<Value>> index() {
        return DSpan<SpanSize::Index<Value>>{};
    }

    static constexpr Span index(std::size_t value) {
        return Span{value};
    }

    template<std::size_t From, std::size_t To>
    static constexpr DSpan<SpanSize::Interval<From, To>> interval() {
        return DSpan<SpanSize::Interval<From, To>>{};
    }

    template<std::size_t Size>
    static constexpr DSpan<SpanSize::Interval<Size>> interval(std::size_t from, std::size_t to) {
        return DSpan<SpanSize::Interval<Size>>{from, to};
    }

    static constexpr Span interval(std::size_t from, std::size_t to) {
        return Span{from, to};
    }

    static constexpr DSpan<SpanSize::All> all() {
        return DSpan<SpanSize::All>{};
    }
};

#endif //DCONTAINERS_SPANWRAPPER_HPP
