#include "rclcpp/rclcpp.hpp"
#include "custom_srv_interfaces/srv/novel.hpp"

class AddTwoIntsServer : public rclcpp::Node
{
public:
  AddTwoIntsServer() : Node("add_two_ints_server")
  {
    // 创建一个服务端
    service_ = this->create_service<custom_srv_interfaces::srv::Novel>("add_two_ints",std::bind(&AddTwoIntsServer::handle_service, this, std::placeholders::_1, std::placeholders::_2));

    RCLCPP_INFO(this->get_logger(), "AddTwoInts服务准备就绪！");
  }

private:
  // 服务端回调函数
  void handle_service(
    const std::shared_ptr<custom_srv_interfaces::srv::Novel::Request> request,
    std::shared_ptr<custom_srv_interfaces::srv::Novel::Response> response)
  {
    // 日志
    RCLCPP_INFO(this->get_logger(),"接收到客户端发来的请求: a=%ld, b=%ld",request->a, request->b);

    // 处理逻辑
    response->sum = request->a + request->b;
  }

  // 声明一个服务端
  rclcpp::Service<custom_srv_interfaces::srv::Novel>::SharedPtr service_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AddTwoIntsServer>());
  rclcpp::shutdown();
  return 0;
}
