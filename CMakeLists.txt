cmake_minimum_required (VERSION 3.8)
project ("NetTool")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

add_definitions(-DFMT_HEADER_ONLY -D_WIN32_WINNT=0x0601)


set(QT_DIR C:/Qt/4.3.5)
set(QT_QMAKE_EXECUTABLE ${QT_DIR}/bin/qmake.exe)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 COMPONENTS QtGui) 

#set(BOOST_DIR D:/Local/boost_1_80_0)
set(BOOST_DIR C:/Local/boost_1_81_0)


include_directories(Core)
include_directories(${BOOST_DIR} ${QT_DIR}/include ${QT_DIR}/include/QtCore ${QT_DIR}/include/QtGui)


add_subdirectory(Core)
add_subdirectory(Src)
add_subdirectory(Test)
