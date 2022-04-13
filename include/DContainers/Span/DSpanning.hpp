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

#ifndef DCONTAINERS_DSPANNING_HPP
#define DCONTAINERS_DSPANNING_HPP

#include "Spanning.hpp"

namespace mdc {

    /***
     * @brief Describe length or position of a DSpan
     */
    struct SpanSize {
        /***
         * @brief General size to be extended, cannot be directly used
         */
        class Size {
        };

        /***
         * @brief Full span across the entire domain
         */
        class All : public Size {
        };

        /***
         * @brief Single element spanned, equals to Interval<Value, Value>
         * @tparam Value Index to the element spanned
         */
        template<std::size_t Value>
        class Index : public Size {
        };

        template<std::size_t ...S> requires (sizeof...(S) == 1 || sizeof...(S) == 2)
        class Interval : public Size {
        };

        /***
         * @brief Interval described by its size
         * @tparam Size Number of elements spanned
         */
        template<std::size_t Size>
        class Interval<Size> : public Size {
        };

        /***
         * @brief Interval described by its first and last indices
         * @tparam From First index spanned (included)
         * @tparam To Last index spanned (included)
         */
        template<std::size_t From, std::size_t To>
        class Interval<From, To> : public Size {
        };

    };


    /***
     * @brief Compile-time description on a span
     * @tparam S SpanSize class indicating length or position of the span
     */
    template<typename S> requires (std::derived_from<S, SpanSize::Size> && !std::same_as<S, SpanSize::Size>)
    class DSpanning : public mdc::Spanning {
    };

    /***
     * @brief Compile-time description of the entire span
     * @see SpanSize::All
     */
    template<>
    class DSpanning<SpanSize::All> : public mdc::Spanning {
    public:
        constexpr DSpanning() : mdc::Spanning(mdc::Spanning::all()) {};
    };

    /***
     * @brief Compile-time description of a single element spanned, equals to DSpan<SpanSize::Interval<Value,Value>>
     * @see SpanSize::Index<Value>
     * @see DSpan<SpanSize::Interval<From, To>
     */
    template<std::size_t Value>
    class DSpanning<SpanSize::Index<Value>> : public mdc::Spanning {
    public:
        explicit constexpr DSpanning() : mdc::Spanning(Value) {}
    };

    /***
     * @brief Compile-time description of a span going from one index to another
     * @see SpanSize::Interval<From, To>
     */
    template<std::size_t From, std::size_t To>
    class DSpanning<SpanSize::Interval<From, To>> : public mdc::Spanning {
    public:
        constexpr DSpanning() : mdc::Spanning(From, To) {}
    };

    /***
     * @brief Compile-time description of a span of a given size (i.e. length)
     * @see SpanSize::Interval<Size>
     */
    template<std::size_t Size>
    class DSpanning<SpanSize::Interval<Size>> : public mdc::Spanning {
    public:
        /***
         * @param from First index spanned
         * @param to Last index spanned
         * @throws std::length_error When template Size and parameter size (= to - from + 1) are not equal
         */
        constexpr DSpanning(std::size_t from, std::size_t to) : mdc::Spanning(from, to) {
            if (to - from + 1 != Size)
                throw std::length_error(
                        "Span passed as parameter (from=" + std::to_string(from) + ", to=" + std::to_string(to) + ")" +
                        " has a different size than template (Size=" + std::to_string(Size) + ")");
        }
    };

}

#endif //DCONTAINERS_DSPANNING_HPP
