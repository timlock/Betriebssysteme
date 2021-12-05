cmake_minimum_required(VERSION 3.21)
project(webBot)

set(CMAKE_CXX_STANDARD 14)

add_executable(clionBot main.cpp Queue.cpp Queue.h WebBot.cpp WebBot.h
        web_request/web_request.c web_request/web_request.cpp web_request/ web_request/web_request.h

        web_request/curl/curl.h
        web_request/curl/curlver.h
        web_request/curl/easy.h
        web_request/curl/mprintf.h
        web_request/curl/multi.h
        web_request/curl/stdcheaders.h
        web_request/curl/system.h
        web_request/curl/typecheck-gcc.h
        web_request/curl/urlapi.h

        )

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(clionBot ${CURL_LIBRARIES})

