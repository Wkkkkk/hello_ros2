//
// Created by zhihui on 3/13/20.
//

#ifndef SERVER_LOGIN_HANDLER_H
#define SERVER_LOGIN_HANDLER_H

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
                LoginSignature, zhihui::test::proto::LoginRequest,
                zhihui::test::proto::LoginResponse,
        "/zhihui.test.proto.Localization/Login")

        class LoginHandler : public async_grpc::RpcHandler<LoginSignature> {
        public:
            void OnRequest(const zhihui::test::proto::LoginRequest &request) override;

        private:
            int sum_ = 0;
        };
    }
}


#endif //SERVER_LOGIN_HANDLER_H
