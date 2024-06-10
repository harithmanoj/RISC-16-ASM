/**
 * @file easyParse.h 
 * @author Harith Manoj (harithpub@gmail.com)
 * @brief Easy Parse library to simplifiy string processing.
 * @date 02 May 2024
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


#ifndef INCLUDE_EASYPARSELIB_EASYPARSE_H_INCLUDED

/// @brief include\easyParseLib\easyParse.h Header Guard 
#define INCLUDE_EASYPARSELIB_EASYPARSE_H_INCLUDED

#include <string_view>
#include <cstdint>
#include <string>
#include <stdexcept>

#include <easyMathLib/easyMath.h>

/// @brief encapsulate parser library code.
namespace easyParse
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
     * @brief Convert hex character to numeric value.
     * 
     * @param[in] ch hex character.
     * @return std::uint8_t value.
     */
    [[nodiscard]] constexpr inline std::uint8_t hexDigitConverter(char ch) noexcept
    {
        if(easyMath::valueBetweenInclusive<char>(ch, 'a', 'f'))
            return ch - 'a' + 10;
        else if (easyMath::valueBetweenInclusive<char>(ch, 'A', 'F'))
            return ch - 'A' + 10;
        else
            return ch - '0';
    }

    /**
     * @brief Convert digit character to numeric value.
     * 
     * @param[in] ch digit character.
     * @return std::uint8_t value.
     */
    [[nodiscard]] constexpr inline std::uint8_t digitConverter(char ch) noexcept
    {
        return ch - '0';
    }

    /**
     * @brief Check if char is a valid Octal character.
     * 
     * @param[in] ch character to evaluate.
     */
    [[nodiscard]] constexpr inline bool isOctalDigit(char ch) noexcept
    {
        return easyMath::valueBetweenInclusive<char>(ch, '0', '7');
    }

    /**
     * @brief Check if char is a valid hex character.
     * 
     * @param[in] digit character to evaluate.
     */
    [[nodiscard]] constexpr inline bool isHexDigit(char digit) noexcept
    {
        if(easyMath::valueBetweenInclusive<char>(digit, '0', '9'))
            return digit - '0';
        else if(easyMath::valueBetweenInclusive<char>(digit, 'a', 'f'))
            return digit - 'a' + 10;
        else if(easyMath::valueBetweenInclusive<char>(digit, 'A', 'F'))
            return digit - 'A' + 10;
        else
            return 0;
    }

    /**
     * @brief Check if char is a valid Decimal character.
     * 
     * @param[in] ch character to evaluate.
     */
    [[nodiscard]] constexpr inline bool isDecDigit(char ch) noexcept
    {
        return easyMath::valueBetweenInclusive<char>(ch, '0', '9');
    }

    /**
     * @brief Convert binary number stream to unsigned integer.
     * 
     * Convert `min(bitSize(Integer), numberString.size())` number of digits to unsigned integer.
     * 
     * @tparam Integer Return type.
     * @param[in] numberString number string.
     * @return Integer Converted integer
     */
    template<easyMath::UnsignedIntegral Integer>
    [[nodiscard]] constexpr inline Integer convertBinaryString(std::string_view numberString) noexcept
    {
        Integer ret = 0ull;

        auto size = easyMath::min({easyMath::bitSize<Integer>(), numberString.size()});

        for(std::size_t i = (numberString.size() - size); i < numberString.size(); ++i)
        {
            ret = ret << 1;
            ret |= (numberString[i] == '1' ? 1 : 0);
        }

        return ret;
    }

    namespace impl_detail_
    {
        /**
         * @brief Generic Convert digit to number. returns -1 as default.
         * 
         * @tparam perBitSize Size of each digit in bits.
         * @param[in] digit digit to convert.
         * @return SizeCapableUint<perBitSize> converted value.
         */
        template<std::size_t perBitSize>
        [[nodiscard]] constexpr easyMath::SizeCapableUint<perBitSize> convertDigit_(char digit) noexcept
        {
            return {-1ull};
        }

        /**
         * @brief Convert Octal to 3 bit value.
         * 
         * @param[in] digit digit to convert.
         * @return SizeCapableUint<3> converted value.
         */
        template<>
        [[nodiscard]] constexpr easyMath::SizeCapableUint<3> convertDigit_<3>(char digit) noexcept
        {
            return digitConverter(digit);
        }

        /**
         * @brief Convert Hex to 4 bit value.
         * 
         * @param[in] digit digit to convert.
         * @return SizeCapableUint<4> converted value.
         */
        template<>
        [[nodiscard]] constexpr easyMath::SizeCapableUint<4> convertDigit_<4>(char digit) noexcept
        {
            return hexDigitConverter(digit);
        }

        /**
         * @brief Convert Digits of number string composed of digits of base 2^n where n is an integer.
         * 
         * @tparam Integer Type to store data.
         * @tparam perBitSize size of each digit in bits.
         * @param[inout] numberString number string, consumed digits are erased from string.
         * @param[in] carrySize number of bits carried over from last.
         * @param[in] carry value carried over from last.
         * @return std::tuple<Integer, std::size_t, SizeCapableUint<perBitSize>> 
         *      `<0>`: Value converted from string.
         *      `<1>`: Number of bits to be carried over.
         *      `<2>`: Value of bits to be carried over.
         */
        template<easyMath::UnsignedIntegral Integer, std::size_t perBitSize>
        [[nodiscard]] constexpr inline std::tuple<Integer, std::size_t, easyMath::SizeCapableUint<perBitSize>> convertDirectTranslatableBaseString_(
            std::string_view& numberString, 
            std::size_t carrySize, 
            easyMath::SizeCapableUint<perBitSize> carry
        ) noexcept
        {
            using CarryCountT = easyMath::SizeCapableUint<perBitSize>;

            std::tuple<Integer, std::size_t, CarryCountT> ret = {
                static_cast<Integer>(0u),
                0ull,
                static_cast<CarryCountT>(0u),
            };

            auto& retVal = std::get<0>(ret);
            auto& retCarrySize = std::get<1>(ret);
            auto& retCarryVal = std::get<2>(ret);
            

            std::size_t size = 0;
            auto numberStringSize = numberString.size() * perBitSize;

            if((easyMath::bitSize<Integer>() - carrySize) < numberStringSize)
                size = easyMath::bitSize<Integer>() - carrySize;
            else
                size = numberStringSize;

            auto count = easyMath::DivideRoundUp(size, perBitSize);

            retVal = carry;

            for(std::size_t i = (numberString.size() - count); (i < numberString.size()) && (size > 0); ++i)
            {
                if(size >= perBitSize)
                {
                    retVal = retVal << perBitSize;
                    retVal |= static_cast<Integer>(impl_detail_::convertDigit_<perBitSize>(numberString[i])) & easyMath::nBitMask<Integer>(perBitSize);
                    size -= perBitSize;
                }
                else
                {
                    retCarrySize = perBitSize - size;
                    retVal = retVal << size;
                    retCarryVal = static_cast<Integer>(impl_detail_::convertDigit_<perBitSize>(numberString[i])) & easyMath::nBitMask<Integer>(perBitSize);
                    auto mask = easyMath::nBitMask<Integer>(size);
                    retVal = retCarryVal & mask;
                    retCarryVal &= ~mask;
                    retCarryVal = retCarryVal >> size;
                    size = 0;
                }
            }

            if(numberString.size() > count)
                numberString = numberString.substr(0, numberString.size() - count);
            else
                numberString = {};

            return ret;
        }
    }

    /**
     * @brief Convert hex string to unsigned integer.
     * 
     * @tparam Integer Destination type.
     * @param[inout] numberString hex string, removes processed digits.
     * @param[inout] numberString number string, consumed digits are erased from string.
     * @param[in] carrySize number of bits carried over from last.
     * @param[in] carry value carried over from last.
     * @return std::tuple<Integer, std::size_t, std::uint8_t
     *      `<0>`: Value converted from string.
     *      `<1>`: Number of bits to be carried over.
     *      `<2>`: Value of bits to be carried over.
     */
    template<easyMath::UnsignedIntegral Integer>
    [[nodiscard]] constexpr inline std::tuple<Integer, std::size_t, std::uint8_t> convertHexString(
        std::string_view& numberString,
        std::size_t carrySize,
        std::uint8_t carry
    ) noexcept
    {
        return impl_detail_::convertDirectTranslatableBaseString_<Integer, 4>(numberString, carrySize, carry);
    }

    /**
     * @brief Convert hex string to unsigned integer.
     * 
     * @tparam Integer Destination type.
     * @param[inout] numberString hex string.
     * @return Integer Value converted from string.
     */
    template<easyMath::UnsignedIntegral Integer>
    [[nodiscard]] constexpr inline Integer convertHexString(
        std::string_view numberString
    ) noexcept
    {
        return std::get<0>(impl_detail_::convertDirectTranslatableBaseString_<Integer, 4>(numberString, 0, 0));
    }

    /**
     * @brief Convert octal string to unsigned integer.
     * 
     * @tparam Integer Destination type.
     * @param[inout] numberString octal string, removes processed digits.
     * @param[inout] numberString number string, consumed digits are erased from string.
     * @param[in] carrySize number of bits carried over from last.
     * @param[in] carry value carried over from last.
     * @return std::tuple<Integer, std::size_t, std::uint8_t
     *      `<0>`: Value converted from string.
     *      `<1>`: Number of bits to be carried over.
     *      `<2>`: Value of bits to be carried over.
     */
    template<easyMath::UnsignedIntegral Integer>
    [[nodiscard]] constexpr inline 
        std::tuple<Integer, std::size_t, std::uint8_t> convertOctString(
        std::string_view& numberString,
        std::size_t carrySize,
        std::uint8_t carry
    ) noexcept
    {
        return impl_detail_::convertDirectTranslatableBaseString_<Integer, 3>(numberString, carrySize, carry);
    }

    /**
     * @brief Convert octal string to unsigned integer.
     * 
     * @tparam Integer Destination type.
     * @param[inout] numberString octal string.
     * @return Integer Value converted from string.
     */
    template<easyMath::UnsignedIntegral Integer>
    [[nodiscard]] constexpr inline Integer convertOctString(
        std::string_view numberString
    ) noexcept
    {
        return std::get<0>(impl_detail_::convertDirectTranslatableBaseString_<Integer, 3>(numberString, 0, 0));
    }

    /**
     * @brief Convert a decimal number string to unsigned integral format.
     * 
     * @tparam Integer Type to store data.
     * @param[in] numberString decimal string.
     * @return Integer Converted Integer.
     */
    template<easyMath::UnsignedIntegral Integer>
    [[nodiscard]] constexpr inline Integer convertDecimalString(std::string_view numberString) noexcept
    {
        Integer ret = 0ull;

        for(auto digit : numberString)
        {
            ret *= 10;
            ret += digit - '0';
        }

        return ret;
    }

    constexpr std::string_view WHITESPACE_STRING = " \n\t\r\0";

    /**
     * @brief Find offset at which the first non-whitespace character exists, evalution from given offset.
     * 
     * @param[in] line line to evaluate.
     * @param[in] offset offset to start from.
     * @return std::size_t offset of first non-whitespace character from offset.
     */
    [[nodiscard]] constexpr inline std::size_t advanceOverWhiteSpace(
        std::string_view line,
        std::size_t offset = 0
    ) noexcept
    {
        return line.find_first_not_of(WHITESPACE_STRING, offset);
    }

    /**
     * @brief Generate substring after stripping leading 
     * and lagging whitespace.
     * 
     * @param[in] line line to strip whitespace from.
     * @return std::string_view returns whitespace stripped string.
     */
    [[nodiscard]] constexpr inline std::string_view stripWhiteSpace(
        const std::string_view& line
    ) noexcept
    {
        auto begin = line.find_first_not_of(WHITESPACE_STRING);
        auto end = line.find_last_not_of(WHITESPACE_STRING);

        if(begin == line.npos)
            return {};

        return line.substr(begin, end - begin + 1);
    }

    /**
     * @brief Generate substring after stripping leading 
     * and lagging whitespace.
     * 
     * @param[in] line line to strip whitespace from.
     * @param[in] commentDelim character indicating begining of comment.
     * @return std::string_view returns whitespace stripped string.
     */
    [[nodiscard]] constexpr inline std::string_view stripCommentsAndWhiteSpace(
        const std::string_view& line, 
        char commentDelim
    ) noexcept
    {
        auto commentCharacter = line.find_first_of(commentDelim);
        return stripWhiteSpace(line.substr(0, commentCharacter));
    }

    /**
     * @brief Check if substring is part of line.
     * 
     * @param[in] line line to check.
     * @param[in] substr substring to scan for.
     * @return true substring is present.
     * @return false substring not present.
     */
    [[nodiscard]] constexpr inline bool isSubStringPresent(
        const std::string_view& line, 
        const std::string_view& substr
    ) noexcept
    {
        if(line.size() < substr.size())
            return false;

        auto checkSize = line.size() - substr.size() + 1;

        for(std::size_t i = 0; i < checkSize; ++i)
            if(line.substr(i, substr.size()) == substr)
                return true;

        return false;
    }

    [[nodiscard]] constexpr inline bool validateHexString(std::string_view numberString) noexcept
    {
        for(auto digit : numberString)
        {
            auto isDigit = easyMath::valueBetweenInclusive(digit, '0', '9')
                || easyMath::valueBetweenInclusive(digit, 'a', 'f')
                || easyMath::valueBetweenInclusive(digit, 'A', 'F');

            if(!isDigit)                
                return false;
        }

        return true;
    }
    

    [[nodiscard]] constexpr inline bool validateOctString(std::string_view numberString) noexcept
    {
        for(auto digit : numberString)
        {
            auto isDigit = easyMath::valueBetweenInclusive(digit, '0', '7');

            if(!isDigit)                
                return false;
        }

        return true;
    }

    [[nodiscard]] constexpr inline bool validateDecString(std::string_view numberString) noexcept
    {
        for(auto digit : numberString)
        {
            auto isDigit = easyMath::valueBetweenInclusive(digit, '0', '9');

            if(!isDigit)                
                return false;
        }

        return true;
    }

    [[nodiscard]] constexpr inline bool validateBinString(std::string_view numberString) noexcept
    {
        for(auto digit : numberString)
        {
            auto isDigit = (digit == '0') || (digit == '1');

            if(!isDigit)                
                return false;
        }

        return true;
    }

    

    /**
     * @brief Convert escaped string to ascii characters.
     * 
     * @throw (1) `std::invalid_argument`: If `escapedString` not begining with `'\'`.
     * @throw (2) `std::invalid_argument`: If `escapedString` starts with \[x|X] and 
     *  contains non hex digits, or starts with \[o|O] and 
     *  contains non octal digits, or starts with \ and 
     *  contains non decimal digits.
     * 
     * @param[in] escapedString escaped string to convert.
     * @return char ascii character.
     */
    [[nodiscard]] constexpr inline char convertEscapedString(std::string_view escapedString)
    {
        if(escapedString[0] != '\\')
            throw std::invalid_argument("Expected escaped string starting with \\");
        
        auto iterator = escapedString.begin() + 1;
        char ch = *iterator;

        switch (ch)
        {
        case '\'':  return '\'';
        case '\"':  return '\"';
        case '?':   return '?';
        case '\\':  return '\\';
        case 'a':   return '\a';
        case 'b':   return '\b';
        case 'f':   return '\f';
        case 'n':   return '\n';
        case 'r':   return '\r';
        case 't':   return '\t';
        case 'v':   return '\v';
        case 'x': [[fallthrough]];
        case 'X': 
            {
                escapedString = escapedString.substr(2);
                if(!validateHexString(escapedString))
                    throw std::invalid_argument("Escaped [x|X] is not followed by hex number string.");

                return static_cast<char>(convertHexString<std::uint8_t>(escapedString));
            }
        case 'o': [[fallthrough]];
        case 'O': 
            {
                escapedString = escapedString.substr(2);
                
                if(!validateOctString(escapedString))
                    throw std::invalid_argument("Escaped [o|O] is not followed by oct number string.");

                return static_cast<char>(convertOctString<std::uint8_t>(escapedString));
            }
        default: 
            {
                escapedString = escapedString.substr(1);
                
                if(!validateDecString(escapedString))
                    throw std::invalid_argument("Escaped string is not decimal number string.");
                return static_cast<char>(convertDecimalString<std::uint8_t>(escapedString));
            }            
        }
    }

    /**
     * @brief Advance over text and convert escaped characters to characters.
     * 
     * - Support for : \x or \X followed by hex string, \o or \O followed by octal string.
     * - Support for : \ followed by decimal string.
     * - Support for 2 character (\a model) escape sequences.
     * - Returns \ for incomplete escape sequences.
     * supported by \ref `gen_asm::convertEscapedString`.
     * 
     * @throw (1) `std::invalid_argument`: If `iterator >= sentinal`.
     * @throw (2) `std::invlaid_argument`: If `"\x"` or `"\X"` argument passed without value.
     * @throw (3) see \ref `gen_asm::convertEscapedString` exceptions (2).
     * 
     * @param[inout] iterator iterator to start from.
     * @param[in] sentinal iterator to one over last character.
     * @return std::pair<char, bool> </br>
     * char : Actual character rathar than escape string. </br>
     * bool : True if character was escaped.
     */
    [[nodiscard]] constexpr inline std::pair<char, bool> advanceOverText(
        std::string_view::const_iterator& iterator,
        std::string_view::const_iterator sentinal
    )
    {
        if(iterator >= sentinal)
            throw std::invalid_argument("Empty String to advance over");

        auto begin = iterator;
        char ch = *iterator;
        ++iterator;            

        if(ch == '\\')
        {
            if(iterator < sentinal)
            {
                ch = *iterator;
                ++iterator;
                switch (ch)
                {
                case 'x': [[fallthrough]];
                case 'X':
                    {
                        if(iterator >= sentinal)
                            throw std::invalid_argument("Expected Value after \\[x,X]");

                        auto hexBegin = iterator - 2;
                        ch = *iterator;
                        while (
                            (iterator < sentinal)
                            && (std::isxdigit(ch))
                        )
                        {
                            ++iterator;
                            if(iterator < sentinal)
                                ch = *iterator;
                            else 
                                break;
                        }

                        return 
                        {
                            convertEscapedString(std::string_view(hexBegin, iterator)), 
                            true
                        };
                    }
                case 'o':  [[fallthrough]];
                case 'O':
                    {
                        auto octBegin = iterator - 2;
                        ch = *iterator;
                        while (
                            (iterator < sentinal)
                            && (ch < '8') && (ch >= '0')
                        )
                        {
                            ++iterator;
                            if(iterator < sentinal)
                                ch = *iterator;
                            else 
                                break;                          
                        }

                        return 
                        {
                            convertEscapedString(std::string_view(octBegin, iterator)), 
                            true
                        };
                    }
                default:
                    auto singleBegin = iterator - 2;
                    return 
                    {
                        convertEscapedString(std::string_view(singleBegin, iterator)),
                        true
                    };
                }
            }
            else
            {
                return {'\\', true};
            }
            
        }
        else 
        {
            return {ch, false};
        }
    }

    /**
     * @brief Extract string till a delimiter, modify initial string to remove the exxtracted string and delimiter.
     * 
     * @param[inout] string string to process. modified to be string after delimiter.
     * @param[in] delim delimiter character.
     * @return std::string_view extracted string.
     */
    [[nodiscard]] constexpr inline std::string_view extractTillDelimiter(
        std::string_view& string,
        char delim = ','
    ) noexcept
    {
        auto find = string.find_first_of(delim);
        auto ret = string;
        if(find == string.npos)
            string = {};
        else
        {
            ret = string.substr(0, find);
            string = string.substr(find + 1);
        }
        return ret;
    }

    /**
     * @brief Split string into substring based on location of the delimiters.
     * 
     * eg : Hello uo, awr; asdf; asda, sad , asd a , a
     *  will be split using {' ', ',', ';', ','} to
     *      "Hello", "uo", " awr", " asdf; asda", " sad ", " asd a ", " a".
     * 
     * @param[in] string string to split.
     * @param[in] delim delimiter to split string.
     * @return std::vector<std::string_view> list of split substrings.
     */
    [[nodiscard]] inline std::vector<std::string_view> splitUsingDelimiterList(std::string_view string, std::vector<char> delim = {','})
    {
        std::vector<std::string_view> ret;

        for(std::size_t i = 0; (i < delim.size()) && !string.empty(); ++i)
        {
            ret.push_back(extractTillDelimiter(string, delim[i]));
        }

        while(!string.empty())
            ret.push_back(extractTillDelimiter(string, delim.back()));

        return ret;
    }

    /**
     * @brief Advance through string while ignoring any substring enclosed by '' or "".
     * 
     * - Ignores any substring enclosed by "" or ''
     * - " and ' can be escaped by \
     * - \" & \' inside text substring will not be taken as closing of string.
     * 
     * calling on `hello"asd""asd"aa'a'` will return
     * `helloaa` and `-1`.
     * 
     * @param[inout] iterator iterator to use.
     * @param[inout] sentinal sentinal to guard against out of range access.
     * @return std::pair<char, bool> next valid character & whether any substring was skipped.
     */
    constexpr inline std::pair<char, bool> advanceSkipReportQuotedText(
        std::string_view::const_iterator& iterator,
        std::string_view::const_iterator sentinal
    )
    {
        std::pair<char, bool> eval;
        char& charUnderEval = eval.first;
        bool& isEscaped = eval.second;

        bool isText = false;
        bool isDoubleQuote = false;

        eval = advanceOverText(iterator, sentinal);

        std::pair<char, bool> ret;

        isDoubleQuote = (charUnderEval == '\"');
        isText = (!isEscaped ) && (isDoubleQuote || (charUnderEval == '\''));

        if(!isText)
            return {charUnderEval, false};

        while(iterator < sentinal)
        {
            while ((iterator < sentinal) && (isText))
            {
                eval = advanceOverText(iterator, sentinal);

                if(isText)
                {
                    if((charUnderEval == '\"') && isDoubleQuote && (!isEscaped))
                    {
                        isText = false;
                        isDoubleQuote = false;
                    }
                    else if ((charUnderEval == '\'') && (!isDoubleQuote) && (!isEscaped))
                    {
                        isText = false;
                        isDoubleQuote = false;
                    }
                }
            }

            if(iterator < sentinal)
            {
                eval = advanceOverText(iterator, sentinal);

                isDoubleQuote = (charUnderEval == '\"');
                isText = (!isEscaped ) && (isDoubleQuote || (charUnderEval == '\''));

                if(!isText)
                    return {charUnderEval, true};
            }
        }

        return {-1, true};
    }

    /**
     * @brief Advance through string while ignoring any substring enclosed by '' or "".
     * 
     * - Ignores any substring enclosed by "" or ''
     * - " and ' can be escaped by \
     * - \" & \' inside text substring will not be taken as closing of string.
     * 
     * calling on `hello"asd""asd"aa'a'` will return
     * `helloaa` and `-1`.
     * 
     * @param[inout] iterator iterator to use.
     * @param[inout] sentinal sentinal to guard against out of range access.
     * @return char next valid character.
     */
    constexpr inline char advanceAndIgnoreQuotedText(
        std::string_view::const_iterator& iterator,
        std::string_view::const_iterator sentinal
    ) noexcept
    {
        return advanceSkipReportQuotedText(iterator, sentinal).first;
    }
    
    /**
     * @brief Get vector of stringes that were not enclosed by "" or '' in input string.
     * 
     * @param[in] string string to evaluate.
     * @return std::vector<std::string_view> list of non enclosed sub-strings.
     */
    [[nodiscard]] inline std::vector<std::string_view> extractNonText(std::string_view string)
    {
        auto iterator = string.begin();
        auto sentinal = string.end();
        std::vector<std::string_view> ret;

        auto begin = string.begin();

        while (iterator < sentinal)
        {
            auto temp = iterator;
            auto eval = advanceSkipReportQuotedText(iterator, sentinal);
            if(eval.second)
            {
                ret.push_back(
                    string.substr(std::ranges::distance(string.begin(), begin), std::ranges::distance(begin, temp))
                );
                begin = iterator;
            }
        }
        return ret;
    }

    /**
     * @brief Convert number string to integer.
     * 
     * Support for:
     * - 0x or 0X prefix hex.
     * - 0b or 0B prefix octal.
     * - 0 prefix octal.
     * - No prefix decimal
     * - Negative numbers will produce 2's complement.
     * 
     * @throw (1) `std::invalid_argument` : Empty string or only '-' passed.
     * @throw (2) `std::invalid_argument` : Invalid characters (non hex in a 0x string or non binary in 0b ...).
     * 
     * @tparam UInteger Type of integer.
     * @param[in] numberString number string to convert.
     * @return UInteger Converted integer.
     */
    template<easyMath::UnsignedIntegral UInteger>
    [[nodiscard]] inline UInteger convertNumberString(std::string_view numberString)
    {
        if(numberString.size() == 0)
            throw std::invalid_argument("empty string");

        bool isNegative = (numberString[0] == '-');

        if(isNegative)
            numberString = numberString.substr(1);
        
        if(numberString.size() == 0)
            throw std::invalid_argument("string with - only");

        UInteger ret = 0;

        if(numberString[0] == '0')
        {
            if(numberString.size() == 1)    
                return 0;
            else if((numberString[1] == 'x') || (numberString[1] == 'X'))
            {
                if(validateHexString(numberString.substr(2)))
                    ret = convertHexString<UInteger>(numberString.substr(2));
                else
                    throw std::invalid_argument("Invalid characters in hex number string");
            }
            else if((numberString[1] == 'b') || (numberString[1] == 'B'))
            {
                if(validateBinString(numberString.substr(2)))
                    ret = convertBinaryString<UInteger>(numberString.substr(2));
                else
                    throw std::invalid_argument("Invalid characters in binary number string");
            }
            else if(validateOctString(numberString.substr(1)))
                ret = convertOctString<UInteger>(numberString.substr(1));
            else
                throw std::invalid_argument("Invalid characters in oct number string");
        }
        else if(validateDecString(numberString))
            ret = convertDecimalString<UInteger>(numberString);
        else
            throw std::invalid_argument("Invalid characters in decimal number string");

        if(isNegative)
            ret = ~ret + 1;
        
        return ret;
    }

    /**
     * @brief Validates number string, for decimal, 
     * octal (if prefixed by 0), 
     * hex (if prefixed by 0x), 
     * binary (if prefixed by )
     * 
     * @param[in] numberString string to validate.
     * @return bool true if valid number string.
     */
    [[nodiscard]] inline bool validateNumberString(std::string_view numberString)
    {
        if(numberString.size() == 0)
            throw std::invalid_argument("empty string");

        if((numberString[0] == '-'))
            numberString = numberString.substr(1);
        
        if(numberString.size() == 0)
            throw std::invalid_argument("string with - only");

        if(numberString[0] == '0')
        {
            if(numberString.size() == 1)    
                return true;
            else if((numberString[1] == 'x') || (numberString[1] == 'X'))
                return validateHexString(numberString.substr(2));
            else if((numberString[1] == 'b') || (numberString[1] == 'B'))
                return validateBinString(numberString.substr(2));
            else 
                return validateOctString(numberString.substr(1));
        }
        else 
            return validateDecString(numberString);
    }

    /**
     * @brief Check wether the root line at offset has a substring matching check.
     * 
     * @param[in] root line to evaluate.
     * @param[in] check substring to match.
     * @param[in] offset location to match.
     * @return true match exits.
     * @return false match do not exists, also if `offset >= root.size()`
     */
    [[nodiscard]] constexpr inline bool isExactSubstr(std::string_view root, std::string_view check, std::size_t offset = 0) noexcept
    {
        if(offset >= root.size())
            return false;
        else 
            return (root.substr(offset, check.size()) == check);
    }

}


#endif // INCLUDE_EASYPARSELIB_EASYPARSE_H_INCLUDED


