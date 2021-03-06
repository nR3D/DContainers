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

#ifndef DCONTAINERS_SPANNING_HPP
#define DCONTAINERS_SPANNING_HPP

#include <cstddef>


namespace mdc {

    /***
     * @brief Represent a span of indices from one index to another
     * @note Span is a structural type
     * @warning Span is not default constructible
     */
    struct Spanning {
    private:
        constexpr Spanning() : from(0), to(0), isAll(true) {}

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
         * @param _from starting index (included)
         * @param _to final index (included, not past-the-end)
         */
        constexpr Spanning(std::size_t _from, std::size_t _to) : from(_from), to(_to), isAll(false) {}

        /***
         * @brief Construct a Span object representing a single index, equals to Span(value, value)
         * @param value index
         */
        explicit constexpr Spanning(std::size_t value) : from(value), to(value), isAll(false) {}

        /***
         * @return true iff the two spans are both all or from and to values are respectively equal
         */
        constexpr bool operator==(const Spanning &other) const {
            if (isAll && other.isAll)
                return true;
            if (from == other.from && to == other.to)
                return true;
            return false;
        }

        /***
         * @return Span object representing a full span across the entire domain
         */
        static constexpr Spanning all() {
            return Spanning{};
        }
    };

}

#endif //DCONTAINERS_SPANNING_HPP
