FROM ubuntu:20.04

RUN apt update
RUN apt-get update
RUN apt-get -o Acquire::ForceIPv4=true update
RUN DEBIAN_FRONTEND=noninteractive apt install -yq ninja-build g++ cmake libboost-all-dev
RUN apt install -yq libssl-dev
