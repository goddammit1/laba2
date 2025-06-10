cmake_minimum_required(VERSION 3.27)
project(temp1)

set(CMAKE_CXX_STANDARD 17)

add_executable(temp1 main.cpp
        boost/asio/any_completion_executor.hpp)
