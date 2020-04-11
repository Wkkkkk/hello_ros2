//
// Created by zhihui on 3/13/20.
//

#include "login_handler.h"


using namespace message::handler;

void LoginHandler::OnRequest(const proto::LoginRequest &request) {
    if (request.metadata().client_id().empty()) {
        Finish(::grpc::Status(::grpc::INTERNAL, "internal error"));
    }

    auto response = std::make_unique<proto::LoginResponse>();
    response->set_check_information(request.log_information());
    Send(std::move(response));

    std::cout << "LoginHandler: " << request.metadata().client_id() << std::endl;
}
