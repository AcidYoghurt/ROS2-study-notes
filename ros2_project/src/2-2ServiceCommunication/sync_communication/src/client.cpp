#include "rclcpp/rclcpp.hpp"
#include <srv_interfaces/srv/add_two_ints.hpp>

using namespace std::chrono_literals;

class AddTwoIntsClient : public rclcpp::Node{
public:
    AddTwoIntsClient():rclcpp::Node("add_two_ints_client")
    {
        // 创建一个客户端
        client = this->create_client<srv_interfaces::srv::AddTwoInts>("/add_two_ints");

        // 发送请求
        int a=3;
        int b=5;
        this->async_send_request(a,b);
    }

private:
    // 客户端发送请求的函数（注意这个不是回调函数）
    void async_send_request(int a,int b)
    {
        // 等待服务端上线
        while (!client->wait_for_service(1s)) 
        {
            if (!rclcpp::ok()) 
            {
                RCLCPP_ERROR(this->get_logger(), "等待服务端上线时，客户端被中断");
                return;
            }
            RCLCPP_INFO(this->get_logger(), "正在等待服务端上线");
        }

        // 构建请求消息
        auto request = std::make_shared<srv_interfaces::srv::AddTwoInts::Request>();
        request->a = a;
        request->b = b;

        // 客户端发送服务请求给服务端
        // async_send_request表示异步发送请求
        auto future = client->async_send_request(request);  
        
        // 阻塞线程，直到服务端响应客户端
        // 虽然发送请求写的是 call_async，但 spin_until_future_complete 让它变成了同步
        auto status = rclcpp::spin_until_future_complete(this->get_node_base_interface(), future);

        // 处理服务端响应
        if (status == rclcpp::FutureReturnCode::SUCCESS)
        {
            RCLCPP_INFO(this->get_logger(),"服务端响应: %ld + %ld = %ld",request->a, request->b, future.get()->sum);
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "呼叫服务失败！");
        }
    }

    // 声明一个客户端
    rclcpp::Client<srv_interfaces::srv::AddTwoInts>::SharedPtr client;
};


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<AddTwoIntsClient>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
