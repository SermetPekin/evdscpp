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

#include "e_utils.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include <sstream>
#include <algorithm>
#include <utility>
#include <functional>
#include "../extern/nlohmann/json.hpp"

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
    config.auto_confirm = true;     // if true it will not ask for confirmation before requesting which is convenient when many indexes are given.

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

    for (auto &CurrentIndex : poptions.indexes)
    {

        try
        {
            auto df = get_series(CurrentIndex, config);

            df_current = df;
            std::string f_name = getShortFilename(CurrentIndex);
            df.to_csv("data_" + f_name + ".csv", ',');
        }
        catch (const std::exception &ex)
        {
            std::cerr << "passing: " << CurrentIndex << ex.what() << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
