#pragma once

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

namespace learn_compose {

class Talker : public rclcpp::Node {
public:
  explicit Talker(const rclcpp::NodeOptions &options);

private:
  int32_t count_;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

} // namespace learn_compose
