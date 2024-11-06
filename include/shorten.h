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

#ifndef getShortFilenames_

#define getShortFilenames_

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>

#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

std::string normalizeDelimiters(const std::string &input)
{
    std::string result = input;

    std::replace(result.begin(), result.end(), '\n', '-');

    std::string normalized;
    bool lastWasHyphen = false;
    for (char c : result)
    {
        if (c == '-')
        {
            if (!lastWasHyphen)
            {
                normalized += c;
                lastWasHyphen = true;
            }
        }
        else
        {
            normalized += c;
            lastWasHyphen = false;
        }
    }

    // Remove trailing hyphen if present
    if (!normalized.empty() && normalized.back() == '-')
    {
        normalized.pop_back();
    }

    return normalized;
}

std::string capitalizeWords(const std::string &input)
{
    std::string result;
    bool capitalizeNext = true;

    for (char c : input)
    {
        if (std::isalnum(c))
        {
            if (capitalizeNext)
            {
                result += std::toupper(c);
                capitalizeNext = false;
            }
            else
            {
                result += std::tolower(c);
            }
        }
        else
        {
            capitalizeNext = true;
        }
    }
    return result;
}

std::string getShortFilename(const std::string &input)
{

    if (input.size() < 9 || input.find("bie_") != std::string::npos)
    {

        return input;
    }

    std::string cleaned;

    for (char c : input)
    {
        if (std::isalnum(c))
        {
            cleaned += c;
        }
        else if (c == '-' || c == '_' || c == '.')
        {
            cleaned += ' ';
        }
    }

    cleaned = capitalizeWords(cleaned);

    if (cleaned.length() > 15)
    {
        cleaned = cleaned.substr(0, 15);
    }

    return cleaned;
}

#endif // getShortFilenames_
