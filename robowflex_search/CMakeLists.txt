cmake_minimum_required(VERSION 3.1...3.10)
project(robowflex_search
  VERSION 1.0
  LANGUAGES CXX
)

add_compile_options(-std=c++17)
set(LIBRARY_NAME ${PROJECT_NAME})

set(CMAKE_MODULE_PATH
  "${CMAKE_MODULE_PATH}"
  "${CMAKE_ROOT_DIR}/cmake/Modules"
  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

include(CompileOptions)
include(HelperFunctions)

find_package(Boost REQUIRED COMPONENTS serialization)

find_package(catkin QUIET COMPONENTS
  robowflex_library
  moveit_planners_ims
)
find_package(search REQUIRED)

catkin_package(
  LIBRARIES 
    ${LIBRARY_NAME}
    /usr/local/lib/${search_LIBRARIES}
  CATKIN_DEPENDS
    robowflex_library
    moveit_planners_ims
  DEPENDS
  INCLUDE_DIRS 
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${search_INCLUDE_DIRS}
)

include_directories(SYSTEM ${CMAKE_CURRENT_LIST_DIR}/include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
link_directories(${catkin_LIBRARY_DIRS})

add_library(${LIBRARY_NAME}
  src/search_interface.cpp
#  src/search_trajectory.cpp
)
set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION})
target_link_libraries(${LIBRARY_NAME}
                      ${catkin_LIBRARIES}
                      ${Boost_LIBRARIES}
                      /usr/local/lib/${search_LIBRARIES}
)

