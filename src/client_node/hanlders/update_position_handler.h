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

namespace message {
    namespace handler {

        using namespace zhihui::test;

        DEFINE_HANDLER_SIGNATURE(
                UpdatePositionSignature, async_grpc::Stream<proto::UpdatePositionRequest>,
                google::protobuf::Empty,
        "/zhihui.test.proto.Localization/UpdatePosition")

        class UpdatePositionHandler : public async_grpc::RpcHandler<UpdatePositionSignature> {
        public:
            void OnRequest(const proto::UpdatePositionRequest &request) override;

            void OnReadsDone() override;

        private:
            int sum_ = 0;
        };
    }
}


#endif //SERVER_UPDATE_POSITION_HANDLER_H
