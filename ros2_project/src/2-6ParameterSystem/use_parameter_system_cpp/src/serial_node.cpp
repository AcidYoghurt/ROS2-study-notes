#include <rclcpp/rclcpp.hpp>

class SerialNode : public rclcpp::Node
{
public:
  SerialNode() : Node("serial_node")
  {
    // 声明参数并设置默认值
    this->declare_parameter<std::string>("port", "/dev/ttyUSB0");
    this->declare_parameter<int>("baudrate", 115200);
    this->declare_parameter<int>("timeout_ms", 10);

    // 获取参数
    port = this->get_parameter("port").as_string();
    baudrate = this->get_parameter("baudrate").as_int();
    timeout_ms = this->get_parameter("timeout_ms").as_int();

    // 打印日志
    RCLCPP_INFO(this->get_logger(),"获取到参数: port:%s, baudrate:%d, timeout_ms:%d",port.c_str(),baudrate,timeout_ms);
  }

private:
  std::string port;
  int baudrate;
  int timeout_ms;
};

int main(int argc, char ** argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SerialNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}