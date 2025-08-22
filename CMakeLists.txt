cmake_minimum_required(VERSION 3.10)

project(MySFMLProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MySFMLProject main.cpp)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

target_link_libraries(MySFMLProject
    sfml-system
    sfml-audio
)
