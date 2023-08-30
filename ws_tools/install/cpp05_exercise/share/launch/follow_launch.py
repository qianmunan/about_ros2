from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # 1. 启动 turtlesim_node 节点
    turtle = Node(package="turtlesim",executable="turtlesim_node");

    # 2. 启动自定义的 spawn 节点
    spawn = Node(package="cpp05_exercise",executable="exer01_tf_spawn",parameters=[{"turtle_name":"t2"}])

    return LaunchDescription([turtle])
