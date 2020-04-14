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

namespace Schedule::handler {
    using namespace zhihui::test;

    using FibonacciIncomingType = zhihui::test::proto::FibonacciRequest;
    using FibonacciOutgoingType = zhihui::test::proto::FibonacciResponse;

    DEFINE_HANDLER_SIGNATURE(CalculateFibonacciSignature, FibonacciIncomingType,
            async_grpc::Stream<FibonacciOutgoingType>,
    "/zhihui.test.proto.Schedule/CalculateFibonacci")

    class CalculateFibonacciHandler : public async_grpc::RpcHandler<CalculateFibonacciSignature> {
    public:
        void OnRequest(const FibonacciIncomingType &request) override;

    private:
        int sum_ = 0;
    };
}


#endif //SERVER_GET_INSTRUCTIONS_HANDLER_H
