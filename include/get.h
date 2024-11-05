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


#include "dotenv_.h"

#include <curl/curl.h>

#include "../extern/nlohmann/json.hpp"

#include "header.h"
#include "cache.h"

using namespace evds;

template <typename T>
struct is_cstring : std::false_type
{
};

template <>
struct is_cstring<const char *> : std::true_type
{
};

template <>
struct is_cstring<char *> : std::true_type
{
};

struct CurlDeleter
{
    void operator()(CURL *ptr) const
    {
        curl_easy_cleanup(ptr);
    }
};

struct ResponseData
{
    std::unique_ptr<char[]> memory;
    size_t size = 0;
};

struct GetParams
{
    std::string url;
    std::string api_key;
    std::string proxy_url;
    bool cache = false;
    bool verbose = false;
};

// ...................................................... WriteCallback

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    auto *mem = reinterpret_cast<ResponseData *>(userp);

    auto new_memory = std::make_unique<char[]>(mem->size + realsize + 1);
    if (!new_memory)
        return 0;

    if (mem->memory)
        std::memcpy(new_memory.get(), mem->memory.get(), mem->size);

    std::memcpy(new_memory.get() + mem->size, contents, realsize);
    mem->size += realsize;
    new_memory[mem->size] = 0;

    mem->memory = std::move(new_memory);

    return realsize;
}

// ...................................................... proxy URL
template <typename T>
std::enable_if_t<is_cstring<T>::value, const char *> get_proxy_for_url(T url, T proxy_url)
{
    if (proxy_url && std::strlen(proxy_url) > 0)
        return proxy_url;

    const char *https_proxy = std::getenv("HTTPS_PROXY");
    const char *http_proxy = std::getenv("HTTP_PROXY");

    if (std::strncmp(url, "https", 5) == 0 && https_proxy)
    {
        return https_proxy;
    }
    else if (http_proxy)
    {
        return http_proxy;
    }
    return "";
}

std::string get_request_real(const GetParams &params, bool test);

std::string get_request(const GetParams &params, const Config &config)
{

    bool cache_option = config.cache;

    Cache cache; // ("./.caches");
    std::string cached_result;
    std::string fnc_name("get_request_real");

    std::vector<std::string> v = {params.url, params.api_key, params.proxy_url};
    std::string delim("_");
    std::string params_str = evds::join(v, delim);

    if (cache_option && cache.check_and_load_cache(fnc_name, cached_result, params_str))
    {
        std::cout << evds::divider();
        std::cout << "Loaded data from cache." << std::endl;
        std::cout << evds::divider();

        return cached_result;
    }

    auto res = get_request_real(params, config.test);
    if (cache_option)
        cache.save_cache(fnc_name, res, params_str);
    return res;
}
std::string get_request_real(const GetParams &params, bool test)
{
    //  if (config.test)

    if (!test & !evds::confirm("Request?", params.url, params.verbose))
    {
        std::cout << "Not requesting ...";
        throw std::runtime_error("Request was cancelled ");
    }

    std::cout << "[requesting]";
    CURLcode res;
    ResponseData chunk;

    if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0)
    {
        throw std::runtime_error("Failed to initialize curl.");
    }

    std::unique_ptr<CURL, CurlDeleter> curl(curl_easy_init());
    if (curl)
    {
        std::cout << "url: " << params.url << "\n";
        curl_easy_setopt(curl.get(), CURLOPT_URL, params.url.c_str());

        // Prepare the header with the API key
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        std::string api_key_header = "key: " + params.api_key;
        headers = curl_slist_append(headers, api_key_header.c_str());

        curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &chunk);

        const char *proxy_url_env = get_proxy_for_url(params.url.c_str(), params.proxy_url.c_str());
        if (proxy_url_env && std::strlen(proxy_url_env) > 0)
        {
            curl_easy_setopt(curl.get(), CURLOPT_PROXY, proxy_url_env);
        }

        res = curl_easy_perform(curl.get());

        if (res != CURLE_OK)
        {
            throw std::runtime_error(curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);
    }
    else
    {
        throw std::runtime_error("curl_easy_init() failed.");
    }

    curl_global_cleanup();

    return std::string(chunk.memory.get(), chunk.size);
}

void write_response_to_file(const std::string &content, const std::string &filename)
{
    std::ofstream outfile(filename, std::ios::out | std::ios::trunc);
    if (!outfile)
    {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    outfile << content;
    outfile.close();

    std::cout << "Response written to " << filename << std::endl;
}

std::string read_content_from_file(const std::string &filename)
{
    std::ifstream infile(filename, std::ios::in);
    if (!infile)
    {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << infile.rdbuf();
    infile.close();

    std::cout << "Content read from " << filename << std::endl;

    return buffer.str();
}

std::string getEvds(const std::string &url, const Config &config)
{

    try
    {
        GetParams params;
        params.url = url;
        params.verbose = config.verbose;
        const char *env_apikey = std::getenv("EVDS_APIKEY");

        if (config.test)
        {

            if (env_apikey)
            {
                params.api_key = std::string(env_apikey);
            }
            else
            {
                std::cerr << "Environment variable EVDS_APIKEY is not set." << std::endl;
                return "";
            }
        }
        else
        {
            params.api_key = get_api_key();
        }

        params.proxy_url = "";

        std::string response = get_request(params, config);

        return response;
    }
    catch (const std::exception &ex)
    {
        // std::cerr << "Error: " << ex.what() << std::endl;
    }
    throw std::runtime_error("Request was cancelled");
    // return nullptr;
    return "";
}
