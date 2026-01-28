#include <rclcpp/rclcpp.hpp>
#include <boost/filesystem.hpp>

class UseBoostNode : public rclcpp::Node
{
public:
    UseBoostNode() : Node("use_boost_node")
    {
        boost::filesystem::path dir("ros2_boost_demo");

        if (!boost::filesystem::exists(dir)) {
            RCLCPP_INFO(get_logger(), "目录不存在，正在创建该目录...");
            boost::filesystem::create_directories(dir);
        } else {
            RCLCPP_INFO_THROTTLE(get_logger(),*this->get_clock(),1000, "该目录已存在！");
        }
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<UseBoostNode>());
    rclcpp::shutdown();
    return 0;
}
