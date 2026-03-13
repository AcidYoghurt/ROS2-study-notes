cmake_minimum_required(VERSION 3.8)
project(8-2learn_executors)

# 如果使用 GCC 或 Clang 编译器，开启常用的警告选项【不必理会】
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

# 添加可执行文件
ament_auto_add_executable(
        learn_single-threaded-executor
        src/learn_single-threaded-executor.cpp
)
ament_auto_add_executable(
        learn_multi-threaded-executor
        src/learn_multi-threaded-executor.cpp
)

# 测试与代码规范检查相关【不必理会】
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_auto_package()

