#include <rclcpp/rclcpp.hpp>

class TestNode1 : public rclcpp::Node
{
public:
    TestNode1():rclcpp::Node("TestNode1")
    {
        RCLCPP_INFO(this->get_logger(),"你好世界1");
    }
};

int main(int argc,char **argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<TestNode1>();
    rclcpp::spin(node);
    rclcpp::shutdown();
}