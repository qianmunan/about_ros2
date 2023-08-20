from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

import os
from ament_index_python import get_package_share_directory

def generate_launch_description():
    include_launch = IncludeLaunchDescription(
            launch_description_source = PythonLaunchDescriptionSource(
                launch_file_path=os.path.join(
                    get_package_share_directory("cpp01_launch"),
                    "launch",
                    "py04_args_launch.py"
                    )
                ),
            launch_arguments={
                "background_r": "200",
                "background_g": "100",
                "background_b": "70",
                }.items()
            )

    return LaunchDescription([include_launch])
