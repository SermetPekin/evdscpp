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
#include <string>
#include <vector>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <iterator>

#pragma once

namespace evds
{

    using string_v = std::vector<std::string>;

    static const std::string Index_delimiter = "-";

    static const std::string domain("https://evds2.tcmb.gov.tr/");

    std::vector<std::string> splitString(const std::string &str, const std::string &delimiters)
    {
        /*
        splitString("ABC-DEF,GHI" , ",-")

        */
        std::regex re("[" + delimiters + "]");

        std::vector<std::string> result{
            std::sregex_token_iterator(str.begin(), str.end(), re, -1),
            std::sregex_token_iterator()};

        result.erase(std::remove_if(result.begin(), result.end(),
                                    [](const std::string &s)
                                    { return s.empty(); }),
                     result.end());

        return result;
    }

    std::vector<std::string> splitString(const std::string &str, char delimiter)
    {
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string item;

        while (std::getline(ss, item, delimiter))
        {
            result.push_back(item);
        }

        return result;
    }

    // .................................................................................... join

    std::string join(const std::vector<std::string> &elements, const std::string &delimiter)
    {
        std::ostringstream oss;
        auto it = elements.begin();

        if (it != elements.end())
        {
            oss << *it++; // Write the first element
        }

        while (it != elements.end())
        {
            oss << delimiter;
            oss << *it++;
        }

        return oss.str();
    }

    // .................................................................................... Config
    struct Config
    {

        std::vector<std::string> indexes;
        bool verbose = false;
        std::string start_date = "01-01-2000";
        std::string end_date = "31-12-2100";

        bool test = false;
        std::string frequency = "default";   // daily | monthly | weekly | annually | semimonthly | semiannually | business
        std::string formulas = "default";    // | level | percentage_change | difference |  year_to_year_percent_change | year_to_year_differences |
        std::string aggregation = "default"; //  | avg      |min    | max    | first    | last    |    sum
        bool cache = true;

        bool auto_confirm = true;
    };

}