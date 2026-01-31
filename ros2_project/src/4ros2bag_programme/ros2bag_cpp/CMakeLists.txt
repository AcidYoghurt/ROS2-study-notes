cmake_minimum_required(VERSION 3.8)
project(ros2bag_cpp)

# 如果使用 GCC 或 Clang 编译器，开启常用的警告选项【不必理会】
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

# 添加可执行文件 simple_bag_writer
ament_auto_add_executable(
        simple_bag_writer
        src/simple_bag_writer.cpp
)

# 添加可执行文件 simple_bag_recorder
ament_auto_add_executable(
        simple_bag_recorder
        src/simple_bag_recorder.cpp
)

# 测试与代码规范检查相关【不必理会】
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_auto_package()
