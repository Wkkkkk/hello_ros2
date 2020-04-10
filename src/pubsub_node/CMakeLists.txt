cmake_minimum_required(VERSION 3.5)
project(pubsub_node)

# Default to C99
if (NOT CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 99)
endif ()

# Default to C++14
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
find_package(rclcpp REQUIRED)
find_package(tutorials_msgs REQUIRED)

include_directories(include)

add_library(${PROJECT_NAME} SHARED
        src/publisher.cpp
        src/subscriber.cpp)
target_compile_definitions(${PROJECT_NAME}
        PRIVATE "MINIMAL_COMPOSITION_DLL")
#target_link_libraries(${PROJECT_NAME} )
ament_target_dependencies(${PROJECT_NAME} rclcpp tutorials_msgs)

add_executable(compose_test src/test.cpp)
target_link_libraries(compose_test ${PROJECT_NAME})
ament_target_dependencies(compose_test rclcpp tutorials_msgs)

install(TARGETS
        ${PROJECT_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(TARGETS
        compose_test
        DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY
        launch
        DESTINATION share/${PROJECT_NAME}/
        )

if (BUILD_TESTING)
    find_package(ament_lint_auto REQUIRED)
    # the following line skips the linter which checks for copyrights
    # uncomment the line when a copyright and license is not present in all source files
    #set(ament_cmake_copyright_FOUND TRUE)
    # the following line skips cpplint (only works in a git repo)
    # uncomment the line when this package is not in a git repo
    #set(ament_cmake_cpplint_FOUND TRUE)
    ament_lint_auto_find_test_dependencies()
endif ()

ament_package()
