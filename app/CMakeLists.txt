cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# create project
project(KanjiReview)

set(CMAKE_CXX_STANDARD 23)

if(MSVC)
    add_compile_options(/utf-8)
endif()


# create library from source files (excluding main.cpp)
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_library("${PROJECT_NAME}Lib" STATIC ${SOURCES} ${HEADERS})
target_include_directories("${PROJECT_NAME}Lib" PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# add dependencies
include(cmake/CPM.cmake)
CPMAddPackage(
    NAME asio
    GITHUB_REPOSITORY chriskohlhoff/asio
    GIT_TAG "asio-1-36-0")

CPMAddPackage(
    NAME Crow
    GITHUB_REPOSITORY CrowCpp/Crow
    VERSION 1.3.1
    EXCLUDE_FROM_ALL YES
    OPTIONS "CROW_USE_BOOST OFF" "CROW_INSTALL OFF"
)
CPMAddPackage("gh:gabime/spdlog@1.16.0")
CPMAddPackage("gh:nlohmann/json@3.12.0")
CPMAddPackage(
    NAME sqlite3
    VERSION 3.51.1
    URL https://sqlite.org/2025/sqlite-amalgamation-3510100.zip
)

add_library(sqlite3 STATIC ${sqlite3_SOURCE_DIR}/sqlite3.c)
target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})

# link dependencies to library
target_link_libraries("${PROJECT_NAME}Lib" PUBLIC sqlite3 nlohmann_json::nlohmann_json spdlog)

# set up Asio for Crow (standalone, no Boost)
if(asio_ADDED)
    add_library(asio INTERFACE)
    target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
    target_compile_definitions(asio INTERFACE ASIO_STANDALONE)
    add_library(asio::asio ALIAS asio)
endif()

# add executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE "${PROJECT_NAME}Lib" Crow::Crow asio)


add_subdirectory(test)

