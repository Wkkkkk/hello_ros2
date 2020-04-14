//
// Created by zhihui on 4/11/20.
//

#include <async_grpc/client.h>
#include <async_grpc/execution_context.h>
#include <grpc++/grpc++.h>
#include <glog/logging.h>

#include "schedule_node/data_context_interface.h"
#include "calculate_fibonacci_handler.h"
#include "sum_numbers_handler.h"
#include "broadcast_handler.h"

using namespace Schedule;
using namespace Schedule::handler;
using namespace async_grpc;

void action_test() {
    int order = 10;
    Client <CalculateFibonacciSignature> client(::grpc::CreateChannel(
            kTestingServerAddress, ::grpc::InsecureChannelCredentials()));
    proto::FibonacciRequest request;
    request.mutable_metadata()->set_client_id("client1");
    request.set_order(order);

    // write request to server
    /// this can only be called once
    client.Write(request);

    proto::FibonacciResponse response;
    while (client.StreamRead(&response)) {
        if (!response.is_done()) {
            LOG(INFO) << "get response: " << response.next_one();
        } else {
            LOG(INFO) << "get result\n: " << response.sequence().DebugString();
        }
    }
    auto status = client.StreamFinish();
    LOG(INFO) << "get all response.";
}

void service_test() {
    Client <SumNumbersSignature> client(::grpc::CreateChannel(
            kTestingServerAddress, ::grpc::InsecureChannelCredentials()));

    for (int i = 0; i < 10; ++i) {
        proto::SumRequest request;
        request.mutable_metadata()->set_client_id("ServiceClient1");
        request.set_order(i);

        // write request to server
        client.Write(request);
    }
    client.StreamWritesDone();

    if (client.StreamFinish().ok()) {
        proto::SumResponse response = client.response();
        LOG(INFO) << "get sum result: " << response.DebugString();
    } else {
        LOG(ERROR) << "get sum result: " << client.StreamFinish().error_message() << std::endl;
    }
}

void publisher_test() {
    Client <BroadcastSignature> client(::grpc::CreateChannel(
            kTestingServerAddress, ::grpc::InsecureChannelCredentials()));

    proto::BroadcastMessage request;
    request.set_topic("Test");
    request.set_message("Hello World!");

    client.Write(request);

    auto response = client.response();
    LOG(INFO) << "get null response";
}

int main(int, char **) {
    publisher_test();

    return 0;
}
