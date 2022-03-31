#ifndef DCONTAINERS_DSPAN_HPP
#define DCONTAINERS_DSPAN_HPP

#include "Span.hpp"

template<std::size_t ...T>
requires (sizeof...(T) >= 0 && sizeof...(T) <= 2)
class DSpan : public Span {};

template<>
class DSpan<> : public Span {
private:
    constexpr DSpan() : Span(Span::all()) {};

public:
    static constexpr DSpan<> all() {
        return DSpan<>{};
    }
};

template<std::size_t Value>
class DSpan<Value> : public Span {
public:
    explicit constexpr DSpan() : Span(Value) {}
};

template<std::size_t From, std::size_t To>
class DSpan<From, To> : public Span {
public:
    constexpr DSpan() : Span(From, To) {}
};

#endif //DCONTAINERS_DSPAN_HPP
