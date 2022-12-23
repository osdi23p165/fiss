#include <iostream>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "cmake/build/state_transfer.grpc.pb.h"
#include "cmake/build/state_transfer.pb.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;



int main(){
    std::string target_str = "localhost:51209";

    auto channel = grpc::CreateChannel(target_str,grpc::InsecureChannelCredentials());


    std::unique_ptr<state_transfer::OLAPServer::Stub> stub;

    stub = state_transfer::OLAPServer::NewStub(channel);

    grpc::ClientContext context;


    auto stream = stub->TransferUpdate(&context);



    for (int j = 0; j < 100; j++){

        state_transfer::UpdateReq req; 
        req.set_shard_id(1); 
        req.set_col_id("customer.balance");
        auto ups = req.mutable_updates(); 

        for(int i = 0; i<20; i++){

           auto up = new state_transfer::Update(); 
           up->set_row_index(i);
           up->set_value(std::to_string(j * 100 +i));

           
           ups->AddAllocated(up); 
        }


        stream->Write(req);

        std::cout << "transferred update: " << j << std::endl;
        state_transfer::UpdateResp resp;
        stream->Read(&resp);
        std::cout << "received resp: " << resp.res() << std::endl; 
    }

    stream->WritesDone();
    grpc::Status status = stream->Finish();
    if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        std::cout << "RPC failed";
    }


    grpc::ClientContext context2;
    auto stream2 = stub->TransferOrder(&context2);

    for (int j = 0; j < 100; j++){
        state_transfer::OrderReq req; 
        req.set_shard_id(2); 
        auto ods = req.mutable_orders(); 

        for(int i = 0; i<20; i++){

            auto od = new state_transfer::Order(); 
            od->set_txn_id(j*1000+i);
            od->set_txn_type(10001);
            ods->AddAllocated(od); 
        }

        stream2->Write(req);
        std::cout << "transferred order" << j << std::endl;

        state_transfer::OrderResp resp;
        stream2->Read(&resp);

        std::cout << "received resp" << resp.res() << std::endl; 

    }

    stream2->WritesDone();
    status = stream2->Finish();
    if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        std::cout << "RPC failed";
    }

}