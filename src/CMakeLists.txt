
set(SOURCES
    
main.cpp
    main.cpp

)

add_library(evdscpp_lib ${SOURCES})

target_link_libraries(evdscpp_lib PRIVATE CURL::libcurl)

add_executable(evdscpp main.cpp)
target_link_libraries(evdscpp PRIVATE evdscpp_lib)
