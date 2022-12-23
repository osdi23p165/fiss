#!/bin/bash

protoc -I=. --python_out=pysrc state_transfer.proto

## Next step -> CP these files to the OLAP/OLTP folder 
