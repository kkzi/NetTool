file(GLOB_RECURSE SRCS *.h *.cpp)
#file(GLOB_RECURSE MOC_FILES ${CMAKE_SOURCE_DIR}/out/core/*.cpp)
add_library(Core STATIC ${SRCS})