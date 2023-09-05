cmake_minimum_required(VERSION 3.23)
project(Frontier_Game)

set(CMAKE_CXX_STANDARD 14)

add_executable(Frontier_Game main.cpp)

include_directories("/opt/homebrew/Cellar/nlohmann-json/3.11.2/include")
