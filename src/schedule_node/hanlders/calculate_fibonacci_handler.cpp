//
// Created by zhihui on 3/13/20.
//

#include "calculate_fibonacci_handler.h"

using namespace Schedule;
using namespace Schedule::handler;

CalculateFibonacciHandler::~CalculateFibonacciHandler() {
    GetUnsynchronizedContext<DataContextInterface>()->remove_action_node(client_);
    LOG(INFO) << "connection is broken";
}

void CalculateFibonacciHandler::OnRequest(const FibonacciIncomingType &request) {
    if (request.metadata().client_id().empty()) {
        Finish(::grpc::Status(::grpc::INTERNAL, "unnamed client"));
    }

    int64_t order = request.order();
    std::string client_id = request.metadata().client_id();
    LOG(INFO) << "Get Request from: " << client_id;
    // verify the request here
    if (order > 0 && order < 100) {
        /// WARNING: this code block needs to return quickly to avoid blocking the handler

        // get a reusable client for each user
        // client's name is request.client_id
        auto client = GetUnsynchronizedContext<DataContextInterface>()
                ->find_or_create_action_client(request.metadata().client_id());
        client_ = client;
        std::weak_ptr <ActionClient> client_weak_ptr(client);

        // if we have sent a goal before, cancel the current one
        // if we haven't sent a goal, calling cancel_goal() won't have impacts
        if (!client->is_goal_done()) {
            client->cancel_goal();
            LOG(WARNING) << "goal has yet been done, cancel it";
        }

        // get the writer through which we can send back responses to clients
        auto writer = this->GetWriter();

        // create a new goal
        ActionClient::ActionMessage::Goal goal;
        goal.order = request.order();
        client->set_goal(goal);

        client->set_feedback_callback([client_weak_ptr, writer](ActionClient::ActionFeedback feedback) {
            if (auto client = client_weak_ptr.lock()) {
                int num = feedback->partial_sequence.back();
                std::string node_id = client->get_name();

                auto response = std::make_unique<FibonacciOutgoingType>();
                response->mutable_metadata()->set_client_id(node_id);
                response->set_is_done(false);
                response->set_next_one(num);

                if (writer.Write(std::move(response))) {
                    LOG(INFO) << "Write response: " << num;
                } else {
                    // cancel the goal when the grpc connection is broken(cos writer has been destroyed)
                    client->cancel_goal();

                    LOG(WARNING) << "grpc connection is broken, cancel the goal";
                }
            }
        });

        client->set_result_callback([client_weak_ptr, writer](const ActionClient::ActionResult &result) {
            if (auto client = client_weak_ptr.lock()) {
                std::string node_id = client->get_name();

                auto response = std::make_unique<FibonacciOutgoingType>();
                response->mutable_metadata()->set_client_id(node_id);
                response->set_is_done(true);
                for (auto number : result.result->sequence) {
                    response->mutable_sequence()->add_num(number);
                }

                if (writer.Write(std::move(response))) {
                    writer.WritesDone();
                    LOG(INFO) << "send back result";
                } else {
                    LOG(WARNING) << "grpc connection is broken, drop the result";
                }
            }
        });

        // send the goal
        client->send_goal();
    } else {
        LOG(WARNING) << "discard invalid order: " << order << " from: " << request.metadata().client_id();
    }
}