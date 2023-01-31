project(NetTool)
file(GLOB_RECURSE SRCS *.h *.cpp)
#file(GLOB_RECURSE MOC_FILES ${CMAKE_SOURCE_DIR}/out/src/*.cpp)
add_executable(${PROJECT_NAME} WIN32 dpi.manifest ${SRCS})

target_link_directories(${PROJECT_NAME} PRIVATE ${QT_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE QtGuid QtCored QtNetwork qtmaind )
#target_link_libraries(${PROJECT_NAME} PRIVATE QtGui QtCore qtmain)
target_link_libraries(${PROJECT_NAME} PRIVATE Core Imm32 Winmm Ws2_32)
