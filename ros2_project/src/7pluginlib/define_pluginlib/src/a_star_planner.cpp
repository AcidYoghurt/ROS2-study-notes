#include <iostream>
#include <define_pluginlib/planner_base.hpp>
#include <define_pluginlib/a_star_planner.hpp>

namespace plan_system
{
    void AStarPlanner::initialize()
    {
        std::cout<<"现在使用的是A*算法"<<std::endl;
    }
    void AStarPlanner::plan()
    {
        std::cout<<"开始执行导航算法"<< std::endl;
    }
    AStarPlanner::~AStarPlanner() = default;
}

// 导出为插件
#include <pluginlib/class_list_macros.hpp>
// 参数1：要导出的类
// 参数2：要导出的类的基类
PLUGINLIB_EXPORT_CLASS(plan_system::AStarPlanner, plan_system::PlannerBase)
