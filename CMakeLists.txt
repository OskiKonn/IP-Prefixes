cmake_minimum_required(VERSION 3.14)

project(IPv4Prefixes)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(app
    src/main.cpp
    src/IPv4PrefixSet.cpp
)
target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(tests
    src/tests.cpp
    src/IPv4PrefixSet.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(tests
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

