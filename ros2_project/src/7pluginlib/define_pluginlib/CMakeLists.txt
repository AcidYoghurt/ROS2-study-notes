cmake_minimum_required(VERSION 3.8)
project(define_pluginlib)

# 如果使用 GCC 或 Clang 编译器，开启常用的警告选项【不必理会】
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

# 导出库
ament_auto_add_library(
        a_star_planner          # 【参数1】库的名字（CMake target 名）
        # - 生成的 target 叫 my_lib
        # - 实际生成的文件通常是 libmy_lib.so
        # - 其他包通过 ament_target_dependencies(... my_lib) 使用它

        SHARED          # 【参数2】库的类型
        # - SHARED：动态库（.so）
        # - STATIC：静态库（.a）
        # - 如果不写，默认是 SHARED（ROS2 中几乎都用 SHARED）

        src/a_star_planner.cpp  # 【参数3】库的源文件列表
        # - 可以写多个 cpp
        # - 只写算法/逻辑代码
        # - 不应该包含 main()
)

# 导出插件描述文件
# 参数1：功能包的名字
# 参数2：插件描述文件的位置
pluginlib_export_plugin_description_file(define_pluginlib plugin.xml)

# 测试与代码规范检查相关【不必理会】
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


ament_auto_package()