#  Application Module

## Objective
This part contains a C++ gRPC framework for building asynchronous, multi-threaded gRPC servers. 
The framework is simple to use and supports arbitrary RPC types, i.e. all combinations of streaming and unary requests and responses.

## Background
gRPC is Google's open source RPC system which relies on Protobuf as a message serialization format.

gRPC makes it convenient through its protobuf compiler plugin to implement synchronous servers. 
For Robotics, sensor data (IMU, odometry, fixed-frame poses) and intermediate local SLAM results need to be continuously streamed to the cloud Server instance. 
Strict time ordering is only required for sensor data of the same type. 

So we built a small server framework on top of the existing gRPC async mechanisms to make it convenient to write asynchronous gRPC servers.

## Usage

### Example: Server offering unary RPC method “GetSquare()”

We start by defining the gRPC service as a proto service definition:

```proto
syntax = "proto3";

package proto;

message GetSquareRequest {
  int32 input = 1;
}

message GetSquareResponse {
  int32 output = 1;
}

service Math {
  rpc GetSquare(GetSquareRequest) returns (GetSquareResponse);
}
```

Note that the above defines an RPC that expects a unary request and generates a unary response from the server.
For streaming RPCs the request or response type would have been prepended with a `stream` annotation (see next example).

Next we define an implementation of the business logic that consumes the request and generates the response, i.e. the RPC handler.

```C++
DEFINE_HANDLER_SIGNATURE(
    GetSquareSignature, proto::GetSquareRequest, proto::GetSquareResponse,
    "/proto.Math/GetSquare")

class GetSquareHandler : public RpcHandler<GetSquareSignature> {
 public:
  void OnRequest(const proto::GetSquareRequest& request) override {
    auto response =
        cartographer::common::make_unique<proto::GetSquareResponse>();
    response->set_output(request.input() * request.input());
    Send(std::move(response));
  }
};
```

When a client establishes a connection to the server to call the `GetSquare` method, a new instance of a `GetSquareHandler` is instantiated.
The lifetime of each handler is therefore request scoped.
The `OnRequest()` method is invoked with the deserialized client request and the handler now has a chance to react to the request.
In this case we perform the calculation, populate a `GetSquareResponse` proto and pass it to the `Send()` method to be shipped to the client.
Note that for unary response RPCs calling `Send()` finishes the connection with `::grpc::Status::Ok`.
Note that we could have decided to call also call e.g. Finish(`PERMISSION_DENIED`) to error out instead of providing a response.

Next we set up the server and register our message handler.

```C++
Server::Builder server_builder;
server_builder.SetServerAddress("localhost:50051");
server_builder.SetNumGrpcThreads(2);
server_builder.SetNumEventThreads(2);
server_builder.RegisterHandler<GetSquareHandler>();
auto server = server_builder.Build();
server.Start();
server.WaitForShutdown();
```

The server address sets the interfaces and port on which the server will offer the gRPC service.
Number of gRPC threads specifies the number of threads the server will use for running the network stack and performing proto serialization / deserialization.
Number of event threads determines the number of threads running the business logic, i.e. the `GetSquareHandler` implemented above.

Note that there is no guarantee on which thread the handler will be created, but once the handler has been created it will always executed for the lifetime of the exchange on the same thread.
There is therfore no requirement for the `GetSquareHandler` to be thread-safe.

## How to use

```
# run pub-sub in one tmd
source install/setup.bash
ros2 launch application_node application_test.launch.py

# in another tmd
source install/setup.bash
ros2 run application_node node_test
```
