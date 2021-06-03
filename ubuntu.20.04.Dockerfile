FROM ubuntu:20.04

RUN apt update
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt install -yq ninja-build g++ cmake libboost-all-dev python3 python3-pip
RUN apt install -yq libssl-dev
RUN pip3 install --upgrade cpm-cli
RUN pip3 install pyyaml
RUN pip3 install requests
RUN pip3 install six
