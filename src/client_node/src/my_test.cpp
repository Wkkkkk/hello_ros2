//
// Created by zhihui on 4/11/20.
//

#include <async_grpc/client.h>
#include <async_grpc/execution_context.h>
#include <grpc++/grpc++.h>

#include "client_node/data_context_interface.h"
#include "get_instructions_handler.h"

using namespace server;
using namespace message::handler;
using namespace async_grpc;

int main(int argc, char **argv) {
    int order = 5;
    Client <GetInstructionsSignature> client(::grpc::CreateChannel(
            kTestingServerAddress, ::grpc::InsecureChannelCredentials()));
    proto::LoginRequest request;
    request.mutable_metadata()->set_client_id("client1");
    request.set_order(order);
    client.Write(request);

    LOG(INFO) << "client write done";
    std::vector<int> fibonacci_list = {1, 2, 3, 5, 8, 13, 21};
    proto::InstructionResponse response;
    LOG(INFO) << "before response";
    for (int i = 0; i < order; ++i) {
        client.StreamRead(&response);
        LOG(INFO) << "get response" << response.timestamp();
    }

    LOG(INFO) << "get all response";
    return 0;
}
