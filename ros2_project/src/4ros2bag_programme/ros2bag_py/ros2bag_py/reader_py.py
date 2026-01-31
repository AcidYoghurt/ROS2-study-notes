# 1. 导包:
import rclpy                            # 导入 ROS 2 Python 客户端库
from rclpy.node import Node             # 导入 Node 类, 用于创建节点
import rosbag2_py                       # 导入 rosbag2_py 库, 用于读取 bag 文件
from rclpy.logging import get_logger    # 导入日志记录器

# 3. 定义节点类:
class SimpleBagPlayer(Node):
    def __init__(self):
        super().__init__('simple_bag_player_py') # 调用父类 (Node) 的初始化方法

        # 3-1. 创建读取对象:
        self.reader = rosbag2_py.SequentialReader() # 创建 SequentialReader 对象, 用于顺序读取 bag 文件

        # 3-2. 设置读取的目标文件、话题等参数:
        # 注意要跟序列化的目标文件一致
        storage_options = rosbag2_py._storage.StorageOptions(uri="my_bag_py", storage_id='sqlite3')  # 配置存储选项, 包括目标文件 (uri) 和存储格式 (storage_id)
        converter_options = rosbag2_py._storage.ConverterOptions('', '')                             # 配置转换选项 (这里为空字符串表示默认设置)
        self.reader.open(storage_options, converter_options)                                         # 打开 bag 文件进行读取, 传入存储选项和转换选项

    # 3-3. 读消息:
    def read(self):
        # 使用 while 循环不断读取 bag 文件中的消息
        while self.reader.has_next():
            msg = self.reader.read_next()                                                           # 读取下一条消息, 返回一个元组 (topic name, serialized_msg, timestamp)
            get_logger("rclpy").info("topic = %s, time = %d, value=%s" % (msg[0], msg[2], msg[1]))  # 记录消息的详细信息, 包括话题名称、时间戳和消息内容

# 主函数
def main(args=None):
    # 2. 初始化 ROS 客户端:
    rclpy.init(args=args)       # 初始化 rclpy 库, 传入命令行参数

    # 4. 调用 spin 函数, 并传入对象:
    reader = SimpleBagPlayer()  # 创建 SimpleBagPlayer 对象
    reader.read()               # 调用 read 方法, 开始读取 bag 文件中的消息
    rclpy.spin(reader)          # 使用 rclpy.spin 方法使节点保持活跃, 以便处理回调函数

    # 5. 释放资源:
    rclpy.shutdown()            # 关闭 rclpy 库, 释放资源

if __name__ == '__main__':
    main()