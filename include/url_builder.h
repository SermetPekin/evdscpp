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
#include <iostream>
#include <unordered_map>
#include <string>
#include "index.h"

namespace evds
{

    class UrlBuilder
    {
    public:
        /*
        see get_url
        --------------
        Both series and datagroup was implemented
        get_url function will decide which function to call


        */

        UrlBuilder(const Index &ind, const Config &config = Config()) : config_(config), index_v(ind)
        {

            series_ = index_v.get();
        }

        std::string get_url()
        {
            auto v = index_v.get_v();

            if (v.size() == 1 && v[0].find("bie_") != std::string::npos)
                return build_dg();
            return build();
        }
        //   series for the URL
        UrlBuilder &setSeries(const std::string &series)
        {
            series_ = series;
            return *this;
        }

        std::string convert_series() const
        {

            auto s = evds::splitString(series_, ",-");
            return evds::join(s, "-");
        }
        std::string multiply(const std::string &val) const
        {

            std::vector<std::string> v;

            for (const auto &a : evds::splitString(series_, ",-")) // config_.indexes
            {
                v.push_back(val);
            }

            std::string str = evds::join(v, "-");

            return str;
        }

        std::string aggregation_url() const
        {
            if (config_.frequency == "default")
            {
                return "";
            }

            if (config_.aggregation == "default")
            {
                auto m_ag = multiply("avg");
                m_ag = "&aggregationTypes=" + m_ag;
                return m_ag;
            }
            std::string res("&aggregationTypes=");
            res = res + config_.aggregation;

            return res;
        }

        std::string formulas_url() const
        {

            static const std::unordered_map<std::string, std::string> formulasDict = {
                {"level", "0"},

                {"percentage_change", "1"},
                {"pc", "1"},

                {"difference", "2"},
                {"d", "2"},

                {"yoy", "3"},
                {"yoy_p", "3"},
                {"yoy_pc", "3"},
                {"yoy_percent", "3"},

                {"yoy_diff", "4"},
                {"yoy_d", "4"},

                {"pc_end", "5"},

                {"dif_end", "6"},
                {"mov_ave", "7"},
                {"ma", "7"},
                {"mov_sum", "8"},
                {"ms", "8"},
            };

            auto it = formulasDict.find(config_.formulas);
            if (it != formulasDict.end())
            {

                std::string m_ag = multiply(it->second);
                m_ag = "&formulas=" + m_ag;
                return m_ag;

                // return "&formulas=" + it->second;
            }
            else
            {
                return "";
            }
        }

        std::string
        frequency_url() const
        {
             

            static const std::unordered_map<std::string, std::string> freqDict = {
                {"daily", "1"},
                {"business", "2"},
                {"weekly", "3"},
                {"semimonthly", "4"},
                {"monthly", "5"},
                {"quarterly", "6"},
                {"semiannually", "7"},
                {"annual", "8"},
                {"annually", "8"}};

            auto it = freqDict.find(config_.frequency);
            if (it != freqDict.end())
            {

                return "&frequency=" + it->second;
            }
            else
            {
                return "";
            }
        }

        // final URL
        std::string
        build() const
        {
            std::ostringstream oss;
            oss << domain << "service/evds/series=" << convert_series();
            oss << "&startDate=" << config_.start_date;
            oss << "&endDate=" << config_.end_date;
            oss << frequency_url();
            oss << aggregation_url();
            oss << formulas_url();

            oss << "&type=json";

            return oss.str();
        }

        std::string build_dg() const
        {
             
            std::ostringstream oss;
            oss << domain << "service/evds/datagroup=" << series_;
            oss << "&startDate=" << config_.start_date;
            oss << "&endDate=" << config_.end_date;
            oss << "&type=json";
            return oss.str();
        }

    private:
        Config config_;
        std::string series_;
        Index index_v;
    };
}