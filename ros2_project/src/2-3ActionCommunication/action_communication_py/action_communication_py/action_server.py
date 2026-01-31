import time

import rclpy
from rclpy.node import Node

# ActionServer：Action 服务端封装
# GoalResponse：是否接受目标
# CancelResponse：是否接受取消请求
from rclpy.action import ActionServer, GoalResponse, CancelResponse

# 引入自定义的Action接口
from action_interfaces.action import Fibonacci


class FibonacciActionServer(Node):
    """
    Fibonacci Action 服务端节点

    这个节点的职责是：
    - 接收客户端发送的 Goal（目标）
    - 执行任务（这里是计算斐波那契数列）
    - 在执行过程中发布 Feedback（反馈）
    - 在结束时返回 Result（结果）
    - 支持任务取消
    """

    def __init__(self):
        super().__init__('fibonacci_action_server')

        # 创建一个 ActionServer
        # 这一行代码内部会自动创建：
        # 1. 目标传递服务（Send Goal）
        # 2. 结果传递服务（Get Result）
        # 3. 取消执行服务（Cancel Goal）
        # 4. 反馈话题（Feedback Topic）
        # 5. 状态话题（Status Topic）
        self._action_server = ActionServer(
            self,
            Fibonacci,                                  # Action 类型
            'fibonacci',                                # Action 名称
            execute_callback=self.execute_callback,     # 执行任务回调
            goal_callback=self.goal_callback,           # 目标传递服务回调
            cancel_callback=self.cancel_callback        # 取消任务回调
        )

    # 执行任务的回调函数
    # 这里面包含了 ：
    # - Action执行 结果传递服务 服务端回调
    # - Action执行 反馈话题 发布者发布
    async def execute_callback(self, goal_handle):
        """
        Action 的核心执行函数

        这个函数：
        - 运行在服务端
        - 执行耗时任务
        - 通过 goal_handle 发布反馈
        - 最终返回 Result
        """
        self.get_logger().info('开始执行目标任务')

        feedback_msg = Fibonacci.Feedback() # 构造 Feedback 消息对象
        result = Fibonacci.Result()         # 构造 Result 消息对象
        sequence = [0, 1]                   # 初始化斐波那契数列

        # 根据 Goal 中的参数决定计算长度
        for i in range(2, goal_handle.request.order):
            # 检查是否有取消请求
            if goal_handle.is_cancel_requested:
                goal_handle.canceled()                  # 标记该 Goal 已被取消
                self.get_logger().info('目标任务已被取消')
                return Fibonacci.Result()               # 返回一个空结果
            
            # 任务没有被取消
            else:
                # 计算下一个斐波那契数
                sequence.append(sequence[i - 1] + sequence[i - 2])

                # 反馈话题（Feedback Topic）
                # 将当前计算进度作为反馈发送给客户端
                feedback_msg.partial_sequence = sequence    # 构造Feedback类型的消息
                goal_handle.publish_feedback(feedback_msg)  # 发布者发布反馈信息
                time.sleep(1)                               # 模拟耗时操作

        # 结果传递服务（Get Result）
        goal_handle.succeed()                                   # 任务正常完成，标记为成功
        result.sequence = sequence                              # 构造Result类型的消息
        self.get_logger().info('目标任务执行完成，返回最终结果')
        return result                                           # 返回 Result（客户端通过 get_result_async() 获取）

    # 目标传递服务 中 服务端的回调函数
    def goal_callback(self, goal_request):
        """
        当客户端发送 Goal 时调用

        参数：
          - goal_request：客户端发送的目标参数（Goal）
        返回值：
          - GoalResponse.ACCEPT：接受该目标
          - GoalResponse.REJECT：拒绝该目标
        """
        self.get_logger().info(f'收到新的目标请求，order = {goal_request.order}')

        # 这里直接无条件接受
        return GoalResponse.ACCEPT

    # 取消执行服务服务 中 服务端的回调函数
    def cancel_callback(self, goal_handle):
        """
        当客户端请求取消 Goal 时调用

        参数：
          - goal_handle：当前正在执行的目标句柄
        返回值：
          - CancelResponse.ACCEPT：接受取消
          - CancelResponse.REJECT：拒绝取消
        """
        self.get_logger().info('收到取消目标的请求')
        return CancelResponse.ACCEPT                # 允许客户端取消任务，此时goal_handle中的is_cancel_requested属性会变为true
        #return CancelResponse.REJECT               # 拒绝客户端取消任务，此时goal_handle中的is_cancel_requested属性不会变

def main():
    rclpy.init()
    node = FibonacciActionServer()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()
