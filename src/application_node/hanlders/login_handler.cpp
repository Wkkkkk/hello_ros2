//
// Created by zhihui on 3/13/20.
//

#include "login_handler.h"
#include "application_node/data_context_interface.h"

using namespace Application;
using namespace Application::handler;

void LoginHandler::OnRequest(const LoginIncomingType &request) {
    std::string client_id = request.metadata().client_id();
    if (client_id.empty()) {
        Finish(::grpc::Status(::grpc::INTERNAL, "unnamed client"));
    }

    // verify the client info here
    //

    // create response
    auto response = std::make_unique<LoginOutgoingType>();
    response->mutable_metadata()->set_client_id(client_id);
    response->set_is_logged(true);

    // register a client for each user
    // client's name is request.client_id
    auto client = GetUnsynchronizedContext<DataContextInterface>()->find_or_create_action_client(client_id);

    // send response
    Send(std::move(response));
}
