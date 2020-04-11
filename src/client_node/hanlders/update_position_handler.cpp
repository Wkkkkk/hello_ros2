//
// Created by zhihui on 3/13/20.
//

#include "update_position_handler.h"

using namespace message::handler;

void UpdatePositionHandler::OnRequest(const proto::UpdatePositionRequest &request) {
    std::cout << "UpdatePositionHandler: " << request.position().pos().x()
              << request.position().pos().y() << request.position().pos().z() << std::endl;
}

void UpdatePositionHandler::OnReadsDone() {
    this->Send(std::make_unique<google::protobuf::Empty>());
}
