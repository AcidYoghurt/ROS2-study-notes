import rclpy
from rclpy.node import Node
from custom_srv_interfaces.srv import Novel

class AddTwoIntsClient(Node):

    def __init__(self):
        super().__init__('add_two_ints_client')

        # 创建客户端
        self.client = self.create_client(Novel, 'add_two_ints')

        # 等待服务端上线
        while not self.client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('正在等待服务端上线...')
            if not rclpy.ok():
                self.get_logger().error('等待服务端时被中断')
                return

        # 构建请求
        request = Novel.Request()
        request.a = 3
        request.b = 5

        # 发送请求（异步）
        future = self.client.call_async(request)

        # 等待响应
        rclpy.spin_until_future_complete(self, future)

        if future.done():
            response = future.result()
            self.get_logger().info(f'服务端响应: {request.a} + {request.b} = {response.sum}')
        else:
            self.get_logger().error('服务调用失败')


def main(args=None):
    rclpy.init(args=args)
    node = AddTwoIntsClient()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
