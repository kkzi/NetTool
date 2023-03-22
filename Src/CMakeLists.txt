project(NetTool)
file(GLOB_RECURSE SRCS *.h *.cpp)
#file(GLOB_RECURSE MOC_FILES ${CMAKE_SOURCE_DIR}/out/src/*.cpp)
add_executable(${PROJECT_NAME} WIN32 dpi.manifest ${SRCS} ../Res/nettool.qrc ../Res/nettool.rc)

target_link_directories(${PROJECT_NAME} PRIVATE ${QT_DIR}/lib)
#target_link_libraries(${PROJECT_NAME} PRIVATE Qt4::QtGui Qt4::QtNetwork)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Gui Qt::Network Qt::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Core Imm32 Winmm legacy_stdio_definitions)

