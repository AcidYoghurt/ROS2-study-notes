from launch import LaunchDescription
from launch_ros.actions import Node

# 定义要启动的节点
def test_node():
    test_node1 = Node(
        package='test_launch_cpp',
        executable='test_node1',
        name="test_node1",
        output='both',
    )

    test_node2 = Node(
        package='test_launch_cpp',
        executable='test_node2',
        name="test_node2",
        output='both',
    )

    return [test_node1,test_node2]


def generate_launch_description():
    test_node_ = test_node()

    # 返回 LaunchDescription
    return LaunchDescription(test_node_)
