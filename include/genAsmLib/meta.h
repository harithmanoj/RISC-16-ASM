/**
 * @file meta.h 
 * @author Harith Manoj (harithpub@gmail.com)
 * @brief Meta information holder.
 * @date 09 June 2024
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

#ifndef INCLUDE_GENASMLIB_META_H_INCLUDED

/// @brief include\genAsmLib\meta.h Header Guard 
#define INCLUDE_GENASMLIB_META_H_INCLUDED

#include <string_view>
#include <cstdint>

namespace gen_asm
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
}

#endif // INCLUDE_GENASMLIB_META_H_INCLUDED
