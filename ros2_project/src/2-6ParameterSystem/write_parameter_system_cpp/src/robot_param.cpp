#include "rclcpp/rclcpp.hpp"

class ParamServerNode : public rclcpp::Node
{
public:
  ParamServerNode() : Node("param_server_node")
  {
    // 声明参数（相当于提供参数）
    this->declare_parameter<double>("robot_speed", 1.0);

    // 注册参数修改回调
    callback_handle_ = this->add_on_set_parameters_callback(
      std::bind(&ParamServerNode::parameters_callback, this, std::placeholders::_1)
    );

    RCLCPP_INFO(this->get_logger(), "参数服务器准备就绪");
  }

private:
  // 参数修改回调函数
  // 参数 parameters 是一个vector类型，包含本次参数修改请求中的所有参数（参数名 + 新值 + 类型）。这些参数还未正式写入系统，当前回调用于决定是否允许修改。
  // 如果返回的 result.successful==true，那么系统就会修改参数
  // 如果返回的 result.successful==false，那么系统就不修改参数
  rcl_interfaces::msg::SetParametersResult parameters_callback(const std::vector<rclcpp::Parameter> & parameters)
  {
    rcl_interfaces::msg::SetParametersResult result;

    for (const auto & param : parameters)
      if (param.get_name() == "robot_speed")
      {
        double value = param.as_double();
        if (value < 0)
        {
          result.successful = false;
          result.reason = "robot_speed 值在必须 大于0";
          RCLCPP_INFO(this->get_logger(), "%s 参数非法，不修改",param.get_name().c_str());
        }
        else
        {
          result.successful = true;
          RCLCPP_INFO(this->get_logger(), "%s 参数改变为：%.2f",param.get_name().c_str(), value);
        }
      }

    return result;
  }

  OnSetParametersCallbackHandle::SharedPtr callback_handle_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ParamServerNode>());
  rclcpp::shutdown();
  return 0;
}