//
// Created by zhihui on 3/13/20.
//

#ifndef SERVER_BIDIRECTIONAL_TEST_H
#define SERVER_BIDIRECTIONAL_TEST_H

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
                BidirectionalTestSignature, async_grpc::Stream<proto::testMessage>,
                async_grpc::Stream<proto::testMessage>,
        "/zhihui.test.proto.Localization/BidirectionalTest")

        class BidirectionalTest : public async_grpc::RpcHandler<BidirectionalTestSignature> {
        public:
            void OnRequest(const proto::testMessage &request) override;

            void OnReadsDone() override;

        private:
            int sum_ = 0;
        };
    }
}
class bidirectional_test {

};


#endif //SERVER_BIDIRECTIONAL_TEST_H
