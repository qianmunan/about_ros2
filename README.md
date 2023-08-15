# about_ros2
这是关于ROS2 humble 的一些学习记录
## 安装 🥰

<font color="red">请注意：虽然安装比较简单，但是安装过程比较耗时，需要耐心等待。</font>


### 设置语言环境
请先检查本地语言环境是否支持UTF-8编码，可调用如下指令检查并设置UTF-8编码：
``` bash
locale  # 检查是否支持 UTF-8

# 终端输出
LANG=en_US.UTF-8
LANGUAGE=
LC_CTYPE="en_US.UTF-8"
LC_NUMERIC=zh_CN.UTF-8
LC_TIME=zh_CN.UTF-8
LC_COLLATE="en_US.UTF-8"
LC_MONETARY=zh_CN.UTF-8
LC_MESSAGES="en_US.UTF-8"
LC_PAPER=zh_CN.UTF-8
LC_NAME=zh_CN.UTF-8
LC_ADDRESS=zh_CN.UTF-8
LC_TELEPHONE=zh_CN.UTF-8
LC_MEASUREMENT=zh_CN.UTF-8
LC_IDENTIFICATION=zh_CN.UTF-8
LC_ALL=

# 如果不是上述内容，请使用一下命令更正
# but 可能改变你电脑的语言环境
sudo apt update && sudo apt install locales
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8
```


<font color="green">注意：语言环境可以不同，但必须支持UTF-8编码。</font>


### 启动Ubuntu universe存储库
**图形化操作:**
请打开软件与更新(Software & Updates)窗口，确保启动了universe存储库，以保证可以下载”社区维护的免费和开源软件“，操作示例如下：


### 设置软件源
先将ROS 2 apt存储库添加到系统，用apt授权我们的GPG密钥：
```bash
sudo apt update && sudo apt install curl gnupg lsb-release
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
```
然后将存储库添加到源列表：
```bash
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(source /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
```

### 安装ROS2
首先更新apt存储库缓存：
``` bash
sudo apt update
```
然后升级已安装的软件(ROS2软件包建立在经常更新的Ubuntu系统上，在安装新软件包之前请确保您的系统是最新的)：
``` bash
sudo apt upgrade
```
安装桌面版ROS2(建议)，包含：ROS、RViz、示例与教程，安装命令如下：
``` bash
sudo apt install ros-humble-desktop
```

### 配置环境
终端下，执行ROS2程序时，需要调用如下命令配置环境：
```bash
source /opt/ros/humble/setup.bash
```
每次新开终端时，都得执行上述命令，或者也可以执行如下命令，将配置环境指令写入 ”~/.bashrc“ 文件，那么每次新启动终端时，不需要在手动配置环境：
``` bash
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
```
到目前为止，ROS2就已经安装且配置完毕了。

### 此外
为了我们方便在终端中调试和编译我们的项目，再此还需要安装一个可以分栏的终端模拟器和colco构建工具
#### 安装colcon构建工具
colcon是一个命令行工具，用于改进编译，测试和使用多个软件包的工作流程。它实现过程自动化，处理需求并设置环境以便于使用软件包。ROS2中便是使用colcon作为包构建工具的，但是ROS2中没有默认安装colcon，需要自行安装，安装命令如下：
```bash
sudo apt install python3-colcon-common-extensions
```
#### 安装终端模拟器 terminator
```bash
# 安装
sudo apt install terminator
```

## 验证 😜
在ROS2中已经内置了一些案例，安装完毕之后，就可以运行这些案例，以测试ROS2的安装与配置是否正常，在此，我们选用ROS2内置的小乌龟案例，具体操作如下。

1.打开两个终端(可以使用快捷键Ctrl + Alt + T)；

2.终端1中输入指令：`ros2 run turtlesim turtlesim_node`，执行完毕，会启动一个绘有小乌龟的窗口；

3.终端2中输入指令：`ros2 run turtlesim turtle_teleop_key`，执行完毕，可以在此终端中通过键盘控制乌龟运动。

运行结果示例如下：



<font color="red">如果想要让小乌龟动起来，请将光标放在第二条命令所在的终端下，进行方向的键入</font>

## 第一个项目 🤔
### 创建功能包
终端下，进入`first_helloworld/src`目录，使用如下指令创建一个C++功能包：
```bash
ros2 pkg create pkg01_helloworld_cpp --build-type ament_cmake --dependencies rclcpp --node-name helloworld
```
执行完毕，在src目录下将生成一个名为`pkg01_helloworld_cpp`的目录，且目录中已经默认生成了一些子级文件与文件夹。

