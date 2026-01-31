import rclpy
from rclpy.node import Node

class SerialNode(Node):
    def __init__(self):
        super().__init__('serial_node')

        # 声明参数并设置默认值
        self.declare_parameter('port', '/dev/ttyUSB0')
        self.declare_parameter('baudrate', 115200)
        self.declare_parameter('timeout_ms', 10)

        # 获取参数
        self.port = self.get_parameter('port').get_parameter_value().string_value
        self.baudrate = self.get_parameter('baudrate').get_parameter_value().integer_value
        self.timeout_ms = self.get_parameter('timeout_ms').get_parameter_value().integer_value

        # 打印日志
        self.get_logger().info(f"获取到参数: port:{self.port}, baudrate:{self.baudrate}, timeout_ms:{self.timeout_ms}")

def main(args=None):
    rclpy.init(args=args)   # 初始化rclpy
    node = SerialNode()     # 新建一个节点
    rclpy.spin(node)        # 保持节点运行，检测是否收到退出指令(Ctrl+C)
    rclpy.shutdown()        # 当接收到退出指令时，退出spin函数，执行shutdown函数来关闭rclpy

if __name__=="__main__":
    main()
