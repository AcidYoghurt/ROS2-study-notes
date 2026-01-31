#include <rclcpp/logging.hpp>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/node.hpp>
#include <custom_msg_interfaces/msg/test.hpp>


class TestCustomMsgNode : public rclcpp::Node
{
public:
    TestCustomMsgNode(std::string name):rclcpp::Node(name)
    {
        // 构建一个自定义消息类型的变量
        custom_msg_interfaces::msg::Test test_msg;
        test_msg.header.frame_id = "base_link";
        test_msg.header.stamp = this->get_clock()->now();
        test_msg.flag = true;

        // DEBUG自定义消息
        RCLCPP_INFO(this->get_logger(),"测试自定义消息：\ntest_msg.header.frame_id: %s\ntest_msg.header.stamp: %d\ntest_msg.flag: %d",test_msg.header.frame_id.c_str(),test_msg.header.stamp.sec,test_msg.flag);
    }
};

int main(int argc, char ** argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TestCustomMsgNode>("TestCustomMsgNode");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}