[![CMake Build and Test](https://github.com/SermetPekin/evdscpp/actions/workflows/cmake-single-platform.yml/badge.svg?1)](https://github.com/SermetPekin/evdscpp/actions/workflows/cmake-single-platform.yml?1)

## evdscpp

To build use this 
```bash  

mkdir build 
cd build

cmake .. 
make 
./evdscpp 

```
 

# evdscpp

**evdscpp** is a C++ library for fast, efficient, and user-friendly interaction with the EVDS API Server. Designed with performance in mind, it provides built-in caching, an Excel export option, and an intuitive user interface for configuring and retrieving data. `evdscpp` can be extended for integration with other C++ projects and offers options for use as a standalone application.

## Features

- **Fast Data Fetching**: Optimized C++ code enables quick and efficient data retrieval from the EVDS API.
- **Caching**: Caches results locally to avoid redundant requests, minimizing API calls and improving performance.
- **Excel Export**: Automatically exports data to Excel format for easy analysis and sharing.
- **Configuration Support**: Reads configuration files for API keys, date ranges, and other options.
- **Command-Line Interface**: Simple and flexible CLI for specifying data queries and configuration options.
- **Extensible for Other Projects**: Can be used as a library in other C++ projects, enabling seamless data access from the EVDS API.

## Build Instructions

To build `evdscpp`, use the following commands:

```bash
cd src/build
cmake ..
make
./evdscpp
```




## Environment Configuration

This project uses an `.env` file to store sensitive information, such as your EVDS API key. The `.env` file should be created in the root directory of the project to keep your API keys and other sensitive information secure and organized.

### Example `.env` File

Below is an example of what your `.env` file might look like. Replace `ABCDEFG` with your actual API key.

```
# .env file

# EVDS API key for authenticating API requests
EVDS_APIKEY=ABCDEFG
```

### Usage

The program automatically reads the `.env` file and retrieves the `EVDS_APIKEY` value when making requests to the EVDS API. Ensure that this file is present in the root directory before running the program.


### How to Get Your EVDS API Key

You can obtain a free EVDS API key by registering on the EVDS website provided by the Central Bank of Turkey. Visit [EVDS API Documentation and Key Registration](https://evds2.tcmb.gov.tr/index.php?/evds/userDocs) to sign up and receive your API key.




## Example Usage

```bash
# 1. each index will have its own file :
./evdscpp TP.DK.USD.A,TP.DK.EUR.A


# 2 two indexes in the same file :
./evdscpp TP.DK.USD.A-TP.DK.EUR.A

# 3. Using indexes with named arguments:
./evdscpp TP.DK.USD.A,TP.DK.EUR.A --start_date 01-01-2021 --end_date 31-12-2021 --cache true

# Another example with a different index:
./evdscpp bie_yssk --cache true

# Another example with a 3 different indexes :
# this will create two different files. One for the first index `bie_yssk` and one for these indexes TP.DK.USD.A-TP.DK.EUR.A 
./evdscpp bie_yssk,TP.DK.USD.A-TP.DK.EUR.A --cache true

```



## Manual Compilation

If you prefer to compile `evdscpp` manually, you can use the following `g++` command. Make sure you have all necessary dependencies, including `libcurl` and `nlohmann-json`, installed and available in your include/library paths.

```bash
g++ -g -o ./evdscpp ./src/main.cpp -lcurl -I./include -L./extern/nlohmann --std=c++20

```

### Explanation of the Command

- `-g`: Enables debugging information in the compiled binary.
- `-o ./evdscpp`: Specifies the output path and name of the compiled executable (`./evdscpp`).
- `./src/main.cpp`: The main source file for your program.
- `-lcurl`: Links the `libcurl` library, necessary for making HTTP requests.
- `-L./extern/nlohmann`: Specifies the path to the `nlohmann-json` library. Adjust this path based on your setup.
- `--std=c++20`: Specifies the C++ standard to use (C++20).

After running this command, you should have an executable `evdscpp` located in the current directory.



## Configuration

`evdscpp` reads configuration parameters directly from the command line. You can specify options such as:

- **`--start_date`**: The start date for the data request (format: DD-MM-YYYY).
- **`--end_date`**: The end date for the data request (format: DD-MM-YYYY).
- **`--cache`**: Set to `true` to enable caching, which reduces redundant API requests.

If no start or end dates are specified, `evdscpp` defaults to a predefined date range.

## Integration with Other C++ Projects

`evdscpp` is designed to be easily integrated into other C++ projects, allowing developers to fetch data from the EVDS API with minimal setup. Simply include the library in your project and use the available functions.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.


## About This Project

This project is an open-source data wrapper designed to connect to the EVDS API provided by the Central Bank of the Republic of Türkiye. It enables users to easily access and retrieve economic data through a simplified interface.

**Note:** This project is not affiliated with, endorsed, or supported by the Central Bank of Turkey. It is an independent tool developed solely to assist users in interacting with the EVDS API.
