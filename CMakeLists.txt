cmake_minimum_required (VERSION 3.8)
project ("CMakeProject1")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

add_definitions(-DFMT_HEADER_ONLY)

file(GLOB_RECURSE SRCS Src/*.h Src/*.cpp)
file(GLOB_RECURSE MOC_FILES out/moc/*.cpp)
add_executable(demo WIN32 dpi.manifest ${SRCS} ${MOC_FILES})


set(QT_DIR C:/Qt/4.3.5)
set(QT_QMAKE_EXECUTABLE ${QT_DIR}/bin/qmake.exe)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 REQUIRED QtGui) 


include_directories(${QT_DIR}/include ${QT_DIR}/include/QtGui)
target_link_directories(demo PRIVATE ${QT_DIR}/lib)
#target_link_libraries(demo PRIVATE QtGuid QtCored)
target_link_libraries(demo PRIVATE QtGui QtCore)
target_link_libraries(demo PRIVATE qtmaind Imm32 Winmm Ws2_32)
