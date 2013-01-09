cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_add_executable(example examples/example.cpp)
#rosbuild_link_boost(main thread)
#target_link_libraries(example ${PROJECT_NAME})

find_package(OpenCV REQUIRED)
find_package(X11)
rosbuild_add_library(video_tracker src/video_tracker.cpp)
rosbuild_add_library(particle_filter src/particle_filter.cpp)
rosbuild_add_library(mouse_control src/mouse_control.cpp)
rosbuild_add_executable(main src/main.cpp)
rosbuild_add_executable(run_pf src/run_pf.cpp)
rosbuild_add_executable(test_mouse src/test_mouse.cpp)
target_link_libraries(test_mouse mouse_control ${X11_LIBRARIES})
target_link_libraries(video_tracker ${OpenCV_LIBS})
target_link_libraries(particle_filter video_tracker)
target_link_libraries(main particle_filter mouse_control ${X11_LIBRARIES})
target_link_libraries(run_pf particle_filter mouse_control ${X11_LIBRARIES})
