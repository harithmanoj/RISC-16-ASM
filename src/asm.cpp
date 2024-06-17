/**
 * @file asm.cpp 
 * @author Harith Manoj (harithpub@gmail.com)
 * @brief Translation unit for all assembly code.
 * @date 12 June 2024
 * 
 * @copyright Copyright (C) 2024
 * 
 * 
 *                      GNU GENERAL PUBLIC LICENSE 
 *                        Version 3, 29 June 2007 
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version. 
 *  
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 *  
 * You should have received a copy of the GNU General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#include <fstream>
#include <iostream>


#include <genAsmLib/tokeniser.h>
#include <genAsmLib/addressResolver.h>
#include <genAsmLib/symbolTable.h>
#include <genAsmLib/fileReader.h>


namespace risc16
{
    struct AssemblerTraits
    {
        using BasicType = std::uint16_t;
        using LargestType = std::uint64_t;
        using WordType = std::uint16_t;
        using AddressType = WordType;
        using BlockSizeType = easyMath::MaxCapableUint<4>;
        using RegisterCodeType = easyMath::MaxCapableUint<8>;
        using ModifierCodeType = easyMath::MaxCapableUint<0>;
        using OpCodeType = easyMath::MaxCapableUint<8 + 5>;
        using TranslationId = std::size_t;

        constexpr static std::array<std::string_view, 4> sizeTypes = {
            ".word",
            ".dword",
            ".qword"
        };

        inline static BlockSizeType resolveSize(std::string_view str)
        {
            auto ret = std::ranges::find(sizeTypes, str);
            if(ret != sizeTypes.end())
                return std::ranges::distance(sizeTypes.begin(), ret) + 2;
            
            throw std::invalid_argument("Invalid Size Type");
        }

        constexpr static std::array<std::string_view, 8> regNames = {
            "bp", "sp", "ra", "fa1", "fa2"
        };

        inline static RegisterCodeType resolveRegister(std::string_view str)
        {
            auto ret = std::ranges::find(regNames, str);
            if(ret != regNames.end())
                return std::ranges::distance(regNames.begin(), ret) + 1;

            if(str[0] == 'r')
            {
                str = str.substr(1);
                if(easyParse::validateDecString(str))
                    return easyParse::convertDecimalString<RegisterCodeType>(str);
                else
                    throw std::invalid_argument("Invalid register name");
            }
            else if(easyParse::validateNumberString(str))
                return easyParse::convertNumberString<RegisterCodeType>(str);
            else
                throw std::invalid_argument("Invalid register name");            
        }

        constexpr inline static ModifierCodeType resolveModifier(std::string_view) noexcept { return {}; }

        constexpr inline static bool checkIfModifier(std::string_view) noexcept { return false; }

        static constexpr std::array<std::string_view, 8 + 5> instrList = {
            "add", "addi", "nand", "lui", "lw", "sw", "beq", "jalr",
            "movi", "push", "pop", "call", "ret"
        };

        inline static std::string_view instrString(OpCodeType val)    
        {
            return instrList[val];
        }

        inline static OpCodeType resolveOpCode(std::string_view str)
        {
            for(std::size_t i = 0; i < instrList.size(); ++i)
                if(str == instrList[i])
                    return i;
                
            throw std::invalid_argument("Invalid instruction");
        }

        inline static std::size_t getSizeInBasic(BlockSizeType sz) noexcept
        {
            if(sz == gen_asm::literal::NO_DATA)
                return 0;
            else if(sz == gen_asm::literal::ASCII_DATA)
                return 1;
            else if(sz == 2)
                return 1;
            else if(sz == 3)
                return 2;
            else if(sz == 4)
                return 4;
            else 
                return 0;
        }

        inline static std::size_t getInstrWidthInBasic(OpCodeType op) noexcept
        {
            return 1;
        }

    };
}


