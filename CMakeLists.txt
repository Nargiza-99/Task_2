cmake_minimum_required(VERSION 2.8.12)

project(Task_2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(HEADERS "ccl_algorithm.h" "niblack_algorithm.h" "vec_methods.h" "frame_rectangle.h")
set(SOURCES "Task_2_1.cpp" "ccl_algorithm.cpp" "niblack_algorithm.cpp" "vec_methods.cpp" "frame_rectangle.cpp")


find_package(Qt5Core)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME}  ${OpenCV_LIBS} )


