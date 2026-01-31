import rclpy
from rclpy.node import Node
from custom_msg_interfaces.msg import Test


class TestCustomMsgNode(Node):
    # 构造函数，在每个类对象初始化时都会执行
    def __init__(self,name):
        super().__init__(name)

        # 构建一个自定义消息类型的变量
        msg = Test()
        msg.header.frame_id="base_link"
        msg.header.stamp=self.get_clock().now().to_msg()
        msg.flag=True
        
        # 日志输出，debug自定义消息
        self.get_logger().info(f"测试自定义消息：\ntest_msg.header.frame_id: {msg.header.frame_id}\ntest_msg.header.stamp: {msg.header.stamp}\ntest_msg.flag: {msg.flag}")

def main(args=None):
    rclpy.init(args=args)                           # 初始化rclpy
    node = TestCustomMsgNode("TestCustomMsgNode")   # 新建一个节点
    rclpy.spin(node)                                # 保持节点运行，检测是否收到退出指令(Ctrl+C)
    rclpy.shutdown()                                # 当接收到退出指令时，退出spin函数，执行shutdown函数来关闭rclpy
