cmake_minimum_required(VERSION 3.8)
project(recognizer)

# 如果使用 GCC 或 Clang 编译器，开启常用的警告选项【不必理会】
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_auto REQUIRED) # 查找 ament_cmake_auto（ament_cmake 的自动化封装）
ament_auto_find_build_dependencies()    # 根据 package.xml 中声明的依赖，自动查找构建依赖

# 定义一个库
ament_auto_add_library(
        recognizer
        SHARED
        src/recognizer.cpp
)

# 测试与代码规范检查相关【不必理会】
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_auto_package()    # 自动完成 install、export，并调用 ament_package()
