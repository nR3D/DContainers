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


#endif //DCONTAINERS_SPAN_HPP