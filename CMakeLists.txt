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


set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(BOOST_ROOT $ENV{BOOST-LATEST})
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(libs/imgui libs/imgui/backends)
set(IMGUI_DX12_SRCS 
    libs/imgui/imgui.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/backends/imgui_impl_win32.cpp
    libs/imgui/backends/imgui_impl_dx12.cpp
)
aux_source_directory(code SRCS)
add_executable(${PROJECT_NAME} WIN32 ${IMGUI_DX12_SRCS} ${SRCS} res/dpi.manifest res/nettool.rc)

