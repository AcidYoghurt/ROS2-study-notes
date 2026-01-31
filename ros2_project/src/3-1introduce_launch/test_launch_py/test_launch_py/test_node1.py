import rclpy
from rclpy.node import Node

class TestNode1(Node):
    def __init__(self):
        super().__init__("TestNode1")

        self.get_logger().info("你好世界1")

def main(args=None):
    rclpy.init(args=args)   # 初始化rclpy
    node = TestNode1()      # 新建一个节点
    rclpy.spin(node)        # 保持节点运行，检测是否收到退出指令(Ctrl+C)
    rclpy.shutdown()        # 当接收到退出指令时，退出spin函数，执行shutdown函数来关闭rclpy

if __name__=="__main__":
    main()