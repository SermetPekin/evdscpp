
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
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <functional>

class Cache
{

public:
    Cache(const std::string &cache_dir, bool verbosein) : cache_dir_(cache_dir), verbose(verbosein)
    {

        std::filesystem::create_directories(cache_dir_);
    }

    Cache()
    {

        std::filesystem::create_directories(cache_dir_);
    }

    template <typename... Args>
    bool check_and_load_cache(const std::string &function_name, std::string &result, const Args &...args)
    {
        std::string hash = generate_hash(function_name, args...);
        std::string file_path = get_cache_file_path(hash);

        if (std::filesystem::exists(file_path))
        {
            result = load_cache(file_path);
            return true;
        }

        return false;
    }

    template <typename... Args>
    void save_cache(const std::string &function_name, const std::string &data, const Args &...args)
    {

        std::string hash = generate_hash(function_name, args...);
        std::string file_path = get_cache_file_path(hash);
        save_to_file(file_path, data);
        if (verbose)
            std::cout << "[saving cache] " << file_path << "\n";
    }

private:
    std::string cache_dir_ = "./.caches";
    bool verbose = true;

    template <typename... Args>
    std::string generate_hash(const std::string &function_name, const Args &...args) const
    {
        std::stringstream ss;
        ss << function_name;
        (void)(ss << ... << args);
        return std::to_string(std::hash<std::string>{}(ss.str()));
    }

    std::string get_cache_file_path(const std::string &hash) const
    {
        return cache_dir_ + "/" + hash + ".cache";
    }

    std::string load_cache(const std::string &file_path) const
    {

        if (verbose)
            std::cout << "[loading cache] " << file_path << "\n";

        std::ifstream file(file_path, std::ios::in | std::ios::binary);
        if (!file)
        {
            throw std::runtime_error("Could not open cache file for reading");
        }

        std::string data((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
        file.close();
        return data;
    }

    void save_to_file(const std::string &file_path, const std::string &data) const
    {
        std::ofstream file(file_path, std::ios::out | std::ios::binary);
        if (!file)
        {
            throw std::runtime_error("Could not open cache file for writing");
        }

        file.write(data.c_str(), data.size());
        file.close();
    }
};
