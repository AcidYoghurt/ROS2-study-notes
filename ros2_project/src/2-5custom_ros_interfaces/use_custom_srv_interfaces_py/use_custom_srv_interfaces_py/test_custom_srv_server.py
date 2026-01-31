import rclpy
from rclpy.node import Node
from custom_srv_interfaces.srv import Novel

class AddTwoIntsServer(Node):

    def __init__(self):
        super().__init__('add_two_ints_server')

        # 创建服务端
        self.service = self.create_service(Novel,'add_two_ints',self.handle_service)

        self.get_logger().info('AddTwoInts 服务准备就绪！')

    # 服务端回调函数
    def handle_service(self, request, response):
        self.get_logger().info(f'接收到客户端请求: a={request.a}, b={request.b}')

        response.sum = request.a + request.b
        return response


def main(args=None):
    rclpy.init(args=args)
    node = AddTwoIntsServer()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()
