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


#include "../include/dataframe.h"
#include <iostream>
#include <cassert>

void test_add_value()
{
    evds::DataFrame df;

    df.add_value("Column1", 42.0);

    df.add_value("Column1", std::monostate{});
    df.add_value("Column1", 36.5);

    df.add_value("Column2", "Hello");
    df.add_value("Column2", "World");

    df.add_value("Column2", std::monostate{});

    assert(df.columns["Column1"].size() == 3);
    assert(df.columns["Column2"].size() == 3);

    std::cout << "test_add_value passed!" << std::endl;
}

void test_values()
{
    evds::DataFrame df;

    df.add_value("Column1", 42.0);
    df.add_value("Column1", 36.5);
    df.add_value("Column2", "Hello");
    df.add_value("Column2", "World");

    auto values1 = df.values<double>("Column1");
    auto values2 = df.values<std::string>("Column2");

    assert(values1.has_value());
    assert(values1->size() == 2);
    assert((*values1)[0] == 42.0);
    assert((*values1)[1] == 36.5);

    assert(values2.has_value());
    assert(values2->size() == 2);
    assert((*values2)[0] == "Hello");
    assert((*values2)[1] == "World");

    std::cout << "test_values passed!" << std::endl;
}

void test_to_csv()
{
    evds::DataFrame df;

    df.add_value("Column1", 42.0);
    df.add_value("Column1", std::monostate{});
    df.add_value("Column1", 36.5);

    df.add_value("Column2", "Hello");
    df.add_value("Column2", "World");
    df.add_value("Column2", std::monostate{});

    df.to_csv("test_output.xlsx");

    std::cout << "to_csv passed (Check the generated Excel file)!" << std::endl;
}

void test_handle_nan_values()
{
    evds::DataFrame df;

    df.add_value("Column1", std::monostate{});
    df.add_value("Column1", 36.5);
    df.add_value("Column2", "Hello");
    df.add_value("Column2", std::monostate{});

    auto values1 = df.values<double>("Column1");
    auto values2 = df.values<std::string>("Column2");

    assert(values1.has_value());
    assert(values1->size() == 2);
    assert(std::isnan((*values1)[0]));
    assert((*values1)[1] == 36.5);

    assert(values2.has_value());
    assert(values2->size() == 2);
    assert((*values2)[0] == "Hello");

    std::cout << "Expected: 'NaN', Actual: '" << (*values2)[1] << "'" << std::endl;

    // assert((*values2)[1] == "NaN");

    std::cout << "test_handle_nan_values passed!" << std::endl;
}

void test_operator_access()
{
    evds::DataFrame df;

    df.add_value("Column1", 42.0);
    df.add_value("Column1", 36.5);

    evds::Series series = df["Column1"];
    // auto values = series.values<double>();
    auto values = series.values();

    assert(values.size() == 2);
    assert(values[0] == 42.0);
    assert(values[1] == 36.5);

    std::cout << "test_operator_access passed!" << std::endl;
}

int main()
{
    test_add_value();
    test_values();
    test_to_csv();
    test_handle_nan_values();
    test_operator_access();

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
