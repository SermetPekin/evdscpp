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


#include "get_series.h"
#include "dotenv_.h"
#include "shorten.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include <sstream>
#include <algorithm>
#include <utility>

#include "../extern/nlohmann/json.hpp"

struct ParseArgsOptions
{
    int argc;
    char **argv;
    std::vector<std::string> &indexes;
};

std::unordered_map<std::string, std::string> parseArgs(ParseArgsOptions &options)
{
    std::unordered_map<std::string, std::string> args;

    bool namedArgsFound = false;

    for (int i = 1; i < options.argc; ++i)
    {
        std::string arg = options.argv[i];

        if (arg.rfind("--", 0) == 0)
        {
            namedArgsFound = true;
            std::string key = arg.substr(2);
            if (i + 1 < options.argc)
            {
                args[key] = options.argv[++i];
            }
            else
            {
                args[key] = "true";
            }
        }
        else if (!namedArgsFound)
        {
            std::istringstream iss(arg);
            std::string index;
            while (std::getline(iss, index, ','))
            {
                options.indexes.push_back(index);
            }
        }
    }

    return args;
}

#include <unordered_map>
#include <string>
#include <functional>

void setConfigOptions(const std::unordered_map<std::string, std::string> &args, Config &config)
{
    std::unordered_map<std::string, std::function<void(const std::string &)>> configSetters = {
        {"cache", [&](const std::string &val)
         { config.cache = (val == "true"); }},
        {"test", [&](const std::string &val)
         { config.test = (val == "true"); }},
        {"verbose", [&](const std::string &val)
         { config.verbose = (val == "true"); }},

        {"start_date", [&](const std::string &val)
         { config.start_date = val; }},
        {"end_date", [&](const std::string &val)
         { config.end_date = val; }},
        {"frequency", [&](const std::string &val)
         { config.frequency = val; }},
        {"formulas", [&](const std::string &val)
         { config.formulas = val; }},
        {"aggregation", [&](const std::string &val)
         { config.aggregation = val; }}};

    for (const auto &arg : args)
    {
        if (configSetters.count(arg.first))
        {
            configSetters[arg.first](arg.second);
        }
    }
}

#include <iostream>

void show_usage()
{
    std::cout << "Usage:\n";
    std::cout << "  evdscpp <indexes> [options]\n\n";
    std::cout << "Description:\n";
    std::cout << "  evdscpp is a command-line tool for requesting data from the EVDS API.\n\n";

    std::cout << "Arguments:\n";
    std::cout << "  <indexes>                 Comma-separated list of indexes to request.\n";
    std::cout << "                            Example: TP.DK.USD.A,TP.DK.EUR.A\n";
    std::cout << "                            Use hyphen (-) to group indexes in a single file.\n\n";

    std::cout << "Options:\n";
    std::cout << "  --start_date <date>       Specify the start date (format: DD-MM-YYYY).\n";
    std::cout << "                            Example: --start_date 01-01-2021\n";
    std::cout << "  --end_date <date>         Specify the end date (format: DD-MM-YYYY).\n";
    std::cout << "                            Example: --end_date 31-12-2021\n";
    std::cout << "  --cache <true|false>      Enable or disable caching.\n";
    std::cout << "                            Example: --cache true\n";
    std::cout << "  --frequency <frequency>   Set the frequency (e.g., daily, monthly, annual).\n";
    std::cout << "                            Example: --frequency monthly\n";
    std::cout << "  --formulas <formulas>     Set the formulas (e.g., avg, sum).\n";
    std::cout << "                            Example: --formulas avg\n";
    std::cout << "  --aggregation <type>      Set the aggregation type.\n";
    std::cout << "                            Example: --aggregation avg\n\n";

    std::cout << "Examples:\n";
    std::cout << "  # 1. Each index will have its own file:\n";
    std::cout << "  ./evdscpp TP.DK.USD.A,TP.DK.EUR.A\n\n";

    std::cout << "  # 2. Two indexes in the same file:\n";
    std::cout << "  ./evdscpp TP.DK.USD.A-TP.DK.EUR.A\n\n";

    std::cout << "  # 3. Using indexes with named arguments:\n";
    std::cout << "  ./evdscpp TP.DK.USD.A,TP.DK.EUR.A --start_date 01-01-2021 --end_date 31-12-2021 --cache true\n\n";

    std::cout << "  # Another example with a different index:\n";
    std::cout << "  ./evdscpp bie_yssk --cache true\n\n";

    std::cout << "  # Another example with three different indexes:\n";
    std::cout << "  # This will create two different files. One for `bie_yssk` and one for `TP.DK.USD.A-TP.DK.EUR.A`\n";
    std::cout << "  ./evdscpp bie_yssk,TP.DK.USD.A-TP.DK.EUR.A --cache true\n\n";

    std::cout << "Notes:\n";
    std::cout << "  - The <indexes> argument is required.\n";
    std::cout << "  - Options are optional but provide more control over the query.\n";
    std::cout << "  - examples and code on https://github.com/SermetPekin/evdscpp\n";
}

int main(int argc, char *argv[])
{
    
    evds::Config config;
    config.cache = false;
    config.start_date = "01-01-2020";
    config.end_date = "31-12-2025";
    config.test = false;
    config.verbose = false;
    config.frequency = "default";   // | monthly | weekly | annually | semimonthly | semiannually | business
    config.formulas = "default";    // | level | percentage_change | difference |  year_to_year_percent_change | year_to_year_differences |
    config.aggregation = "default"; //  | avg      |min    | max    | first    | last    |    sum
 
    std::vector<std::string> indexes;
    ParseArgsOptions poptions{argc, argv, indexes}; // Pass indexes by reference

    auto args = parseArgs(poptions);
    config.indexes = poptions.indexes;
  

   

    setConfigOptions(args, config);

    if (poptions.indexes.empty())
    {
        std::cerr << "No indexes provided." << std::endl;

        show_usage();

        return EXIT_SUCCESS;
    }

    DataFrame df_current;

    for (auto &x : poptions.indexes)
    {

        try
        {
            auto df = get_series(x, config);

            df_current = df;
            std::string f_name = getShortFilename(x);
            df.to_csv("data_" + f_name + ".csv", ',');
        }
        catch (const std::exception &ex)
        {
            std::cerr << "passing: " << ex.what() << std::endl;
             
        }
    }

    return EXIT_SUCCESS;
}
