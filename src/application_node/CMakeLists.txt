cmake_minimum_required(VERSION 3.5)
project(application_node)

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
find_package(service_node REQUIRED)
find_package(action_node REQUIRED)
## find protobuf
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
if (Protobuf_INCLUDE_DIRS)
    message(WARNING "Can't find Protobuf_INCLUDE_DIRS,
        it should be $ENV{VCPKG_DIR}/installed/x64-linux/include")
    set(Protobuf_INCLUDE_DIRS $ENV{VCPKG_DIR}/installed/x64-linux/include)
endif ()

## find grpc
find_package(gRPC CONFIG REQUIRED)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
## find async_grpc
find_package(async_grpc REQUIRED)
## find glog
find_package(glog CONFIG REQUIRED)

include_directories(
        include
        hanlders
        ${CMAKE_CURRENT_BINARY_DIR} # for generated protobuf messages
)
message(STATUS "Messages are built at: ${CMAKE_CURRENT_BINARY_DIR}")

# generate all messages
set(PROTO_MESSAGES test)
set(hw_proto_srcs_list "")
set(hw_grpc_srcs_list "")
foreach (message_name ${PROTO_MESSAGES})
    # Proto file
    get_filename_component(hw_proto "./proto/${message_name}.proto" ABSOLUTE)
    get_filename_component(hw_proto_path "${hw_proto}" PATH)
    message(STATUS "Found proto ${hw_proto}")

    # Generated sources
    set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/${message_name}.pb.cc")
    set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${message_name}.pb.h")
    set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/${message_name}.grpc.pb.cc")
    set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${message_name}.grpc.pb.h")
    add_custom_command(
            OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
            COMMAND ${_PROTOBUF_PROTOC}
            ARGS --grpc_out "generate_mock_code=true:${CMAKE_CURRENT_BINARY_DIR}"
            --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
            -I "${hw_proto_path}" -I "${Protobuf_INCLUDE_DIRS}"
            --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
            "${hw_proto}"
            DEPENDS "${hw_proto}")
    list(APPEND hw_proto_srcs_list "${hw_proto_srcs}")
    list(APPEND hw_grpc_srcs_list "${hw_grpc_srcs}")
endforeach (message_name)

# client module
add_library(${PROJECT_NAME} SHARED
        ${hw_proto_srcs_list}
        ${hw_grpc_srcs_list}
        src/data_context_interface.cpp
        src/data_server.cpp
        hanlders/calculate_fibonacci_handler.cpp
        hanlders/login_handler.cpp
        hanlders/sum_numbers_handler.cpp
        hanlders/broadcast_handler.cpp
        )
target_compile_definitions(${PROJECT_NAME}
        PRIVATE "MINIMAL_COMPOSITION_DLL")
target_link_libraries(${PROJECT_NAME}
        async_grpc
        gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts
        protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite
        glog::glog
        )
ament_target_dependencies(${PROJECT_NAME} rclcpp action_node service_node tutorials_msgs)

add_executable(node src/main.cpp)
target_link_libraries(node ${PROJECT_NAME})
ament_target_dependencies(node rclcpp)

add_executable(node_test src/test.cpp)
target_link_libraries(node_test ${PROJECT_NAME})
ament_target_dependencies(node_test rclcpp)

install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION include/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.h"
        PATTERN ".git" EXCLUDE)

install(TARGETS
        ${PROJECT_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        )

install(TARGETS
        node
        node_test
        DESTINATION lib/${PROJECT_NAME}
        )

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
    #  set(ament_cmake_cpplint_FOUND TRUE)
    ament_lint_auto_find_test_dependencies()
endif ()

ament_package()
