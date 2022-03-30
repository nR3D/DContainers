#ifndef DCONTAINERS_SPAN_HPP
#define DCONTAINERS_SPAN_HPP

#include <cstddef>

/***
 * @brief Represent a span of indices from one index to another
 */
struct Span {
private:
    constexpr Span() : from(0), to(0), isAll(true) {}
public:
    /***
     * @brief First index spanned
     */
    const std::size_t from;

    /***
     * @brief Last index spanned (included)
     */
    const std::size_t to;

    /***
     * @brief true only if the object it's been constructed using static method all()
     * @see Span::all()
     */
    const bool isAll;

    /***
     * @brief Construct a Span object representing an interval between two indices
     * @param from starting index (included)
     * @param to final index (included, not past-the-end)
     */
    constexpr Span(std::size_t _from, std::size_t _to) : from(_from), to(_to), isAll(false) {}

    /***
     * @brief Construct a Span object representing a single index, equals to Span(value, value)
     * @param value index
     */
    explicit constexpr Span(std::size_t value) : from(value), to(value), isAll(false) {}

    constexpr bool operator==(const Span& other) const {
        if(isAll && other.isAll)
            return true;
        if(from == other.from && to == other.to)
            return true;
        return false;
    }

    /***
     * @return Span object representing a full span, equals to Span(0, container.size())
     */
    static constexpr Span all()  {
        return Span{};
    }
};


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


class SpanWrapper {
public:
    template<std::size_t Value>
    static constexpr DSpan<Value> index() { return DSpan<Value>{}; }

    template<std::size_t From, std::size_t To>
    static constexpr DSpan<From, To> interval() { return DSpan<From, To>{}; }

    static constexpr DSpan<> all() { return DSpan<>::all(); }
};


#endif //DCONTAINERS_SPAN_HPP
