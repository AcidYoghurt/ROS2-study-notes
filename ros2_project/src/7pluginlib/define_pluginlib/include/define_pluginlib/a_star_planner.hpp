#pragma once    // 头文件保护，用宏定义让文件只编译一次，防止同一个代码里面包含了多个 planner_base.hpp
#include <define_pluginlib/planner_base.hpp>

namespace plan_system
{
    class AStarPlanner : public PlannerBase
    {
    public:
        void initialize() override;
        void plan() override;
        ~AStarPlanner() override;
    };
}