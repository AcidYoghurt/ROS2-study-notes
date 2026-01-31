import os
from glob import glob
from setuptools import find_packages, setup

package_name = 'test_launch_py'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        # 注册包
        ('share/ament_index/resource_index/packages',
         ['resource/' + package_name]),

        # 安装 package.xml
        ('share/' + package_name, ['package.xml']),

        # 安装 launch 文件
        (os.path.join('share', package_name, 'launch'), glob('launch/*.launch.py')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='miku',
    maintainer_email='1310946137@qq.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'test_node1=test_launch_py.test_node1:main',    # 节点1
            'test_node2=test_launch_py.test_node2:main',    # 节点2
        ],
    },
)
