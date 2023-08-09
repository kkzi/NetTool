cmake_minimum_required (VERSION 3.8)
project ("NetTool")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(/Zc:__cplusplus)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

add_definitions(-DFMT_HEADER_ONLY -D_WIN32_WINNT=0x0601)

string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")


#find_package(zstd CONFIG REQUIRED)

#set(QT_DIR C:/Qt/4.8.7)
set(QT_DIR C:/Local/Qt/6.5.2)
#set(QT_QMAKE_EXECUTABLE ${QT_DIR}/bin/qmake.exe)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${QT_DIR}/lib/cmake")
find_package(Qt6 REQUIRED Gui Widgets Network) 

set(BOOST_DIR $ENV{BOOST})
set(Boost_USE_STATIC_LIB ON)
add_definitions(-DBOOST_DATE_TIME_NO_LIB)

include_directories(Core)
include_directories(${BOOST_DIR} ${QT_DIR}/include ${QT_DIR}/include/QtCore ${QT_DIR}/include/QtGui ${QT_DIR}/include/QtWidgets)

add_subdirectory(Core)
add_subdirectory(Src)
#add_subdirectory(Test) 
