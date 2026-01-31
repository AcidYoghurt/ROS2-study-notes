// 同步通信与异步通信的service是一样的
#include "rclcpp/rclcpp.hpp"
#include <srv_interfaces/srv/add_two_ints.hpp>

class AddTwoIntsServer : public rclcpp::Node
{
public:
  AddTwoIntsServer() : Node("add_two_ints_server")
  {
    // 创建一个服务端
    service_ = this->create_service<srv_interfaces::srv::AddTwoInts>("add_two_ints",std::bind(&AddTwoIntsServer::handle_service, this, std::placeholders::_1, std::placeholders::_2));

    RCLCPP_INFO(this->get_logger(), "AddTwoInts服务准备就绪！");
  }

private:
  // 服务端回调函数
  void handle_service(
    const std::shared_ptr<srv_interfaces::srv::AddTwoInts::Request> request,
    std::shared_ptr<srv_interfaces::srv::AddTwoInts::Response> response)
  {
    // 日志
    RCLCPP_INFO(this->get_logger(),"接收到客户端发来的请求: a=%ld, b=%ld",request->a, request->b);

    // 处理逻辑
    response->sum = request->a + request->b;
  }

  // 声明一个服务端
  rclcpp::Service<srv_interfaces::srv::AddTwoInts>::SharedPtr service_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AddTwoIntsServer>());
  rclcpp::shutdown();
  return 0;
}
