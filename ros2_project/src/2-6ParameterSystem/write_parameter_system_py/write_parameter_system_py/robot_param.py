import rclpy
from rclpy.node import Node
from rcl_interfaces.msg import SetParametersResult

class ParamServerNode(Node):

    def __init__(self):
        super().__init__('param_server_node')

        # 声明参数（默认值 1.0）
        self.declare_parameter('robot_speed', 1.0)

        # 注册参数修改回调
        self.add_on_set_parameters_callback(self.parameter_callback)

        self.get_logger().info("参数服务器准备就绪")

    # 参数回调
    def parameter_callback(self, params):
        for param in params:
            if param.name == 'robot_speed':
                value = param.value

                # 参数合法性检查
                if value <= 0.0 or value > 5.0:
                    return SetParametersResult(
                        successful=False,
                        reason="robot_speed 的取值范围必须在 (0.0, 5.0]"
                    )

                self.get_logger().info(f"robot_speed 更新为 {value}")

        return SetParametersResult(successful=True)


def main(args=None):
    rclpy.init(args=args)
    node = ParamServerNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()