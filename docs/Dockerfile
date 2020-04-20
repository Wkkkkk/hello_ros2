FROM osrf/ros:eloquent-desktop as builder
LABEL description="ubuntu18.04 with ros-eloquent-desktop installed"
##################################################
## add aliyun source
COPY sources.list /etc/apt/sources.list

## build dependencies.
# software-properties-common for add-apt-repository
# ca-certificates for verification
RUN apt-get update && apt-get install -y \
    software-properties-common \
    ca-certificates \
    sudo \
    build-essential \
    cmake \
    vim \
    git \
    wget \
    curl \
    unzip \
    tar \ 
    --fix-missing \
    && rm -rf /var/lib/apt/lists/*

## install vcpkg
RUN cd /tmp \
    && git clone https://gitee.com/Will_1989/vcpkg.git && cd vcpkg \
    && ./bootstrap-vcpkg.sh

## add environment variables
ENV VCPKG_DIR /tmp/vcpkg
ENV CMAKE_PREFIX_PATH $VCPKG_DIR/installed/x64-linux:/usr/local/lib:$CMAKE_PREFIX_PATH
ENV LD_LIBRARY_PATH $VCPKG_DIR/installed/x64-linux/lib:/usr/local/lib:$LD_LIBRARY_PATH

## install libraries
RUN /tmp/vcpkg/vcpkg install grpc glog yaml-cpp

## install unsupported libraries
# async_grpc
RUN cd /tmp \
    && git clone https://gitee.com/Will_1989/async_grpc.git \
    && cd async_grpc && mkdir build && cd build \
    && cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake \
    && make install

## build target project
RUN cd /tmp \
    && git clone https://gitee.com/Will_1989/hello_ros2.git

COPY build.sh /tmp/hello_ros2
RUN cd /tmp/hello_ros2 && ./build.sh

EXPOSE 50051