### 编辑源文件
进入`pkg01_helloworld_cpp/src`目录，该目录下有一`helloworld.cpp`文件，修改文件内容如下：
```cpp
#include "rclcpp/rclcpp.hpp"

int main(int argc, char ** argv)
{
  // 初始化 ROS2
  rclcpp::init(argc,argv);
  // 创建节点
  auto node = rclcpp::Node::make_shared("helloworld_node");
  // 输出文本
  RCLCPP_INFO(node->get_logger(),"hello world!");
  // 释放资源
  rclcpp::shutdown();
  return 0;
}
```

### 编辑配置文件
在步骤1创建功能包时所使用的指令已经默认生成且配置了配置文件，不过实际应用中经常需要自己编辑配置文件，所以在此对相关内容做简单介绍，所使用的配置文件主要有两个，分别是功能包下的package.xml与CMakeLists.txt。
#### 1.package.xml
文件内容如下：

```xml
<?xml version="1.0"?>
<?xml-model href="http://download.ros.org/schema/package_format3.xsd" schematypens="http://www.w3.org/2001/XMLSchema"?>
<package format="3">
  <name>pkg01_helloworld_cpp</name>
  <version>0.0.0</version>
  <description>TODO: Package description</description>
  <maintainer email="ros2@todo.todo">ros2</maintainer>
  <license>TODO: License declaration</license>

  <buildtool_depend>ament_cmake</buildtool_depend>

  <!-- 所需要依赖 -->
  <depend>rclcpp</depend>

  <test_depend>ament_lint_auto</test_depend>
  <test_depend>ament_lint_common</test_depend>

  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
```
注释部分以后需要根据实际的包依赖进行添加或修改。

#### 2.CMakeLists.txt
文件内容如下：
```c
cmake_minimum_required(VERSION 3.8)
project(pkg01_helloworld_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# 引入外部依赖包
find_package(rclcpp REQUIRED)

# 映射源文件与可执行文件
add_executable(helloworld src/helloworld.cpp)
# 设置目标依赖库
ament_target_dependencies(
  helloworld
  "rclcpp"
)
# 定义安装规则
install(TARGETS helloworld
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
```
中文注释部分以后可能需要根据实际情况修改。

### 编译项目
终端下进入到工作空间，执行如下指令：
```bash
colcon build
```

### 执行项目
终端下进入到工作空间，执行如下指令：
```bash
. install/setup.bash
ros2 run pkg01_helloworld_cpp helloworld
```
程序执行，在终端下将输出文本："hello world!"。

### 运行优化
::: danger 注意
虽然这样设置的运行优化，在一定程度上方便了我们使用，但是，我认为没有必要<br>
其实就是一条命令的事情，`source install/setup.bash`, 没必要按下面设置<br>
如果你就是想优化，当这个danger不存在。
:::

每次终端中执行工作空间下的节点时，都需要调用. install/setup.bash指令，使用不便，优化策略是，可以将该指令的调用添加进~/setup.bash，操作格式如下：
``` bash
echo "source /{工作空间路径}/install/setup.bash" >> ~/.bashrc
```
示例：
``` bash
echo "source /home/ros2/ws00_helloworld/install/setup.bash" >> ~/.bashrc
```
以后再启动终端时，无需手动再手动刷新环境变量，使用更方便。
## 命令集 🤨

无论是使用C++还是Python编写ROS2程序，都需要依赖于工作空间，在此，我们先实现工作空间的创建与编译，打开终端，输入如下指令：
```bash
mkdir -p first_helloworld/src #创建工作空间以及子级目录 src，工作空间名称可以自定义
cd first_helloworld #进入工作空间
colcon build #编译
```
上述指令执行完毕，将创建`first_helloworld`目录，且该目录下包含build、install、log、src共四个子级目录。

### 创建功能包
终端下，进入`first_helloworld/src`目录，使用如下指令创建一个C++功能包：
```bash
ros2 pkg create pkg01_helloworld_cpp --build-type ament_cmake --dependencies rclcpp --node-name helloworld
```

### 编译
终端下进入到工作空间，执行如下指令：
```bash
colcon build
```

### 运行
终端下进入到工作空间，执行如下指令：
```bash
. install/setup.bash
ros2 run pkg01_helloworld_cpp helloworld
