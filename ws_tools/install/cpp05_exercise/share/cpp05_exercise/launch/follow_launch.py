from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # 1. 启动 turtlesim_node 节点
    turtle = Node(package="turtlesim",executable="turtlesim_node");

    # 2. 启动自定义的 spawn 节点
    spawn = Node(package="cpp05_exercise",executable="exer01_tf_spawn",parameters=[{"turtle_name":"t2"}])
    
    # 分别广播两只乌龟相对于world的坐标变换
    broadcaster1 = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="broa1")
    broadcaster2 = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="broa2",parameters=[{"turtle": "t2"}])
    
    # 4. 创建监听节点
    listener = Node(package="cpp05_exercise", executable="exer03_tf_listener", parameters=[{"father_frame": "t2","child_frame": "turtle1"}])

    return LaunchDescription([turtle,spawn,broadcaster1,broadcaster2,listener])
