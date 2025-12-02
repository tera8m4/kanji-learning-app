cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# create project
project(MyProject)

set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    add_compile_options(/utf-8)
endif()


# add executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")
add_executable(main WIN32 ${SOURCES} ${HEADERS})
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# add dependencies
include(cmake/CPM.cmake)

CPMAddPackage("gh:gabime/spdlog@1.16.0")
CPMAddPackage("gh:webview/webview#0.12.0")
CPMAddPackage("gh:nlohmann/json@3.12.0")
CPMAddPackage(
    NAME sqlite3
    VERSION 3.51.1
    URL https://sqlite.org/2025/sqlite-amalgamation-3510100.zip
)

add_library(sqlite3 STATIC ${sqlite3_SOURCE_DIR}/sqlite3.c)
target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})

# link dependencies
target_link_libraries(main PRIVATE webview::core sqlite3 nlohmann_json::nlohmann_json spdlog) 


