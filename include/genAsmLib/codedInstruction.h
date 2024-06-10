/**
 * @file codedInstruction.h 
 * @author Harith Manoj (harithpub@gmail.com)
 * @brief Coded Instruction Manager.
 * @date 28 May 2024
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


#include <bitset>
#include <array>
#include <ranges>

#include <easyMathLib/easyMath.h>

namespace gen_asm
{


    template<std::size_t widthMax>
    class CodedInstruction
    {
        std::bitset<widthMax> data_;
    public:

        struct FieldInfo
        {
            std::size_t offset;
            std::size_t size;
        };

        template<
            easyMath::UnsignedIntegral ValueType, 
            std::ranges::range FieldInfoContainer, 
            std::ranges::range DataContainer
        >
            requires std::ranges::sized_range<FieldInfoContainer> && std::ranges::sized_range<DataContainer>
                && std::same_as<FieldInfo, typename FieldInfoContainer::value_type>
                && std::same_as<ValueType, typename DataContainer::value_type>
        void load(
            const FieldInfoContainer& fieldInfo,
            const DataContainer& dataList
        )
        {
            if(fieldInfo.size() != dataList.size())
                throw std::invalid_argument("Not matching field Info and data");

            for(std::size_t i = 0; i < fieldInfo.size(); ++i)
            {
                const FieldInfo& info = fieldInfo[i];
                auto value = dataList[i];

                auto mask = easyMath::nBitMask<ValueType>(info.size);

                data_ &= ~(static_cast<std::bitset<widthMax>>(mask) << info.offset);
                data_ |= (static_cast<std::bitset<widthMax>>(value & mask) << info.offset);
            }
        }

        template<easyMath::UnsignedIntegral ValueType = std::uint64_t>
        ValueType access(std::size_t offset, std::size_t size)
        {
            return ((
                data_ & (easyMath::nBitMask<std::bitset<widthMax>>(size) << offset)
            ) >> offset).to_ullong();
            
        }

        std::bitset<widthMax>& data() noexcept { return data_; }
        
        const std::bitset<widthMax>& data() const noexcept { return data_; }
    };
}
