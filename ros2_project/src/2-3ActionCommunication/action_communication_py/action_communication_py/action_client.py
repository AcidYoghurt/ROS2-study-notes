import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient

from action_interfaces.action import Fibonacci

class FibonacciActionClient(Node):
    # 构造函数
    def __init__(self):
        super().__init__('fibonacci_action_client')

        # 创建Action客户端
        self._client = ActionClient(self, Fibonacci, 'fibonacci')

        # 等待 Action 服务端上线
        # 如果Action服务端还没启动，这里会阻塞等待
        self._client.wait_for_server()

        # 构建一个Goal消息
        # Fibonacci.Goal 对应 .action 文件中的 Goal 部分
        goal_msg = Fibonacci.Goal()
        goal_msg.order = 10
        
        # 异步发送 Goal（即 目标传递服务 中 客户端发送请求给服务端）
        self._send_goal_future = self._client.send_goal_async(
            goal_msg,                                           # 请求的内容
            feedback_callback=self.feedback_callback            # 注册反馈回调函数（反馈话题的回调函数）
        )

        # 注册 目标传递服务 响应回调函数
        # 当 Action 服务端对 Goal 作出“是否接受”的决定后，该 future 完成，并自动调用 goal_response_callback
        self._send_goal_future.add_done_callback(self.goal_response_callback)

    # 反馈话题 的回调函数
    # 当服务端在 execute_callback 中调用 publish_feedback 时触发
    def feedback_callback(self, feedback_msg):
        self.get_logger().info(f'接受服务端反馈: {feedback_msg.feedback.partial_sequence}')

    # 目标传递服务 中 客户端接收服务端数据的回调函数
    def goal_response_callback(self, future):
        # 获取futre中的内容
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().info('目标被拒绝！')
            return
        else:
            self.get_logger().info('目标被接受！')

            # 结果传递服务（Get Result）
            # 向 结果传递服务 发送请求，请求该 Goal 的最终执行结果
            self._get_result_future = goal_handle.get_result_async()                # 向 结果传递服务 发出请求（异步调用），get_result_async函数会返回一个future对象
            self._get_result_future.add_done_callback(self.get_result_callback)     # 注册 结果传递服务 的响应回调函数。当服务端处理完毕后的结果返回时，自动调用 get_result_callback函数

    # 结果传递服务 中 客户端接收服务端数据的回调函数
    def get_result_callback(self, future):
        result = future.result().result
        self.get_logger().info(f'结果: {result.sequence}')


def main():
    rclpy.init()
    node = FibonacciActionClient()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
