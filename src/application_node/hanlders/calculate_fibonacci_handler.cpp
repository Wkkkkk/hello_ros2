//
// Created by zhihui on 3/13/20.
//

#include "calculate_fibonacci_handler.h"

using namespace Schedule;
using namespace Schedule::handler;

CalculateFibonacciHandler::~CalculateFibonacciHandler() {
    LOG(INFO) << "connection is broken";
    // when the connection is broken, this handler will destruct
    // stop the action client from working, and release it
    GetUnsynchronizedContext<DataContextInterface>()->remove_action_node(client_);
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

        // hold this shared_ptr in handler
        client_ = client;

        // if this client has sent a goal twice and the former goal is running, cancel the former one
        if (!client->is_goal_done()) {
            LOG(WARNING) << "goal has yet finished, cancel the former one";
            client->cancel_goal();
        }

        // get the writer through which we can send back responses to clients
        auto writer = this->GetWriter();

        // create a new goal
        ActionClient::ActionMessage::Goal goal;
        goal.order = request.order();
        client->set_goal(goal);

        // set feedback callback
        /// If you capture a shared pointer with a lambda, that lambda will contain a shared pointer.
        /// This is a typical scenario where the object owns a reference to itself, and thus can never be deleted.
        /// To fix this, first establish a rule: never capture a std::shared_ptr<> in a lambda.
        /// A simple solution is to use std::weak_ptr<>.
        std::weak_ptr <ActionClient> client_weak_ptr(client);
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
                }
            }
        });

        // set result callback
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
                }
            }
        });

        // send the goal
        client->send_goal();
    } else {
        LOG(WARNING) << "discard invalid order: " << order << " from: " << request.metadata().client_id();
    }
}