#ifndef DCONTAINERS_SPAN_HPP
#define DCONTAINERS_SPAN_HPP

#include <cstddef>

/***
 * @brief Represent a span of indices from one index to another
 */
class Span {
private:
    std::size_t _from, _to;
    const bool _all;
    constexpr Span() : _from(0), _to(0), _all(true) {}
public:
    /***
     * @brief Construct a Span object representing an interval between two indices
     * @param from starting index (included)
     * @param to final index (included, not past-the-end)
     */
    Span(std::size_t from, std::size_t to) : _from(from), _to(to), _all(false) {}

    /***
     * @brief Construct a Span object representing a single index, equals to Span(value, value)
     * @param value index
     */
    explicit Span(std::size_t value) : _from(value), _to(value), _all(false) {}

    /***
     * @return First index spanned (included)
     */
    std::size_t constexpr from() const {
        return _from;
    }

    /***
     * @return Last index spanned (included)
     */
    std::size_t constexpr to() const {
        return _to;
    }

    /***
     * @return true only if the object it's been constructed using static method all()
     * @see Span::all()
     */
    bool constexpr isAll() const {
        return _all;
    }

    /***
     * @return Span object representing a full span, equals to Span(0, container.size())
     */
    static constexpr Span all()  {
        return Span{};
    }
};


#endif //DCONTAINERS_SPAN_HPP
