//
// Created by zhihui on 3/13/20.
//

#ifndef SERVER_GET_INSTRUCTIONS_HANDLER_H
#define SERVER_GET_INSTRUCTIONS_HANDLER_H

#include <memory>

#include <async_grpc/rpc_handler.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/empty.pb.h>

#include "test.pb.h"
#include "test.grpc.pb.h"
#include "application_node/data_context_interface.h"

namespace Application::handler {
    using namespace zhihui::test;
    using namespace Action;

    using FibonacciIncomingType = zhihui::test::proto::FibonacciRequest;
    using FibonacciOutgoingType = zhihui::test::proto::FibonacciResponse;

    DEFINE_HANDLER_SIGNATURE(CalculateFibonacciSignature, FibonacciIncomingType,
            async_grpc::Stream<FibonacciOutgoingType>,
    "/zhihui.test.proto.Application/CalculateFibonacci")

    class CalculateFibonacciHandler : public async_grpc::RpcHandler<CalculateFibonacciSignature> {
    public:
        ~CalculateFibonacciHandler();

        void OnRequest(const FibonacciIncomingType &request) override;

    private:
        std::shared_ptr <ActionClient> client_;
    };
}


#endif //SERVER_GET_INSTRUCTIONS_HANDLER_H
