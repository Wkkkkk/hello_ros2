# A grpc-ROS2 Framework Test

## Objective

This repository aims to integrate async-gRPC and ROS2 framework to support action/service/topic-based communication.

The framework should be easy to use and support both common and user-defined data types.

It's worth noting that all tasks are handled by independent nodes of the system.

## Node dependency graph
![avatar](./docs/dep.png)

## Architecture
![avatar](./docs/architecture.png)

## Submodule
* Pub/Sub

See [README](/src/pubsub_node/README.md) page for details.

* Action

See [README](/src/action_node/README.md) page for details.
