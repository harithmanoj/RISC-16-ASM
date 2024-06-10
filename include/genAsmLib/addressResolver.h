/**
 * @file addressResolver.h 
 * @author Harith Manoj (harithpub@gmail.com)
 * @brief Address Resolver class.
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


#ifndef INCLUDE_GENASMLIB_ADDRESSRESOLVER_H_INCLUDED

/// @brief include\genAsmLib\addressResolver.h Header Guard 
#define INCLUDE_GENASMLIB_ADDRESSRESOLVER_H_INCLUDED


#include "tokeniser.h"

namespace gen_asm
{
    template<class AddrTraits, class IsaTraits>
    concept AddressResolverTraitModel = requires(
        const AddrTraits& trait,
        const typename IsaTraits::BlockSizeType& sz,
        const typename IsaTraits::OpCodeType& op
    )
    {
        requires IsaTraitModel<IsaTraits>;

        { trait.getSizeInBasic(sz) } -> std::same_as<std::size_t>;
        noexcept(trait.getSizeInBasic(sz));

        { trait.getInstrWidthInBasic(op) } -> std::same_as<std::size_t>;
        noexcept(trait.getInstrWidthInBasic(op));
    };

    template<IsaTraitModel IsaTraits, AddressResolverTraitModel<IsaTraits> AddrTraits>
    class AddressResolver
    {
        AddrTraits traitObj_;

        typename IsaTraits::AddressType codeAddressOffset_;
        typename IsaTraits::AddressType dataAddressOffset_;

    public:
        
        template<class... Args>
        inline AddressResolver(Args&&... args) noexcept 
            : traitObj_(std::forward<Args&&>(args)...), 
            codeAddressOffset_(0), dataAddressOffset_(0) {}

        inline typename IsaTraits::AddressType getCodeAddressOffset() const noexcept 
        {
            return codeAddressOffset_;
        }

        inline typename IsaTraits::AddressType getDataAddressOffset() const noexcept 
        {
            return dataAddressOffset_;
        }

        inline void updateOffsets(const SymbolToken<IsaTraits>& symbol)
        {
            if(symbol.symbolType == SymbolType::DATA)
                dataAddressOffset_ += 
                    traitObj_.getSizeInBasic(symbol.blockSizeCode) 
                    * symbol.init_value.size();
        }

        inline void updateOffsets(const InstructionToken<IsaTraits>& instr)
        {
            codeAddressOffset_ += traitObj_.getInstrWidthInBasic(instr.opCode);
        }
    };

}


#endif // INCLUDE_GENASMLIB_ADDRESSRESOLVER_H_INCLUDED

