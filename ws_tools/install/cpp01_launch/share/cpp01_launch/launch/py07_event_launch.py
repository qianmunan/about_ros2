from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess,RegisterEventHandler,LogInfo
from launch.substitutions import FindExecutable
from launch.event_handlers import OnProcessStart, OnProcessExit

def generate_launch_description():

    turtle = Node(package="turtlesim", executable="turtlesim_node")
    spawn = ExecuteProcess(
            cmd = [
                FindExecutable(name="ros2"),

                ]
            )
