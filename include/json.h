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


#include "../extern/nlohmann/json.hpp"
#include "dataframe.h"
#include <iostream>
#include <sstream>
#include <regex>

using json = nlohmann::json;

namespace evds
{
    
    bool is_date_string(const std::string &str)
    {
        const std::regex full_date_pattern(R"(\b\d{2}-\d{2}-\d{4}\b)");
        const std::regex year_month_pattern(R"(\b\d{4}-\d{1,2}\b)");

        return std::regex_match(str, full_date_pattern) || std::regex_match(str, year_month_pattern);
    }

    
    std::string convert_year_month_to_date(const std::string &str)
    {
        const std::regex year_month_pattern(R"(\b(\d{4})-(\d{1,2})\b)");
        std::smatch match;

        if (std::regex_match(str, match, year_month_pattern))
        {
            std::string year = match[1].str();
            std::string month = match[2].str();

            if (month.size() == 1)
            {
                month = "0" + month;
            }

            return "01-" + month + "-" + year;
        }

        return str;  
    }

    void parse_json_line(const nlohmann::json &j, DataFrame &df)
    {
        for (auto &[key, value] : j.items())
        {
            if (key == "UNIXTIME")
            {
                continue;  
            }

            if (value.is_null())
            {
                df.add_value(key, std::monostate{});  
            }
            else if (value.is_string())
            {
                std::string str_value = value.get<std::string>();

              
                if (is_date_string(str_value))
                {
                    if (str_value.find('-') != std::string::npos && str_value.size() > 7)
                    {
                        df.add_value(key, str_value); 
                        // Already in 'dd-mm-yyyy' format
                    }
                    else
                    {
                        df.add_value(key, convert_year_month_to_date(str_value)); 
                        // Convert 'yyyy-m' or 'yyyy-mm' to '01-mm-yyyy'
                    }
                }
                else
                {
                    // Attempt  numeric type
                    try
                    {
                        if (str_value.find('.') != std::string::npos)
                        {
                            //  possibly numeric =>  double
                            double double_value = std::stod(str_value);
                            df.add_value(key, double_value);
                        }
                        else
                        {
                            // long long
                            long long long_value = std::stoll(str_value);
                            df.add_value(key, long_value);
                        }
                    }
                    catch (const std::invalid_argument &)
                    {
                         
                        df.add_value(key, str_value);
                    }
                    catch (const std::out_of_range &)
                    {
                        
                        df.add_value(key, str_value);
                    }
                }
            }
            else if (value.is_number_integer())
            {
                df.add_value(key, static_cast<long long>(value.get<long long>()));
            }
            else if (value.is_number_float())
            {
                df.add_value(key, static_cast<double>(value.get<double>()));
            }
        }
    }

    void parse_json_line(const std::string &line, DataFrame &df)
    {
        nlohmann::json j = nlohmann::json::parse(line);
        parse_json_line(j, df);
    }
}
