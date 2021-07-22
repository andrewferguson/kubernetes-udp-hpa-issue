# Download ubuntu from the Docker Hub
FROM ubuntu:groovy

# Install dependencies
RUN apt-get update

# Extra dependencies for testing / debugging
RUN apt-get -y install python3 curl wget netcat screen python3-pip nano

# Install server
COPY ./server.c ./
RUN gcc server.c -o server
