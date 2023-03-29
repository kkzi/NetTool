cmake_minimum_required(VERSION 3.10)
project(NetTool2)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    add_compile_options($<$<CONFIG:>:/MT> $<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT>)
    add_compile_options(/Zc:__cplusplus)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif (MSVC)


add_subdirectory(libs/hello_imgui)
include_directories(
    libs/hello_imgui/src
    ${HELLOIMGUI_IMGUI_SOURCE_DIR}
)
list(APPEND CMAKE_MODULE_PATH ${HELLOIMGUI_CMAKE_PATH})
include(hello_imgui_add_app)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(BOOST_ROOT $ENV{BOOST-LATEST})
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

aux_source_directory(code SRCS)
hello_imgui_add_app(${PROJECT_NAME} ${SRCS} res/dpi.manifest res/nettool.rc)

