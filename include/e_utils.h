#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <functional>
#include <fstream>

#include <type_traits>

struct ParseArgsOptions
{
    int argc;
    char **argv;
    std::vector<std::string> &indexes;
};

std::vector<std::string> indexes_from_file(const std::string &file_path);


template <typename T>
bool looks_like_filename(const T &name, const std::string& extension = ".txt") {
    static_assert(std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>, 
                  "T must be either std::string or std::string_view");
    return name.find(extension) != std::string::npos;
}


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
                if (looks_like_filename(index))
                    options.indexes = indexes_from_file(index);
                else
                    options.indexes.push_back(index);
            }
        }
    }

    return args;
}




std::vector<std::string> indexes_from_file(const std::string &file_path)
{
    std::vector<std::string> indexes;
    std::ifstream file(file_path);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << file_path << std::endl;
        return indexes;
    }

    std::string line;

    while (std::getline(file, line))
    {
        indexes.push_back(line);
    }

    file.close();
    return indexes;
}


void setConfigOptions(const std::unordered_map<std::string, std::string> &args, Config &config)
{
    std::unordered_map<std::string, std::function<void(const std::string &)>> configSetters = {
        {"cache", [&](const std::string &val)
         { config.cache = (val == "true"); }},
        {"test", [&](const std::string &val)
         { config.test = (val == "true"); }},

        //  auto_confirm
        {"confirm", [&](const std::string &val)
         { config.auto_confirm = (val == "true"); }},

        {"auto_confirm", [&](const std::string &val)
         { config.auto_confirm = (val == "true"); }},

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
