from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

"""
在launch文件启动时候，动态修改turtlesim的背景颜色
实现
   1. 声明变量
   2. 调用参数
   3. 执行launch文件时动态导入参数
"""

def generate_launch_description():

    bg_r = DeclareLaunchArgument(name="backg_r",default_value="255")

    turtle = Node(
            package = "turtlesim",
            executable="turtlesim_node",
            parameters=[{"background_r": LaunchConfiguration("backg_r")}]
            )

    return LaunchDescription([bg_r, turtle])
