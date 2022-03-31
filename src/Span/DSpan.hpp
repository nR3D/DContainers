#ifndef DCONTAINERS_DSPAN_HPP
#define DCONTAINERS_DSPAN_HPP

#include "Span.hpp"

struct SpanSize {
    class Size {};

    class All : public Size {};

    template<std::size_t Value>
    class Index : public Size {};

    template<std::size_t ...S>
    requires (sizeof...(S) == 1 || sizeof...(S) == 2)
    class Interval : public Size {};

    template<std::size_t Size>
    class Interval<Size> : public Size {};

    template<std::size_t From, std::size_t To>
    class Interval<From, To> : public Size {};

};

template<typename S>
requires (std::derived_from<S, SpanSize::Size> && !std::same_as<S, SpanSize::Size>)
class DSpan : public Span {};

template<>
class DSpan<SpanSize::All> : public Span {
public:
    constexpr DSpan() : Span(Span::all()) {};
};

template<std::size_t Value>
class DSpan<SpanSize::Index<Value>> : public Span {
public:
    explicit constexpr DSpan() : Span(Value) {}
};

template<std::size_t From, std::size_t To>
class DSpan<SpanSize::Interval<From, To>> : public Span {
public:
    constexpr DSpan() : Span(From, To) {}
};

template<std::size_t Size>
class DSpan<SpanSize::Interval<Size>> : public Span {
public:
    constexpr DSpan(std::size_t from, std::size_t to) : Span(from, to) {}
};

#endif //DCONTAINERS_DSPAN_HPP
