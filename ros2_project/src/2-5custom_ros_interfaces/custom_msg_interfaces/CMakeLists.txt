cmake_minimum_required(VERSION 3.8)
project(custom_msg_interfaces)

# 如果使用 GCC 或 Clang 编译器，开启常用的警告选项【不必理会】
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

# 生成 ROS 2 接口（msg / srv / action）
rosidl_generate_interfaces(
  ${PROJECT_NAME}        # 接口所属的包名，一般使用当前工程名
  "msg/Test.msg"         # 要生成的消息文件路径（相对于包根目录）
  DEPENDENCIES std_msgs  # 该消息中使用到的其他消息包依赖
)

# 测试与代码规范检查相关【不必理会】
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_auto_package()
