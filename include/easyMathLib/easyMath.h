/**
 * @file easyMath.h 
 * @author Harith Manoj (harithpub@gmail.com)
 * @brief EasyMath library methods
 * @date 12 April 2024
 * 
 * @copyright Copyright (C) 2024
 * 
 * 
 *                      APACHE LICENSE 2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#ifndef INCLUDE_EASYMATHLIB_EASYMATH_H_INCLUDED

/// @brief include\easyMathLib\easyMath.h Header Guard 
#define INCLUDE_EASYMATHLIB_EASYMATH_H_INCLUDED


#include <cmath>
#include <vector>
#include <ranges>
#include <cstdint>

/// @brief encapsulate math library code.
namespace easyMath
{

    /// @brief Namespace to store meta information, like version.
    namespace meta
    {
        /// @brief Version as a string.
        constexpr std::string_view versionString = "0.0.1.alpha";

        /// @brief Major Version
        constexpr std::uint64_t versionMajor = 0ull;

        /// @brief Minor Version
        constexpr std::uint64_t versionMinor = 0ull;

        /// @brief Revision Version
        constexpr std::uint64_t versionRevision = 1ull;
        
        /// @brief Enum of different build types.
        enum class BuildType
        {
            /// @brief Alpha release (functionality added, minimal testing).
            ALPHA, 

            /// @brief Beta release (tests added).
            BETA, 

            /// @brief Stable release (tested extensively).
            STABLE
        };

        /// @brief Version Build Type.
        constexpr BuildType versionBuild = BuildType::ALPHA;
    }

    /**
     * @brief identify whether the value is between the two values. (not inclusive)
     * 
     * @tparam T Type of value to check.
     * @param[in] value value to evaluate.
     * @param[in] begin range begin.
     * @param[in] end range end.
     * @return true begin < value < end.
     */
    template<class T>
    [[nodiscard]] inline constexpr bool valueBetween(T value, T begin, T end) noexcept
    {
        return (value > begin) && (value < end);
    }
    
    /**
     * @brief Identify whether the value is between the two values. (inclusive)
     * 
     * @tparam T Type of value to check.
     * @param[in] value value to evaluate.
     * @param[in] begin range begin.
     * @param[in] end range end.
     * @return true begin <= value <= end.
     */
    template<class T>
    [[nodiscard]] inline constexpr bool valueBetweenInclusive(T value, T begin, T end) noexcept
    {
        return (value >= begin) && (value <= end);
    }

    /**
     * @brief Check if type is numeric type, must have mathamatical operator and converters.
     * 
     * @tparam Numeric Type to check.
     * 
     * Must overload:
     * 
     * @conceptMember{Math Operators}
     * 
     * - `[Numeric] operator + (const Numeric&, const Numeric&);`
     * - `[Numeric] operator - (const Numeric&, const Numeric&);`
     * - `[Numeric] operator * (const Numeric&, const Numeric&);`
     * - `[Numeric] operator / (const Numeric&, const Numeric&);`
     * - `Numeric& operator += (Numeric&, const Numeric&);`
     * - `Numeric& operator -= (Numeric&, const Numeric&);`
     * - `Numeric& operator *= (Numeric&, const Numeric&);`
     * - `Numeric& operator /= (Numeric&, const Numeric&);`
     * 
     * @conceptMember{Relational Operators}
     * 
     * - `[bool] operator == (const Numeric&, const Numeric&);`
     * - `[bool] operator != (const Numeric&, const Numeric&);`
     * - `[bool] operator <= (const Numeric&, const Numeric&);`
     * - `[bool] operator >= (const Numeric&, const Numeric&);`
     * - `[bool] operator < (const Numeric&, const Numeric&);`
     * - `[bool] operator > (const Numeric&, const Numeric&);`
     * 
     * @conceptMember{Type Conversion}
     * 
     * - Type convert to and from: `std::uint64_t`.
     * - Type convert to and from: `std::uint32_t`.
     * - Type convert to and from: `std::uint16_t`.
     * - Type convert to and from: `std::uint8_t`.
     * - Type convert to and from: `std::int64_t`.
     * - Type convert to and from: `std::int32_t`.
     * - Type convert to and from: `std::int16_t`.
     * - Type convert to and from: `std::int8_t`.
     * - Type convert to and from: `float`.
     * - Type convert to and from: `double`.
     * - Type convert to and from: `long double`.
     * 
     */
    template<class Numeric>
    concept NumericType = requires(const Numeric& clhs, const Numeric& crhs, Numeric& lhs, std::uint64_t cuintVal, std::int64_t cintVal)
    {
        {clhs + crhs} -> std::convertible_to<Numeric>;
        {clhs - crhs} -> std::convertible_to<Numeric>;
        {clhs * crhs} -> std::convertible_to<Numeric>;
        {clhs / crhs} -> std::convertible_to<Numeric>;
        {clhs == crhs} -> std::convertible_to<bool>;
        {clhs != crhs} -> std::convertible_to<bool>;
        {clhs < crhs} -> std::convertible_to<bool>;
        {clhs <= crhs} -> std::convertible_to<bool>;
        {clhs >= crhs} -> std::convertible_to<bool>;
        {clhs > crhs} -> std::convertible_to<bool>;        
        {lhs += crhs} -> std::same_as<Numeric&>;
        {lhs -= crhs} -> std::same_as<Numeric&>;
        {lhs *= crhs} -> std::same_as<Numeric&>;
        {lhs /= crhs} -> std::same_as<Numeric&>;
        requires std::convertible_to<Numeric, std::uint64_t>;
        requires std::convertible_to<Numeric, std::uint32_t>;
        requires std::convertible_to<Numeric, std::uint16_t>;
        requires std::convertible_to<Numeric, std::uint8_t>;
        requires std::convertible_to<Numeric, std::int64_t>;
        requires std::convertible_to<Numeric, std::int32_t>;
        requires std::convertible_to<Numeric, std::int16_t>;
        requires std::convertible_to<Numeric, std::int8_t>;
        requires std::convertible_to<std::uint64_t, Numeric>;
        requires std::convertible_to<std::uint32_t, Numeric>;
        requires std::convertible_to<std::uint16_t, Numeric>;
        requires std::convertible_to<std::uint8_t, Numeric>;
        requires std::convertible_to<std::int64_t, Numeric>;
        requires std::convertible_to<std::int32_t, Numeric>;
        requires std::convertible_to<std::int16_t, Numeric>;
        requires std::convertible_to<std::int8_t, Numeric>;
        requires std::convertible_to<Numeric, float>;
        requires std::convertible_to<Numeric, double>;
        requires std::convertible_to<Numeric, long double>;
        requires std::convertible_to<float, Numeric>;
        requires std::convertible_to<double, Numeric>;
        requires std::convertible_to<long double, Numeric>;
    };

    /**
     * @brief Numeric traits that can be automatically deduced. (eg concept based).
     * 
     * @tparam Numeric Type for traits to be deduced.
     */
    template<NumericType Numeric>
    struct SelfCheckingNumericTraits
    {
        /// @brief Checks whether type has a static member function to evaluate add overflow.
        ///
        /// Member Function must be of signature: `[bool] willAddOverflow([const] Numeric[&] lhs, [const] Numeric[&] rhs, [const] Numeric[&] max);`
        static constexpr bool isOverflowCheckable = requires(const Numeric& lhs, const Numeric& rhs, const Numeric& max)
        {
            { Numeric::willAddOverflow(lhs, rhs, max) } -> std::convertible_to<bool>;
        };

        /**
         * @brief Checks whether type has a static member function to evaluate subtract underflow.
         * 
         * Member Function must of signature: `[bool] willSubtractUnderflow([const] Numeric[&] lhs, [const] Numeric[&] rhs, [const] Numeric[&] lowest);`
         * 
         */
        static constexpr bool isUnderflowCheckable = requires(const Numeric& lhs, const Numeric& rhs, const Numeric& lowest)
        {
            { Numeric::willSubtractUnderflow(lhs, rhs, lowest) } -> std::convertible_to<bool>;
        };

        /**
         * @brief Checks whether type is custom sized.
         * 
         * Must have static member function:
         * `[std::size_t] bitSize() noexcept;`
         * 
         */
        static constexpr bool isCustomSized = requires 
        {
            {Numeric::bitSize()} -> std::convertible_to<std::size_t>;
            noexcept(Numeric::bitSize());
        };

        /**
         * @brief Checks whether type has method to generate bit masks.
         * 
         * Must have static member function:
         * `Numeric nBitMask(std::size_t) noexcept;`
         * 
         */
        static constexpr bool hasBitMaskGenerator = requires(std::size_t maskSize)
        {
            { Numeric::nBitMask(maskSize) } -> std::same_as<Numeric>;
            noexcept(Numeric::nBitMask(maskSize));
        };
    };

    /**
     * @brief Specializable and extended `std::numeric_limits`.
     * 
     * @tparam Numeric Type for traits to be provided.
     */
    template<NumericType Numeric>
    struct NumericTraits : public std::numeric_limits<Numeric>, public SelfCheckingNumericTraits<Numeric>
    {

    };

    namespace impl_detail_
    {
        /**
         * @brief Check if type has bitwise operators.
         * 
         * @tparam Integer type to check.
         * 
         * Must Satisfy: `NumericType`.
         * 
         * Must Overload:
         * 
         * @conceptMember{Bitwise Operators}
         * 
         * - `[Integer] operator & (const Integer&, const Integer&);`
         * - `[Integer] operator | (const Integer&, const Integer&);`
         * - `[Integer] operator ^ (const Integer&, const Integer&);`
         * - `Integer& operator &= (Integer&, const Integer&);`
         * - `Integer& operator |= (Integer&, const Integer&);`
         * - `Integer& operator ^= (Integer&, const Integer&);`
         * 
         */
        template<class Integer>
        concept BitWiseOperable_ = requires(const Integer& clhs, const Integer& crhs, Integer& lhs)
        {
            requires NumericType<Integer>;
            {clhs & crhs} -> std::convertible_to<Integer>;
            {clhs | crhs} -> std::convertible_to<Integer>;
            {clhs ^ crhs} -> std::convertible_to<Integer>;
            {lhs &= crhs} -> std::same_as<Integer&>;
            {lhs |= crhs} -> std::same_as<Integer&>;
            {lhs ^= crhs} -> std::same_as<Integer&>;
        };
    }

    /// @brief extendable concept for Integral types, extend by specializing `NumericTraits`.
    template<class Integer>
    concept Integral = NumericTraits<Integer>::is_integer;

    /// @brief extendable concept for Unsigned Integral types, extend by specializing `NumericTraits`.
    template<class UInt>
    concept UnsignedIntegral = (!NumericTraits<UInt>::is_signed) && Integral<UInt> && impl_detail_::BitWiseOperable_<UInt>;

    /// @brief extendable concept for Unsigned Integral types, extend by specializing `NumericTraits`.
    template<class SInt>
    concept SignedIntegral = (NumericTraits<SInt>::is_signed) && Integral<SInt>;

    /**
     * @brief Check if value given is power of 2.
     * 
     * @tparam Integer Type of value to check.
     * @param[in] x value to check
     */
    template<Integral Integer>
    [[nodiscard]] inline constexpr bool isPowerOfTwo(Integer x) noexcept
    {
        return (x != Integer(0)) && ((x & (x - Integer(1))) == Integer(0));
    }


    /**
     * @brief cieling of log base 2.
     * 
     * @tparam Integer Type of value to take log of.
     * @param[in] val value to find log of.
     * @return Integer log value.
     */
    template<Integral Integer>
    [[nodiscard]] inline constexpr Integer cilog2(Integer val) noexcept
    { 
        return (val != 0) ? 1 + cilog2(val >> 1) : 0; 
    }

    /**
     * @brief Check if addition of arguments will overflow.
     * 
     * @tparam Numeric Numeric type to use.
     * @param[in] lhs left hand side of operation.
     * @param[in] rhs right hand side of operation.
     * @param[in] max maximum allowed (or possible) value.
     * @return true if addition overflows.
     */
    template<class Numeric>
    [[nodiscard]] inline constexpr bool willAddOverflow(
        const Numeric& lhs,
        const Numeric& rhs,
        const Numeric& max = NumericTraits<Numeric>::max()
    ) noexcept
    {
        if constexpr (NumericTraits<Numeric>::isOverflowCheckable)
            return Numeric::willAddOverflow(lhs, rhs, max);
        else
            return ((max - lhs) < rhs);
    }

    /**
     * @brief Chech if subtraction of arguments will underflow. 
     * 
     * @tparam Numeric type to use.
     * @param[in] lhs left hand side of operation.
     * @param[in] rhs right hand side of operation.
     * @param[in] lowest lowest allowed (or possible) value.
     * @return true if subtraction underflows.
     */
    template<class Numeric>
    [[nodiscard]] inline constexpr bool willSubtractUnderflow(
        const Numeric& lhs,
        const Numeric& rhs,
        const Numeric& lowest = NumericTraits<Numeric>::lowest()
    ) noexcept
    {
        if constexpr (NumericTraits<Numeric>::isUnderflowCheckable)
            return Numeric::willSubtractUnderflow(lhs, rhs, lowest);
        else
            return lhs < (lowest + rhs);
    }

    /**
     * @brief Add lhs, rhs, and previous overflow and report if overflow exists.
     * 
     * @tparam Numeric Basic type to use.
     * @param[inout] lhs left hand side of add operation.
     * @param[in] rhs right hand side of add operation.
     * @param[in] lastOpOverflow overlfow in LSBs add operation.
     * @param[in] max maximum allowed (or possible) value.
     * @return true if add overflows.
     */
    template<class Numeric>
    inline constexpr bool addAndCheckOverflow(
        Numeric& lhs,
        const Numeric& rhs,
        bool lastOpOverflow = false,
        const Numeric& max = NumericTraits<Numeric>::max()
    ) noexcept
    {
        std::uint8_t addOverflowPrevious = (lastOpOverflow? 1 : 0);
        auto overflow = willAddOverflow<Numeric>(lhs, rhs, max);
        lhs += rhs;
        overflow = overflow || willAddOverflow<Numeric>(lhs, addOverflowPrevious, max);
        lhs += addOverflowPrevious;
        return overflow;
    }

    /**
     * @brief Add lhs, rhs, and previous overflow and report if overflow exists.
     * 
     * @tparam Numeric Basic type to use.
     * @param[inout] sum destination of add operation.
     * @param[in] lhs left hand side of add operation.
     * @param[in] rhs right hand side of add operation.
     * @param[in] lastOpOverflow overlfow in LSBs add operation.
     * @param[in] max maximum allowed (or possible) value.
     * @return true if add overflows.
     */
    template<class Numeric>
    inline constexpr bool addAndCheckOverflow(
        Numeric& sum,
        const Numeric& lhs,
        const Numeric& rhs,
        bool lastOpOverflow = false,
        const Numeric& max = NumericTraits<Numeric>::max()
    ) noexcept
    {
        sum = lhs;
        return addAndCheckOverflow<Numeric>(sum, rhs, lastOpOverflow, max);
    }


    /**
     * @brief Acquire bitsize of each item.
     * 
     * @tparam Integer type of item to evaluate.
     * @return std::size_t number of bits.
     */
    template<Integral Integer>
    [[nodiscard]] inline consteval std::size_t bitSize() noexcept
    {
        if constexpr (NumericTraits<Integer>::isCustomSized)
            return Integer::bitSize();
        else
            return sizeof(Integer) * 8;
    }

    /// @brief Implementation helper code.
    namespace impl_detail_
    {
        /**
         * @brief Selects either 32 or 64 bit integer (signed | unsigned) 
         * depending on size.
         * 
         * @tparam bitSize size in bits required.
         * @tparam isSigned wether signed integer to use.
         */
        template<std::size_t bitSize, bool isSigned = false>
        using Integer32ThresholdSize_ = std::conditional_t<
            bitSize <= 32,
            std::conditional_t<isSigned, std::int32_t, std::uint32_t>,
            std::conditional_t<isSigned, std::int64_t, std::uint64_t>
        >;

        /**
         * @brief Selects either 16, 32 or 64 bit integer (signed | unsigned) 
         * depending on size.
         * 
         * @tparam bitSize size in bits required.
         * @tparam isSigned wether signed integer to use.
         */
        template<std::size_t bitSize, bool isSigned = false>
        using Integer16ThresholdSize_ = std::conditional_t<
            bitSize <= 16,
            std::conditional_t<isSigned, std::int16_t, std::uint16_t>,
            Integer32ThresholdSize_<bitSize, isSigned>
        >;

        
        /**
         * @brief Selects either 8, 16, 32 or 64 bit integer (signed | unsigned) 
         * depending on size.
         * 
         * @tparam bitSize size in bits required.
         * @tparam isSigned wether signed integer to use.
         */
        template<std::size_t bitSize, bool isSigned = false>
        using Integer8ThresholdSize_ = std::conditional_t<
            bitSize <= 8,
            std::conditional_t<isSigned, std::int8_t, std::uint8_t>,
            Integer16ThresholdSize_<bitSize, isSigned>
        >;

    }

    /**
     * @brief Selects Appropriate unsigned integer type depending on size 
     * required.
     * 
     * @tparam bitSize size in bits required.
     */
    template<std::size_t bitSize>
    using SizeCapableUint = impl_detail_::Integer8ThresholdSize_<bitSize, false>;

    /**
     * @brief Selects Appropriate signed integer type depending on size 
     * required.
     * 
     * @tparam bitSize size in bits required.
     */
    template<std::size_t bitSize>
    using SizeCapableInt = impl_detail_::Integer8ThresholdSize_<bitSize, true>;

    /**
     * @brief Selects Appropriate unsigned integer type depending on max 
     * value required.
     * 
     * @tparam maxValue maximum value to be supported.
     */
    template<std::size_t maxValue>
    using MaxCapableUint = SizeCapableUint<cilog2(maxValue)>;
    
    namespace impl_detail_
    {
        /**
         * @brief Select an item from list according to a comparison predicate.
         * 
         * `Result[0] = data[0];`
         * 
         * `Result[i] = (data[i] op Result[i - 1]) ? data[i] : Result[i - 1];`
         * 
         * `Result = Result[size - 1];`
         * 
         * @tparam Op Operator functor type.
         * @tparam Container Container to store data.
         * @param[in] data data to operate on.
         * @param[inout] op operator functor object.
         * @return const Container::value_type& const reference to result.
         */
        template<class Op, class Container>
        [[nodiscard]] inline constexpr const typename Container::value_type& 
            cascadeOp_(const Container& data, Op&& op = {}) noexcept
        {
            auto it = data.begin();

            for(auto i = data.begin(); i < data.end(); ++i)
                if(op(*i, *it))
                    it = i;

            return *it;
        }
    }

    /**
     * @brief Acquire const reference to lowest value in range.
     * 
     * @tparam Container Range type.
     * @param[in] data data to find min from.
     * @return const Container::value_type& reference to minimum value.
     */
    template<class Container>
    [[nodiscard]] inline constexpr const typename Container::value_type&
        min(const Container& data) noexcept
    {
        return impl_detail_::cascadeOp_<
            std::less<typename Container::value_type>
        >(data, {});
    }

    /**
     * @brief Acquire const reference to largest value in range.
     * 
     * @tparam Container Range type.
     * @param[in] data data to find max from.
     * @return const Container::value_type& reference to maximum value.
     */
    template<class Container>
    [[nodiscard]] inline constexpr const typename Container::value_type&
        max(const Container& data) noexcept
    {
        return impl_detail_::cascadeOp_<
            std::greater<typename Container::value_type>
        >(data, {});
    }

    /**
     * @brief Acquire lowest value in range (for temperory ranges).
     * 
     * @tparam Container Range type.
     * @param[in] data data to find min from.
     * @return Container::value_type minimum value.
     */
    template<class Container>
    [[nodiscard]] inline constexpr typename Container::value_type
        min(Container&& data) noexcept
    {
        return impl_detail_::cascadeOp_<
            std::less<typename Container::value_type>
        >(std::move(data), {});
    }

    /**
     * @brief Acquire largest value in range (for temperory ranges).
     * 
     * @tparam Container Range type.
     * @param[in] data data to find max from.
     * @return Container::value_type maximum value.
     */
    template<class Container>
    [[nodiscard]] inline constexpr typename Container::value_type
        max(Container&& data) noexcept
    {
        return impl_detail_::cascadeOp_<
            std::greater<typename Container::value_type>
        >(std::move(data), {});
    }

    /**
     * @brief Acquire lowest value from initializer list.
     * 
     * @tparam Element Element type of the list.
     * @param[in] data data to find min from.
     * @return Element minimum value.
     */
    template<class Element>
    [[nodiscard]] inline constexpr Element
        min(std::initializer_list<Element>&& data) noexcept
    {
        return impl_detail_::cascadeOp_<
            std::less<Element>
        >(std::move(data), {});
    }

    /**
     * @brief Acquire largest value from initializer list.
     * 
     * @tparam Element Element type of the list.
     * @param[in] data data to find max from.
     * @return Element maximum value.
     */
    template<class Element>
    [[nodiscard]] inline constexpr Element
        max(std::initializer_list<Element>&& data) noexcept
    {
        return impl_detail_::cascadeOp_<
            std::greater<Element>
        >(std::move(data), {});
    }

    /**
     * @brief Get rounded up result of division.
     * 
     * @tparam Numeric Number type.
     * @param[in] numerator numerator of division.
     * @param[in] denominator denominator of division.
     * @return Numeric division result.
     */
    template<Integral Numeric>
    [[nodiscard]] inline constexpr Numeric DivideRoundUp(
        const Numeric& numerator,
        const Numeric& denominator
    ) noexcept
    {
        auto remainder = numerator % denominator;
        auto ret = numerator / denominator;

        if(remainder != 0)
            ++ret;
        
        return ret;        
    }

    /**
     * @brief Select Larger type from the two. (Selection based on `bitSize` method).
     */
    template<Integral TypeA, Integral TypeB>
    using LargerType = std::conditional_t<bitSize<TypeA>() < bitSize<TypeB>(), TypeB, TypeA>;

    /**
     * @brief Select Smaller type from the two. (Selection based on `bitSize` method).
     */
    template<Integral TypeA, Integral TypeB>
    using SmallerType = std::conditional_t<bitSize<TypeA>() < bitSize<TypeB>(), TypeA, TypeB>;

    /**
     * @brief Generate mask to mask lower n bits.
     * 
     * May use static member function.
     * 
     * @param[in] size number of bits to be masked.
     * @return Integer Mask.
     */
    template<class Integer>
    [[nodiscard]] constexpr inline Integer nBitMask(std::size_t size) noexcept
    {
        constexpr auto hasMember = NumericTraits<Integer>::hasBitMaskGenerator;

        if constexpr (hasMember)
            return Integer::nBitMask(size);
        else
            return (static_cast<Integer>(1) << size) - 1;
    }



    /**
     * @brief Split Integer to multiple smaller integers, (concatenate in binary to restore).
     * 
     * @tparam Source Source Type.
     * @tparam destSize_ destination size.
     * @tparam Destination destination type.
     * 
     * Requires `destSize_ <= bitSize<Destination>()`.
     * 
     * @param[in] value source
     * @return std::array<Destination, DivideRoundUp<std::size_t>(bitSize<Source>(), destSize_)> array of destination integers.
     */
    template<UnsignedIntegral Source, std::size_t destSize_, UnsignedIntegral Destination = SizeCapableUint<destSize_>>
        requires (destSize_ <= bitSize<Destination>())
    [[nodiscard]] constexpr inline 
        std::array<Destination, DivideRoundUp<std::size_t>(bitSize<Source>(), destSize_)> 
        splitInteger(Source value) noexcept
    {
        std::array<Destination, DivideRoundUp<std::size_t>(bitSize<Source>(), destSize_)> ret;

        for(std::size_t i = 0; i < ret; ++i)
        {
            ret[i] = static_cast<decltype(ret)::value_type>(value & nBitMask<Source>(destSize_));
            value = value >> destSize_;
        }

        return ret;
    }
}

#endif // INCLUDE_EASYMATHLIB_EASYMATH_H_INCLUDED
