
include_directories(../Core ../Src)


add_executable(TestTcpServer TestTcpServer.cpp)
target_link_directories(TestTcpServer PRIVATE ${QT_DIR}/lib)
target_link_libraries(TestTcpServer PRIVATE QtCored Core)

add_executable(TestHexUtil TestHexUtil.cpp)
target_link_directories(TestHexUtil PRIVATE ${QT_DIR}/lib)
target_link_libraries(TestHexUtil PRIVATE QtCored ws2_32)
