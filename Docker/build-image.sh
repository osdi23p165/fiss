#!/bin/bash

#exit if any command fails
set -e 

pushd ../
docker build -f Docker/Dockerfile -t htap:test .
docker tag htap:test 10.22.1.9:5000/htap
docker push 10.22.1.9:5000/htap
popd 
