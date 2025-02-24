# generated from catkin/cmake/template/pkg.context.pc.in
CATKIN_PACKAGE_PREFIX = ""
PROJECT_PKG_CONFIG_INCLUDE_DIRS = "${prefix}/include".split(';') if "${prefix}/include" != "" else []
PROJECT_CATKIN_DEPENDS = "std_msgs;sensor_msgs;geometry_msgs;visualization_msgs;shape_msgs;octomap_msgs;trajectory_msgs;object_recognition_msgs;moveit_msgs;roscpp;rosbag;urdf;srdfdom;pluginlib;geometric_shapes;moveit_core;moveit_ros_planning;tf2_ros;xmlrpcpp".replace(';', ' ')
PKG_CONFIG_LIBRARIES_WITH_PREFIX = "-lrobowflex_library".split(';') if "-lrobowflex_library" != "" else []
PROJECT_NAME = "robowflex_library"
PROJECT_SPACE_DIR = "/usr/local"
PROJECT_VERSION = "1.3.0"
