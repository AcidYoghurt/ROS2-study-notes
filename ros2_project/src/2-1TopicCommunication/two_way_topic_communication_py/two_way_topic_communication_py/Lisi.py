import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from std_msgs.msg import UInt32

class WriterLisi(Node):
    def __init__(self,name):
        super().__init__(name)

        # 输出日志，让我们知道节点启动了
        self.get_logger().info(f"作家节点{name}启动")

        # 创建发布者
        self.pub_novel = self.create_publisher(String,"novel",10)
        self.timer = self.create_timer(0.5,self.PubNovel)

        # 创建订阅者
        self.money = UInt32()   # 声明全局变量money
        self.money.data = 80    # 声明初始的稿费
        self.sub_money = self.create_subscription(UInt32,"novel_money",self.SubMoney,10)

    # 计时器回调函数（里面写发布内容）
    def PubNovel(self):
        # 创建消息
        novel = String()
        novel.data = "小说"
        # 发布消息
        self.pub_novel.publish(novel)

        # 日志打印
        self.get_logger().info(f"我发布了{novel.data}")

    # 收钱的回调函数（接收者）
    def SubMoney(self,sub_money:UInt32):
        # 处理逻辑
        self.money.data += sub_money.data

        # 日志打印
        self.get_logger().info(f"我已经收到{sub_money.data}元稿费，现在我的稿费总共有{self.money.data}元")

def main(args=None):
    rclpy.init(args=args)               # 初始化rclpy
    node = WriterLisi("LiSi")           # 新建一个节点
    rclpy.spin(node)                    # 保持节点运行，检测是否收到退出指令(Ctrl+C)
    rclpy.shutdown()                    # 当接收到退出指令时，退出spin函数，执行shutdown函数来关闭rclpy


