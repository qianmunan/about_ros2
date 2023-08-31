# 实现乌龟护航案例
"""
需求： 实现乌龟护航案例
    先实现一只乌龟（back）护航，需求back最终位置处于turtle1的正后方两米处
思路：
    1. 发布目标点相对于turtle1的坐标变换
    2. 监听back与目标点坐标的相对位置嘎u纳西
流程：
    1. 抽取参数
    2. 创建turtlesim_node节点，并生成新乌龟
    3. 发布坐标变换
    4. 要监听坐标变换
"""
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration


def generate_launch_description():

    #1. 抽取参数
    escort_back = DeclareLaunchArgument(name="turtle_back",default_value="turtle_back");
    escort_left = DeclareLaunchArgument(name="turtle_left",default_value="turtle_left")
    escort_right = DeclareLaunchArgument(name="turtle_right",default_value="turtle_right")
    
    #2. 创建turtlesim_node节点，并生成新乌龟
    
    master = Node(package="turtlesim", executable="turtlesim_node")
    
    spawn_back = Node(package="cpp05_exercise",executable="exer01_tf_spawn",name="spawn_back",parameters=[{"x": 2.0,"y": 5.0,"turtle_name": LaunchConfiguration("turtle_back")}])
    
    spawn_left = Node(package="cpp05_exercise",executable="exer01_tf_spawn",name="spawn_left",parameters=[{"x":3.0,"y":9.0,"turtle_name":LaunchConfiguration("turtle_left")}])
    
    spawn_right = Node(package="cpp05_exercise",executable="exer01_tf_spawn",name="spawn_right",parameters=[{"x":4.0,"y":2.0,"turtle_name":LaunchConfiguration("turtle_right")}])
    
    #3. 发布坐标变换
    turtle1_world = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="turtle1_world")

    back_world = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="back_world",
                      parameters=[{"turtle": LaunchConfiguration("turtle_back")}])
    left_world = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="left_world",
                      parameters=[{"turtle": LaunchConfiguration("turtle_left")}]
                      )
    right_world = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="right_world",
                       parameters=[{"turtle": LaunchConfiguration("turtle_right")}]
                       )
    escort_goal_back = Node(package="tf2_ros",executable="static_transform_publisher",name="escort_goal_back",
                            arguments=["--frame-id","turtle1","--child-frame-id","escort_goal_back","--x","-1.5"]
                            )
    escort_goal_left = Node(package="tf2_ros",executable="static_transform_publisher",name="escort_left",
                            arguments=["--frame-id","turtle1","--child-frame-id","escort_goal_left","--x","1.5"]
                            )
    escort_goal_right = Node(package="tf2_ros",executable="static_transform_publisher",name="escort_goal_right",
                             arguments=["--frame-id","turtle1","--child-frame-id","escort_goal_right","--x","-1.5"]
                             )

    #4. 要监听坐标变换
    back_escort_goal_back = Node(package="cpp05_exercise", executable="exer03_tf_listener", name="back_escort_goal_back",parameters=[{"father_frame":LaunchConfiguration("turtle_back"),"child_frame":"escort_goal_back"}])

    left_escort_goal_left = Node(package="cpp05_exercise",executable="exer03_tf_listener",name="left_escort_goal_left",parameters=[{"father_frame":LaunchConfiguration("turtle_left"),"child_frame":"escort_goal_left"}])

    right_escort_goal_right = Node(package="cpp05_exercise",executable="exer03_tf_listener",name="right_escort_goal_right",parameters=[{"father_frame":LaunchConfiguration("turtle_right"),"child_frame":"escort_goal_right"}])

    

    return LaunchDescription([escort_back,escort_left,escort_right,
                              master,spawn_back,spawn_left,spawn_right,
                              turtle1_world,back_world,left_world,right_world,
                              escort_goal_back,escort_goal_left,escort_goal_right,
                              back_escort_goal_back,left_escort_goal_left,right_escort_goal_right])
