//
// Created by zhihui on 3/13/20.
//

#include "client_node/data_context_interface.h"
#include "get_instructions_handler.h"
#include "action_node/action_client.h"

using namespace message::handler;

void GetInstructionsHandler::OnRequest(const zhihui::test::proto::LoginRequest &request) {
    if (request.metadata().client_id().empty()) {
        Finish(::grpc::Status(::grpc::INTERNAL, "internal error"));
    }

    std::cout << "Get Request from: " << request.metadata().client_id() << std::endl;
    int64_t order = request.order();
    if (order > 0 && order < 100) {
        // a new client for each request
        // TODO build a map which ties client-id with an action client, in order to reuse it
        auto client = std::make_shared<ActionClient>("test");
        GetUnsynchronizedContext<server::DataContextInterface>()->add_node(client);

        // get the writer
        auto writer = this->GetWriter();

        ActionClient::ActionMessage::Goal goal;
        goal.order = request.order();
        client->set_goal(goal);
        client->set_processor([client, writer](ActionClient::ActionFeedback feedback) {
            int num = feedback->partial_sequence.back();
            auto response = std::make_unique<proto::InstructionResponse>();
            response->set_timestamp(num);

            if (writer.Write(std::move(response))) {
                std::cout << "Write response: " << num << std::endl;
            } else {
                // cancel the goal when the grpc connection is broken(cos writer has been destroyed)
                client->cancel_goal();
            }
        });

        client->send_goal();
    } else {
        std::cout << "Discard invalid order: " << order << " from: " << request.metadata().client_id() << std::endl;
    }
}
