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

namespace message::handler {
    using namespace zhihui::test;

    DEFINE_HANDLER_SIGNATURE(
            GetInstructionsSignature, zhihui::test::proto::LoginRequest,
            async_grpc::Stream<zhihui::test::proto::InstructionResponse>,
    "/zhihui.test.proto.Localization/GetInstructions")

    class GetInstructionsHandler : public async_grpc::RpcHandler<GetInstructionsSignature> {
    public:
        void OnRequest(const zhihui::test::proto::LoginRequest &request) override;

    private:
        int sum_ = 0;
    };
}


#endif //SERVER_GET_INSTRUCTIONS_HANDLER_H
