#  Pub-Sub Module

## Background
There are **three forms of communication** in ROS1/ROS2: topics, services, and actions. 
Topic publishers broadcast to multiple subscribers, but communication is one-way.

ROS 1 uses TCP as the underlying transport, which is unsuitable for lossy networks such as wireless links. 
With ROS 2 relying on DDS which uses UDP as its transport, we can give control over the level of reliability a node can expect and act accordingly.

## QoS profiles
A QoS profile defines a set of policies, including durability, reliability, queue depth and sample history storage. 
The base QoS profile includes settings for the following policies:

* History.
    * Keep last: only store up to N samples, configurable via the queue depth option.
    * Keep all: store all samples, subject to the configured resource limits of the DDS vendor.

* Depth.
    * Size of the queue: only honored if used together with “keep last”.

* Reliability.
    * Best effort: attempt to deliver samples, but may lose them if the network is not robust.
    * Reliable: guarantee that samples are delivered, may retry multiple times.

* Durability.
    * Transient local: only applies to DataWriters. DataWriter becomes responsible of persisting samples until a corresponding DataReader becomes available.
    * Volatile: no attempt is made to persist samples.
    
## How to choose
ROS 2 will provide QoS profiles based on the following use cases:

* Default QoS settings for publishers and subscriptions

In order to make the transition from ROS 1 to ROS 2, exercising a similar network behavior is desirable. 

**By default, publishers and subscriptions are reliable in ROS 2, have volatile durability, and “keep last” history.**

* Services

In the same vein as publishers and subscriptions, services are reliable. It is especially important for services to use volatile durability, as otherwise service servers that re-start may receive outdated requests.

* Sensor data

For sensor data, in most cases it’s more important to receive readings in a timely fashion, rather than ensuring that all of them arrive. That is, developers want the latest samples as soon as they are captured, at the expense of maybe losing some. For that reason the sensor data profile uses best effort reliability and a smaller queue depth.

* Parameters

Parameters are based on services, and as such have a similar profile. The difference is that parameters use a much larger queue depth so that requests do not get lost when, for example, the parameter client is unable to reach the parameter service server.

Profiles allow developers to focus on their applications without worrying about every QoS setting in the DDS specification