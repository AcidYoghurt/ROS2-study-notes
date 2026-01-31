#include "rclcpp/rclcpp.hpp"
#include <srv_interfaces/srv/add_two_ints.hpp>

using namespace std::chrono_literals;

class AddTwoIntsClient : public rclcpp::Node
{
public:
    AddTwoIntsClient(): Node("add_two_ints_client")
    {
        client_ = this->create_client<srv_interfaces::srv::AddTwoInts>("/add_two_ints");
        int a = 3;
        int b = 5;
        async_send_request(a, b);
    }

private:
    // 异步发送请求
    void async_send_request(int a, int b)
    {
        // 等待服务端上线（这里是阻塞的，但只发生在构造阶段，一般可接受）
        while (!client_->wait_for_service(1s))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_ERROR(
                    this->get_logger(),
                    "等待服务端上线时，客户端被中断");
                return;
            }
            RCLCPP_INFO(this->get_logger(), "正在等待服务端上线");
        }

        auto request =std::make_shared<srv_interfaces::srv::AddTwoInts::Request>();
        request->a = a;
        request->b = b;

        // 真正的异步：注册回调
        // 当客户端接收到服务端返回的请求时，才进入回调函数处理服务端发送过来的数据
        client_->async_send_request(request,std::bind(&AddTwoIntsClient::response_callback,this,std::placeholders::_1));
    }

    // service 响应回调
    void response_callback(rclcpp::Client<srv_interfaces::srv::AddTwoInts>::SharedFuture future)
    {
        auto response = future.get();
        RCLCPP_INFO(this->get_logger(),"服务端响应: %ld",response->sum);
    }

private:
    rclcpp::Client<srv_interfaces::srv::AddTwoInts>::SharedPtr client_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<AddTwoIntsClient>();
    rclcpp::spin(node);     // 只 spin，不阻塞等待 future
    rclcpp::shutdown();
    return 0;
}
