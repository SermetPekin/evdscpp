
cmake_minimum_required(VERSION 3.20)
project(evdscpp LANGUAGES CXX)


# Top-level CMakeLists.txt

# Set output directories for executables and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)



# Enable testing
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(extern/nlohmann)  # External dependencies
include_directories(extern/dotenv)  # External dependencies

find_package(CURL REQUIRED)

add_subdirectory(src)

add_subdirectory(tests)




# #=======================================================================================================================
# # Valgrind check target
# #=======================================================================================================================
# find_program(VALGRIND "valgrind")

# if (VALGRIND)
#     add_custom_target(valgrind_check
#         COMMAND ${VALGRIND} --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose
#         $<TARGET_FILE:evdscpp>
#         COMMENT "Running Valgrind memory check on evdscpp"
#         DEPENDS evdscpp
#     )
# else()
#     message(WARNING "Valgrind not found. 'valgrind_check' target will not be available.")
# endif()



# Enable AddressSanitizer for memory error detection
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")


#=======================================================================================================================
# Build instructions
#=======================================================================================================================
# To build and run the project:
# mkdir build
# cd build
# cmake ..
# make
# ./evdscpp
# 
# To run Valgrind check:
# make valgrind_check


