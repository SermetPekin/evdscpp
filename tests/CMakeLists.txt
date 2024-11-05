
# Enable testing
enable_testing()

# Define test sources
set(TEST_SOURCES
    test_dataframe.cpp
)

# Add the test executable
add_executable(test_evdscpp ${TEST_SOURCES})

# Link the test executable with the main library and dependencies
target_link_libraries(test_evdscpp PRIVATE evdscpp_lib CURL::libcurl)

# Include directories for external dependencies (if needed)
target_include_directories(test_evdscpp PRIVATE ../include)
target_include_directories(test_evdscpp PRIVATE ../extern/nlohmann)
target_include_directories(test_evdscpp PRIVATE ../extern/dotenv)

# Register tests with CTest
add_test(NAME test_evdscpp COMMAND test_evdscpp)
