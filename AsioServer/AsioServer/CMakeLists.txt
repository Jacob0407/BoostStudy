# CMakeList.txt: AsioServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# set(BOOST_ROOT "F:\\codeproject\\thirdpart\\boost_1_73_0")
# set(BOOST_INCLUDE_DIR "F:\\codeproject\\thirdpart\\boost_1_73_0")
# set(BOOST_LIBRARY_DIR "F:\\codeproject\\thirdpart\\boost_1_73_0\\stage\\lib")

include_directories($ENV{BOOST_INCLUDE_DIR})
link_directories($ENV{BOOST_LIBRARY_DIR})

# 将源代码添加到此项目的可执行文件。
add_executable (AsioServer "AsioServer.cpp" "AsioServer.h")

target_link_libraries(AsioServer $ENV{BOOST_LIBRARY_DIR})

# TODO: 如有需要，请添加测试并安装目标。
