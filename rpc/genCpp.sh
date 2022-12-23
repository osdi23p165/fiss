#!/bin/bash

protoc -I=. --cpp_out=cppsrc state_transfer.proto

## Next step -> CP these files to the OLAP/OLTP folder 
#cp cppsrc/state_transfer.pb.cc ../OLTP/grpc/src
#cp cppsrc/state_transfer.pb.h ../OLTP/grpc/include
cp cppsrc/state_transfer.pb.h ../OLAP/grpc/inclued
cp cppsrc/state_transfer.pb.cc ../OLAP/grpc/include
## FIXIT: This can be simplified after using CMAKE files. 
