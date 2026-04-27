cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(QuantumTTT)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(SFML)

add_executable(QuantumTTT src/main.cpp)
target_link_libraries(QuantumTTT SFML::Graphics SFML::Window SFML::System)
