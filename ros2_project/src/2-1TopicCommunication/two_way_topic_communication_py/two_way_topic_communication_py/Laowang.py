import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from std_msgs.msg import UInt32

class SingleDogNode(Node):

    # 构造函数
    def __init__(self,name):
        super().__init__(name)

        # 输出日志，让我们知道节点启动了
        self.get_logger().info(f"单身狗节点{name}启动")
        
        # 创建一个订阅者来订阅李四的小说，话题名为novel
        self.sub_novel = self.create_subscription(String,"novel",self.sub_novel_callback,10)

        # 创建一个发布者来给稿费，话题名为novel_money
        self.pub_money = self.create_publisher(UInt32,"novel_money",10)

    # 订阅者(小说)的回调函数
    def sub_novel_callback(self,msg:String):
        # 1. 订阅者接收数据（接收数据后发布消息）
        self.get_logger().info(f"我收到小说：{msg.data}")
        
        # 2. 订阅者发布数据（这里是订阅者看完小说，给发布者打钱，其实就是发布者）
        # 逻辑贯通：老王接收到李四的小说后，就应该给他稿费
        money = UInt32()
        money.data = 10 # 表示10元
        self.pub_money.publish(money)
        self.get_logger().info(f"我已经发送{money.data}元稿费")
    

def main(args=None):
    rclpy.init(args=args)               # 初始化rclpy
    node = SingleDogNode("LaoWang")     # 新建一个节点
    rclpy.spin(node)                    # 保持节点运行，检测是否收到退出指令(Ctrl+C)
    rclpy.shutdown()                    # 当接收到退出指令时，退出spin函数，执行shutdown函数来关闭rclpy

