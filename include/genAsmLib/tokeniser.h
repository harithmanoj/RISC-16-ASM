/**
 * @file tokeniser.h 
 * @author Harith Manoj (harithpub@gmail.com)
 * @brief Tokeniser and assorted functions definition.
 * @date 09 May 2024
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


#ifndef INCLUDE_GENASMLIB_TOKENISER_H_INCLUDED

/// @brief include\genAsmLib\tokeniser.h Header Guard 
#define INCLUDE_GENASMLIB_TOKENISER_H_INCLUDED

#include <string_view>
#include <cstdint>
#include <concepts>
#include <vector>
#include <ranges>
#include <string>

#include <easyParseLib/easyParse.h>
#include <easyMathLib/easyMath.h>

namespace gen_asm
{

    /// @brief Implementation detail codes.
    namespace impl_detail_
    {
  
    }

    /**
     * @brief Check if type has all types required by assembler as members.
     * 
     * @tparam Trait type to check.
     * 
     * Must Have:
     * 
     * @conceptMember{Types}
     * 
     * - `BasicType` satisfying `easyMath::Integral`.
     *      - Smallest type (most basic, usually an 8 bit type or same as `WordType`).
     * - `LargestType` satisfying `easyMath::Integral`.
     *      - Largest possible type needed by assembler. (to manage any data types such as QWORD).
     * - `WordType` satisfying `easyMath::Integral`.
     *      - Word type of the ISA, usually a type, with size that is greater than the word length of ISA.
     * - `AddressType` satisfying `easyMath::Integral`.
     *      - Address type of the ISA, usually a type, with size that is greater than all addresses in ISA.
     * - `BlockSize` satisfying `easyMath::Integral`.
     *      - BlockSize type of the ISA, usually a type, that can encode all assembly data types.
     * - `RegisterCode` satisfying `easyMath::Integral`.
     *      - Register code type of the ISA, usually a type, that can encode all assembly registers.
     * - `ModifierCode` satisfying `easyMath::Integral`.
     *      - ModifierCode type of the ISA, usually a type, that can encode all assembly instruction modifiers.
     * - `OpCodeType` satisfying `easyMath::Integral`.
     *      - OpCode type of the ISA, usually a type, with size that is greater than all op codes in ISA.
     * 
     */
    template<class Trait>
    concept IsaTraitModel = requires() 
    {
        typename Trait::BasicType;
        typename Trait::LargestType;
        typename Trait::WordType;
        typename Trait::AddressType;
        typename Trait::BlockSizeType;
        typename Trait::RegisterCodeType;
        typename Trait::ModifierCodeType;
        typename Trait::OpCodeType;

        requires easyMath::Integral<typename Trait::BasicType>;
        requires easyMath::Integral<typename Trait::LargestType>;
        requires easyMath::Integral<typename Trait::WordType>;
        requires easyMath::Integral<typename Trait::AddressType>;
        requires easyMath::Integral<typename Trait::BlockSizeType>;
        requires easyMath::Integral<typename Trait::RegisterCodeType>;
        requires easyMath::Integral<typename Trait::ModifierCodeType>;
        requires easyMath::Integral<typename Trait::OpCodeType>;
    };
    
    /**
     * @brief Model to check if class has all needed Tokenizer traits.
     * 
     * @tparam TokenizerTrait type to check.
     * @tparam IsaTraits ISA trait class, must satisfy IsaTraitModel.
     * 
     * Must Have:
     * 
     * @conceptMember{Methods}
     * - `IsaTraits::BlockSizeType resolveSize(std::string_view)`
     *      - Encode size string. Must throw on invalid string.
     * - `IsaTraits::RegisterCodeType resolveRegister(std::string_view)`
     *      - Encode register name string. Must throw on invalid string.
     * - `IsaTraits::ModifierCodeType resolveModifier(std::string_view)`
     *      - Encode modifier string. Must throw on invalid string.
     * - `[bool] checkIfModifier(std::string_view)`
     *      - Check if string is modifier.
     * - `IsaTraits::OpCodeType resolveOpCode(std::string_view)`
     *      - Encode instruction name (add, addi etc). Must throw on invalid string.
     * 
     */
    template<class TokenizerTrait, class IsaTraits>
    concept TokenizerTraitModel = requires(const TokenizerTrait& trait, std::string_view str)
    {
        requires IsaTraitModel<IsaTraits>;
        { trait.resolveSize(str) } -> std::same_as<typename IsaTraits::BlockSizeType>;
        { trait.resolveRegister(str) } -> std::same_as<typename IsaTraits::RegisterCodeType>;
        { trait.resolveModifier(str) } -> std::same_as<typename IsaTraits::ModifierCodeType>;
        { trait.checkIfModifier(str) } -> std::convertible_to<bool>;
        { trait.resolveOpCode(str) } -> std::same_as<typename IsaTraits::OpCodeType>;

    };

    /// @brief Hold all compile time constants 
    namespace literal
    {
        // Size Symbols

        /// @brief Encode symbols with no size data.
        constexpr std::uint8_t NO_DATA = 0;

        /// @brief Encode symbols with ascii sized data.
        constexpr std::uint8_t ASCII_DATA = 1;

        // Symbol switches

        constexpr std::string_view EXPORT_SWITCH = ".export";
        constexpr std::string_view DATA_SWITCH = ".data";
        constexpr std::string_view CONST_SWITCH = ".const";
        
        // Data type switches
        constexpr std::string_view ASCII_SWITCH = ".ascii";
    }

    /// @brief Symbol type enumeration.
    enum class SymbolType
    {
        
        /// @brief Encode Address label symbols.
        JUMP,

        /// @brief Encode data memory location symbols.
        DATA,

        /// @brief Encode data with assembly time constants.
        CONST
    };

    /**
     * @brief Struct to encapsulate all symbol tokens.
     * 
     * @tparam IsaTraits All ISA types.
     */
    template<IsaTraitModel IsaTraits>
    struct SymbolToken
    {
        /// @brief Label of the symbol.
        std::string symbolName;

        /// @brief Whether symbol should be exported beyond the translational unit.
        bool isExport;

        /// @brief Type of symbol: jump, data, const.
        SymbolType symbolType;

        /// @brief type of symbol data unit in case of data, const.
        typename IsaTraits::BlockSizeType blockSizeCode;

        /// @brief Initial value of the data, in case of data, const
        std::vector<typename IsaTraits::LargestType> init_value;
    };

    /// @brief Alias to describe a index - data pair, with data of type `T`.
    template<class T>
    using IndexedData = std::pair<std::size_t, T>;

    /**
     * @brief Struct to encapsulate all tokenized elements of an instruction.
     * 
     * @tparam IsaTraits All ISA types.
     * 
     */
    template<IsaTraitModel IsaTraits>
    struct InstructionToken
    {
        /// @brief op code of instruction.
        typename IsaTraits::OpCodeType opCode;

        /// @brief vector of all encoded register arguments and their position in argument list (0, n).
        std::vector<IndexedData<typename IsaTraits::RegisterCodeType>> registerArgs;

        /// @brief vector of all immediate argument values and their position in argument list (0, n).
        std::vector<IndexedData<typename IsaTraits::LargestType>> immediateArgs;

        /// @brief vector of all encoded modifier arguments and their position in argument list (0, n).
        std::vector<IndexedData<typename IsaTraits::ModifierCodeType>> modifierArgs;

        /// @brief vector of all encoded symbol arguments, their indices and their position in argument list (0, n).
        std::vector<IndexedData<std::tuple<std::string, std::size_t, std::size_t>>> symbolArgs;
    };

    /**
     * @brief Class to tokenize lines of assembly code.
     * 
     * @tparam IsaTraits All ISA types.
     * @tparam TokenizerTraits Trait class for all necessary types and methods.
     * 
     */
    template<IsaTraitModel IsaTraits, TokenizerTraitModel<IsaTraits> TokenizerTraits>
    class Tokenizer
    {
        TokenizerTraits traitObj_;

        std::string_view strippedLineUnderEval_;
        SymbolToken<IsaTraits> symbolToken_;
        InstructionToken<IsaTraits> instructionToken_;
        bool isSymbol_;
        std::size_t cursor_;

        inline void evaluateIsSymbol_() noexcept
        {
            auto iterator = strippedLineUnderEval_.begin();
            cursor_ = 0;

            while(iterator < strippedLineUnderEval_.end())
            {
                auto begin = iterator;
                auto val = easyParse::advanceAndIgnoreQuotedText(iterator, strippedLineUnderEval_.end());
                if(val == ':')
                {
                    isSymbol_ = true;
                    cursor_ = std::ranges::distance(strippedLineUnderEval_.begin(), begin);
                    return;
                }
                else if(val == -1)
                {
                    isSymbol_ = false;
                    cursor_ = std::ranges::distance(strippedLineUnderEval_.begin(), iterator);
                    return;
                }
            }

            isSymbol_ = false;
            cursor_ = 0ull;
        }

        inline void advanceCursor(std::size_t count) noexcept
        {
            cursor_ = easyParse::advanceOverWhiteSpace(strippedLineUnderEval_, cursor_ + count);
        }

        inline bool invalidCursor() noexcept
        {
            return cursor_ >= strippedLineUnderEval_.size();
        }

        inline void tokenizeInstruction_()
        {

            auto split = easyParse::splitUsingDelimiterList(strippedLineUnderEval_, {' ', ','});

            instructionToken_.opCode = traitObj_.resolveOpCode(split[0]);
            
            for(std::size_t i = 1; i < split.size(); ++i)
            {
                auto arg = easyParse::stripWhiteSpace(split[i]);
                
                if(arg.empty())
                    throw std::invalid_argument("Empty argument to instruction");
                
                if(arg[0] == '%')
                    instructionToken_.registerArgs.push_back({ i - 1, traitObj_.resolveRegister(arg.substr(1))});
                else if(arg[0] == '$')
                    instructionToken_.immediateArgs.push_back({ i - 1, easyParse::convertNumberString<typename IsaTraits::LargestType>(arg.substr(1))});
                else if((arg[0] == '\'') && (arg.back() == '\'') && (arg.size() > 2))
                {
                    if(arg.size() == 3)
                        instructionToken_.immediateArgs.push_back({ i - 1, arg[1]});
                    else
                        instructionToken_.immediateArgs.push_back({ i - 1, easyParse::convertEscapedString(arg.substr(1, arg.size() - 2))});
                }
                else if(traitObj_.checkIfModifier(arg))
                    instructionToken_.modifierArgs.push_back({ i - 1, traitObj_.resolveModifier(arg)});
                else
                {
                    auto indexBegin = arg.find_first_of('[');
                    auto symbol = arg.substr(0, indexBegin);

                    std::size_t indexPrimary = 0;
                    std::size_t indexSecondary = 0;
                    std::string symbolName = static_cast<std::string>(symbol);

                    if(indexBegin != arg.npos)
                    {
                        auto indexEnd = arg.find_first_of(']', indexBegin);
                        auto index = arg.substr(indexBegin + 1, indexEnd - indexBegin - 1);
                        
                        if(index.empty())
                            throw std::invalid_argument("Symbol index empty");
                        
                        indexPrimary = easyParse::convertNumberString<std::size_t>(index);

                        indexBegin = easyParse::advanceOverWhiteSpace(arg, indexEnd + 1);
                        
                        if(indexBegin < arg.size())
                        {
                            if(arg[indexBegin] != '[')
                                throw std::invalid_argument("Unexpected character after first index of symbol");

                            if(arg.back() != ']')
                                throw std::invalid_argument("Unexpected character at end of symbol argument");
                            
                            index = arg.substr(indexBegin + 1, arg.size() - indexBegin - 2);

                            if(index.empty())
                                throw std::invalid_argument("Symbol index empty");

                            indexSecondary = easyParse::convertNumberString<std::size_t>(index);
                        }
                    }
                
                    instructionToken_.symbolArgs.push_back({i - 1, {symbolName, indexPrimary, indexSecondary}});
                }
            }
        }

        inline void validateSymbolName_()
        {
            bool invalidBegin = easyMath::valueBetweenInclusive(symbolToken_.symbolName[0], '0', '9')
                || (symbolToken_.symbolName[0] == '@');

            if(invalidBegin)
                throw std::invalid_argument("Invalid symbol Name (invalid begin character)");

            for(auto i : symbolToken_.symbolName)
            {
                bool valid = easyMath::valueBetweenInclusive(i, 'a', 'z')
                    || easyMath::valueBetweenInclusive(i, 'A', 'Z')
                    || easyMath::valueBetweenInclusive(i, '0', '9')
                    || (i == '_')
                    || (i == '@');
                if(!valid)
                    throw std::invalid_argument("Invalid symbol Name");
            }
        }

        inline void tokenizeSymbol_()
        {

            symbolToken_.symbolName = easyParse::stripWhiteSpace(
                strippedLineUnderEval_.substr(
                    0, 
                    cursor_
                )
            );


            validateSymbolName_();

            advanceCursor(1);

            symbolToken_.isExport = easyParse::isExactSubstr(strippedLineUnderEval_, literal::EXPORT_SWITCH, cursor_);

            if(symbolToken_.isExport)
                advanceCursor(literal::EXPORT_SWITCH.size());

            if(invalidCursor())
            {
                symbolToken_.symbolType = SymbolType::JUMP;
                return;
            }

            if(easyParse::isExactSubstr(strippedLineUnderEval_, literal::DATA_SWITCH, cursor_))
            {
                symbolToken_.symbolType = SymbolType::DATA;
                advanceCursor(literal::DATA_SWITCH.size());
            } 
            else if(easyParse::isExactSubstr(strippedLineUnderEval_, literal::CONST_SWITCH, cursor_))
            {
                symbolToken_.symbolType = SymbolType::CONST;
                advanceCursor(literal::CONST_SWITCH.size());
            }
            else
                throw std::invalid_argument("Symbol which is not jump, must have .data or .const switches");
            

            if(invalidCursor())
                throw std::invalid_argument("Data or Const symbols must have size switch, element count and (if const) intial values");

            if(easyParse::isExactSubstr(strippedLineUnderEval_, literal::ASCII_SWITCH, cursor_))
            {
                symbolToken_.blockSizeCode = literal::ASCII_DATA;
                advanceCursor(literal::ASCII_SWITCH.size());
            }
            else if(strippedLineUnderEval_[cursor_] == '.')
            {
                auto end = strippedLineUnderEval_.find_first_of(easyParse::WHITESPACE_STRING, cursor_);
                auto sizeString = easyParse::stripWhiteSpace(strippedLineUnderEval_.substr(
                    cursor_, 
                    (end == strippedLineUnderEval_.npos)? 
                        strippedLineUnderEval_.npos 
                        : (end - cursor_)
                ));

                symbolToken_.blockSizeCode = traitObj_.resolveSize(sizeString);
                cursor_ = end;
                advanceCursor(0);
            }
            else
                throw std::invalid_argument("Data or Const symbols must have size switch, which begins with \'.\'");

            if(symbolToken_.blockSizeCode != literal::ASCII_DATA)
            {
                if(invalidCursor())
                    throw std::invalid_argument("Non Ascii Data or const types must have element count inside \'[]\'");

                if(strippedLineUnderEval_[cursor_] != '[')
                    throw std::invalid_argument("Non Ascii Data or const types must have element count inside \'[]\'");

                advanceCursor(1);
                auto end = strippedLineUnderEval_.find(']', cursor_);
                
                if(end == strippedLineUnderEval_.npos)  
                    throw std::invalid_argument("Expected \']\'");
                
                auto elementCount = easyParse::convertNumberString<std::size_t>(
                    strippedLineUnderEval_.substr(cursor_, end - cursor_)
                );

                cursor_ = end + 1;
                symbolToken_.init_value.resize(elementCount);
                advanceCursor(0);
            }

            if(invalidCursor())
            {
                if(symbolToken_.symbolType == SymbolType::CONST)
                    throw std::invalid_argument("Const symbol requires initial values");
                
                if(symbolToken_.blockSizeCode == literal::ASCII_DATA)
                    throw std::invalid_argument("ASCII data requires initial value");

                return;
            }

            if(symbolToken_.blockSizeCode != literal::ASCII_DATA)
            {
                auto splitData = easyParse::splitUsingDelimiterList(strippedLineUnderEval_.substr(cursor_), {','});


                auto min = easyMath::min({symbolToken_.init_value.size(), splitData.size()});

                for(std::size_t i = 0; i < min; ++i)
                    symbolToken_.init_value[i] 
                        = easyParse::convertNumberString<typename IsaTraits::LargestType>(
                            easyParse::stripWhiteSpace(splitData[i])
                        );
                
                for(std::size_t i = min; i < symbolToken_.init_value.size(); ++i)
                    symbolToken_.init_value[i] = 0;
            }
            else
            {
                if((strippedLineUnderEval_[cursor_] != '\"') || (strippedLineUnderEval_.back() != '\"'))
                    throw std::invalid_argument("ASCII data requires initital value within \'\"\'");
                
                auto text = strippedLineUnderEval_.substr(cursor_ + 1, strippedLineUnderEval_.size() - cursor_ - 2);

                if(!text.empty())
                {
                    symbolToken_.init_value.reserve(text.size() + 1);

                    auto iterator = text.begin();
                    auto sentinal = text.end();

                    while(iterator < sentinal)
                        symbolToken_.init_value.push_back(easyParse::advanceOverText(iterator, sentinal).first);
                }

                symbolToken_.init_value.push_back('\0');
            }
        }

    public:
        
        template<class... Args>
        inline Tokenizer(Args&&... args) : traitObj_(std::forward<Args&&>(args)...) {}

        inline void tokenize(std::string_view line, bool shouldTokenizeSymbol = true)
        {
            instructionToken_ = {};
            symbolToken_ = {};

            strippedLineUnderEval_ = easyParse::stripCommentsAndWhiteSpace(line, ';');
            
            if(!isBlank())
            {
                evaluateIsSymbol_();
                if(isInstruction())
                    tokenizeInstruction_();
                else if(isSymbol() && shouldTokenizeSymbol)
                    tokenizeSymbol_();
            }
        }

        inline bool isBlank() const noexcept { return strippedLineUnderEval_.empty(); }
        inline bool isSymbol() const noexcept { return (!isBlank()) && isSymbol_; }
        inline bool isInstruction() const noexcept { return (!isBlank()) && (!isSymbol_); }

        inline operator bool() const noexcept { return isBlank(); }

        inline const SymbolToken<IsaTraits>& getSymbol() const noexcept { return symbolToken_; }
        inline const InstructionToken<IsaTraits>& getInstruction() const noexcept { return instructionToken_; }

    };


}


#endif // INCLUDE_GENASMLIB_TOKENISER_H_INCLUDED

