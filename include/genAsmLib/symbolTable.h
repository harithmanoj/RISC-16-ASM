/**
 * @file symbolTable.h 
 * @author Harith Manoj (harithpub@gmail.com)
 * @brief Symbol table declaration.
 * @date 26 May 2024
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

#ifndef INCLUDE_GENASMLIB_SYMBOLTABLE_H_INCLUDED

/// @brief include\genAsmLib\symbolTable.h Header Guard 
#define INCLUDE_GENASMLIB_SYMBOLTABLE_H_INCLUDED

#include <variant>

#include "tokeniser.h"


namespace gen_asm
{
    
    template<class Traits, class IsaTraits>
    concept SymbolTraitModel = requires(
        const Traits& trait,
        const IsaTraits::BlockSizeType& sizeType
    )
    {
        
        typename Traits::TranslationId;

        requires easyMath::Integral<typename Traits::TranslationId>;

        requires IsaTraitModel<IsaTraits>;

        { trait.getSizeInBasic(sizeType) } -> std::same_as<std::size_t>;
        noexcept(trait.getSizeInBasic(sizeType));
    };

    namespace impl_detail_
    {
        template<IsaTraitModel IsaTraits, SymbolTraitModel<IsaTraits> SymbolTraits>
        struct BasicSymbol_
        {
            typename SymbolTraits::TranslationId translationUnitId;
            std::string symbolName;
            bool isExport;
        };

    }


    template<IsaTraitModel IsaTraits, SymbolTraitModel<IsaTraits> SymbolTraits>
    struct JumpSymbol : public impl_detail_::BasicSymbol_<IsaTraits, SymbolTraits>
    {
        typename IsaTraits::AddressType codeAddressOffset;
    };

    template<IsaTraitModel IsaTraits, SymbolTraitModel<IsaTraits> SymbolTraits>
    struct DataSymbol : public impl_detail_::BasicSymbol_<IsaTraits, SymbolTraits>
    {
        typename IsaTraits::AddressType dataAddressOffset;
        typename IsaTraits::BlockSizeType sizeType;
        std::size_t elementCount;
    };

    template<IsaTraitModel IsaTraits, SymbolTraitModel<IsaTraits> SymbolTraits>
    struct ConstSymbol : public impl_detail_::BasicSymbol_<IsaTraits, SymbolTraits>
    {
        typename IsaTraits::BlockSizeType sizeType;
        std::vector<typename IsaTraits::LargestType> init_value;
    };

    template<class Resolver, class IsaTraits>
    concept AddressResolverModel = requires(
        const Resolver& cres, Resolver& res,
        const SymbolToken<IsaTraits>& symbol,
        const InstructionToken<IsaTraits>& instr
    )
    {
        requires IsaTraitModel<IsaTraits>;

        { cres.getCodeAddressOffset() } -> std::same_as<typename IsaTraits::AddressType>;
        { cres.getDataAddressOffset() } -> std::same_as<typename IsaTraits::AddressType>;
        { res.updateOffsets(symbol) };
        { res.updateOffsets(instr) };
    };

    template<
        IsaTraitModel IsaTraits, 
        SymbolTraitModel<IsaTraits> SymbolTraits, 
        AddressResolverModel<IsaTraits> AddressResolver
    >
    class SymbolTable
    {
        AddressResolver& addressResolver_;
        SymbolTraits traitObj_;

        using Var_t = std::variant<
            JumpSymbol<SymbolTraits, IsaTraits>, 
            DataSymbol<SymbolTraits, IsaTraits>, 
            ConstSymbol<SymbolTraits, IsaTraits>
        >;

        using SymbolList = std::vector<Var_t>;

        SymbolList symbols_;

        std::size_t codeBaseAddress_;
        std::size_t dataBaseAddress_;

        const impl_detail_::BasicSymbol_<IsaTraits, SymbolTraits>& getLabelData(const Var_t& var) const
        {
            return std::visit([](auto&& arg) -> const impl_detail_::BasicSymbol_<IsaTraits, SymbolTraits>& {return arg;}, var);
        }

        auto findSymbol_(std::string name, typename SymbolTraits::TranslationId id) const
        {
            for(auto i = symbols_.begin(); i < symbols_.end(); ++i)
            {
                const auto& labelData = getLabelData(*i);

                if(labelData.symbolName == name)
                {
                    if((labelData.translationUnitId == id) || labelData.isExport)
                        return i;
                }
            }

            return symbols_.end();
        }

        void findIfSymbol_(const impl_detail_::BasicSymbol_<IsaTraits, SymbolTraits>& check) const
        {
            for(const auto& label : symbols_)
            {
                const auto& labelData = getLabelData(label);

                if(labelData.symbolName == check.symbolName)
                {
                    if(labelData.translationUnitId == check.translationUnitId)
                        throw std::domain_error("Symbol name already exists in same translation unit");
                    
                    if(labelData.isExport || check.isExport)
                        throw std::domain_error("Symbol name already exists, (either the existing symbol or new symbol is exported)");
                }
            }
        }
    
        void addJumpSymbol_(typename SymbolTraits::TranslationId id, const SymbolToken<IsaTraits>& symbol)
        {
            JumpSymbol<SymbolTraits, IsaTraits> entry = {};

            entry.symbolName = symbol.symbolName;
            entry.translationUnitId = id;
            entry.isExport = symbol.isExport;
            entry.codeAddressOffset = addressResolver_.getCodeAddressOffset();

            findIfSymbol_(entry);
            
            symbols_.emplace_back(entry);
        }

        void addDataSymbol_(typename SymbolTraits::TranslationId id, const SymbolToken<IsaTraits>& symbol)
        {
            DataSymbol<SymbolTraits, IsaTraits> entry = {};

            entry.symbolName = symbol.symbolName;
            entry.translationUnitId = id;
            entry.isExport = symbol.isExport;
            entry.sizeType = symbol.blockSizeCode;
            entry.elementCount = symbol.init_value.size();
            entry.dataAddressOffset = addressResolver_.getDataAddressOffset();

            findIfSymbol_(entry);

            addressResolver_.updateOffsets(symbol);
            symbols_.emplace_back(entry);
        }

        void addConstSymbol_(typename SymbolTraits::TranslationId id, const SymbolToken<IsaTraits>& symbol)
        {
            ConstSymbol<SymbolTraits, IsaTraits> entry = {};

            entry.symbolName = symbol.symbolName;
            entry.translationUnitId = id;
            entry.isExport = symbol.isExport;
            entry.sizeType = symbol.blockSizeCode;
            entry.init_value = symbol.init_value;

            findIfSymbol_(entry);

            symbols_.emplace_back(entry);
        }

    public:

        template<class... Args>
        inline SymbolTable(AddressResolver& addr, Args&&... args) 
            : addressResolver_(addr), traitObj_(std::forward<Args&&>(args)...), symbols_({}), codeBaseAddress_(0), dataBaseAddress_(0) {}
        
        inline auto begin() const { return symbols_.begin(); }
        inline auto cbegin() const { return symbols_.cbegin(); }

        inline auto end() const { return symbols_.end(); }
        inline auto cend() const { return symbols_.cend(); }

        inline std::size_t size() const noexcept { return symbols_.size(); }
        inline const auto& operator [] (std::size_t i) const { return symbols_[i]; }

        inline void addSymbol(
            typename SymbolTraits::TranslationId id, 
            const SymbolToken<IsaTraits>& symbol
        )
        {
            switch (symbol.symbolType)
            {
            case SymbolType::JUMP:
                addJumpSymbol_(id, symbol);
                break;
            case SymbolType::DATA:
                addDataSymbol_(id, symbol);
                break;
            case SymbolType::CONST:
                addConstSymbol_(id, symbol);
                break;
            }
        }

        inline void setBaseAddress(std::size_t code, std::size_t data) noexcept
        {
            codeBaseAddress_ = code;
            dataBaseAddress_ = data;
        }

        inline std::pair<std::size_t, std::size_t> getBaseAddress() const noexcept { return {codeBaseAddress_, dataBaseAddress_}; }

        inline typename IsaTraits::LargestType resolveSymbol(
            typename SymbolTraits::TranslationId id, 
            const std::tuple<std::string, std::size_t, std::size_t>& data
        )
        {
            auto iter = findSymbol_(std::get<0>(data), id);
            
            if(iter == symbols_.end())
                throw std::invalid_argument("unidentified symbol");

            return std::visit(
                [&](auto&& arg) -> typename IsaTraits::LargestType
                {
                    if constexpr(std::same_as<JumpSymbol<SymbolTraits, IsaTraits>, std::decay_t<decltype(arg)>>)
                    {
                        if((std::get<1>(data) != 0) || (std::get<2>(data) != 0))
                            throw std::invalid_argument("Jump symbols may not have non-zero subscripts");
                        else
                            return arg.codeAddressOffset;
                    }
                    else if constexpr(std::same_as<DataSymbol<SymbolTraits, IsaTraits>, std::decay_t<decltype(arg)>>)
                    {
                        if(std::get<1>(data) < arg.elementCount)
                        {
                            auto size = traitObj_.getSizeInBasic(arg.sizeType);
                            if(std::get<2>(data) < size)
                            {
                                return dataBaseAddress_ + arg.dataAddressOffset + (size * std::get<1>(data)) + std::get<2>(data);
                            }
                            else
                                throw std::out_of_range("Index out of range for splitting element");
                        }
                        else
                            throw std::out_of_range("Index out of range of array");
                    }
                    else if constexpr(std::same_as<ConstSymbol<SymbolTraits, IsaTraits>, std::decay_t<decltype(arg)>>)
                    {
                        if(std::get<1>(data) < arg.init_value.size())
                        {
                            auto size = traitObj_.getSizeInBasic(arg.sizeType);
                            if(std::get<2>(data) < size)
                            {
                                return arg.init_value[std::get<1>(data)] >> (size * std::get<2>(data));
                            }
                            else
                                throw std::out_of_range("Index out of range for splitting element");
                        }
                        else
                            throw std::out_of_range("Index out of range of array");
                    }

                }, *iter
            );
        }
    };
}


#endif // INCLUDE_GENASMLIB_SYMBOLTABLE_H_INCLUDED


