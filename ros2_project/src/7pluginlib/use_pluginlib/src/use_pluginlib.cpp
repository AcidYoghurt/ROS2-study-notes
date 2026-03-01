// 导入ROS2依赖
#include <rclcpp/rclcpp.hpp>
#include <pluginlib/class_loader.hpp>

// 导入插件接口
#include <define_pluginlib/planner_base.hpp>

class test_node : public rclcpp::Node
{
public:
    test_node() : Node("test")
    {
        // 使用ClassLoader
        loader = std::make_unique<pluginlib::ClassLoader<plan_system::PlannerBase>>(    // 模板参数(这里指的是plan_system::PlannerBase)：插件接口类型（所有插件必须继承的基类）
            "define_pluginlib",                                                         // 参数1：功能包名字
            "plan_system::PlannerBase"                                                  // 参数2：基类的名字
        );

        // 根据插件名字创建插件对象
        planner = loader->createSharedInstance("plan_system/AStarPlanner");

        // 调用插件里面的函数
        planner->initialize();
        planner->plan();
    }
private:
    std::unique_ptr<pluginlib::ClassLoader<plan_system::PlannerBase>> loader;
    std::shared_ptr<plan_system::PlannerBase> planner;
};


int main(int argc,char** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<test_node>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

