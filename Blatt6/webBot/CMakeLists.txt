cmake_minimum_required(VERSION 3.21)
project(webBot)

set(CMAKE_CXX_STANDARD 14)

add_executable(webBot main.cpp Queue.cpp Queue.h WebBot.cpp WebBot.h
        web_request/web_request.c web_request/web_request.cpp web_request/ web_request/web_request.h
        )

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(webBot ${CURL_LIBRARIES})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")