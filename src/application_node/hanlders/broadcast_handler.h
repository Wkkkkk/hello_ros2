//
// Created by zhihui on 4/14/20.
//

#ifndef SCHEDULE_NODE_BROADCAST_HANDLER_H
#define SCHEDULE_NODE_BROADCAST_HANDLER_H


#include <memory>

#include <async_grpc/rpc_handler.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/empty.pb.h>

#include "test.pb.h"
#include "test.grpc.pb.h"

namespace Application::handler {
    using namespace zhihui::test;

    using BroadcastIncomingType = zhihui::test::proto::BroadcastMessage;
    using BroadcastOutgoingType = google::protobuf::Empty;

    DEFINE_HANDLER_SIGNATURE(BroadcastSignature, BroadcastIncomingType, BroadcastOutgoingType,
    "/zhihui.test.proto.Application/Broadcast")

    class BroadcastHandler : public async_grpc::RpcHandler<BroadcastSignature> {
    public:
        void OnRequest(const BroadcastIncomingType &request) override;

    private:
        int sum_ = 0;
    };
}


#endif //SCHEDULE_NODE_BROADCAST_HANDLER_H
