cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(KanjiReviewParent)

set(UI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ui")
set(UI_DIST_DIR "${UI_SOURCE_DIR}/dist")
set(INDEX_HTML_PATH "${UI_DIST_DIR}/index.html")

find_program(NPM_EXECUTABLE npm REQUIRED)

add_custom_command(
    OUTPUT "${INDEX_HTML_PATH}"
    COMMAND ${NPM_EXECUTABLE} install
    COMMAND ${NPM_EXECUTABLE} run build
    WORKING_DIRECTORY "${UI_SOURCE_DIR}"
    COMMENT "Building React UI with npm..."
    VERBATIM
)

add_custom_target(BuildUI ALL
    DEPENDS "${INDEX_HTML_PATH}"
)

add_subdirectory(app)
