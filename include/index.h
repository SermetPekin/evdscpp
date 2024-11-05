
/*
* evdscpp: An open-source data wrapper for accessing the EVDS API.
 * Author: Sermet Pekin
 * 
 * MIT License
 * 
 * Copyright (c) 2024 Sermet Pekin
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <set>
#include <cassert>
#include "types.h"

namespace evds
{

    std::vector<std::string> splitString(const std::string &str, const std::set<char> &delimiters)
    {
        std::vector<std::string> tokens;
        std::string token;

        for (char ch : str)
        {
            if (delimiters.find(ch) != delimiters.end())
            {
                if (!token.empty())
                {
                    tokens.push_back(token);
                    token.clear();
                }
            }
            else
            {
                token += ch;
            }
        }

        // Add the last token if any
        if (!token.empty())
        {
            tokens.push_back(token);
        }

        return tokens;
    }

    // Example class for extending functionality
    class Index
    {
    private:
        string_v index_v;

    public:
        Index(const std::string &tmpl)
        {
            index_v = fromTemplate(tmpl);
        }
        Index(const string_v &tmpl)
        {
            index_v = fromVector(tmpl);
        }

        std::string get()
        {

            std::string result = join(index_v, Index_delimiter);
            return result;
        }

        string_v get_v()
        {
            return index_v;
        }

        string_v fromTemplate(const std::string &tmpl)
        {
            string_v v;

            if (tmpl.starts_with("bie_"))
            {
                v.push_back(tmpl);
                return v;
            }

            std::set<char> delimiters = {'-', '\n'};

            v = splitString(tmpl, delimiters);

            return v;
        }

        string_v fromVector(const string_v &tmpl)
        {
             

            return tmpl;
        }
    };

}