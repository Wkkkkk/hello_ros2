//
// Created by zhihui on 3/14/20.
//

#include "upload_pointcloud_handler.h"

using namespace message::handler;

void UploadPointCloudHandler::OnRequest(const proto::UploadPointCloudRequest &request) {
    std::cout << "UploadPointCloudHandler: " << request.pointcloud_data().point_data_size() << std::endl;
}

void UploadPointCloudHandler::OnReadsDone() {
    this->Send(std::make_unique<google::protobuf::Empty>());
}
