import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class GetHello(Node):
    # 构造函数，在每个类对象初始化时都会执行
    def __init__(self,name):
        super().__init__(name)

        # 创建一个订阅者（Python中不需要声明）
        self.get_hello = self.create_subscription(String,"/HelloWorld",self.get_hello_callback,10)

    # 订阅者回调函数（这里msg:String其实是规定变量msg必须是std_msgs.msg.String类型）
    def get_hello_callback(self,msg:String):
        # 注意：因为std_msgs.msg.String里面封装了一个str类型的data属性，所以我们实际上是操纵这个data属性
        self.get_logger().info(f"订阅者接收到消息: {msg.data}")

def main(args=None):
    rclpy.init(args=args)       # 初始化rclpy
    node = GetHello("SubHello") # 新建一个节点
    rclpy.spin(node)            # 保持节点运行，检测是否收到退出指令(Ctrl+C)
    rclpy.shutdown()            # 当接收到退出指令时，退出spin函数，执行shutdown函数来关闭rclpy

