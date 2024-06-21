/**
 * @file fileReader.h 
 * @author Harith Manoj (harithpub@gmail.com)
 * @brief Read files.
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


#include <fstream>
#include <string>
#include <algorithm>
#include <array>
#include <string_view>
#include <utility>
#include <filesystem>

#include <easyMathLib/easyMath.h>

namespace gen_asm
{

    template<std::size_t BUFF_SIZE = 100>
    class FileReader
    {
        std::array<std::string, BUFF_SIZE> buffer_;
        
        std::size_t readEnd_;
        std::size_t cursor_;
        std::size_t lineCount_;
        std::string fileName_;

        std::ifstream reader_;


    public:
        inline FileReader() 
            : fileName_(), reader_(), readEnd_(0), cursor_(0), lineCount_(0) {}

        inline void bufferFill()
        {
            if(cursor_ == readEnd_)
            {
                cursor_ = 0;
                readEnd_ = 0;
            }
            else
            {
                for(std::size_t i = 0; i < (readEnd_ - cursor_); ++i)
                    buffer_[i] = buffer_[cursor_ + i];
                
                readEnd_ = (readEnd_ - cursor_);
                cursor_ = 0;
            }

            for(std::size_t i = readEnd_; i < BUFF_SIZE; ++i)
            {
                std::getline(reader_, buffer_[i]);
                
                if(reader_)
                    ++readEnd_;
                else 
                    break;
            }
        }

        inline std::string read()
        {
            std::string ret;

            if(cursor_ >= readEnd_)
                bufferFill();

            ret.reserve(buffer_.at(cursor_).size());
            for(auto ch : buffer_[cursor_])
                if(easyMath::valueBetweenInclusive<char>(ch, 'A', 'Z'))
                    ret.push_back((ch - 'A') + 'a');
                else
                    ret.push_back(ch);

            ++cursor_;
            ++lineCount_;
            return ret;
        }

        inline bool eof()     const noexcept  { return reader_.eof() && (cursor_ >= readEnd_); }
        inline bool good()    const noexcept  { return reader_.good(); }
        inline bool fail()    const noexcept  { return reader_.fail(); }
        inline bool bad()     const noexcept  { return reader_.bad(); }
        inline auto rdState() const noexcept  { return reader_.rdstate(); }

        std::pair<const std::string&, std::size_t> getId() { return {fileName_, lineCount_}; }

        void reload(std::string_view fileName)
        {
            if(!std::filesystem::is_regular_file(fileName))
                throw std::invalid_argument("Not a file");
                
            readEnd_ = 0;
            cursor_ = 0;
            lineCount_ = 0;
            fileName_ = fileName;
            if(reader_.is_open())
                reader_.close();
            reader_.open(fileName);
            if(!reader_)
                throw std::invalid_argument("unknown error");
            bufferFill();
        }

        inline void clearErrors() noexcept
        {
            reader_.clear();
        }
    };

}
