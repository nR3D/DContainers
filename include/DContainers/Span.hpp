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

#ifndef DCONTAINERS_SPAN_HPP
#define DCONTAINERS_SPAN_HPP

#include "DContainers/Span/Spanning.hpp"
#include "DContainers/Span/DSpanning.hpp"

namespace mdc {

    /***
     * @brief Wrapper class to offer a common interface between Spanning and DSpanning
     * @note Compile-time methods (i.e. returning DSpanning objects) can be used in any runtime context,
     *       hence the lack of a runtime all() method
     */
    class Span {
    public:

        /***
        * @brief Runtime span across a single index, equals to interval(value,value)
        * @param value Index of the element spanned
        * @return Span of an index
        */
        static constexpr auto of(std::size_t value) {
            return mdc::Spanning{value};
        }

        /***
         * @brief Runtime span across an interval of two indices
         * @param from First index spanned (included)
         * @param to Last index spanned (included)
         * @return Span of an interval between two indices
         */
        static constexpr auto of(std::size_t from, std::size_t to) {
            return mdc::Spanning{from, to};
        }

        /***
         * @brief Compile-time span across a single index, equals to interval<Value,Value>()
         * @tparam Value Index of the element spanned
         * @return DSpan of an index
         */
        template<std::size_t Value>
        static constexpr auto of() {
            return mdc::DSpanning<mdc::SpanSize::Index<Value>>{};
        }

        /***
         * @brief Compile-time span across an interval, where first and last indices are expressed at compile-time
         * @tparam From First index spanned (included)
         * @tparam To Last index spanned (included)
         * @return DSpan of an interval across two given indices
         */
        template<std::size_t From, std::size_t To>
        static constexpr auto of() {
            return mdc::DSpanning<mdc::SpanSize::Interval<From, To>>{};
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
        static constexpr auto of(std::size_t from, std::size_t to) {
            return mdc::DSpanning<mdc::SpanSize::Interval<Size>>{from, to};
        }

        /***
         * @brief Compile-time description of a span across the entire domain
         * @return DSpan of a full span
         */
        static constexpr auto all() {
            return mdc::DSpanning<mdc::SpanSize::All>{};
        }
    };
}



#endif //DCONTAINERS_SPAN_HPP
