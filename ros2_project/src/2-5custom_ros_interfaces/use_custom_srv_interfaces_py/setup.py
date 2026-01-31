from setuptools import find_packages, setup

package_name = 'use_custom_srv_interfaces_py'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
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
            'AddTwoIntsServer=use_custom_srv_interfaces_py.test_custom_srv_server:main',
            'AddTwoIntsClient=use_custom_srv_interfaces_py.test_custom_srv_client:main'
        ],
    },
)
