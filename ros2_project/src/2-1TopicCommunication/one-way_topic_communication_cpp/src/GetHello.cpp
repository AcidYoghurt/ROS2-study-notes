#include <std_msgs/msg/string.hpp>
#include <functional>
#include <rclcpp/logging.hpp>
#include <rclcpp/node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/subscription.hpp>
#include <std_msgs/msg/string.hpp>
#include <string>

class GetHello : public rclcpp::Node{
public:
    GetHello(std::string name) : rclcpp::Node(name){
        // 创建一个订阅者
        get_hello = this->create_subscription<std_msgs::msg::String>("/HelloWorld",10,std::bind(&GetHello::SubHello,this,std::placeholders::_1));
    }

private:
    // 订阅者所需的回调函数
    void SubHello(std_msgs::msg::String msg){
        RCLCPP_INFO(this->get_logger(),"订阅者接收到消息：%s",msg.data.c_str());
    }

    // 声明一个订阅者指针
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr get_hello;
};

int main(int argc, char ** argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<GetHello>("SubHello");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}