/*
    需求：录制控制乌龟运动的速度指令
    流程：
        1. 包含头文件；
        2. 初始化ROS2客户端；
        3. 自定义节点类；
            3-1. 创建录制对象；
            3-2. 设置磁盘文件；
            3-3. 写数据（创建一个速度订阅方，回调函数中执行写出操作）
        4. 调用spin函数，并传入节点对象指针；
        5. 资源释放
*/

// 1. 包含头文件
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rosbag2_cpp/writer.hpp"

// 3. 自定义节点类
class simpleBagWriter : public rclcpp::Node
{
public:
    simpleBagWriter(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "消息录制对象创建！");
        
        // 3-1. 创建录制对象
        writer_ = std::make_unique<rosbag2_cpp::Writer>();
        
        // 3-2. 设置磁盘文件
        writer_->open("may_bag"); // 相对路径，是工作空间的直接子级
        
        // 3-3. 写数据（创建一个速度订阅方，回调函数中执行写出操作）
        // 参数：话题名称，队列中最大保存的数据数，回调函数
        sub_ = this->create_subscription<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10, std::bind(&simpleBagWriter::do_writer_msg, this, std::placeholders::_1));
    }

private:
    // 数据写入函数
    void do_writer_msg(std::shared_ptr<rclcpp::SerializedMessage> msg)
    {
        RCLCPP_INFO(this->get_logger(), "数据写出....");
        
        writer_->write(msg, "/turtle1/cmd_vel", "geometry_msgs/msg/Twist", this->now());
        /*
        参数：
        std::shared_ptr<const rclcpp::SerializedMessage> message, // 被写出的消息
        const std::string &topic_name,                            // 话题名称
        const std::string &type_name,                             // 消息类型
        const rclcpp::Time &time                                  // 时间戳
        */
    }

    std::unique_ptr<rosbag2_cpp::Writer> writer_; // 声明一个指向rosbag2_cpp::Writer对象的智能指针
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_; // 声明一个订阅者指针
};

// 主函数
int main(int argc, char *argv[])
{
    // 2. 初始化ROS2客户端
    rclcpp::init(argc, argv);

    // 4. 调用spin函数，并传入节点对象指针
    auto node = std::make_shared<simpleBagWriter>("simple_bag_play_node_cpp"); // 新建一个节点
    rclcpp::spin(node);

    // 5. 资源释放
    rclcpp::shutdown(); // 关闭ROS2客户端
    return 0;
}