#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"
#include <functional>

// 创建一个类节点，名字叫做SingleDogNode，继承自Node
class SingleDogNode : public rclcpp::Node
{
public:
    // 构造函数，有一个参数为节点名称
    SingleDogNode(std::string name) : Node(name) // 有参构造函数
    {
        // 打印一句自我介绍
        RCLCPP_INFO(this->get_logger(), "大家好，我是单身狗%s。", name.c_str());

        // 创建一个订阅者来订阅李四的小说，话题名为novel
        sub_novel = this->create_subscription<std_msgs::msg::String>("novel", 10, std::bind(&SingleDogNode::topic_callback, this, std::placeholders::_1));

        // 创建一个发布者来给稿费，话题名为novel_money
        pub_money = this->create_publisher<std_msgs::msg::UInt32>("novel_money", 10);
    }

private:
    // 声明一个订阅者（成员变量），用于订阅小说
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_novel;

    // 声明一个发布者（成员变量），用于订阅完小说后付钱
    rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr pub_money;

    // 收到话题数据的回调函数
    void topic_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        // 1. 订阅者接收数据（接收数据后发布消息）
        RCLCPP_INFO(this->get_logger(), "朕已阅：'%s'", msg->data.c_str());

        // 2. 订阅者发布数据（这里是订阅者看完小说，给发布者打钱，其实就是发布者）
        // 逻辑贯通：老王接收到李四的小说后，就应该给他稿费
        std_msgs::msg::UInt32 money;
        money.data = 10;
        pub_money->publish(money);
        RCLCPP_INFO(this->get_logger(), "我已经发送%d元稿费", money.data);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);                                   // 初始化rclcpp
    auto node = std::make_shared<SingleDogNode>("LaoWang");     // 新建一个节点
    rclcpp::spin(node);                                // 保持节点运行，检测时候收到退出指令
    rclcpp::shutdown();                                         // rcl关闭
    return 0;
}