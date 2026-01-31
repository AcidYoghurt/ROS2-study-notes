import rclpy                                        # ROS 2 Python 客户端库
from rclpy.node import Node                         # ROS 2 节点类
from rclpy.serialization import serialize_message   # 用于序列化 ROS 消息
from geometry_msgs.msg import Twist                 # 导入 Twist 消息类型
import rosbag2_py                                   # rosbag2 Python 接口库


# 3. 定义节点类:
class SimpleBagRecorder(Node):
    def __init__(self, name):
        super().__init__(name) # 调用父类构造函数
        self.get_logger().info("大家好, 我是 %s" % name) # 打印日志信息, 显示节点名称

        # 3-1. 创建写出对象:
        self.writer = rosbag2_py.SequentialWriter() # 创建 SequentialWriter 对象用于写入 bag 文件

        # 3-2. 设置写出的目标文件、话题等参数:
        storage_options = rosbag2_py._storage.StorageOptions(uri='my_bag_py', storage_id='sqlite3')  # 指定存储选项, 包括文件路径和存储格式
        converter_options = rosbag2_py._storage.ConverterOptions("", "")                             # 指定转换选项, 这里为空
        self.writer.open(storage_options, converter_options)                                        # 打开 bag 文件进行写入

        # 创建话题元数据, 包含话题名称、消息类型和序列化格式
        topic_info = rosbag2_py._storage.TopicMetadata(
            name='/turtle1/cmd_vel',             # 话题名称
            type='geometry_msgs/msg/Twist',      # 消息类型
            serialization_format='cdr'           # 序列化格式, 常见序列化格式有: CDR, Protobuf, JSON, BSON
        )
        self.writer.create_topic(topic_info)     # 创建记录的话题

        # 创建订阅者对象, 订阅话题 "/turtle1/cmd_vel", 回调函数为 self.topic_callback
        self.subscription = self.create_subscription(Twist, '/turtle1/cmd_vel', self.topic_callback, 10)
        self.subscription                        # 防止订阅对象被垃圾回收

    def topic_callback(self, msg):
        # 3-3. 写出消息。
        self.writer.write(
            '/turtle1/cmd_vel',                  # 话题名称
            serialize_message(msg),              # 序列化后的消息
            self.get_clock().now().nanoseconds   # 当前时间戳
        )

def main(args=None):
    # 2. 初始化 ROS 客户端:
    rclpy.init(args=args)                        # 初始化 ROS 客户端
    # 4. 调用 spin 函数, 并传入对象:
    node = SimpleBagRecorder("Aminuosi")         # 创建 SimpleBagRecorder 节点实例
    rclpy.spin(node)                             # 保持节点运行, 等待回调函数执行
    # 5. 释放资源:
    rclpy.shutdown()                             # 关闭 ROS 客户端

if __name__ == '__main__':
    main()