#ifndef DCONTAINERS_SPAN_HPP
#define DCONTAINERS_SPAN_HPP

#include <cstddef>

class Span {
private:
    std::size_t _from, _to;
    const bool _all;
    constexpr Span() : _from(0), _to(0), _all(true) {}
public:
    Span(std::size_t from, std::size_t to) : _from(from), _to(to), _all(false) {}

    explicit Span(std::size_t value) : _from(value), _to(value), _all(false) {}

    std::size_t constexpr from() const {
        return _from;
    }

    std::size_t constexpr to() const {
        return _to;
    }

    bool constexpr isAll() const {
        return _all;
    }

    static constexpr Span all()  {
        return Span{};
    }
};


#endif //DCONTAINERS_SPAN_HPP
