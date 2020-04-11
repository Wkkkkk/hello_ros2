//
// Created by zhihui on 3/14/20.
//

#ifndef SERVER_UPLOAD_POINTCLOUD_HANDLER_H
#define SERVER_UPLOAD_POINTCLOUD_HANDLER_H

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
                UploadPointCloudSignature, async_grpc::Stream<proto::UploadPointCloudRequest>,
                google::protobuf::Empty,
        "/zhihui.test.proto.Localization/UploadPointCloud")

        class UploadPointCloudHandler : public async_grpc::RpcHandler<UploadPointCloudSignature> {
        public:
            void OnRequest(const proto::UploadPointCloudRequest &request) override;

            void OnReadsDone() override;

        private:
            int sum_ = 0;
        };
    }
}


#endif //SERVER_UPLOAD_POINTCLOUD_HANDLER_H
