import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class PubHello(Node):
    # 构造函数，在每个类对象初始化时都会执行
    def __init__(self,name):
        super().__init__(name)

        # 创建一个发布者（Python中不需要声明）
        self.pub_hello = self.create_publisher(String,"/HelloWorld",10)

        # 创建一个定时器，用于定时给发布者发布消息（Python中不需要声明）
        self.timer = self.create_timer(0.5,self.timer_callback)
    
    # 定时器的回调函数
    def timer_callback(self):
        # 构建消息并发布
        msg = String()
        msg.data = "你好世界"
        self.pub_hello.publish(msg)

        # DEBUG，显示当前函数确实在运行，发布者确实在发布
        self.get_logger().info(f"发布者发布消息: {msg.data}")

def main(args=None):
    rclpy.init(args=args)       # 初始化rclpy
    node = PubHello("SayHello") # 新建一个节点
    rclpy.spin(node)            # 保持节点运行，检测是否收到退出指令(Ctrl+C)
    rclpy.shutdown()            # 当接收到退出指令时，退出spin函数，执行shutdown函数来关闭rclpy

