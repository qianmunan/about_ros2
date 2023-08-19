"""
需求： 演示Node的使用
:param: package    被执行的程序所属的功能包
:param: executable 可执行程序
:param: name       节点名称
:param: namespace  设置命名空间
:param: remappings 实现话题重映射
:param: arguments  为节点传参
                   xx yy zz --ros-args
:param: ros_arguments 为节点传参
                   --ros-args xx yy
:param: exec_name  设置程序标签
:param: parameters 设置参数
"""

from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    """
    turtle1 = Node(
            package = "turtlesim",
            executable = "turtlesim_node",
            exec_name = "my_label",
            ros_arguments = ["--remap","__ns:=/t2"],
            # ros2 run turtlesim turtlesim_node --ros-args --remap __ns:=/t1
            )
    """
    turtle2 = Node(
            package = "turtlesim",
            executable = "turtlesim_node",
            name = "haha",
            #parameters = [{"background_r": 255, "background_g": 0, "background_b": 0}]
            # 方式二： 读取yaml文件, 通过绝对路径读取
            parameters = [os.path.join(get_package_share_directory("cpp01_launch"), "config", "haha.yaml")]
            )


    return LaunchDescription([turtle2])
