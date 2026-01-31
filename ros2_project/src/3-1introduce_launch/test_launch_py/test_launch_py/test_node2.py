import rclpy
from rclpy.node import Node

class TestNode2(Node):
    def __init__(self):
        super().__init__("TestNode2")

        self.get_logger().info("你好世界2")

def main(args=None):
    rclpy.init(args=args)   # 初始化rclpy
    node = TestNode2()      # 新建一个节点
    rclpy.spin(node)        # 保持节点运行，检测是否收到退出指令(Ctrl+C)
    rclpy.shutdown()        # 当接收到退出指令时，退出spin函数，执行shutdown函数来关闭rclpy

if __name__=="__main__":
    main()