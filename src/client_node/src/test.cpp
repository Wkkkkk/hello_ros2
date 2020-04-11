//
// Created by zhihui on 4/11/20.
//

#include <memory>

#include <rclcpp/rclcpp.hpp>

#include <async_grpc/client.h>
#include <async_grpc/execution_context.h>
#include <async_grpc/retry.h>
#include <grpc++/grpc++.h>
#include <gtest/gtest.h>

#include "client_node/data_server.h"
#include "client_node/data_context_interface.h"
#include "get_instructions_handler.h"

namespace server {
    using namespace message::handler;
    using namespace async_grpc;

    class ServerTest : public ::testing::Test {
    protected:
        void SetUp() override {
            int argc;
            char **argv;
            rclcpp::init(argc, argv);

            server_ = std::make_unique<DataServer>();
            client_channel_ = ::grpc::CreateChannel(
                    kTestingServerAddress, ::grpc::InsecureChannelCredentials());


            server_->initSlamThread();
            server_->Start();
        }

        void TearDown() override {
            server_->Shutdown();
            rclcpp::shutdown();
        }

        std::unique_ptr <DataServer> server_;
        std::shared_ptr<::grpc::Channel> client_channel_;
    };

    TEST_F(ServerTest, StartAndStopServerTest
    ) {
}

//    TEST_F(ServerTest, ProcessUnaryRpcTest) {
//        Client<LoginSignature> client(client_channel_);
//        proto::LoginRequest request;
//        request.mutable_metadata()->set_client_id("test");
//        request.set_log_information("Hello");
//        grpc::Status status;
//        EXPECT_TRUE(client.Write(request, &status));
//        EXPECT_EQ(client.response().check_information(), "Hello");
//    }
//
//    TEST_F(ServerTest, RetryWithUnrecoverableError) {
//        Client<LoginSignature> client(
//                client_channel_, common::FromSeconds(5),
//                CreateUnlimitedConstantDelayStrategy(common::FromSeconds(1),
//                                                     {::grpc::INTERNAL}));
//        proto::LoginRequest request;
//        request.mutable_metadata()->set_client_id("");
//        EXPECT_FALSE(client.Write(request));
//    }
//
//    TEST_F(ServerTest, ProcessRpcClientStreamTest) {
//        Client<UpdatePositionSignature> client(client_channel_);
//        for (int i = 0; i < 3; ++i) {
//            proto::UpdatePositionRequest request;
//            EXPECT_TRUE(client.Write(request));
//        }
//        EXPECT_TRUE(client.StreamWritesDone());
//        EXPECT_TRUE(client.StreamFinish().ok());
//        EXPECT_EQ(client.response().GetDescriptor()->name(), "Empty");
//    }

TEST_F(ServerTest, ProcessRpcServerStreamTest
) {
Client <GetInstructionsSignature> client(client_channel_);
proto::LoginRequest request;
request.mutable_metadata()->set_client_id("client1");
request.set_order(5);
EXPECT_TRUE(client
.
Write(request)
);

LOG(INFO)
<< "client write done";
std::vector<int> fibonacci_list = {1, 2, 3, 5, 8, 13, 21};
EXPECT_LE(request
.

order(), fibonacci_list

.

size()

);
proto::InstructionResponse response;
LOG(INFO)
<< "before response";
for (
int i = 0;
i < 5; ++i) {
EXPECT_TRUE(client
.
StreamRead(&response)
);
EXPECT_EQ(response
.

timestamp(), fibonacci_list[i]

);
LOG(INFO)
<< "get response";
}
}

//    TEST_F(ServerTest, ProcessRpcBidirectionalTest) {
//        Client<BidirectionalTestSignature> client(client_channel_);
//
//        for (int i = 0; i < 10; ++i) {
//            proto::testMessage request;
//            request.set_index(i);
//            EXPECT_TRUE(client.Write(request));
//
//            proto::testMessage response;
//            EXPECT_TRUE(client.StreamRead(&response));
//            EXPECT_EQ(response.index(), i + 1);
//        }
//        EXPECT_TRUE(client.StreamWritesDone());
//        EXPECT_TRUE(client.StreamFinish().ok());
//    }
}