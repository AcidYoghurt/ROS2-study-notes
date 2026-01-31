#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

#include "action_interfaces/action/fibonacci.hpp"

using Fibonacci = action_interfaces::action::Fibonacci;
using GoalHandleFibonacci = rclcpp_action::ClientGoalHandle<Fibonacci>;

class FibonacciActionClient : public rclcpp::Node
{
public:
    // 构造函数
    FibonacciActionClient(): Node("fibonacci_action_client")
    {
        // 创建 Action 客户端
        this->client_ = rclcpp_action::create_client<Fibonacci>(this,"fibonacci");

        // 等待 Action 服务端上线
        // 如果 Action 服务端还没启动，这里会阻塞等待
        this->client_->wait_for_action_server();

        // 构建一个 Goal 消息
        // Fibonacci::Goal 对应 .action 文件中的 Goal 部分
        Fibonacci::Goal goal_msg;
        goal_msg.order = 10;

        // 异步发送 Goal（即 目标传递服务 中 客户端发送请求给服务端）
        rclcpp_action::Client<Fibonacci>::SendGoalOptions options;
        options.feedback_callback =std::bind(&FibonacciActionClient::feedback_callback,this,std::placeholders::_1,std::placeholders::_2);
        options.result_callback =std::bind(&FibonacciActionClient::result_callback,this,std::placeholders::_1);
        this->client_->async_send_goal(goal_msg, options);
    }

private:
    // 反馈话题 的回调函数
    // 当服务端在 execute_callback 中调用 publish_feedback 时触发
    void feedback_callback(GoalHandleFibonacci::SharedPtr goal_handle,const std::shared_ptr<const Fibonacci::Feedback> feedback)
    {
        RCLCPP_INFO(this->get_logger(),"接收服务端反馈: [%d, %d, %d ...]",feedback->partial_sequence[0],feedback->partial_sequence[1],feedback->partial_sequence.back());
    }

    // 结果传递服务 中 客户端接收服务端数据的回调函数
    void result_callback(const GoalHandleFibonacci::WrappedResult & result)
    {
        RCLCPP_INFO(this->get_logger(), "结果:");
        for (auto number : result.result->sequence) 
        {
            RCLCPP_INFO(this->get_logger(), "%d", number);
        }
    }

    // 声明一个Action服务端
    rclcpp_action::Client<Fibonacci>::SharedPtr client_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FibonacciActionClient>());
  rclcpp::shutdown();
  return 0;
}
