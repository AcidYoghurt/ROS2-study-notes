#pragma once    // 头文件保护，用宏定义让文件只编译一次，防止同一个代码里面包含了多个 planner_base.hpp

// ifdef 和 define是旧的头文件保护，现在都用pragam
// #ifndef PLANNER_BASE_HPP  // 如果 PLANNER_BASE_HPP 没有被定义
// #define PLANNER_BASE_HPP  // 定义 PLANNER_BASE_HPP 这个宏

namespace plan_system
{
    class PlannerBase
    {
    public:
        virtual void initialize() = 0;
        virtual void plan() = 0;
        virtual ~PlannerBase() = default;
    };
}

// #endif

