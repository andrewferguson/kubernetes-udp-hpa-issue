#!/bin/bash

docker build -t andrewferguson/udptest:latest . && \
docker tag andrewferguson/udptest:latest andrewferguson/udptest:latest && \
docker push andrewferguson/udptest:latest && \
echo "All done!"
