
include_directories(../Core ../Src)


add_executable(TestTcpServer TestTcpServer.cpp)
target_link_directories(TestTcpServer PRIVATE ${QT_DIR}/lib)
target_link_libraries(TestTcpServer PRIVATE Qt4::QtGui Qt4::QtCore qtmain Core)

add_executable(TestHexUtil TestHexUtil.cpp)
target_link_directories(TestHexUtil PRIVATE ${QT_DIR}/lib)
target_link_libraries(TestHexUtil PRIVATE Qt4::QtGui Qt4::QtCore qtmain Core ws2_32)
