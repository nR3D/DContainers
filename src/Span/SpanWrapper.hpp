#ifndef DCONTAINERS_SPANWRAPPER_HPP
#define DCONTAINERS_SPANWRAPPER_HPP

#include "Span.hpp"
#include "DSpan.hpp"

class SpanWrapper {
public:
    template<std::size_t Value>
    static constexpr DSpan<Value> index() {
        return DSpan<Value>{};
    }

    static constexpr Span index(std::size_t value) {
        return Span{value};
    }

    template<std::size_t From, std::size_t To>
    static constexpr DSpan<From, To> interval() {
        return DSpan<From, To>{};
    }

    static constexpr Span interval(std::size_t from, std::size_t to) {
        return Span{from, to};
    }

    static constexpr DSpan<> all() {
        return DSpan<>::all();
    }
};

#endif //DCONTAINERS_SPANWRAPPER_HPP
