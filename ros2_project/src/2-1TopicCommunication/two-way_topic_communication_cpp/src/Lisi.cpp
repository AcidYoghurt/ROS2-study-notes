#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"
#include <functional>

class TopicPublisher : public rclcpp::Node
{
public:
    // 构造函数，有一个参数为节点名称
    TopicPublisher(std::string name) : Node(name) // 有参构造函数
    {
        // 先来个自我介绍吧（打印日志）
        RCLCPP_INFO(this->get_logger(), "大家好，我是%s。", name.c_str());

        // 创建发布者
        pub_novel = this->create_publisher<std_msgs::msg::String>("novel", 10);
        pub_novel_timer = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&TopicPublisher::PubNovel, this)); // 创建定时器，500ms为周期，定时发布

        // 创建订阅者
        money.data = 80; // 声明初始的稿费
        sub_money = this->create_subscription<std_msgs::msg::UInt32>("novel_money", 10, std::bind(&TopicPublisher::SubMoney, this, std::placeholders::_1));
    }

private:
    // 声明话题发布者指针
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_novel;
    // 声明定时器指针
    rclcpp::TimerBase::SharedPtr pub_novel_timer;

    // 声明总的稿费
    std_msgs::msg::UInt32 money;
    // 声明一个订阅者
    rclcpp::Subscription<std_msgs::msg::UInt32>::SharedPtr sub_money;

    // 计时器回调函数（里面写发布内容）
    void PubNovel()
    {
        // 创建消息
        std_msgs::msg::String novel;
        novel.data = "小说";

        // 日志打印
        RCLCPP_INFO(this->get_logger(), "我已经发布了：%s", novel.data.c_str());

        // 发布消息
        pub_novel->publish(novel);
    }

    // 收钱的回调函数（接收者）
    void SubMoney(const std_msgs::msg::UInt32::SharedPtr sub_money)
    {
        money.data += sub_money->data;
        RCLCPP_INFO(this->get_logger(), "我已经收到%d元稿费，现在我的稿费总共有%d元", sub_money->data, money.data);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);                               // 初始化 rclcpp
    auto node = std::make_shared<TopicPublisher>("Lisi");   // 新建一个节点
    rclcpp::spin(node);                           // 保持节点运行，检测到接收时退出指令
    rclcpp::shutdown();                                    // rcl 关闭
    return 0;
}
