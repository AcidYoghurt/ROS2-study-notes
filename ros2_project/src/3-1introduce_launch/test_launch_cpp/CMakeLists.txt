cmake_minimum_required(VERSION 3.8)
project(test_launch_cpp)

# 如果使用 GCC 或 Clang 编译器，开启常用的警告选项【不必理会】
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

# 添加可执行文件
ament_auto_add_executable(
        test_node1
        src/test_node1.cpp
)
ament_auto_add_executable(
        test_node2
        src/test_node2.cpp
)

# 测试与代码规范检查相关【不必理会】
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_auto_package(
  INSTALL_TO_SHARE
  launch              # 在构建并安装包时，把 launch/ 目录一并安装到包的 share 目录中。
)
