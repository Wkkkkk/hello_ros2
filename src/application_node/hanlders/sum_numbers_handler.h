//
// Created by zhihui on 3/13/20.
//

#ifndef SERVER_UPDATE_POSITION_HANDLER_H
#define SERVER_UPDATE_POSITION_HANDLER_H

#include <memory>

#include <async_grpc/rpc_handler.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/empty.pb.h>

#include "test.pb.h"
#include "test.grpc.pb.h"

namespace Application::handler {
    using namespace zhihui::test;

    using SumIncomingType = zhihui::test::proto::SumRequest;
    using SumOutgoingType = zhihui::test::proto::SumResponse;

    DEFINE_HANDLER_SIGNATURE(
            SumNumbersSignature, async_grpc::Stream<SumIncomingType>, SumOutgoingType,
    "/zhihui.test.proto.Application/SumNumbers")

    class SumNumbersHandler : public async_grpc::RpcHandler<SumNumbersSignature> {
    public:
        void OnRequest(const SumIncomingType &request) override;

        void OnReadsDone() override;

    private:
        std::string client_id_;
        std::vector <int64_t> number_list_;
    };
}


#endif //SERVER_UPDATE_POSITION_HANDLER_H
