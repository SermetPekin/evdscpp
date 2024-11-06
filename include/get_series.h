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

#include <cstdio> // for sprintf
#include "header.h"
#include "json.h"
#include "series.h"
#include "dataframe.h"
#include "url_builder.h"
#include "get.h"
#include "shorten.h"

using namespace evds;

static inline bool TEST = false;

#include <type_traits>

template <typename T>
struct deduce_series_type;

template <>
struct deduce_series_type<double>
{
    using type = std::optional<double>;
};

template <>
struct deduce_series_type<long long>
{
    using type = std::optional<long long>;
};

template <>
struct deduce_series_type<std::string>
{
    using type = std::optional<std::string>;
};

DataFrame get_series(std::string &str, const Config &config = Config(), bool verbose = false)
{

    // str = normalizeDelimiters(str );

    evds::Index index(str);
    std::cout << "index1 : " << index.get() << "\n";

    evds::UrlBuilder urlBuilder(index, config);

    // std::string url = urlBuilder.setSeries(istr.get()).build();

    std::string url = urlBuilder.get_url();
    if (verbose)
        std::cout << "Generated URL: " << url << std::endl;

    DataFrame df;

    std::string res;

 
    res = getEvds(url, config);

    json parsed_json = json::parse(res);

    for (const auto &item : parsed_json["items"]) // TODO possible break for future changes
    {

        evds::parse_json_line(item, df);
    }

    return df;
}

std::vector<double> check_df(DataFrame &df, const std::string &col)
{

    Series series = df[col];

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "[produced series] %s", col.c_str());
    std::cout << buffer << std::endl;
    auto vals = series.values();
    return vals;
}
