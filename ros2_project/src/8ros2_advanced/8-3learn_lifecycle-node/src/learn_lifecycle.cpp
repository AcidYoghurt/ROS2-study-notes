#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class LearnLifeCycleNode : public rclcpp_lifecycle::LifecycleNode
{
public:
  LearnLifeCycleNode() :
    rclcpp_lifecycle::LifecycleNode("lifecyclenode"),  // 初始化节点
    timer_period_(1.0)                                    // 初始化
  {
    RCLCPP_INFO(get_logger(), "%s: 已创建", get_name());
  }

protected:
  CallbackReturn on_configure(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "on_configure(): 创建定时器资源");

    timer_ = this->create_wall_timer(
        std::chrono::seconds(static_cast<int>(timer_period_)),
        std::bind(&LearnLifeCycleNode::timer_callback, this)
      );

    timer_->cancel();  // 先不启动

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "on_activate(): 启动定时器");

    if (timer_) {
      timer_->reset();  // 启动定时器
    }

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "on_deactivate(): 停止定时器");

    if (timer_) {
      timer_->cancel();  // 停止但不销毁
    }

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "on_cleanup(): 释放资源");

    timer_.reset();  // 真正销毁资源

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "on_shutdown(): 最终清理");

    timer_.reset(); // 运行到结束时调用的是on_shutdown，所以必须要释放资源

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_error(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_ERROR(get_logger(),"on_error(): 从状态 [%s] 进入错误处理阶段",previous_state.label().c_str());

    // 防御式停止定时器
    if (timer_) {
      try {
        timer_->cancel();
      } catch (...) {
        RCLCPP_WARN(get_logger(), "取消定时器时发生异常");
      }
    }

    // 尝试释放资源
    timer_.reset();

    RCLCPP_INFO(get_logger(), "错误处理完成，返回 unconfigured 状态");

    // 返回 SUCCESS -> 回到 unconfigured
    return CallbackReturn::SUCCESS;
  }

private:
  // 定时器回调函数
  void timer_callback()
  {
    RCLCPP_INFO(get_logger(), "定时器打印进行中...");
  }

  rclcpp::TimerBase::SharedPtr timer_;
  double timer_period_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<LearnLifeCycleNode>();

  rclcpp::spin(node->get_node_base_interface());

  rclcpp::shutdown();
  return 0;
}