from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess
# 将命令封装为对象
from launch.substitutions import FindExecutable

def generate_launch_description():

    turtle = Node(
            package = "turtlesim",
            executable = "turtlesim_node"
            )
    # 封装指令
    cmd = ExecuteProcess(
            #cmd = ["ros2 topic echo /turtle1/pose"],
            cmd = [FindExecutable(name="ros2"),"topic", "echo", "/turtle1/pose"],
            output = "both",
            shell = True
            )

    return LaunchDescription([turtle,cmd])
