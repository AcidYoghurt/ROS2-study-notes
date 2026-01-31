#include <rclcpp/rclcpp.hpp>

class TestNode2 : public rclcpp::Node
{
public:
    TestNode2():rclcpp::Node("TestNode2")
    {
        RCLCPP_INFO(this->get_logger(),"你好世界2");
    }
};

int main(int argc,char **argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<TestNode2>();
    rclcpp::spin(node);
    rclcpp::shutdown();
}