//
// Created by zhihui on 3/13/20.
//

#include "bidirectional_test.h"

using namespace message::handler;

void BidirectionalTest::OnRequest(const proto::testMessage &request) {

    //TODO set position
    auto response = std::make_unique<proto::testMessage>();
    response->set_index(request.index() + 1);

    //TODO use Writer
    Send(std::move(response));

    LOG(INFO) << "BidirectionalTest: " << request.index();
}

void BidirectionalTest::OnReadsDone() {
    LOG(INFO) << "BidirectionalTest OnReadsDone";
    Finish(::grpc::Status(::grpc::OK, "OnReadsDone"));
}
